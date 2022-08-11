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
#define COT_OBJECT        (uint16_t)0x1006

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/* type functions */
static uint32_t COTSyncCycleSize (struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width);
static CO_ERR   COTSyncCycleRead (struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTSyncCycleWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTSyncCycleInit (struct CO_OBJ_T *obj, struct CO_NODE_T *node);

/******************************************************************************
* PUBLIC GLOBALS
******************************************************************************/

const CO_OBJ_TYPE COTSyncCycle = { COTSyncCycleSize, COTSyncCycleInit, COTSyncCycleRead, COTSyncCycleWrite, 0 };

/******************************************************************************
* PRIVATE TYPE FUNCTIONS
******************************************************************************/

static uint32_t COTSyncCycleSize(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    return uint32->Size(obj, node, width);
}

static CO_ERR COTSyncCycleRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    return uint32->Read(obj, node, buffer, size);
}

static CO_ERR COTSyncCycleWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    CO_ERR   result;
    CO_SYNC *sync;
    uint32_t nus, ous;

    CO_UNUSED(size);

    result  = CO_ERR_NONE;
    sync    = &node->Sync;
    nus     = *(uint32_t *)buffer;
    ous     = 0;

    /*
     * Fetch old settings (will be restored in
     * case producer reactivation went wrong
     */
    (void)uint32->Read(obj, node, &ous, sizeof(ous));

    result = uint32->Write(obj, node, &nus, sizeof(nus));
    if (result != CO_ERR_NONE) {
        return CO_ERR_OBJ_RANGE;
    }

    /* Reactivate sync producer with new cycle value */
    if ((sync->CobId & CO_SYNC_COBID_ON) != 0) {
        COSyncProdActivate(sync);
        if (node->Error == CO_ERR_SYNC_RES) {
            /*
             * Restore old SYNC cycle value because used timer has
             * resolution that is not able to produce SYNCs with new
             * cycle.
             *
             * Object write access was successful once already,
             * no result check is needed.
             */
            (void)uint32->Write(obj, node, &ous, sizeof(ous));
            sync->Cycle = ous;
            result      = CO_ERR_OBJ_RANGE;
        }
    }
    return (result);
}

static CO_ERR COTSyncCycleInit(struct CO_OBJ_T *obj, struct CO_NODE_T *node)
{
    CO_ERR result = CO_ERR_TYPE_INIT;

    CO_UNUSED(node);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);

    /* check for sync producer object */
    if (CO_DEV(COT_OBJECT, 0) == CO_GET_DEV(obj->Key)) {
        result = CO_ERR_NONE;
    }
    return (result);
}
