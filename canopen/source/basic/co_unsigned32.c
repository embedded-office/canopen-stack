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

static uint32_t COTUInt32Size (struct CO_OBJ_T *, struct CO_NODE_T *, uint32_t);
static CO_ERR   COTUInt32Read (struct CO_OBJ_T *, struct CO_NODE_T *, void*, uint32_t);
static CO_ERR   COTUInt32Write(struct CO_OBJ_T *, struct CO_NODE_T *, void*, uint32_t);

/******************************************************************************
* PUBLIC GLOBALS
******************************************************************************/

const CO_OBJ_TYPE COTUInt32 = { COTUInt32Size, 0, COTUInt32Read, COTUInt32Write };

/******************************************************************************
* FUNCTIONS
******************************************************************************/

static uint32_t COTUInt32Size(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    uint32_t result = (uint32_t)0;

    UNUSED(node);
    UNUSED(width);

    if (CO_IS_DIRECT(obj->Key)) {
#if UINTPTR_MAX >= UINT32_MAX
        result = (uint32_t)COT_ENTRY_SIZE;
#endif
    } else {
        /* check for valid reference */
        if ((obj->Data) != (CO_DATA)0) {
            result = (uint32_t)COT_ENTRY_SIZE;
        }
    }
    return (result);
}

static CO_ERR COTUInt32Read(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *val, uint32_t len)
{
    CO_ERR  result = CO_ERR_NONE;
    uint32_t value;

    UNUSED(node);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(val, CO_ERR_BAD_ARG);

    if (CO_IS_DIRECT(obj->Key)) {
#if UINTPTR_MAX >= UINT32_MAX
        value = (uint32_t)(obj->Data);
#else
        /* Your pointer width is no suitable for storing 32bit values.
         * Consider allocating and referencing a variable.
         */
        result = CO_ERR_OBJ_RANGE;
#endif
    } else {
        value = *((uint32_t *)(obj->Data));
    }
    if (CO_IS_NODEID(obj->Key) != 0) {
        value += node->NodeId;
    }
    if (len == COT_ENTRY_SIZE) {
        *((uint32_t *)val) = value;
    } else {
        result = CO_ERR_BAD_ARG;
    }
    return (result);
}

static CO_ERR COTUInt32Write(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *val, uint32_t len)
{
    CO_ERR  result = CO_ERR_NONE;
    uint32_t value;
    uint32_t oldValue;

    UNUSED(node);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(val, CO_ERR_BAD_ARG);

    value = *((uint32_t *)val);
    if (len == COT_ENTRY_SIZE) {
        if (CO_IS_NODEID(obj->Key) != 0) {
            value -= node->NodeId;
        }
        if (CO_IS_DIRECT(obj->Key)) {
#if UINTPTR_MAX >= UINT32_MAX
            oldValue = obj->Data;
            obj->Data = (CO_DATA)(value);
#else
            /* Your pointer width is no suitable for storing 32bit values.
             * Consider allocating and referencing a variable.
             */
            result = CO_ERR_OBJ_RANGE;
#endif
        } else {
            oldValue = *((uint32_t *)(obj->Data));
            *((uint32_t *)(obj->Data)) = value;
        }
        if (CO_IS_PDO_ASYNC(obj->Key)) {
            if (value != oldValue) {
                COTPdoTrigObj(node->TPdo, obj);
            }
        }
    } else {
        result = CO_ERR_BAD_ARG;
    }
    return (result);
}