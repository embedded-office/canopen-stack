
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
#if USE_PARAMS

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

#define CO_PARA_STORE_SIG    0x65766173     /*!< store parameter signature   */
#define CO_PARA_RESTORE_SIG  0x64616F6c     /*!< restore parameter signature */

/******************************************************************************
* GLOBAL CONSTANTS
******************************************************************************/

const CO_OBJ_TYPE COTPara = { 0, 0, COTypeParaRead, COTypeParaWrite };

/******************************************************************************
* FUNCTIONS
******************************************************************************/

/*
* see function definition
*/
void COParaStore(CO_PARA *pg, CO_NODE *node)
{
    uint32_t bytes;

    /* argument chekcs */
    if ((pg == 0) || (node == 0)) {
        return;
    }
    /* call nvm write driver function */
    if ((pg->Value & CO_PARA___E) != 0) {
        bytes = COIfNvmWrite(&node->If, pg->Offset, pg->Start, pg->Size);
        if (bytes != pg->Size) {
            node->Error = CO_ERR_IF_NVM_WRITE;
        }
    }
}

/*
* see function definition
*/
void COParaRestore(CO_PARA *pg, CO_NODE *node)
{
    int16_t err;

    /* argument checks */
    if ((pg == 0) || (node == 0)) {
        return;
    }

    /* call default callback function */
    if ((pg->Value & CO_PARA___E) != 0) {
        err = COParaDefault(pg);
        if (err != CO_ERR_NONE) {
            node->Error = CO_ERR_PARA_RESTORE;
        }
    }
}

/*
* see function definition
*/
CO_ERR COParaCheck(CO_OBJ* obj, struct CO_NODE_T *node, void *buf, uint32_t size)
{
    CO_ERR    result = CO_ERR_PARA_IDX;
    CO_ERR    err;
    uint32_t  signature;
    CO_DICT  *cod;
    uint8_t   num;
    uint8_t   sub;

    /* argument checks */
    if ((obj == 0) || (buf == 0) || (size != 4)) {
        return (CO_ERR_BAD_ARG);
    }
    
    cod = &node->Dict;

    /* store parameter */
    if (CO_GET_IDX(obj->Key) == 0x1010) {
        err = CODictRdByte(cod, CO_DEV(0x1010,0), &num);
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
    if (CO_GET_IDX(obj->Key) == 0x1011) {
        err = CODictRdByte(cod, CO_DEV(0x1011,0), &num);
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

/*
* see function definition
*/
CO_ERR COTypeParaRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t size)
{
    CO_PARA *pg;

    (void)node;
    /* argument checks */
    if ((obj == 0) || (buf == 0)) {
        return (CO_ERR_BAD_ARG);
    }
    if (size != CO_LONG) {
        return (CO_ERR_BAD_ARG);
    }
    pg = (CO_PARA *)obj->Data;
    if (pg == 0) {
        return (CO_ERR_OBJ_READ);
    }

    /* read parameter */
    *(uint32_t *)buf = pg->Value;

    return (CO_ERR_NONE);
}

/*
* see function definition
*/
CO_ERR COTypeParaWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t size)
{
    CO_ERR    select;
    CO_DICT  *cod;
    CO_OBJ   *pwo;
    CO_PARA  *pg;
    uint16_t  idx;
    uint8_t   num;
    uint8_t   sub;
    
    /* check parameter and configuration */
    select = COParaCheck(obj, node, buf, size);
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
                pg = (CO_PARA *)pwo->Data;
                if (idx == 0x1011) {
                    COParaRestore(pg, node);
                } else {
                    COParaStore(pg, node);
                }
            }
        }
    } else {
        /* save addressed para group */
        pg = (CO_PARA *)obj->Data;
        if (idx == 0x1011) {
            COParaRestore(pg, node);
        } else {
            COParaStore(pg, node);
        }
    }
    return (CO_ERR_NONE);
}
#endif //USE_PARAMS
