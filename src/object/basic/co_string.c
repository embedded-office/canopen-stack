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
* PRIVATE FUNCTIONS
******************************************************************************/

static uint32_t COTStringSize (struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width);
static CO_ERR   COTStringRead (struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTStringInit (struct CO_OBJ_T *obj, struct CO_NODE_T *node);
static CO_ERR   COTStringReset(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t para);

/******************************************************************************
* PUBLIC GLOBALS
******************************************************************************/

const CO_OBJ_TYPE COTString = { COTStringSize, COTStringInit, COTStringRead, 0, COTStringReset };

/******************************************************************************
* FUNCTIONS
******************************************************************************/

static uint32_t COTStringSize(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    uint32_t    strlen = 0;
    CO_OBJ_STR *str;
    uint8_t    *ptr;

    CO_UNUSED(node);
    CO_UNUSED(width);
    ASSERT_PTR_ERR(obj->Data, 0);

    str = (CO_OBJ_STR *)(obj->Data);
    ptr = str->Start;
    while (*ptr != '\0') {
        strlen++;
        ptr++;
    }
    return (strlen);
}

static CO_ERR COTStringRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    uint32_t    offset;
    uint32_t    num;
    CO_OBJ_STR *str;
    uint8_t    *ptr;
    uint8_t    *dst;

    CO_UNUSED(node);
    ASSERT_PTR_ERR(obj->Data, CO_ERR_BAD_ARG);

    str    = (CO_OBJ_STR *)(obj->Data);
    offset = str->Offset;
    ptr    = (uint8_t *)(str->Start) + offset;
    dst    = (uint8_t *)buffer;
    num    = size;
    while ((*ptr != 0) && (num > 0)) {
        *dst = *ptr;
        offset++;
        ptr++;
        dst++;
        num--;
    }
    str->Offset = offset;
    return (CO_ERR_NONE);
}

static CO_ERR COTStringInit(struct CO_OBJ_T *obj, struct CO_NODE_T *node)
{
    CO_OBJ_STR *str;

    CO_UNUSED(node);
    ASSERT_PTR_ERR(obj->Data, CO_ERR_BAD_ARG);

    str = (CO_OBJ_STR *)(obj->Data);
    str->Offset = 0;
    return (CO_ERR_NONE);
}

static CO_ERR COTStringReset(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t para)
{
    CO_OBJ_STR *str;

    CO_UNUSED(node);
    ASSERT_PTR_ERR(obj->Data, CO_ERR_BAD_ARG);

    str = (CO_OBJ_STR *)(obj->Data);
    str->Offset = para;
    return (CO_ERR_NONE);
}
