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
#include "co_emcy.h"

/******************************************************************************
* PRIVATE HELPER FUNCTIONS
******************************************************************************/

static int16_t COEmcySetErr(CO_EMCY *emcy, uint8_t err, uint8_t state);
static int16_t COEmcyGetErr(CO_EMCY *emcy, uint8_t err);
static void    COEmcyUpdate(CO_EMCY *emcy, uint8_t err, CO_EMCY_USR *usr, uint8_t state);
static void    COEmcySend  (CO_EMCY *emcy, uint8_t err, CO_EMCY_USR *usr, uint8_t state);

/******************************************************************************
* PRIVATE HELPER FUNCTION
******************************************************************************/

static int16_t COEmcySetErr(CO_EMCY *emcy, uint8_t err, uint8_t state)
{
    uint8_t result;
    uint8_t byte;
    uint8_t mask;

    if (err >= CO_EMCY_N) {
        err = CO_EMCY_N - 1;
    }
    byte = err >> 3;
    mask = (uint8_t)(1u << (err & 0x7u));
    if ((emcy->Err[byte] & mask) == 0) {
        if (state != 0) {
            emcy->Err[byte] |= mask;
            result           = 1;
        } else {
            result           = 0;
        }
    } else {
        if (state == 0) {
            emcy->Err[byte] &= ~mask;
            result           = 1;
        } else {
            result           = 0;
        }
    }
    return (result);
}

static int16_t COEmcyGetErr(CO_EMCY *emcy, uint8_t err)
{
    uint8_t result;
    uint8_t byte;
    uint8_t mask;

    if (err >= CO_EMCY_N) {
        err = CO_EMCY_N - 1;
    }
    byte = err >> 3;
    mask = (uint8_t)(1u << (err & 0x7u));
    if ((emcy->Err[byte] & mask) == 0) {
        result = 0;
    } else {
        result = 1;
    }
    return (result);
}

static void COEmcyUpdate(CO_EMCY *emcy, uint8_t err, CO_EMCY_USR *usr, uint8_t state)
{
    CO_DICT  *dir;
    uint8_t  regbit;
    uint8_t  regmask;
    uint8_t  reg;

    ASSERT_PTR(emcy->Node);
    ASSERT_PTR(emcy->Root);

    if (err >= CO_EMCY_N) {
        err = CO_EMCY_N - 1;
    }
    dir     = &emcy->Node->Dict;
    regbit  =  emcy->Root[err].Reg;
    regmask =  (uint8_t)(1u << regbit);

    (void)CODictRdByte(dir, CO_DEV(0x1001,0), &reg);

    if (state != 0) { /* set error */
        if ((reg & regmask) == 0) {
            reg |= regmask;
            reg |= 0x01;
        }
        COEmcyHistAdd(emcy, err, usr);
        emcy->Cnt[regbit]++;
    } else { /* clear error */
        emcy->Cnt[regbit]--;
        if (emcy->Cnt[regbit] == 0) {
            reg &= ~regmask;
            if ((reg & ~0x01) != 0) {
                reg |= 0x01;
            } else {
                if (emcy->Cnt[0] == 0) {
                    reg &= ~0x01;
                }
            }
        }
    }
    (void)CODictWrByte(dir, CO_DEV(0x1001,0), reg);
}

static void COEmcySend(CO_EMCY *emcy, uint8_t err, CO_EMCY_USR *usr, uint8_t state)
{
    CO_IF_FRM    frm;
    CO_NODE     *node;
    CO_DICT      *dir;
    CO_EMCY_TBL *data;
    uint8_t      n;

    ASSERT_PTR(emcy->Node);
    ASSERT_PTR(emcy->Root);

    node = emcy->Node;
    if ((node->Nmt.Allowed & CO_EMCY_ALLOWED) == 0) {
       return;
    }
    if (err >= CO_EMCY_N) {
        err = CO_EMCY_N - 1;
    }
    dir  = &node->Dict;
    data = &emcy->Root[err];

    (void)CODictRdLong(dir, CO_DEV(0x1014,0), &frm.Identifier);
    frm.DLC = 8;
    if (state == 1) {
        frm.Data[0] = (uint8_t)(data->Code);
        frm.Data[1] = (uint8_t)(data->Code >> 8);
    } else {
        frm.Data[0] = (uint8_t)0;
        frm.Data[1] = (uint8_t)0;
    }
    (void)CODictRdByte(dir, CO_DEV(0x1001,0), &frm.Data[2]);
    for (n=0; n<5; n++) {
        frm.Data[3+n] = 0;
    }
    if (usr != 0) {
        for (n=0; n<5; n++) {
            frm.Data[3+n] = usr->Emcy[n];
        }
    }
    (void)COIfCanSend(&emcy->Node->If, &frm);
}

/******************************************************************************
* PROTECTED API FUNCTIONS
******************************************************************************/

void COEmcyInit(CO_EMCY *emcy, CO_NODE *node, CO_EMCY_TBL *root)
{
    const CO_OBJ_TYPE *hist = CO_TEMCY_HIST;
    CO_OBJ   *obj;
    uint16_t  n;
    uint32_t  size;

    ASSERT_PTR(emcy);
    ASSERT_PTR(node);

    /* initialize emcy structure */
    emcy->Root = root;
    emcy->Node = node;
    for (n=0; n < CO_EMCY_STORAGE; n++) {
        emcy->Err[n] = 0;
    }
    for (n=0; n < CO_EMCY_REG_NUM; n++) {
        emcy->Cnt[n] = 0;
    }

    /* error register is mandatory */
    obj = CODictFind(&node->Dict, CO_DEV(0x1001,0));
    if (obj == 0) {
        node->Error = CO_ERR_CFG_1001_0;
        return;
    } else {
        size = COObjGetSize(obj, node, 1u);
        if (size == 0) {
            node->Error = CO_ERR_CFG_1001_0;
            return;
        }
    }

    /* emergency cob-id is mandatory when an emergency table exists */
    if (root != 0) {
        obj = CODictFind(&node->Dict, CO_DEV(0x1014,0));
        if (obj == 0) {
            node->Error = CO_ERR_CFG_1014_0;
            return;
        } else {
            size = COObjGetSize(obj, node, 4u);
            if (size == 0) {
                node->Error = CO_ERR_CFG_1014_0;
                return;
            }
        }
    }

    obj = CODictFind(&node->Dict, CO_DEV(0x1003,0));
    hist->Init(obj, node);
}

/******************************************************************************
* PUBLIC API FUNCTIONS
******************************************************************************/

void COEmcySet(CO_EMCY *emcy, uint8_t err, CO_EMCY_USR *usr)
{
    int16_t change;

    ASSERT_PTR(emcy);

    change = COEmcySetErr(emcy, err, 1);
    if (change > 0) {
        COEmcyUpdate(emcy, err, usr, 1);
        COEmcySend  (emcy, err, usr, 1);
    }
}

void COEmcyClr(CO_EMCY *emcy, uint8_t err)
{
    int16_t change;

    ASSERT_PTR(emcy);

    change = COEmcySetErr(emcy, err, 0);
    if (change > 0) {
        COEmcyUpdate(emcy, err, 0, 0);
        COEmcySend  (emcy, err, 0, 0);
    }
}

int16_t COEmcyGet(CO_EMCY *emcy, uint8_t err)
{
    int16_t cur;

    ASSERT_PTR_ERR(emcy, -1);

    cur = COEmcyGetErr(emcy, err);
    return (cur);
}

int16_t COEmcyCnt(CO_EMCY *emcy)
{
    int16_t result = -1;
    uint8_t n;

    ASSERT_PTR_ERR(emcy, -1);

    result = 0;
    for (n=0; n < CO_EMCY_REG_NUM; n++) {
        result += emcy->Cnt[n];
    }
    return (result);
}

void COEmcyReset(CO_EMCY *emcy, uint8_t silent)
{
    int16_t change;
    uint8_t n;

    ASSERT_PTR(emcy);

    for (n=0; n < CO_EMCY_N; n++) {
        if (silent == 0) {
            COEmcyClr(emcy, n);
        } else {
            change = COEmcySetErr(emcy, n, 0);
            if (change > 0) {
                COEmcyUpdate(emcy, n, 0, 0);
            }
        }
    }
}
