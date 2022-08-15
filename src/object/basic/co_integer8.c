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

#define COT_ENTRY_SIZE    (uint32_t)1

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static uint32_t COTInt8Size (struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width);
static CO_ERR   COTInt8Read (struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTInt8Write(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);

/******************************************************************************
* PUBLIC GLOBALS
******************************************************************************/

const CO_OBJ_TYPE COTInt8 = { COTInt8Size, 0, COTInt8Read, COTInt8Write, 0 };

/******************************************************************************
* FUNCTIONS
******************************************************************************/

static uint32_t COTInt8Size(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    uint32_t result = (uint32_t)0;

    CO_UNUSED(node);
    CO_UNUSED(width);

    if (CO_IS_DIRECT(obj->Key) != 0) {
        result = (uint32_t)COT_ENTRY_SIZE;
    } else {
        /* check for valid reference */
        if ((obj->Data) != (CO_DATA)0) {
            result = (uint32_t)COT_ENTRY_SIZE;
        }
    }
    return (result);
}

static CO_ERR COTInt8Read(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    CO_ERR  result = CO_ERR_NONE;
    uint8_t value;

    CO_UNUSED(node);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(buffer, CO_ERR_BAD_ARG);

    if (CO_IS_DIRECT(obj->Key) != 0) {
        value = (uint8_t)(obj->Data);
    } else {
        value = *((uint8_t *)(obj->Data));
    }
    if (CO_IS_NODEID(obj->Key) != 0) {
        value += node->NodeId;
    }
    if (size == COT_ENTRY_SIZE) {
        *((uint8_t *)buffer) = value;
    } else {
        result = CO_ERR_BAD_ARG;
    }
    return (result);
}

static CO_ERR COTInt8Write(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    CO_ERR  result = CO_ERR_NONE;
    uint8_t value;
    uint8_t oldValue;

    CO_UNUSED(node);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(buffer, CO_ERR_BAD_ARG);

    value = *((uint8_t *)buffer);
    if (size == COT_ENTRY_SIZE) {
        if (CO_IS_NODEID(obj->Key) != 0) {
            value -= node->NodeId;
        }
        if (CO_IS_DIRECT(obj->Key) != 0) {
            oldValue = (uint8_t)obj->Data;
            obj->Data = (CO_DATA)(value);
        } else {
            oldValue = *((uint8_t *)(obj->Data));
            *((uint8_t *)(obj->Data)) = value;
        }
        if ((CO_IS_ASYNC(obj->Key)  != 0    ) &&
            (CO_IS_PDOMAP(obj->Key) != 0    ) &&
            (oldValue               != value)) {
            COTPdoTrigObj(node->TPdo, obj);
        }
    } else {
        result = CO_ERR_BAD_ARG;
    }
    return (result);
}
