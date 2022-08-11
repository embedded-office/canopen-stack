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

#define COT_ENTRY_SIZE  (uint32_t)4
#define COT_OBJECT      (uint32_t)0x1200
#define COT_OBJECT_NUM  (uint32_t)0xFF   /* 00h..7fh server; 80h..ffh client */

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/* type functions */
static uint32_t COTSdoIdSize (struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width);
static CO_ERR   COTSdoIdRead (struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTSdoIdWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTSdoIdInit (struct CO_OBJ_T *obj, struct CO_NODE_T *node);

/******************************************************************************
* PUBLIC GLOBALS
******************************************************************************/

const CO_OBJ_TYPE COTSdoId = { COTSdoIdSize, COTSdoIdInit, COTSdoIdRead, COTSdoIdWrite, 0 };

/******************************************************************************
* PRIVATE TYPE FUNCTIONS
******************************************************************************/

static uint32_t COTSdoIdSize(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    return uint32->Size(obj, node, width);
}

static CO_ERR COTSdoIdRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    return uint32->Read(obj, node, buffer, size);
}

static CO_ERR COTSdoIdWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    CO_ERR    err = CO_ERR_NONE;
    uint32_t  newval;
    uint32_t  curval;
    uint8_t   num;

    CO_UNUSED(node);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(buffer, CO_ERR_BAD_ARG);
    ASSERT_EQU_ERR(size, 4u, CO_ERR_BAD_ARG);

    newval = *(uint32_t *)buffer;
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

static CO_ERR COTSdoIdInit(struct CO_OBJ_T *obj, struct CO_NODE_T *node)
{
    CO_ERR result = CO_ERR_TYPE_INIT;

    CO_UNUSED(node);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);

    if ((CO_GET_IDX(obj->Key) >= COT_OBJECT) &&
        (CO_GET_IDX(obj->Key) <= COT_OBJECT + COT_OBJECT_NUM)) {
        if ((CO_GET_SUB(obj->Key) == 1) ||
            (CO_GET_SUB(obj->Key) == 2)) {
            result = CO_ERR_NONE;
        }
    }
    return (result);
}
