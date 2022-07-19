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

static uint32_t COTDomainSize (struct CO_OBJ_T *, struct CO_NODE_T *, uint32_t);
static CO_ERR   COTDomainRead (struct CO_OBJ_T *, struct CO_NODE_T *, void*, uint32_t);
static CO_ERR   COTDomainWrite(struct CO_OBJ_T *, struct CO_NODE_T *, void*, uint32_t);
static CO_ERR   COTDomainInit (struct CO_OBJ_T *obj, struct CO_NODE_T *node);

/******************************************************************************
* PUBLIC GLOBALS
******************************************************************************/

const CO_OBJ_TYPE COTDomain = { COTDomainSize, COTDomainInit, COTDomainRead, COTDomainWrite };

/******************************************************************************
* FUNCTIONS
******************************************************************************/

static uint32_t COTDomainSize(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    uint32_t result = 0;
    CO_OBJ_DOM *dom;

    UNUSED(node);
    ASSERT_PTR_ERR(obj->Data, 0);

    dom = (CO_OBJ_DOM *)(obj->Data);
    if ((width > 0) && (width < dom->Size)) {
        result = width;
    } else {
        result = dom->Size;
    }
    return (result);
}

static CO_ERR COTDomainRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t len)
{
    CO_OBJ_DOM *dom;
    uint8_t    *src;
    uint8_t    *dst;
    uint32_t    num;

    UNUSED(node);
    ASSERT_PTR_ERR(obj->Data, CO_ERR_BAD_ARG);

    dom = (CO_OBJ_DOM *)(obj->Data);
    src = (uint8_t *)(dom->Start + dom->Offset);
    num = dom->Size - dom->Offset;
    dst = (uint8_t *)buf;
    while ((len > 0) && (num > 0)) {
        *dst = *src;
        src++;
        dst++;
        len--;
        dom->Offset++;
        num--;
    }
    return (CO_ERR_NONE);
}

static CO_ERR COTDomainWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t len)
{
    CO_OBJ_DOM *dom;
    uint8_t    *src;
    uint8_t    *dst;
    uint32_t    num;

    UNUSED(node);
    ASSERT_PTR_ERR(obj->Data, CO_ERR_BAD_ARG);

    dom = (CO_OBJ_DOM *)(obj->Data);
    dst = (uint8_t *)(dom->Start + dom->Offset);
    num = dom->Size - dom->Offset;
    src = (uint8_t *)buf;
    while ((len > 0) && (num > 0)) {
        *dst = *src;
        src++;
        dst++;
        len--;
        dom->Offset++;
        num--;
    }
    return (CO_ERR_NONE);
}

static CO_ERR COTDomainInit(struct CO_OBJ_T *obj, struct CO_NODE_T *node)
{
    CO_OBJ_DOM *dom;

    UNUSED(node);
    ASSERT_PTR_ERR(obj->Data, CO_ERR_BAD_ARG);

    dom = (CO_OBJ_DOM *)(obj->Data);
    dom->Offset = 0;
    return (CO_ERR_NONE);
}