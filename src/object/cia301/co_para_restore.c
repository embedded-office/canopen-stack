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
#include "co_para_restore.h"

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

#define COT_ENTRY_SIZE       (uint32_t)4
#define COT_OBJECT           (uint16_t)0x1011
#define CO_PARA_RESTORE_SIG  0x64616F6c     /*!< restore parameter signature */

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/* type functions */
static uint32_t COTParaRestoreSize (struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width);
static CO_ERR   COTParaRestoreRead (struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTParaRestoreWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTParaRestoreInit (struct CO_OBJ_T *obj, struct CO_NODE_T *node);

/******************************************************************************
* PUBLIC GLOBALS
******************************************************************************/

const CO_OBJ_TYPE COTParaRestore = { COTParaRestoreSize, COTParaRestoreInit, COTParaRestoreRead, COTParaRestoreWrite, 0 };

/******************************************************************************
* PRIVATE TYPE FUNCTIONS
******************************************************************************/

static uint32_t COTParaRestoreSize(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    const CO_OBJ_TYPE *uint8 = CO_TUNSIGNED8;

    if (CO_GET_SUB(obj->Key) == 0) {
        return uint8->Size(obj, node, width);
    } else {
        return uint32->Size(obj, node, width);
    }
}

static CO_ERR COTParaRestoreRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
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
        if (pg->Default != NULL) {
            *(uint32_t *)buffer = (uint32_t)1;
        } else {
            *(uint32_t *)buffer = (uint32_t)0;
        }
        result = CO_ERR_NONE;        
    }
    return (result);
}

static CO_ERR COTParaRestoreWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
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
        /* check restore signature */
        value = *((uint32_t *)buffer);
        if (value != CO_PARA_RESTORE_SIG) {
            return (CO_ERR_TYPE_WR);
        }

        cod = &node->Dict;
        (void)CODictRdByte(cod, CO_DEV(COT_OBJECT, 0), &num);

        sub = CO_GET_SUB(obj->Key);
        if ((sub == 1) && (num > 1)) {

            /* restore all parameter groups 2..N */
            for (sub = 2; sub <= num; sub++) {
                pwo = CODictFind(cod, CO_DEV(COT_OBJECT, sub));
                if (pwo != NULL) {
                    pg = (CO_PARA *)(pwo->Data);
                    result = COParaRestore(pg, node);
                    if (result != CO_ERR_NONE) {
                        return (result);
                    }
                }
            }
        } else {

            /* restore single parameter group */
            pg = (CO_PARA *)(obj->Data);
            result = COParaRestore(pg, node);
        }
    }
    return (result);
}

static CO_ERR COTParaRestoreInit (struct CO_OBJ_T *obj, struct CO_NODE_T *node)
{
    CO_ERR result = CO_ERR_TYPE_INIT;
    
    CO_UNUSED(node);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);

    /* check for parameter restore object */
    if (CO_GET_IDX(obj->Key) == COT_OBJECT) {

        /* check for max. optional parameter groups */
        if (CO_GET_SUB(obj->Key) > 0x7F) {
            result = CO_ERR_CFG_1011_0;
        } else {
            result = CO_ERR_NONE;
        }
    }
    return (result);
}

/******************************************************************************
* PUBLIC API FUNCTIONS
******************************************************************************/

WEAK_TEST
CO_ERR COParaRestore(struct CO_PARA_T *pg, struct CO_NODE_T *node)
{
    CO_ERR  result = CO_ERR_NONE;
    int16_t err;

    ASSERT_PTR_ERR(pg, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(node, CO_ERR_BAD_ARG);

    /* call default callback function */
    if ((pg->Value & CO_PARA___E) != 0) {
        err = COParaDefault(pg);
        if (err != CO_ERR_NONE) {
            result = CO_ERR_PARA_RESTORE;
        }
    }
    return (result);
}
