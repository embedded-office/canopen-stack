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

#include "co_pdo.h"

#include "co_core.h"

/******************************************************************************
* PRIVATE HELPER FUNCTION PROTOTYPES
******************************************************************************/

static void COTPdoMapClear(CO_TPDO_LINK *map);

/******************************************************************************
* PRIVATE HELPER FUNCTIONS
******************************************************************************/

static void COTPdoMapClear(CO_TPDO_LINK *map)
{
    uint16_t id;

    for (id = 0; id < (CO_TPDO_N << 3); id++) {
        map[id].Obj  = 0;
        map[id].Num  = 0xFFFF;
    }
}

/******************************************************************************
* PROTECTED API FUNCTIONS
******************************************************************************/

void COTPdoInit(CO_TPDO *pdo, CO_NODE *node)
{
    CO_ERR   err;
    uint16_t num;
    uint8_t  on;
    uint8_t  tnum;

    ASSERT_PTR_FATAL(pdo);
    ASSERT_PTR_FATAL(node);
    
    COTPdoMapClear(node->TMap);
    for (num = 0; num < CO_TPDO_N; num++) {
        pdo[num].Node       = node;
        pdo[num].EvTmr      = -1;
        pdo[num].InTmr      = -1;
        pdo[num].Identifier = CO_TPDO_COBID_OFF;
        pdo[num].ObjNum     = 0;
        for (on = 0; on < 8; on++) {
            pdo[num].Map[on]  = 0;
            pdo[num].Size[on] = 0;
        }
        err = CODictRdByte(&node->Dict, CO_DEV(0x1800 + num,0),&tnum);
        if (err == CO_ERR_NONE) {
            COTPdoReset(pdo, num);
        } else {
            node->Error = CO_ERR_NONE;
        }
    }
}

void COTPdoReset(CO_TPDO *pdo, uint16_t num)
{
    CO_TPDO  *wp;
    CO_DICT  *cod;
    CO_TMR   *tmr;
    CO_SYNC  *sync;
    uint32_t  id      = CO_TPDO_COBID_OFF;
    uint16_t  inhibit = 0;
    uint16_t  timer   = 0;
    CO_ERR    err;
    uint8_t   type    = 0;

    wp   = &pdo[num];
    cod  = &wp->Node->Dict;
    tmr  = &wp->Node->Tmr;
    sync = &pdo->Node->Sync;
    if (wp->EvTmr >= 0) {
        (void)COTmrDelete(tmr, wp->EvTmr);
        wp->EvTmr = -1;
    }
    if (wp->InTmr >= 0) {
        (void)COTmrDelete(tmr, wp->InTmr);
        wp->InTmr = -1;
    }
    if ((wp->Flags & CO_TPDO_FLG_S__) != 0) {
        COSyncRemove(sync, num, CO_SYNC_FLG_TX);
    }
    wp->Flags = 0;
    
    /* pdo communication settings */
    err = CODictRdByte(cod, CO_DEV(0x1800 + num, 2), &type);
    if (err != CO_ERR_NONE) {
        pdo->Node->Error = CO_ERR_TPDO_COM_OBJ;
        return;
    }
    (void)CODictRdWord(cod, CO_DEV(0x1800 + num, 3), &inhibit);
    pdo[num].Inhibit = COTmrGetTicks(tmr, inhibit, CO_TMR_UNIT_100US);

    if ((type == 254) || (type == 255)) {
        (void)CODictRdWord(cod, CO_DEV(0x1800 + num, 5), &timer);
    }

    err = CODictRdLong(cod, CO_DEV(0x1800 + num, 1), &id);
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
            COSyncAdd(sync, num, CO_SYNC_FLG_TX, type);
        }
    }
    pdo[num].Event = COTmrGetTicks(tmr, timer, CO_TMR_UNIT_1MS);
    if (pdo[num].Event > 0) {
        pdo[num].EvTmr = COTmrCreate(tmr,
                                     pdo[num].Event + num,
                                     0,
                                     COTPdoTmrEvent,
                                     &pdo[num]);
    }
}

CO_ERR COTPdoGetMap (CO_TPDO *pdo, uint16_t num)
{
    CO_DICT   *cod;
    CO_OBJ   *obj;
    uint32_t  mapping;
    uint16_t  idx;
    uint16_t  on;
    CO_ERR    err;
    uint8_t   mapnum;
    uint8_t   size;
    uint8_t   dlc;

    cod = &pdo[num].Node->Dict;
    idx = 0x1A00 + num;
    err = CODictRdByte(cod, CO_DEV(idx, 0), &mapnum);
    if (err != CO_ERR_NONE) {
        return (CO_ERR_TPDO_MAP_OBJ);
    }

    /* build mapping table */
    dlc = 0;
    for (on=0; on < mapnum; on++) {
        err = CODictRdLong(cod, CO_DEV(idx, 1+on), &mapping);
        if (err != CO_ERR_NONE) {
            return (CO_ERR_TPDO_MAP_OBJ);
        }

        size = (uint8_t)(mapping & 0xFF) >> 3;
        dlc += size;
        if (dlc > 8) {
            return (CO_ERR_TPDO_MAP_OBJ);
        }
        obj = CODictFind(&pdo->Node->Dict, mapping);
        if (obj == 0) {
            return (CO_ERR_TPDO_MAP_OBJ);
        } else {
            pdo[num].Map[on]  = obj;
            pdo[num].Size[on] = size;
            COTPdoMapAdd(pdo->Node->TMap, obj, num);
        }
    }
    pdo[num].ObjNum = mapnum;

    return (CO_ERR_NONE);
}

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

void COTPdoClear(CO_TPDO *pdo, CO_NODE *node)
{
    uint16_t num;
    uint8_t  on;

    ASSERT_PTR(pdo);
    ASSERT_PTR(node);
    
    COTPdoMapClear(node->TMap);
    for (num = 0; num < CO_TPDO_N; num++) {
        pdo[num].Node       = node;
        pdo[num].EvTmr      = -1;
        pdo[num].InTmr      = -1;
        pdo[num].Identifier = CO_TPDO_COBID_OFF;
        pdo[num].ObjNum     = 0;
        for (on = 0; on < 8; on++) {
            pdo[num].Map[on]  = 0;
            pdo[num].Size[on] = 0;
        }
    }
}

void COTPdoTmrEvent (void *parg)
{
    CO_TPDO *pdo;

    pdo = (CO_TPDO *)parg;
    pdo->EvTmr = -1;
    COTPdoTx(pdo);
}

void COTPdoTmrInhibit(void *parg)
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

void COTPdoTx(CO_TPDO *pdo)
{
    CO_TMR    *tmr;
    CO_IF_FRM  frm;
    uint32_t   sz;
    uint8_t    pdosz;
    uint32_t   data;
    uint8_t    num;

    if ((pdo->Node->Nmt.Allowed & CO_PDO_ALLOWED) == 0) {
        return;
    }
    if ( (pdo->Flags & CO_TPDO_FLG__I_) != 0) {
        pdo->Flags |= CO_TPDO_FLG___E;
        return;
    }
    tmr = &pdo->Node->Tmr;
    if (pdo->EvTmr >= 0) {
        (void)COTmrDelete(tmr, pdo->EvTmr);
        pdo->EvTmr = -1;
    }
    if (pdo->Inhibit > 0) {
        pdo->InTmr = COTmrCreate(tmr,
                                 pdo->Inhibit,
                                 0,
                                 COTPdoTmrInhibit,
                                 (void*)pdo);
        if (pdo->InTmr < 0) {
            pdo->Node->Error = CO_ERR_TPDO_INHIBIT;
        } else {
            pdo->Flags |= CO_TPDO_FLG__I_;
        }
    }
    if (pdo->Event > 0) {
        pdo->EvTmr = COTmrCreate(tmr,
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
        pdosz = pdo->Size[num];
        if (pdosz <= 4) {
            /* supported mapping: 1 to 4 bytes */
            sz = COObjGetSize(pdo->Map[num], pdo->Node, 0L);
            if (sz <= (uint32_t)(8 - frm.DLC)) {
                if (pdosz == 3) {
                    /* for 3bytes, read a basic 32bit type */
                    COObjRdValue(pdo->Map[num], pdo->Node, &data, 4u);
                } else {
                    COObjRdValue(pdo->Map[num], pdo->Node, &data, pdosz);
                }
                if (sz == 1u) {
                    CO_SET_BYTE(&frm, data, frm.DLC);
                } else if (sz == 2u) {
                    CO_SET_WORD(&frm, data, frm.DLC);
                } else if (sz == 4u) {
                    if (pdosz == 3) {
                        CO_SET_BYTE(&frm, data, frm.DLC);
                        CO_SET_WORD(&frm, (data >> 8), (frm.DLC + 1));
                    } else {
                        CO_SET_LONG(&frm, data, frm.DLC);
                    }
                }
                frm.DLC += pdosz;
            }
        } else {
            COTpdoReadData(&frm, frm.DLC, pdosz, pdo->Map[num]);
        }
    }

    COPdoTransmit(&frm);
    (void)COIfCanSend(&pdo->Node->If, &frm);
}

/******************************************************************************
* PUBLIC API FUNCTIONS
******************************************************************************/

WEAK_TEST
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

void COTPdoTrigPdo(CO_TPDO *pdo, uint16_t num)
{
    if (num < CO_TPDO_N) {
        COTPdoTx(&pdo[num]);
    } else {
        pdo->Node->Error = CO_ERR_TPDO_NUM_TRIGGER;
    }
}

void CORPdoClear(CO_RPDO *pdo, CO_NODE *node)
{
    int16_t num;
    
    ASSERT_PTR_FATAL(pdo);
    ASSERT_PTR_FATAL(node);

    for (num = 0; num < CO_RPDO_N; num++) {
        pdo[num].Node       = node;
        pdo[num].Identifier = 0;
        pdo[num].ObjNum     = 0;
    }
}

void CORPdoInit(CO_RPDO *pdo, CO_NODE *node)
{
    CO_ERR   err;
    uint8_t  rnum;
    uint16_t num;
    
    ASSERT_PTR_FATAL(pdo);
    ASSERT_PTR_FATAL(node);

    for (num = 0; num < CO_RPDO_N; num++) {
        pdo[num].Node       = node;
        pdo[num].Identifier = 0;
        pdo[num].ObjNum     = 0;
        err = CODictRdByte(&node->Dict, CO_DEV(0x1400 + num, 0), &rnum);
        if (err == CO_ERR_NONE) {
            CORPdoReset(pdo, num);
        } else {
            node->Error = CO_ERR_NONE;
        }
    }
}

CO_ERR CORPdoReset(CO_RPDO *pdo, uint16_t num)
{
    CO_RPDO  *wp;
    CO_DICT  *cod;
    uint32_t  id = CO_RPDO_COBID_OFF;
    CO_ERR    err;
    uint8_t   on;
    uint8_t   type = 0;

    wp             = &pdo[num];
    cod            = &wp->Node->Dict;
    wp->Identifier = 0;
    wp->ObjNum     = 0;
    for (on = 0; on < 8; on++) {
        wp->Map[on]  = 0;
        wp->Size[on] = 0;
    }

    if ((wp->Flag & CO_RPDO_FLG_S_) != 0) {
        COSyncRemove(&pdo->Node->Sync, num, CO_SYNC_FLG_RX);
    }
    wp->Flag = 0;
    
    /* communication */
    err = CODictRdByte(cod, CO_DEV(0x1400 + num, 2), &type);
    if (err != CO_ERR_NONE) {
        pdo->Node->Error = CO_ERR_RPDO_COM_OBJ;
        return (CO_ERR_RPDO_COM_OBJ);
    }
    err = CODictRdLong(cod, CO_DEV(0x1400 + num, 1), &id);
    if (err != CO_ERR_NONE) {
        pdo->Node->Error = CO_ERR_RPDO_COM_OBJ;
        return (CO_ERR_RPDO_COM_OBJ);
    }
    if ((id & CO_RPDO_COBID_EXT) != 0) {
        pdo->Node->Error = CO_ERR_RPDO_COM_OBJ;
        return (CO_ERR_RPDO_COM_OBJ);
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
    return (CO_ERR_NONE);
}

CO_ERR CORPdoGetMap(CO_RPDO *pdo, uint16_t num)
{
    CO_DICT   *cod;
    CO_OBJ   *obj;
    uint32_t  mapping;
    uint16_t  idx;
    uint16_t  link;
    CO_ERR    err;
    uint8_t   on;
    uint8_t   mapnum;
    uint8_t   dlc;
    uint8_t   size;
    uint8_t   dummy = 0;

    cod = &pdo[num].Node->Dict;
    idx = 0x1600 + num;
    err = CODictRdByte(cod, CO_DEV(idx, 0), &mapnum);
    if (err != CO_ERR_NONE) {
        return (CO_ERR_RPDO_MAP_OBJ);
    }

    dlc = 0;
    for (on = 0; on < mapnum; on++) {
        err = CODictRdLong(cod, CO_DEV(idx, 1 + on), &mapping);
        if (err != CO_ERR_NONE) {
            return (CO_ERR_RPDO_MAP_OBJ);
        }

        size = (uint8_t)(mapping & 0xFF) >> 3;
        dlc += size;
        if (dlc > 8) {
            return (CO_ERR_RPDO_MAP_OBJ);
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
            obj = CODictFind(&pdo->Node->Dict, mapping);
            if (obj == 0) {
                return (CO_ERR_RPDO_MAP_OBJ);
            } else {
                pdo[num].Map[on + dummy] = obj;
                pdo[num].Size[on + dummy] = size;
            }
        }
    }
    pdo[num].ObjNum = mapnum + dummy;
    return (CO_ERR_NONE);
}

void CORPdoRx(CO_RPDO *pdo, CO_IF_FRM *frm)
{
    int16_t err = 0;

    err = COPdoReceive(frm);
    if (err == 0) {
        if ((pdo->Flag & CO_RPDO_FLG_S_) == 0) {
            CORPdoWrite(pdo, frm);
        } else {
            COSyncRx(&pdo->Node->Sync, frm);
        }
    }
}

CO_RPDO *CORPdoCheck(CO_RPDO *pdo, CO_IF_FRM *frm)
{
    CO_RPDO *result = NULL;
    uint16_t n;

    n = 0;
    while (n < CO_RPDO_N) {
        if ((pdo[n].Flag & CO_RPDO_FLG__E) != 0) {
            if (pdo[n].Identifier == frm->Identifier) {
                result = &pdo[n];
                break;
            }
        }
        n++;
    }
    return (result);
}

void CORPdoWrite(CO_RPDO *pdo, CO_IF_FRM *frm)
{
    CO_OBJ  *obj;
    uint32_t val32;
    uint16_t val16;
    uint8_t  val08;
    uint8_t  on;
    uint8_t  sz;
    uint8_t  pdosz;
    uint8_t  dlc = 0;

    for (on = 0; on < pdo->ObjNum; on++) {
        obj   = pdo->Map[on];
        pdosz = pdo->Size[on];
        if (obj != 0) {
            if (pdosz <= 4) {
                /* supported mapping: 1 to 4 bytes */
                sz = (uint8_t)COObjGetSize(obj, pdo->Node, 0L);
                if (sz == 1u) {
                    val08 = CO_GET_BYTE(frm, dlc);
                    dlc++;
                    COObjWrValue(obj, pdo->Node, (void *)&val08, sz);
                } else if (sz == 2u) {
                    val16 = CO_GET_WORD(frm, dlc);
                    dlc += 2;
                    COObjWrValue(obj, pdo->Node, (void *)&val16, sz);
                } else if (sz == 4u) {
                    val32 = CO_GET_LONG(frm, dlc);
                    if (pdosz == 3) {
                        val32 &= 0x00FFFFFF;
                    }
                    dlc += pdosz;
                    COObjWrValue(obj, pdo->Node, (void *)&val32, sz);
                }
            } else {
                CORpdoWriteData(frm, dlc, pdosz, obj);
            }
        }
    }
}
