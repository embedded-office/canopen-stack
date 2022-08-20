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
#include "co_para_store.h"

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

#define COT_ENTRY_SIZE       (uint32_t)4
#define COT_OBJECT           (uint16_t)0x1010
#define CO_PARA_STORE_SIG    0x65766173     /*!< store parameter signature   */

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/* type functions */
static uint32_t COTParaStoreSize (struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width);
static CO_ERR   COTParaStoreRead (struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTParaStoreWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTParaStoreInit (struct CO_OBJ_T *obj, struct CO_NODE_T *node);
static CO_ERR   COTParaStoreReset(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t para);

/******************************************************************************
* PUBLIC GLOBALS
******************************************************************************/

const CO_OBJ_TYPE COTParaStore = { COTParaStoreSize, COTParaStoreInit, COTParaStoreRead, COTParaStoreWrite, COTParaStoreReset };

/******************************************************************************
* PRIVATE TYPE FUNCTIONS
******************************************************************************/

static uint32_t COTParaStoreSize(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    const CO_OBJ_TYPE *uint8 = CO_TUNSIGNED8;

    if (CO_GET_SUB(obj->Key) == 0) {
        return uint8->Size(obj, node, width);
    } else {
        return uint32->Size(obj, node, width);
    }
}

static CO_ERR COTParaStoreRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *uint8 = CO_TUNSIGNED8;
    CO_ERR   result;
    CO_PARA *pg;

    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(buffer, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(obj->Data, CO_ERR_OBJ_READ);

    if (CO_GET_SUB(obj->Key) == 0) {
        result = uint8->Read(obj, node, buffer, size);
    } else {
        pg = (CO_PARA *)(obj->Data);
        *(uint32_t *)buffer = pg->Value;
        result = CO_ERR_NONE;
    }
    return (result);
}

static CO_ERR COTParaStoreWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *uint8 = CO_TUNSIGNED8;
    CO_ERR    result = CO_ERR_TYPE_WR;
    CO_DICT  *cod;
    CO_OBJ   *pwo;
    CO_PARA  *pg;
    uint32_t  value;
    uint8_t   num;
    uint8_t   sub;

    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(buffer, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(obj->Data, CO_ERR_BAD_ARG);

    if (CO_GET_SUB(obj->Key) == 0) {
        result = uint8->Write(obj, node, buffer, size);
    } else {
        /* check store signature */
        value = *((uint32_t *)buffer);
        if (value != CO_PARA_STORE_SIG) {
            return (CO_ERR_TYPE_WR);
        }

        cod = &node->Dict;
        (void)CODictRdByte(cod, CO_DEV(COT_OBJECT, 0), &num);

        sub = CO_GET_SUB(obj->Key);
        if ((sub == 1) && (num > 1)) {

            /* store all parameter groups 2..N */
            for (sub = 2; sub <= num; sub++) {
                pwo = CODictFind(cod, CO_DEV(COT_OBJECT, sub));
                if (pwo != NULL) {
                    pg = (CO_PARA *)(pwo->Data);
                    result = COParaStore(pg, node);
                    if (result != CO_ERR_NONE) {
                        return (result);
                    }
                }
            }
        } else {

            /* store single parameter group */
            pg = (CO_PARA *)(obj->Data);
            result = COParaStore(pg, node);
        }
    }
    return (result);
}

static CO_ERR COTParaStoreInit (struct CO_OBJ_T *obj, struct CO_NODE_T *node)
{
    CO_ERR result = CO_ERR_TYPE_INIT;
    
    ASSERT_PTR_ERR(node, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);

    /* check for parameter store object */
    if (CO_GET_IDX(obj->Key) == COT_OBJECT) {

        /* check for number of parameter groups */
        if (CO_GET_SUB(obj->Key) == 0) {

            /* load all parameters from NVM storage */
            result = CONodeParaLoad(node, CO_RESET_NODE);
            if (result != CO_ERR_NONE) {
                return (result);
            }
            result = CONodeParaLoad(node, CO_RESET_COM);
        } else {

            /* check for max. optional parameter groups */
            if (CO_GET_SUB(obj->Key) > 0x7F) {
                result = CO_ERR_CFG_1010_0;
            } else {
                result = CO_ERR_NONE;
            }
        }
    }
    return (result);
}

static CO_ERR COTParaStoreReset(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t para)
{
    CO_ERR result = CO_ERR_TYPE_RESET;

    CO_UNUSED(node);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);

    /* we use reset on subindex 0 to load all parameter groups of a given type */
    if (CO_DEV(COT_OBJECT, 0) == CO_GET_DEV(obj->Key)) {
        result = CONodeParaLoad(node, para);
    }
    return (result);
}

/******************************************************************************
* PROTECTED API FUNCTIONS
******************************************************************************/

WEAK_TEST
CO_ERR CONodeParaLoad(struct CO_NODE_T *node, enum CO_NMT_RESET_T type)
{
    CO_ERR    result = CO_ERR_NONE;
    CO_ERR    err;
    CO_DICT  *cod;
    CO_OBJ   *obj;
    CO_PARA  *pg;
    uint32_t  bytes;
    uint8_t   num = 0;
    uint8_t   sub;

    cod = &node->Dict;
    err = CODictRdByte(cod, CO_DEV(COT_OBJECT, 0), &num);
    if (err != CO_ERR_NONE) {
        node->Error = CO_ERR_NONE;
        return (result);
    }

    /* loop through all parameter groups */
    for (sub = 1; sub <= num; sub++) {
        obj = CODictFind(cod, CO_DEV(COT_OBJECT, sub));
        if (obj != NULL) {

            /* check parameter group type */
            pg = (CO_PARA *)(obj->Data);
            if (pg->Type == type) {
                bytes = COIfNvmRead(&node->If, pg->Offset, pg->Start, pg->Size);
                if (bytes != pg->Size) {
                    node->Error = CO_ERR_IF_NVM_READ;
                    result      = CO_ERR_IF_NVM_READ;
                }
            }
        }
    }
    return (result);
}

/******************************************************************************
* PUBLIC API FUNCTIONS
******************************************************************************/

WEAK_TEST
CO_ERR COParaStore(struct CO_PARA_T *pg, struct CO_NODE_T *node)
{
    CO_ERR   result = CO_ERR_NONE;
    uint32_t bytes;

    ASSERT_PTR_ERR(pg, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(node, CO_ERR_BAD_ARG);

    /* call nvm write driver function */
    if ((pg->Value & CO_PARA___E) != 0) {
        bytes = COIfNvmWrite(&node->If, pg->Offset, pg->Start, pg->Size);
        if (bytes != pg->Size) {
            result = CO_ERR_IF_NVM_WRITE;
        }
    }
    return (result);
}
