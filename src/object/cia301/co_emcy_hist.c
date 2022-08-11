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
#define COT_OBJECT        (uint16_t)0x1003

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/* type functions */
static uint32_t COTEmcyHistSize (struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width);
static CO_ERR   COTEmcyHistRead (struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTEmcyHistWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTEmcyHistInit (struct CO_OBJ_T *obj, struct CO_NODE_T *node);

/******************************************************************************
* PUBLIC GLOBALS
******************************************************************************/

const CO_OBJ_TYPE COTEmcyHist = { COTEmcyHistSize, COTEmcyHistInit, COTEmcyHistRead, COTEmcyHistWrite, 0 };

/******************************************************************************
* PRIVATE TYPE FUNCTIONS
******************************************************************************/

static uint32_t COTEmcyHistSize(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    const CO_OBJ_TYPE *uint8 = CO_TUNSIGNED8;

    if (CO_GET_SUB(obj->Key) == 0) {
        return uint8->Size(obj, node, width);
    } else {
        return uint32->Size(obj, node, width);
    }
}

static CO_ERR COTEmcyHistRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    const CO_OBJ_TYPE *uint8 = CO_TUNSIGNED8;
    CO_ERR   result = CO_ERR_TYPE_RD;
    CO_OBJ  *subObj;
    CO_DICT *cod;
    CO_EMCY *emcy;
    uint8_t  sub;
    uint8_t  map;

    cod  = &node->Dict;
    emcy = &node->Emcy;
    sub  = CO_GET_SUB(obj->Key);

    if (sub == 0) {
        result = uint8->Read(obj, node, buffer, size);
    } else {
        if (sub <= emcy->Hist.Num) {
            /* re-map entries to get newest at start of array */
            if (sub <= emcy->Hist.Off) {
                map = emcy->Hist.Off - (sub - 1);
            } else {
                map = (emcy->Hist.Max - (sub - 1)) +
                       emcy->Hist.Off;
            }

            /* get object entry and stored read value */
            subObj = CODictFind(cod, CO_DEV(COT_OBJECT, map));
            result = uint32->Read(subObj, node, buffer, COT_ENTRY_SIZE);
        } else {
            if (sub < emcy->Hist.Max) {
                *((uint32_t *)buffer) = (uint32_t)0;
            }
        }
    }
    return (result);
}

static CO_ERR COTEmcyHistWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *val, uint32_t size)
{
    CO_ERR   result = CO_ERR_TYPE_WR;
    CO_EMCY *emcy;
    uint8_t  value;

    CO_UNUSED(size);

    /* writing is allowed to subindex 0 only */
    if (CO_GET_SUB(obj->Key) == 0) {
        emcy = &node->Emcy;

        /* only value 0 is allowed to reset history */
        value = (uint8_t)(*(uint8_t *)val);
        if (value == 0) {
            COEmcyHistReset(emcy);
            result = CO_ERR_NONE;
        } else {
            result = CO_ERR_OBJ_RANGE;
        }
    }
    return (result);
}

static CO_ERR COTEmcyHistInit (struct CO_OBJ_T *obj, struct CO_NODE_T *node)
{
    CO_ERR   result = CO_ERR_TYPE_INIT;
    CO_EMCY *emcy;
    CO_OBJ  *subobj;
    CO_DICT *cod;
    uint8_t  sub;

    ASSERT_PTR_ERR(obj,  CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(node, CO_ERR_BAD_ARG);

    /* check for emergency history object */
    if (CO_GET_IDX(obj->Key) == COT_OBJECT) {
        if (CO_GET_SUB(obj->Key) == 0u) {
            emcy = &node->Emcy;

            emcy->Hist.Max = 0;
            emcy->Hist.Num = 0;
            emcy->Hist.Off = 0;

            /* scan through all existing array entries */
            cod = &node->Dict;
            sub = 0;
            do {
                subobj = CODictFind(cod, CO_DEV(COT_OBJECT, sub + 1));
                if (subobj != NULL) {
                    sub++;
                }
            } while (subobj != NULL);

            /* check minimum length of emergency history */
            if (sub < 1) {
                return (result);
            }

            emcy->Hist.Max = sub;
            emcy->Hist.Off = 0;
        }
        result = CO_ERR_NONE;
    }
    return (result);
}

/******************************************************************************
* PROTECTED API FUNCTIONS
******************************************************************************/

void COEmcyHistAdd(CO_EMCY *emcy, uint8_t err, CO_EMCY_USR *usr)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    const CO_OBJ_TYPE *uint8 = CO_TUNSIGNED8;
    CO_NODE *node;
    CO_DICT *cod;
    CO_OBJ  *obj;
    uint32_t val = 0;
    uint8_t  sub;

    /* check if history is active */
    if (emcy->Hist.Max == 0) {
        return;
    }

    /* calculate next position in array */
    node = emcy->Node;
    cod  = &node->Dict;
    emcy->Hist.Off++;
    if (emcy->Hist.Off > emcy->Hist.Max) {
        emcy->Hist.Off = 1;
    }
    sub = emcy->Hist.Off;

    /* write generated value to history */
    val = (uint32_t)emcy->Root[err].Code;
    if (usr != NULL) {
        val |= (((uint32_t)usr->Hist) << 16);
    }
    obj = CODictFind(cod, CO_DEV(COT_OBJECT, sub));
    (void)uint32->Write(obj, node, &val, sizeof(val));

    /* update number of stored entries in history */
    emcy->Hist.Num++;
    if (emcy->Hist.Num > emcy->Hist.Max) {
        emcy->Hist.Num = emcy->Hist.Max;
    } else {
        obj = CODictFind(cod, CO_DEV(COT_OBJECT, 0));
        (void)uint8->Write(obj, node, &(emcy->Hist.Num), sizeof(emcy->Hist.Num));
    }
}

/******************************************************************************
* PUBLIC API FUNCTIONS
******************************************************************************/

WEAK_TEST
void COEmcyHistReset(CO_EMCY *emcy)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    const CO_OBJ_TYPE *uint8 = CO_TUNSIGNED8;
    CO_NODE  *node;
    CO_DICT  *cod;
    CO_OBJ   *obj;
    uint32_t  val32 = 0;
    uint8_t   val08 = 0;
    uint8_t   sub;

    ASSERT_PTR(emcy);
    ASSERT_PTR(emcy->Node);

    node = emcy->Node;
    cod  = &node->Dict;

    /* clear number of emergencies in history */
    obj = CODictFind(cod, CO_DEV(COT_OBJECT, 0));
    if (obj == NULL) {
        node->Error = CO_ERR_NONE;
        return;
    }
    (void)uint8->Write(obj, node, &val08, sizeof(val08));

    /* clear all emergency entries in history */
    for (sub = 1; sub <= emcy->Hist.Max; sub++) {
        obj = CODictFind(cod, CO_DEV(COT_OBJECT, sub));
        (void)uint32->Write(obj, node, &val32, sizeof(val32));
    }

    /* update history state */
    emcy->Hist.Off = 0;
    emcy->Hist.Num = 0;
}
