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
#include "co_para_ctrl.h"

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

#define COT_ENTRY_SIZE      (uint32_t)4
#define COT_OBJECT_STORE    (uint16_t)0x1010
#define COT_OBJECT_RESTORE  (uint16_t)0x1011

#define CO_PARA_STORE_SIG    0x65766173     /*!< store parameter signature   */
#define CO_PARA_RESTORE_SIG  0x64616F6c     /*!< restore parameter signature */

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/* type functions */
static uint32_t COTParaCtrlSize (struct CO_OBJ_T *, struct CO_NODE_T *, uint32_t);
static CO_ERR   COTParaCtrlRead (struct CO_OBJ_T *, struct CO_NODE_T *, void*, uint32_t);
static CO_ERR   COTParaCtrlWrite(struct CO_OBJ_T *, struct CO_NODE_T *, void*, uint32_t);

/* helper functions */
static CO_ERR COParaCheck(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t size);

/******************************************************************************
* PUBLIC GLOBALS
******************************************************************************/

const CO_OBJ_TYPE COTParaCtrl = { COTParaCtrlSize, 0, COTParaCtrlRead, COTParaCtrlWrite };

/******************************************************************************
* PRIVATE TYPE FUNCTIONS
******************************************************************************/

static uint32_t COTParaCtrlSize(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    return uint32->Size(obj, node, width);
}

static CO_ERR COTParaCtrlRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *val, uint32_t len)
{
    CO_PARA *pg;

    UNUSED(node);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(val, CO_ERR_BAD_ARG);
    ASSERT_EQU_ERR(len, 4u, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(obj->Data, CO_ERR_OBJ_READ);

    pg = (CO_PARA *)(obj->Data);
    *(uint32_t *)val = pg->Value;

    return (CO_ERR_NONE);
}

static CO_ERR COTParaCtrlWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *val, uint32_t size)
{
    CO_ERR    select;
    CO_DICT  *cod;
    CO_OBJ   *pwo;
    CO_PARA  *pg;
    uint16_t  idx;
    uint8_t   num;
    uint8_t   sub;

    UNUSED(node);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(val, CO_ERR_BAD_ARG);
    ASSERT_EQU_ERR(size, 4u, CO_ERR_BAD_ARG);

    /* check parameter and configuration */
    select = COParaCheck(obj, node, val, size);
    if (select != CO_ERR_NONE) {
        return (select);
    }
    cod = &node->Dict;
    idx = CO_GET_IDX(obj->Key);
    sub = CO_GET_SUB(obj->Key);
    (void)CODictRdByte(cod, CO_DEV(idx,0), &num);

    /* all parameter groups */
    if ((sub == 1) && (num > 1)) {
        for (sub = 2; sub < num; sub++) {
            pwo = CODictFind(cod, CO_DEV(idx, sub));
            if (pwo != 0) {
                pg = (CO_PARA *)(pwo->Data);
                if (idx == COT_OBJECT_RESTORE) {
                    COParaRestore(pg, node);
                } else {
                    COParaStore(pg, node);
                }
            }
        }
    } else {
        /* save addressed para group */
        pg = (CO_PARA *)(obj->Data);
        if (idx == COT_OBJECT_RESTORE) {
            COParaRestore(pg, node);
        } else {
            COParaStore(pg, node);
        }
    }
    return (CO_ERR_NONE);
}

/******************************************************************************
* PRIVATE HELPER FUNCTIONS
******************************************************************************/

static CO_ERR COParaCheck(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t size)
{
    CO_ERR    result = CO_ERR_PARA_IDX;
    CO_ERR    err;
    uint32_t  signature;
    CO_DICT  *cod;
    uint8_t   num;
    uint8_t   sub;

    UNUSED(size);

    /* store parameter */
    cod = &node->Dict;
    if (CO_GET_IDX(obj->Key) == COT_OBJECT_STORE) {
        err = CODictRdByte(cod, CO_DEV(COT_OBJECT_STORE,0), &num);
        if (err != CO_ERR_NONE) {
            cod->Node->Error = CO_ERR_CFG_1010_0;
            return (err);
        }

        if (num > 0x7F) {
            return (CO_ERR_CFG_1010_0);
        }
        sub = CO_GET_SUB(obj->Key);
        if ((sub < 1) || (sub > num)) {
            return (CO_ERR_BAD_ARG);
        }
        signature = *((uint32_t *)buf);
        if (signature != CO_PARA_STORE_SIG) {
            return (CO_ERR_OBJ_ACC);
        }
        result = CO_ERR_NONE;
    }
    
    /* restore parameters */
    if (CO_GET_IDX(obj->Key) == COT_OBJECT_RESTORE) {
        err = CODictRdByte(cod, CO_DEV(COT_OBJECT_RESTORE,0), &num);
        if (err != CO_ERR_NONE) {
            cod->Node->Error = CO_ERR_CFG_1011_0;
            return (err);
        }
        if (num > 0x7F) {
            return (CO_ERR_CFG_1011_0);
        }
        sub = CO_GET_SUB(obj->Key);
        if ((sub < 1) || (sub > num)) {
            return (CO_ERR_BAD_ARG);
        }
        signature = *((uint32_t *)buf);
        if (signature != CO_PARA_RESTORE_SIG) {
            return (CO_ERR_OBJ_ACC);
        }
        result = CO_ERR_NONE;
    }

    /* bad parameter index */
    if (result != CO_ERR_NONE) {
        cod->Node->Error = CO_ERR_PARA_IDX;
    }
    return (result);
}

/******************************************************************************
* PROTECTED API FUNCTIONS
******************************************************************************/

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
    err = CODictRdByte(cod, CO_DEV(0x1010, 0), &num);
    if (err != CO_ERR_NONE) {
        node->Error = CO_ERR_NONE;
        return (result);
    }
    for (sub = 1; sub <= num; sub++) {
        obj = CODictFind(cod, CO_DEV(0x1010, sub));
        if (obj != 0) {
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

void COParaStore(struct CO_PARA_T *pg, struct CO_NODE_T *node)
{
    uint32_t bytes;

    ASSERT_PTR(pg);
    ASSERT_PTR(node);

    /* call nvm write driver function */
    if ((pg->Value & CO_PARA___E) != 0) {
        bytes = COIfNvmWrite(&node->If, pg->Offset, pg->Start, pg->Size);
        if (bytes != pg->Size) {
            node->Error = CO_ERR_IF_NVM_WRITE;
        }
    }
}

void COParaRestore(struct CO_PARA_T *pg, struct CO_NODE_T *node)
{
    int16_t err;

    ASSERT_PTR(pg);
    ASSERT_PTR(node);

    /* call default callback function */
    if ((pg->Value & CO_PARA___E) != 0) {
        err = COParaDefault(pg);
        if (err != CO_ERR_NONE) {
            node->Error = CO_ERR_PARA_RESTORE;
        }
    }
}
