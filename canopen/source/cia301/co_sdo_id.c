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

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/* type functions */
static uint32_t COTSdoIdSize (struct CO_OBJ_T *, struct CO_NODE_T *, uint32_t);
static CO_ERR   COTSdoIdRead (struct CO_OBJ_T *, struct CO_NODE_T *, void*, uint32_t);
static CO_ERR   COTSdoIdWrite(struct CO_OBJ_T *, struct CO_NODE_T *, void*, uint32_t);

/******************************************************************************
* PUBLIC GLOBALS
******************************************************************************/

const CO_OBJ_TYPE COTSdoId = { COTSdoIdSize, 0, COTSdoIdRead, COTSdoIdWrite, 0 };

/******************************************************************************
* PRIVATE TYPE FUNCTIONS
******************************************************************************/

static uint32_t COTSdoIdSize(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    return uint32->Size(obj, node, width);
}

static CO_ERR COTSdoIdRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *val, uint32_t len)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    return uint32->Read(obj, node, val, len);
}

static CO_ERR COTSdoIdWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *val, uint32_t size)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    CO_ERR    err = CO_ERR_NONE;
    uint32_t  newval;
    uint32_t  curval;
    uint8_t   num;

    UNUSED(node);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(val, CO_ERR_BAD_ARG);
    ASSERT_EQU_ERR(size, 4u, CO_ERR_BAD_ARG);

    newval = *(uint32_t *)val;
    (void)uint32->Read(obj, node, &curval, sizeof(curval));
    num = CO_GET_IDX(obj->Key) & 0x7F;

    if ((curval & CO_SDO_ID_OFF) == 0) {
        if ((newval & CO_SDO_ID_OFF) != 0) {
            err = uint32->Write(obj, node, &newval, sizeof(newval));
            if (err == CO_ERR_NONE) {
                COSdoReset(node->Sdo, num, node);
            }
        } else {
            return (CO_ERR_OBJ_RANGE);
        }
    } else {
        err = uint32->Write(obj, node, &newval, sizeof(newval));
    }
    if (err == CO_ERR_NONE) {
        COSdoEnable(node->Sdo, num);
    }

    return (err);
}
