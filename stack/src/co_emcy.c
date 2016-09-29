/******************************************************************************
* (c) by Embedded Office GmbH & Co. KG, <http://www.embedded-office.com/>
*------------------------------------------------------------------------------
* This file is part of CANopenStack, an open source CANopen Stack.
* Project home page is <https://github.com/MichaelHillmann/CANopenStack.git>.
* For more information on CANopen see <http://www.can-cia.org/>.
*
* CANopenStack is free and open source software: you can redistribute
* it and / or modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation, either version 2 of the
* License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
******************************************************************************/

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_core.h"
#include "co_emcy.h"

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

/******************************************************************************
* GLOBAL CONSTANTS
******************************************************************************/

/*! \brief OBJECT TYPE EMCY HISTORY
*
*    This type is responsible for the access to the EMCY history.
*/
const CO_OBJ_TYPE COTEmcy = { 0, 0, 0, 0, COTypeEmcyRead, COTypeEmcyWrite };

/******************************************************************************
* FUNCTIONS
******************************************************************************/

/*
* see function definition
*/
void COEmcySet(CO_EMCY *emcy, uint8_t err, CO_EMCY_USR *usr)
{
    int16_t chk;
    int16_t change;

    chk = COEmcyCheck(emcy);
    if (chk < 0) {
        return;
    }

    change = COEmcySetErr(emcy, err, 1);
    if (change > 0) {
        COEmcyUpdate(emcy, err, usr, 1);
        COEmcySend  (emcy, err, usr, 1);
    }
}

/*
* see function definition
*/
void COEmcyClr(CO_EMCY *emcy, uint8_t err)
{
    int16_t chk;
    int16_t change;

    chk = COEmcyCheck(emcy);
    if (chk < 0) {
        return;
    }

    change = COEmcySetErr(emcy, err, 0);
    if (change > 0) {
        COEmcyUpdate(emcy, err, 0, 0);
        COEmcySend  (emcy, err, 0, 0);
    }
}

/*
* see function definition
*/
int16_t COEmcyGet(CO_EMCY *emcy, uint8_t err)
{
    int16_t chk;
    int16_t cur;

    chk = COEmcyCheck(emcy);
    if (chk < 0) {
        return (-1);
    }

    cur = COEmcyGetErr(emcy, err);

    return (cur);
}

/*
* see function definition
*/
int16_t COEmcyCnt(CO_EMCY *emcy)
{
    int16_t chk;
    int16_t result = -1;
    uint8_t n;

    chk = COEmcyCheck(emcy);
    if (chk < 0) {
        return (result);
    }

    result = 0;
    for (n=0; n < CO_EMCY_REG_NUM; n++) {
        result += emcy->Cnt[n];
    }

    return (result);
}

/*
* see function definition
*/
void COEmcyReset(CO_EMCY *emcy, uint8_t silent)
{
    int16_t change;
    int16_t chk;
    uint8_t n;

    chk = COEmcyCheck(emcy);
    if (chk < 0) {
        return;
    }

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

/*
* see function definition
*/
void COEmcyHistReset(CO_EMCY *emcy)
{
    CO_NODE    *node;
    CO_DIR     *cod;
    CO_OBJ     *obj;
    uint32_t    val32;
    int16_t     chk;
    uint8_t     sub;
    uint8_t     val08;

    chk = COEmcyCheck(emcy);
    if (chk < 0) {
        return;
    }
    node = emcy->Node;
    cod  = &node->Dir;

    val08 = 0;
    obj = CODirFind(cod, CO_DEV(0x1003, 0));
    if (obj == 0) {
        node->Error = CO_ERR_NONE;
        return;
    }
    (void)COObjWrDirect(obj, &val08, 1);

    val32 = 0;
    for (sub = 1; sub <= emcy->Hist.Max; sub++) {
        obj = CODirFind(cod, CO_DEV(0x1003, sub));
        (void)COObjWrDirect(obj, &val32, 4);
    }

    emcy->Hist.Max = sub - 1;
    emcy->Hist.Off = 0;
    emcy->Hist.Num = 0;
}

/*
* see function definition
*/
void COEmcyInit(CO_EMCY *emcy, CO_NODE *node, CO_EMCY_TBL *root)
{
    CO_OBJ   *obj;
    uint16_t  n;
    uint32_t  size;

    if ((root == 0) || (node == 0) || (emcy == 0)) {
        CONodeFatalError();
        return;
    }
    emcy->Root = root;
    emcy->Node = node;

    for (n=0; n < CO_EMCY_STORAGE; n++) {
        emcy->Err[n] = 0;
    }
    for (n=0; n < CO_EMCY_REG_NUM; n++) {
        emcy->Cnt[n] = 0;
    }
    obj = CODirFind(&node->Dir, CO_DEV(0x1014,0));
    if (obj == 0) {
        node->Error = CO_ERR_CFG_1014_0;
    } else {
        size = COObjGetSize(obj, CO_LONG);
        if (size == 0) {
            node->Error = CO_ERR_CFG_1014_0;
        }
    }
    obj = CODirFind(&node->Dir, CO_DEV(0x1001,0));
    if (obj == 0) {
        node->Error = CO_ERR_CFG_1001_0;
    } else {
        size = COObjGetSize(obj, CO_BYTE);
        if (size == 0) {
            node->Error = CO_ERR_CFG_1001_0;
        }
    }

    COEmcyHistInit(emcy);
}

/*
* see function definition
*/
int16_t COEmcyCheck(CO_EMCY *emcy)
{
    CO_NODE *node;
    int16_t  result = -1;

    if (emcy == 0) {
        CONodeFatalError();
        return (result);
    }

    node = emcy->Node;
    if (node == 0) {
        CONodeFatalError();
        return (result);
    }
    if (emcy->Root == 0) {
        node->Error = CO_ERR_EMCY_BAD_ROOT;
        return (result);
    }
    result = 0;
    return (result);
}

/*
* see function definition
*/
int16_t COEmcyGetErr(CO_EMCY *emcy, uint8_t err)
{
    uint8_t result;
    uint8_t byte;
    uint8_t mask;

    if (err >= CO_EMCY_N) {
        err = CO_EMCY_N - 1;
    }

    byte = err >> 3;
    mask = (uint8_t)1 << (err & 0x7);
    if ((emcy->Err[byte] & mask) == 0) {
        result = 0;
    } else {
        result = 1;
    }

    return (result);
}

/*
* see function definition
*/
int16_t COEmcySetErr(CO_EMCY *emcy, uint8_t err, uint8_t state)
{
    uint8_t result;
    uint8_t byte;
    uint8_t mask;

    if (err >= CO_EMCY_N) {
        err = CO_EMCY_N - 1;
    }
    byte = err >> 3;
    mask = (uint8_t)1 << (err & 0x7);
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

/*
* see function definition
*/
void COEmcySend(CO_EMCY *emcy, uint8_t err, CO_EMCY_USR *usr, uint8_t state)
{
    CO_IF_FRM    frm; 
    CO_NODE     *node;
    CO_DIR      *dir;
    CO_EMCY_TBL *data;
    uint8_t      n;

    node = emcy->Node;
    if ((node->Nmt.Allowed & CO_EMCY_ALLOWED) == 0) {
       return;
    }
    if (err >= CO_EMCY_N) {
        err = CO_EMCY_N - 1;
    }
    dir  = &node->Dir;
    data = &emcy->Root[err];

    (void)CODirRdLong(dir, CO_DEV(0x1014,0),
                      &frm.Identifier);
    frm.DLC = 8;
    if (state == 1) {
        frm.Data[0] = (uint8_t)(data->Code);
        frm.Data[1] = (uint8_t)(data->Code >> 8);
    } else {
        frm.Data[0] = (uint8_t)0;
        frm.Data[1] = (uint8_t)0;
    }
    (void)CODirRdByte(dir, CO_DEV(0x1001,0), &frm.Data[2]);
    for (n=0; n<5; n++) {
        frm.Data[3+n] = 0;
    }
    if (usr != 0) {
        for (n=0; n<5; n++) {
            frm.Data[3+n] = usr->Emcy[n];
        }
    }
    (void)COIfSend(&emcy->Node->If, &frm);
}

/*
* see function definition
*/
void COEmcyUpdate(CO_EMCY *emcy, uint8_t err, CO_EMCY_USR *usr, uint8_t state)
{
    CO_DIR  *dir;
    uint8_t  regbit;
    uint8_t  regmask;
    uint8_t  reg;

    if (err >= CO_EMCY_N) {
        err = CO_EMCY_N - 1;
    }
    dir     = &emcy->Node->Dir;
    regbit  =  emcy->Root[err].Reg;
    regmask =  (uint8_t)(1u << regbit);

    (void)CODirRdByte(dir, CO_DEV(0x1001,0), &reg);

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
    (void)CODirWrByte(dir, CO_DEV(0x1001,0), reg);
}

/*
* see function definition
*/
void COEmcyHistInit(CO_EMCY *emcy)
{
    CO_NODE  *node;
    CO_DIR   *cod;
    CO_OBJ   *obj;
    uint8_t   sub;

    emcy->Hist.Max = 0;
    emcy->Hist.Num = 0;
    emcy->Hist.Off = 0;

    node = emcy->Node;
    cod  = &node->Dir;
    obj  = CODirFind(cod, CO_DEV(0x1003, 0));
    if (obj == 0) {
        node->Error = CO_ERR_NONE;
        return;
    }
    obj  = CODirFind(cod, CO_DEV(0x1003, 1));
    if (obj == 0) {
        node->Error = CO_ERR_CFG_1003_1;
        return;
    }
    sub = 2;
    while (obj != 0) {
        obj = CODirFind(cod, CO_DEV(0x1003, sub));
        if (obj != 0) {
            sub++;
        }
    }
    (void)CONodeGetErr(cod->Node);

    emcy->Hist.Max = sub - 1;
    emcy->Hist.Off = 0;
}

/*
* see function definition
*/
void COEmcyHistAdd(CO_EMCY *emcy, uint8_t err, CO_EMCY_USR *usr)
{
    CO_NODE *node;
    CO_DIR  *cod;
    CO_OBJ  *obj;
    uint32_t val = 0;
    uint8_t  sub;

    if (emcy->Hist.Max == 0) {
        return;
    }
    node = emcy->Node;
    cod  = &node->Dir;
    emcy->Hist.Off++;
    if (emcy->Hist.Off > emcy->Hist.Max) {
        emcy->Hist.Off = 1;
    }
    sub = emcy->Hist.Off;
    val = (uint32_t)emcy->Root[err].Code;
    if (usr != 0) {
        val |= (((uint32_t)usr->Hist) << 16);
    }
    obj = CODirFind(cod, CO_DEV(0x1003, sub));
    COObjWrDirect(obj, &val, 4);

    emcy->Hist.Num++;
    if (emcy->Hist.Num > emcy->Hist.Max) {
        emcy->Hist.Num = emcy->Hist.Max;
    } else {
        obj = CODirFind(cod, CO_DEV(0x1003, 0));
        COObjWrDirect(obj, &(emcy->Hist.Num), 1);
    }
}

/*
* see function definition
*/
int16_t COTypeEmcyRead(CO_OBJ *obj, void *buf, uint32_t len)
{
    CO_NODE *node;
    CO_DIR  *cod;
    CO_EMCY *emcy;
    int16_t  result = CO_ERR_NONE;
    uint8_t  sub;
    uint8_t  map;

    cod  = obj->Type->Dir;
    node = cod->Node;
    emcy = &node->Emcy;
    sub  = CO_GET_SUB(obj->Key);

    if (sub == 0) {
        result = COObjRdDirect(obj, buf, len);
    } else {
        if (sub <= emcy->Hist.Num) {
            if (sub <= emcy->Hist.Off) {
                map = emcy->Hist.Off - (sub - 1);
            } else {
                map = (emcy->Hist.Max - (sub - 1)) +
                       emcy->Hist.Off;
            }
            obj = CODirFind(cod, CO_DEV(0x1003, map));
            result = COObjRdDirect(obj, buf, len);
        }
    }

    return (result);
}

/*
* see function definition
*/
int16_t COTypeEmcyWrite(CO_OBJ *obj, void *buf, uint32_t len)
{
    CO_NODE *node;
    CO_DIR  *cod;
    CO_EMCY *emcy;
    int16_t  result = CO_ERR_TYPE_WR;
    uint8_t  val    = 0;
    uint8_t  sub;

    (void)len;
    cod  = obj->Type->Dir;
    node = cod->Node;
    emcy = &node->Emcy;
    sub  = CO_GET_SUB(obj->Key);

    if (sub == 0) {
        val = (uint8_t)(*(uint32_t*)buf);
        if (val == 0) {
            COEmcyHistReset(emcy);
            result = CO_ERR_NONE;
        }
    }

    return (result);
}
