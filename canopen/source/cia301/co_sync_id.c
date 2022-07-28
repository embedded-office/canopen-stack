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

    UNUSED(size);

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
    CO_ERR result = CO_ERR_TYPE_INIT;

    UNUSED(node);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);

    /* check for emergency cob-id object */
    if (CO_DEV(COT_OBJECT, 0) == CO_GET_DEV(obj->Key)) {
        result = CO_ERR_NONE;
    }
    return (result);
}
