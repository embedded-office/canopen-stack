/******************************************************************************
* (c) by Embedded Office GmbH & Co. KG, http://www.embedded-office.com
*------------------------------------------------------------------------------
* This file is part of CANopenStack, an open source CANopen Stack.
* Project home page is <https://github.com/MichaelHillmann/CANopenStack.git>.
* For more information on CANopen see < http ://www.can-cia.org/>.
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

/******************************************************************************
* EXTERNAL FUNCTIONS
******************************************************************************/

void COTPdoTransmit(CO_IF_FRM *frm); 
int16_t CORPdoReceive(CO_IF_FRM *frm);

/******************************************************************************
* GLOBAL CONSTANTS
******************************************************************************/

/*! \brief OBJECT TYPE: ASYNCHRONOUS TRANSMIT PDO OBJECT
*
*    This object type specializes the general handling of objects for
*    object directory entries, which triggers a TPDO event on changed values.
*/
const CO_OBJ_TYPE COTAsync = { 0, 0, 0, COTypeAsyncCtrl, 0, 0 };

/*! \brief OBJECT TYPE: TRANSMIT PDO EVENT TIMER
*
*    This object type specializes the general handling of the event timer
*    object within the TPDO communication profile.
*/
const CO_OBJ_TYPE COTEvent = { 0, 0, 0, 0, 0, COTypeEventWrite };

/*! \brief OBJECT TYPE: NUMBER OF PDO MAPPINGS
*
*    This type is responsible for the access to the number of PDO mapping
*    entries. Due to the special handling of PDO mapping change accesses, only
*    the write access needs to be handled via the type structure; the read
*    operation can be performed directly.
*/
const CO_OBJ_TYPE COTPdoMapN = { 0, 0, 0, 0, 0, COTypePdoMapNumWrite };

/*! \brief OBJECT TYPE: PDO MAPPING ENTRY
*
*    This type is responsible for the access to the PDO mapping entries. Due
*    to the special handling of PDO mapping change accesses, only the write
*    access needs to be handled via the type structure; the read operation
*    can be performed directly.
*/
const CO_OBJ_TYPE COTPdoMap = { 0, 0, 0, 0, 0, COTypePdoMapWrite };

/*! \brief OBJECT TYPE: PDO IDENTIFIER
*
*    This type is responsible for the access to the PDO communication
*    entries. Due to the special handling of PDO communication change
*    accesses, only the write access needs to be handled via the type
*    structure; the read operation can be performed directly.
*/
const CO_OBJ_TYPE COTPdoId = { 0, 0, 0, 0, 0, COTypePdoComIdWrite };

/*! \brief OBJECT TYPE: PDO TRANSMISSION TYPE
*
*    This type is responsible for the access to the PDO communication
*    entries. Due to the special handling of PDO communication change
*    accesses, only the write access needs to be handled via the type
*    structure; the read operation can be performed directly.
*/
const CO_OBJ_TYPE COTPdoType = { 0, 0, 0, 0, 0, COTypePdoComTypeWrite };

/******************************************************************************
* FUNCTIONS
******************************************************************************/

/*
* see function definition
*/
void COTPdoTrigObj(CO_TPDO *pdo, CO_OBJ *obj)
{
    uint32_t n;
    uint16_t num;

    if (CO_IS_PDOMAP(obj->Key) != 0) {
        for (n=0; n < (CO_TPDO_N << 3); n++) {
            if (pdo->Node->TMap[n].Obj == obj) {
                num = pdo->Node->TMap[n].Num;
                COTPdoTrigPdo(pdo, num);
            }
        }
    } else {
        pdo->Node->Error = CO_ERR_TPDO_OBJ_TRIGGER;
    }
}

/*
* see function definition
*/
void COTPdoTrigPdo(CO_TPDO *pdo, uint16_t num)
{
    if (num < CO_TPDO_N) {
        COTPdoTx(&pdo[num]);
    } else {
        pdo->Node->Error = CO_ERR_TPDO_NUM_TRIGGER;
    }
}

/*
* see function definition
*/
void COTPdoClear(CO_TPDO *pdo, CO_NODE *node)
{
    uint16_t num;
    uint8_t  on;

    /* argument checks */
    if ((pdo == 0) || (node == 0)) {
        CONodeFatalError();
        return;
    }
    
    COTPdoMapClear(node->TMap);
    for (num = 0; num < CO_TPDO_N; num++) {
        pdo[num].Node       = node;
        pdo[num].EvTmr      = -1;
        pdo[num].InTmr      = -1;
        pdo[num].Identifier = CO_TPDO_COBID_OFF;
        pdo[num].ObjNum     = 0;
        for (on = 0; on < 8; on++) {
            pdo[num].Map[on] = 0;
        }
    }
}

/*
* see function definition
*/
void COTPdoInit(CO_TPDO *pdo, CO_NODE *node)
{
    int16_t  err;
    uint16_t num;
    uint8_t  on;
    uint8_t  tnum;

    /* argument checks */
    if ((pdo == 0) || (node == 0)) {
        CONodeFatalError();
        return;
    }
    
    COTPdoMapClear(node->TMap);
    for (num = 0; num < CO_TPDO_N; num++) {
        pdo[num].Node       = node;
        pdo[num].EvTmr      = -1;
        pdo[num].InTmr      = -1;
        pdo[num].Identifier = CO_TPDO_COBID_OFF;
        pdo[num].ObjNum     = 0;
        for (on = 0; on < 8; on++) {
            pdo[num].Map[on] = 0;
        }
        err = CODirRdByte(&node->Dir, CO_DEV(0x1800,0),&tnum);
        if (err == CO_ERR_NONE) {
            COTPdoReset(pdo, num);
        } else {
            node->Error = CO_ERR_NONE;
        }
    }
}

/*
* see function definition
*/
void COTPdoReset(CO_TPDO *pdo, uint16_t num)
{
    CO_TPDO  *wp;
    CO_DIR   *cod;
    uint32_t  id      = CO_TPDO_COBID_OFF;
    uint16_t  inhibit = 0;
    uint16_t  timer   = 0;
    int16_t   err;
    uint8_t   type    = 0;

    wp  = &pdo[num];
    cod = &wp->Node->Dir;
    if (wp->EvTmr >= 0) {
        (void)COTmrDelete(&wp->Node->Tmr, wp->EvTmr);
        wp->EvTmr = -1;
    }
    if (wp->InTmr >= 0) {
        (void)COTmrDelete(&wp->Node->Tmr, wp->InTmr);
        wp->InTmr = -1;
    }
    if ((wp->Flags & CO_TPDO_FLG_S__) != 0) {
        COSyncRemove(&pdo->Node->Sync, num, CO_SYNC_FLG_TX);
    }
    wp->Flags = 0;
    
    /* pdo communication settings */
    err = CODirRdByte(cod, CO_DEV(0x1800 + num, 2), &type);
    if (err != CO_ERR_NONE) {
        pdo->Node->Error = CO_ERR_TPDO_COM_OBJ;
        return;
    }
    err = CODirRdWord(cod, CO_DEV(0x1800 + num, 3), &inhibit);
    if (err != CO_ERR_NONE) {
        err = (int16_t)CO_ERR_NONE;
        pdo->Node->Error = CO_ERR_NONE;
    }

    if (inhibit > 0) {
        inhibit = (inhibit + 9) / 10;
        if (inhibit <= CO_TPDO_TMR_MIN) {
            inhibit = CO_TPDO_TMR_MIN;
        } else {
            inhibit = CO_TPDO_ROUND(inhibit, CO_TPDO_TMR_MIN);
        }
    }
    pdo[num].Inhibit = CO_TPDO_MS(inhibit);

    if ((type == 254) || (type == 255)) {
        err = CODirRdWord(cod, CO_DEV(0x1800 + num, 5), &timer);
        if (err != CO_ERR_NONE) {
            err = (int16_t)CO_ERR_NONE;
            pdo->Node->Error = CO_ERR_NONE;
        }
        if (timer > 0) {
            if (timer <= CO_TPDO_TMR_MIN) {
                timer = CO_TPDO_TMR_MIN;
            } else {
                timer = CO_TPDO_ROUND(timer, CO_TPDO_TMR_MIN);
            }
        }
    } else {
        pdo->Node->Error = CO_ERR_TPDO_COM_OBJ;
        return;
    }
    
    err = CODirRdLong(cod, CO_DEV(0x1800 + num, 1), &id);
    if (err != CO_ERR_NONE) {
        pdo->Node->Error = CO_ERR_TPDO_COM_OBJ;
        return;
    }

    if ((id & CO_TPDO_COBID_REMOTE) == 0) {
        pdo->Node->Error = CO_ERR_TPDO_COM_OBJ;
        return;
    }
    if ((id & CO_TPDO_COBID_EXT) != 0) {
        pdo->Node->Error = CO_ERR_TPDO_COM_OBJ;
        return;
    }
    if ((id & CO_TPDO_COBID_OFF) == 0) {
        pdo[num].Identifier = (id & 0x1FFFFFFF);
    } else {
        pdo[num].Identifier = CO_TPDO_COBID_OFF;
    }
    
    /* pdo mapping settings */
    err = COTPdoGetMap(pdo, num);
    if (err != CO_ERR_NONE) {
        pdo->Node->Error = CO_ERR_TPDO_MAP_OBJ;
        return;
    }
    if (pdo[num].Identifier != CO_TPDO_COBID_OFF) {
        if (type <= 240) {
            pdo[num].Flags |= CO_TPDO_FLG_S__;
            COSyncAdd(&pdo->Node->Sync, num, CO_SYNC_FLG_TX, type);
        }
    }
    pdo[num].Event = CO_TPDO_MS(timer);
    if (pdo[num].Event > 0) {
        pdo[num].EvTmr = COTmrCreate(&pdo->Node->Tmr,
                                     pdo[num].Event + num,
                                     0,
                                     COTPdoTmrEvent,
                                     &pdo[num]);
    }
}

/*
* see function definition
*/
int16_t COTPdoGetMap (CO_TPDO *pdo, uint16_t num)
{
    CO_DIR   *cod;
    CO_OBJ   *obj;
    uint32_t  mapping;
    uint16_t  idx;
    uint16_t  on;
    int16_t   err;
    uint8_t   mapnum;
    uint8_t   dlc;

    cod = &pdo[num].Node->Dir;
    idx = 0x1A00 + num;
    err = CODirRdByte(cod, CO_DEV(idx, 0), &mapnum);
    if (err != CO_ERR_NONE) {
        return (-1);
    }

    /* build mapping table */
    dlc = 0;
    for (on=0; on < mapnum; on++) {
        err = CODirRdLong(cod, CO_DEV(idx, 1+on), &mapping);
        if (err != CO_ERR_NONE) {
            return (-1);
        }

        dlc += (uint8_t)(mapping & 0xFF) >> 3;
        if (dlc > 8) {
            return (-1);
        }
        obj = CODirFind(&pdo->Node->Dir, mapping);
        if (obj == 0) {
            return (-1);
        } else {
            pdo[num].Map[on] = obj;
            COTPdoMapAdd(pdo->Node->TMap, obj, num);
        }
    }
    pdo[num].ObjNum = mapnum;

    return (0);
}

/*
* see function definition
*/
void COTPdoTmrEvent (void *parg)
{
    CO_TPDO *pdo;

    pdo = (CO_TPDO *)parg;
    COTPdoTx(pdo);
}

/*
* see function definition
*/
void COTPdoEndInhibit(void *parg)
{
    CO_TPDO *pdo;
    
    pdo = (CO_TPDO *)parg;
    pdo->Flags &= ~CO_TPDO_FLG__I_;
    pdo->InTmr  = -1;
    if ((pdo->Flags & CO_TPDO_FLG___E) != 0) {
        pdo->Flags &= ~CO_TPDO_FLG___E;
        COTPdoTx(pdo);
    }
}

/*
* see function definition
*/
void COTPdoTx (CO_TPDO *pdo)
{
    CO_IF_FRM frm;
    uint32_t  sz;
    uint32_t  data;
    uint8_t   num;

    if ((pdo->Node->Nmt.Allowed & CO_PDO_ALLOWED) == 0) {
        return;
    }
    if ( (pdo->Flags & CO_TPDO_FLG__I_) != 0) {
        pdo->Flags |= CO_TPDO_FLG___E;
        return;
    }
    
    if (pdo->EvTmr >= 0) {
        (void)COTmrDelete(&pdo->Node->Tmr, pdo->EvTmr);
        pdo->EvTmr = -1;
    }
    if (pdo->Inhibit > 0) {
        pdo->InTmr = COTmrCreate(&pdo->Node->Tmr,
                                 pdo->Inhibit,
                                 0,
                                 COTPdoEndInhibit,
                                 (void*)pdo);
        if (pdo->InTmr< 0) {
            pdo->Node->Error = CO_ERR_TPDO_INHIBIT;
        } else {
            pdo->Flags |= CO_TPDO_FLG__I_;
        }
    }
    if (pdo->Event > 0) {
        pdo->EvTmr = COTmrCreate(&pdo->Node->Tmr,
                               pdo->Event,
                               0,
                               COTPdoTmrEvent,
                               (void*)pdo);
        if (pdo->EvTmr < 0) {
            pdo->Node->Error = CO_ERR_TPDO_EVENT;
        }
    }
    frm.Identifier = pdo->Identifier;
    frm.DLC        = 0;
    for (num = 0; num < pdo->ObjNum; num++) {
        sz = COObjGetSize(pdo->Map[num], 0L);
        if (sz <= (uint32_t)(8 - frm.DLC)) {
            COObjRdValue(pdo->Map[num], &data, CO_LONG, pdo->Node->NodeId);

            if (sz == CO_BYTE) {
                CO_SET_BYTE(&frm, data, frm.DLC);
            } else if (sz == CO_WORD) {
                CO_SET_WORD(&frm, data, frm.DLC);
            } else if (sz == CO_LONG) {
                CO_SET_LONG(&frm, data, frm.DLC);
            }
            frm.DLC += (uint8_t)sz;
        }
    }
    COTPdoTransmit(&frm);

    (void)COIfSend(&pdo->Node->If, &frm);
}

/*
* see function definition
*/
void COTPdoMapClear(CO_TPDO_LINK *map)
{
    uint16_t id;

    for (id = 0; id < (CO_TPDO_N << 3); id++) {
        map[id].Obj  = 0;
        map[id].Num  = 0xFFFF;
    }
}

/*
* see function definition
*/
void COTPdoMapAdd(CO_TPDO_LINK *map, CO_OBJ *obj, uint16_t num)
{
    uint16_t id;
    
    for (id = 0; id < (CO_TPDO_N << 3); id++) {
        if (map[id].Obj == 0) {
            map[id].Obj = obj;
            map[id].Num = num;
            break;
        }
    }
}

/*
* see function definition
*/
void COTPdoMapDelNum(CO_TPDO_LINK *map, uint16_t num)
{
    uint16_t id;
    
    for (id = 0; id < (CO_TPDO_N << 3); id++) {
        if (map[id].Num == num) {
            map[id].Obj = 0;
            map[id].Num = 0xFFFF;
        }
    }
}

/*
* see function definition
*/
void COTPdoMapDelSig(CO_TPDO_LINK *map, CO_OBJ *obj)
{
    uint16_t id;

    for (id = 0; id < (CO_TPDO_N << 3); id++) {
        if (map[id].Obj == obj) {
            map[id].Obj = 0;
            map[id].Num = 0xFFFF;
        }
    }
}

/*
* see function definition
*/
int16_t COTypeAsyncCtrl (CO_OBJ* obj, uint16_t func, uint32_t para)
{
    CO_NODE *node;
    int16_t  result = -1;

    (void)para;

    if (func == CO_TPDO_ASYNC) {
        node = obj->Type->Dir->Node;
        COTPdoTrigObj(node->TPdo, obj);
        result = 0;
    }

    return (result);
}

/*
* see function definition
*/
int16_t COTypeEventWrite(CO_OBJ* obj, void *buf, uint32_t size)
{
    CO_DIR   *cod;
    CO_NMT   *nmt;
    CO_TPDO  *pdo;
    uint32_t  cobid = 0;
    uint16_t  cycTime;
    uint16_t  num;
    int16_t   err;
    int16_t   result = -1;

    err = COObjWrDirect(obj, (void *)buf, size);
    if (err != CO_ERR_NONE) {
        return (-1);
    }
    cod     = obj->Type->Dir;
    nmt     = &cod->Node->Nmt;
    num     = CO_GET_IDX(obj->Key);
    num    &= 0x1FF;
    pdo     = &cod->Node->TPdo[num];
    if (nmt->Mode != CO_OPERATIONAL) {
        return (err);
    }

    
    cycTime = (uint16_t)(*(uint32_t *)buf);
    if (cycTime > 0) {
        if (cycTime <= CO_TPDO_TMR_MIN) {
            cycTime = CO_TPDO_TMR_MIN;
        } else {
            cycTime = CO_TPDO_ROUND(cycTime, CO_TPDO_TMR_MIN);
        }
    }
    pdo->Event = CO_TPDO_MS(cycTime);
    if (pdo->EvTmr >= 0) {
        result = COTmrDelete(&pdo->Node->Tmr, pdo->EvTmr);
        if (result < 0) {
            pdo->Node->Error = CO_ERR_TMR_DELETE;
        }
    }
    if (pdo->InTmr >= 0) {
        result = COTmrDelete(&pdo->Node->Tmr, pdo->InTmr);
        if (result < 0) {
            pdo->Node->Error = CO_ERR_TMR_DELETE;
        }
    }

    (void)CODirRdLong(cod, CO_DEV(0x1800+num, 1), &cobid);
    if (((cobid & CO_TPDO_COBID_OFF) == 0) &&
        (pdo->Event > 0)                   &&
        (nmt->Mode == CO_OPERATIONAL)) {
        pdo->EvTmr = COTmrCreate(&pdo->Node->Tmr,
                                 pdo->Event,
                                 0,
                                 COTPdoTmrEvent,
                                 (void*)pdo);
    }

    return (err);
}

/*
* see function definition
*/
void CORPdoClear(CO_RPDO *pdo, CO_NODE *node)
{
    int16_t num;
    
    if ((pdo == 0) || (node == 0)) {
        CONodeFatalError();
        return;
    }

    for (num = 0; num < CO_RPDO_N; num++) {
        pdo[num].Node       = node;
        pdo[num].Identifier = 0;
        pdo[num].ObjNum     = 0;
    }
}

/*
* see function definition
*/
void CORPdoInit(CO_RPDO *pdo, CO_NODE *node)
{
    int16_t err;
    uint8_t rnum;
    int16_t num;

    if ((pdo == 0) || (node == 0)) {
        CONodeFatalError();
        return;
    }

    for (num = 0; num < CO_RPDO_N; num++) {
        pdo[num].Node       = node;
        pdo[num].Identifier = 0;
        pdo[num].ObjNum     = 0;
        err = CODirRdByte(&node->Dir, CO_DEV(0x1400, 0), &rnum);
        if (err == CO_ERR_NONE) {
            CORPdoReset(pdo, num);
        } else {
            node->Error = CO_ERR_NONE;
        }
    }
}

/*
* see function definition
*/
int16_t CORPdoReset(CO_RPDO *pdo, int16_t num)
{
    CO_RPDO *wp;
    CO_DIR  *cod;
    uint32_t id = CO_RPDO_COBID_OFF;
    int16_t  err;
    uint8_t  on;
    uint8_t  type = 0;

    wp             = &pdo[num];
    cod            = &wp->Node->Dir;
    wp->Identifier = 0;
    wp->ObjNum     = 0;
    for (on = 0; on < 8; on++) {
        wp->Map[on] = 0;
    }

    if ((wp->Flag & CO_RPDO_FLG_S_) != 0) {
        COSyncRemove(&pdo->Node->Sync, num, CO_SYNC_FLG_RX);
    }
    wp->Flag = 0;
    
    /* communication */
    err = CODirRdByte(cod, CO_DEV(0x1400 + num, 2), &type);
    if (err != CO_ERR_NONE) {
        pdo->Node->Error = CO_ERR_RPDO_COM_OBJ;
        return (-1);
    }
    if (type <= 240) {
        pdo->Node->Error = CO_ERR_RPDO_COM_OBJ;
        return (-1);
    }
    err = CODirRdLong(cod, CO_DEV(0x1400 + num, 1), &id);
    if (err != CO_ERR_NONE) {
        pdo->Node->Error = CO_ERR_RPDO_COM_OBJ;
        return (-1);
    }
    if ((id & CO_RPDO_COBID_REMOTE) == 0) {
        pdo->Node->Error = CO_ERR_RPDO_COM_OBJ;
        return (-1);
    }
    if ((id & CO_RPDO_COBID_EXT) != 0) {
        pdo->Node->Error = CO_ERR_RPDO_COM_OBJ;
        return (-1);
    }
    if ((id & CO_RPDO_COBID_OFF) == 0) {
        pdo[num].Identifier = (id & 0x1FFFFFFF);
        pdo[num].Flag       = CO_RPDO_FLG__E;
        if (type <= 240) {
            pdo[num].Flag  |= CO_RPDO_FLG_S_;
        }
    } else {
        pdo[num].Identifier = CO_RPDO_COBID_OFF;
        pdo[num].Flag       = 0;
    }
    
    /* mapping */
    err = CORPdoGetMap(pdo, num);
    if (err != CO_ERR_NONE) {
        pdo->Node->Error = CO_ERR_RPDO_MAP_OBJ;
    }
    if ((pdo[num].Flag & CO_RPDO_FLG__E) != 0) {
        if (type <= 240) {
            COSyncAdd(&pdo[num].Node->Sync, num, CO_SYNC_FLG_RX, type);
        }
    }

    return (0);
}
    
/*
* see function definition
*/
int16_t CORPdoGetMap(CO_RPDO *pdo, uint16_t num)
{
    CO_DIR   *cod;
    CO_OBJ   *obj;
    uint32_t  mapping;
    uint16_t  idx;
    uint16_t  link;
    int16_t   err;
    uint8_t   on;
    uint8_t   mapnum;
    uint8_t   dlc;
    uint8_t   dummy = 0;

    cod = &pdo[num].Node->Dir;
    idx = 0x1600 + num;
    err = CODirRdByte(cod, CO_DEV(idx, 0), &mapnum);
    if (err != CO_ERR_NONE) {
        return (-1);
    }

    dlc = 0;
    for (on = 0; on < mapnum; on++) {
        err = CODirRdLong(cod, CO_DEV(idx, 1 + on), &mapping);
        if (err != CO_ERR_NONE) {
            return (-1);
        }

        dlc += (uint8_t)(mapping & 0xFF) >> 3;
        if (dlc > 8) {
            return (-1);
        }
        link = mapping >> 16;
        if ((link == 2) || (link == 5)) {
            pdo[num].Map[on + dummy] = 0;
            dummy++;
        } else if ((link == 3) || (link == 6)) {
            pdo[num].Map[on + dummy] = 0;
            dummy++;
            pdo[num].Map[on + dummy] = 0;
            dummy++;
        } else if ((link == 4) || (link == 7)) {
            pdo[num].Map[on + dummy] = 0;
            dummy++;
            pdo[num].Map[on + dummy] = 0;
            dummy++;
            pdo[num].Map[on + dummy] = 0;
            dummy++;
            pdo[num].Map[on + dummy] = 0;
            dummy++;
        } else {
            obj = CODirFind(&pdo->Node->Dir, mapping);
            if (obj == 0) {
                return (-1);
            } else {
                pdo[num].Map[on + dummy] = obj;
            }
        }
    }
    pdo[num].ObjNum = mapnum + dummy;

    return (0);
}

/*
* see function definition
*/
void CORPdoRx(CO_RPDO *pdo, uint16_t num, CO_IF_FRM *frm)
{
    int16_t err = 0;

    if ((pdo[num].Flag & CO_RPDO_FLG_S_) == 0) {
        err = CORPdoReceive(frm);
        if (err == 0) {
            CORPdoWrite(&pdo[num], frm);
        }
    } else {
        COSyncRx(&pdo[num].Node->Sync, frm);
    }
}

/*
* see function definition
*/
int16_t CORPdoCheck(CO_RPDO *pdo, CO_IF_FRM *frm)
{
    int16_t  result = -1;
    uint16_t n;

    n = 0;
    while (n < CO_RPDO_N) {
        if ((pdo[n].Flag & CO_RPDO_FLG__E) != 0) {
            if (pdo[n].Identifier == frm->Identifier) {
                result = n;
                break;
            }
        }
        n++;
    }

    return (result);
}

/*
* see function definition
*/
void CORPdoWrite(CO_RPDO *pdo, CO_IF_FRM *frm)
{
    CO_OBJ  *obj;
    uint32_t val32;
    uint16_t val16;
    uint8_t  val08;
    uint8_t  on;
    uint8_t  sz;
    uint8_t  dlc = 0;

    for (on = 0; on < pdo->ObjNum; on++) {
        obj = pdo->Map[on];
        if (obj != 0) {
            sz = (uint8_t)COObjGetSize(obj, 0L);
            if (sz == CO_BYTE) {
                val08 = CO_GET_BYTE(frm, dlc);
                dlc++;
                COObjWrValue(obj, (void *)&val08, sz, pdo->Node->NodeId);
            } else if (sz == CO_WORD) {
                val16 = CO_GET_WORD(frm, dlc);
                dlc += 2;
                COObjWrValue(obj, (void *)&val16, sz, pdo->Node->NodeId);
            } else if (sz == CO_LONG) {
                val32 = CO_GET_LONG(frm, dlc);
                dlc += 4;
                COObjWrValue(obj, (void *)&val32, sz, pdo->Node->NodeId);
            }
        }
    }
}

/*
* see function definition
*/
int16_t COTypePdoMapNumWrite(CO_OBJ* obj, void *buf, uint32_t size)
{
    CO_DIR   *cod;
    uint32_t  id;
    uint32_t  mapentry;
    int16_t   result = CO_ERR_NONE;
    uint16_t  pmapidx;
    uint16_t  pcomidx;
    uint8_t   mapbytes;
    uint8_t   mapnum;
    uint8_t   i;

    if ((obj == 0) || (buf == 0) || (size != CO_LONG)) {
        return (CO_ERR_BAD_ARG);
    }
    if (CO_GET_SUB(obj->Key) != 0) {
        return (CO_ERR_TPDO_MAP_OBJ);
    }
    mapnum = (uint8_t)(*(uint32_t *)buf);
    if (mapnum > 8) {
        return (CO_ERR_OBJ_MAP_LEN);
    }

    cod     = obj->Type->Dir;
    pmapidx = CO_GET_IDX(obj->Key);
    if ((pmapidx >= 0x1600) && (pmapidx <= 0x17FF)) {
    } else if ((pmapidx >= 0x1A00) && (pmapidx <= 0x1BFF)) {
    } else {
        return (CO_ERR_TPDO_MAP_OBJ);
    }

    pcomidx = pmapidx - 0x200;
    (void)CODirRdLong(cod, CO_DEV(pcomidx, 1), &id);
    if ((id & CO_TPDO_COBID_OFF) == 0) {
        result = CO_ERR_OBJ_ACC;
    } else {
        mapbytes = 0;
        for (i = 1; i <= mapnum; i++) {
            result = CODirRdLong(cod, CO_DEV(pmapidx, i), &mapentry);
            if (result != CO_ERR_NONE) {
                return (CO_ERR_OBJ_MAP_TYPE);
            }
            mapbytes += CO_GET_SIZE(mapentry);
        }

        if (mapbytes > 8) {
            return (CO_ERR_OBJ_MAP_LEN);
        }

        result = COObjWrDirect(obj, &mapnum, CO_LONG);
    }

    return (result);
}

/*
* see function definition
*/
int16_t COTypePdoMapWrite(CO_OBJ *obj, void *buf, uint32_t size)
{
    CO_DIR  *cod;
    CO_OBJ  *objm;
    uint32_t map;
    uint32_t id;
    uint32_t maps;
    int16_t  result = CO_ERR_NONE;
    uint16_t pmapidx;
    uint16_t pcomidx;
    uint8_t  mapn;

    (void)size;
    if ((obj == 0) || (buf == 0)) {
        return (CO_ERR_BAD_ARG);
    }
    if (CO_GET_SUB(obj->Key) < 1) {
        return (CO_ERR_TPDO_MAP_OBJ);
    }

    cod     = obj->Type->Dir;
    pmapidx = CO_GET_IDX(obj->Key);
    if ((pmapidx >= 0x1600) && (pmapidx <= 0x17FF)) {
    } else if ((pmapidx >= 0x1A00) && (pmapidx <= 0x1BFF)) {
    } else {
        return (CO_ERR_TPDO_MAP_OBJ);
    }

    pcomidx = pmapidx - 0x200;
    map     = *(uint32_t*)buf;
    (void)CODirRdLong(cod, CO_DEV(pcomidx, 1), &id);
    if ((id & CO_TPDO_COBID_OFF) == 0) {
        result = CO_ERR_OBJ_ACC;
    } else {
        (void)CODirRdByte(cod, CO_DEV(pmapidx, 0), &mapn);
        if (mapn != 0) {
            return (CO_ERR_OBJ_ACC);
        }

        objm = CODirFind(cod, map);
        if (objm == (CO_OBJ*)0) {
            return (CO_ERR_OBJ_MAP_TYPE);
        }

        maps = CO_IS_PDOMAP(objm->Key);
        if (maps == 0) {
            return (CO_ERR_OBJ_MAP_TYPE);
        }

        if ((pmapidx >= 0x1600) && (pmapidx <= 0x17FF)) {
            maps = CO_IS_WRITE(objm->Key);
            if (maps == 0) {
                return (CO_ERR_OBJ_MAP_TYPE);
            }
        } else if ((pmapidx >= 0x1A00) && (pmapidx <= 0x1BFF)) {
            maps = CO_IS_READ(objm->Key);
            if (maps == 0) {
                return (CO_ERR_OBJ_MAP_TYPE);
            }
        }

        result = COObjWrDirect(obj, &map, CO_LONG);
    }

    return (result);
}

/*
* see function definition
*/
int16_t COTypePdoComIdWrite(CO_OBJ* obj, void *buf, uint32_t size)
{
    CO_DIR   *cod;
    CO_NMT   *nmt;
    CO_TPDO  *tpdo = 0;
    CO_RPDO  *rpdo = 0;
    uint32_t  nid;
    uint32_t  oid;
    int16_t   result = CO_ERR_NONE;
    uint16_t  pcomidx;
    uint16_t  num;

    (void)size;
    if ((obj == 0) || (buf == 0)) {
        return (CO_ERR_BAD_ARG);
    }
    if (CO_GET_SUB(obj->Key) != 1) {
        return (CO_ERR_TPDO_COM_OBJ);
    }

    nid = *(uint32_t*)buf;
    if ((nid & CO_TPDO_COBID_EXT) != 0) {
        return (CO_ERR_OBJ_RANGE);
    }

    cod     = obj->Type->Dir;
    nmt     = &cod->Node->Nmt;
    pcomidx = CO_GET_IDX(obj->Key);
    if ((pcomidx >= 0x1400) && (pcomidx <= 0x15FF)) {
        rpdo = cod->Node->RPdo;
        num  = pcomidx & 0x1FF;
    } else if ((pcomidx >= 0x1800) && (pcomidx <= 0x19FF)) {
        if ((nid & CO_TPDO_COBID_REMOTE) == 0) {
            return (CO_ERR_OBJ_RANGE);
        }
        tpdo = cod->Node->TPdo;
        num  = pcomidx & 0x1FF;
    } else {
        return (CO_ERR_TPDO_COM_OBJ);
    }

    (void)COObjRdDirect(obj, &oid, CO_LONG);
    if ((oid & CO_TPDO_COBID_OFF) == 0) {
        if ((nid & CO_TPDO_COBID_OFF) != 0) {
            result = COObjWrDirect(obj, &nid, CO_LONG);
            if (nmt->Mode == CO_OPERATIONAL) {
                if (tpdo != 0) {
                    COTPdoReset(tpdo, num);
                }
                if (rpdo != 0) {
                    CORPdoReset(rpdo, num);
                }
            }
        } else {
            result = CO_ERR_OBJ_RANGE;
        }
    } else {
        if ((nid & CO_TPDO_COBID_OFF) != 0) {
            result = COObjWrDirect(obj, &nid, CO_LONG);
        } else {
            result = COObjWrDirect(obj, &nid, CO_LONG);
            if (nmt->Mode == CO_OPERATIONAL) {
                if (tpdo != 0) {
                    COTPdoReset(tpdo, num);
                }
                if (rpdo != 0) {
                    CORPdoReset(rpdo, num);
                }
            }
        }
    }

    return (result);
}

/*
* see function definition
*/
int16_t COTypePdoComTypeWrite(CO_OBJ* obj, void *buf, uint32_t size)
{
    CO_DIR   *cod;
    uint8_t   type;
    uint32_t  id;
    int16_t   result = CO_ERR_NONE;
    uint16_t  pcomidx;

    if ((obj == 0) || (buf == 0) || (size != CO_LONG)) {
        return (CO_ERR_BAD_ARG);
    }
    if (CO_GET_SUB(obj->Key) != 2) {
        return (CO_ERR_PARA_IDX);
    }

    type = *(uint8_t*)buf;
    if (type < 254) {
        return (CO_ERR_OBJ_ACC);
    }

    cod     = obj->Type->Dir;
    pcomidx = CO_GET_IDX(obj->Key);
    if ((pcomidx >= 0x1400) && (pcomidx <= 0x15FF)) {
    } else if ((pcomidx >= 0x1800) && (pcomidx <= 0x19FF)) {
    } else {
        return (CO_ERR_PARA_IDX);
    }

    (void)CODirRdLong(cod, CO_DEV(pcomidx, 1), &id);
    if ((id & CO_TPDO_COBID_OFF) == 0) {
        result = CO_ERR_OBJ_RANGE;
    } else {
        result = COObjWrDirect(obj, &type, CO_LONG);
    }

    return (result);
}
