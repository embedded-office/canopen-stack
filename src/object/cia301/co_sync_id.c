/******************************************************************************
   Copyright 2020 Embedded Office GmbH & Co. KG

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
******************************************************************************/

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_core.h"

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

#define COT_ENTRY_SIZE    (uint32_t)4
#define COT_OBJECT        (uint16_t)0x1005

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/* type functions */
static uint32_t COTSyncIdSize (struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width);
static CO_ERR   COTSyncIdRead (struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTSyncIdWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTSyncIdInit (struct CO_OBJ_T *obj, struct CO_NODE_T *node);

/******************************************************************************
* PUBLIC GLOBALS
******************************************************************************/

const CO_OBJ_TYPE COTSyncId = { COTSyncIdSize, COTSyncIdInit, COTSyncIdRead, COTSyncIdWrite, 0 };

/******************************************************************************
* PRIVATE TYPE FUNCTIONS
******************************************************************************/

static uint32_t COTSyncIdSize(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    return uint32->Size(obj, node, width);
}

static CO_ERR COTSyncIdRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    return uint32->Read(obj, node, buffer, size);
}

static CO_ERR COTSyncIdWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    CO_ERR result = CO_ERR_NONE;
    CO_SYNC *sync;
    uint32_t nid;
    uint32_t oid;

    CO_UNUSED(size);

    sync = &node->Sync;
    nid = *(uint32_t*)buffer;
    (void)uint32->Read(obj, node, &oid, sizeof(oid));

    /* when current entry is generating SYNCs, bits 0 to 29 shall not be changed */
    if ((oid & CO_SYNC_COBID_ON) != (uint32_t)0) {
        if ((nid & CO_SYNC_COBID_MASK) != (oid & CO_SYNC_COBID_MASK)) {
            result = CO_ERR_OBJ_RANGE;
        } else {
            /* SYNC producer deactivation */
            if ((nid & CO_SYNC_COBID_ON) == 0) {
                COSyncProdDeactivate(sync);
            }
            sync->CobId = nid;
            result = uint32->Write(obj, node, &nid, sizeof(nid));
            if (result != CO_ERR_NONE) {
                result = CO_ERR_OBJ_RANGE;
            }
        }
    } else {
        /* SYNC producer activation */
        if (((nid & CO_SYNC_COBID_ON) != 0)) {
            sync->CobId = nid;
            COSyncProdActivate(sync);
            if (node->Error == CO_ERR_SYNC_RES) {
                /*
                 * Unable to start timer, return back
                 * the old COB-ID and report error
                 */
                sync->CobId = oid;
                result      = CO_ERR_OBJ_RANGE;
                return (result);
            }
        }
        sync->CobId = nid;
        result = uint32->Write(obj, node, &nid, sizeof(nid));
        if (result != CO_ERR_NONE) {
            result = CO_ERR_OBJ_RANGE;
        }
    }
    return (result);
}

static CO_ERR COTSyncIdInit(struct CO_OBJ_T *obj, struct CO_NODE_T *node)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    CO_ERR result = CO_ERR_TYPE_INIT;

    CO_UNUSED(node);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);

    /* check for emergency cob-id object */
    if (CO_DEV(COT_OBJECT, 0) == CO_GET_DEV(obj->Key)) {
        result = uint32->Read(obj, node, &node->Sync.CobId, 4);
        COSyncProdActivate(&node->Sync);
    }
    return (result);
}

/******************************************************************************
* PROTECTED HELPER FUNCTIONS
******************************************************************************/

WEAK_TEST
void COSyncProdActivate(CO_SYNC *sync)
{
    uint32_t ticks, time;
    CO_ERR   err;
    CO_NODE *node;
    int16_t  tid;

    node = sync->Node;

    if ((sync->CobId & CO_SYNC_COBID_ON) == 0) {
        /* 
         * SYNC producer is disabled, activation is skipped 
         * and system is accepted as is.
         */
        return;
    }

    err = CODictRdLong(&node->Dict, CO_DEV(0x1006, 0), &sync->Cycle);
    if (err != CO_ERR_NONE) {
        /*
         * In case of enabled SYNC producer, entry 1006h is mandatory
         * and invalid read operation results in configuration error.
         */
        node->Error = CO_ERR_CFG_1006_0;
        sync->Cycle = 0;
        return;
    }

    time = COTmrGetMinTime(&node->Tmr, CO_TMR_UNIT_100US);
    if ((time * 100) > sync->Cycle) {
        /* 
         * Provided timer driver has small resolution for configured 
         * value, it is not possible to handle SYNCs on requested 
         * communication cycle.
         * 
         * TODO: refactor SYNC producer to use highest possible 
         * resolution of COTmr API (which is currently 100 us).
         */
        node->Error = CO_ERR_SYNC_RES;
        return;
    }

    if (sync->Tmr >= 0) {
        tid = COTmrDelete(&node->Tmr, sync->Tmr);
        if (tid < 0) {
            node->Error = CO_ERR_TMR_DELETE;
        }
    }

    time = (sync->Cycle / 100);
    if (time > 0) {
        ticks = COTmrGetTicks(&node->Tmr, time, CO_TMR_UNIT_100US);
        sync->Tmr = COTmrCreate(&node->Tmr,
            ticks,
            ticks,
            COSyncProdSend,
            sync);
        if (sync->Tmr < 0) {
            node->Error = CO_ERR_TMR_CREATE;
        }
    } else {
        sync->Tmr = -1;
    }
}

WEAK_TEST
void COSyncProdDeactivate(CO_SYNC *sync)
{
    int16_t tid;

    if (sync->Tmr >= 0) {
        tid       = COTmrDelete(&sync->Node->Tmr, sync->Tmr);
        sync->Tmr = -1;
        if (tid < 0) {
            sync->Node->Error = CO_ERR_TMR_DELETE;
        }
    }
}
