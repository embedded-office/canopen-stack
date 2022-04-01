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

#include "co_nmt.h"

#include "co_core.h"

/******************************************************************************
* PRIVATE CONSTANTS
******************************************************************************/

/*! \brief NMT MODE ALLOWED OBJECTS
*
*    This constant codes are used to identify the allowed objects within the
*    specific NMT state machine mode.
*/
static const uint8_t CONmtModeObj[CO_MODE_NUM] = {
    ( 0 ),                       /*!< objects in invalid mode                */

    ( CO_BOOT_ALLOWED ),         /*!< objects in initialization mode         */

    (  CO_SDO_ALLOWED |          /*!< objects in pre-operational mode        */
      CO_SYNC_ALLOWED |
      CO_TIME_ALLOWED |
      CO_EMCY_ALLOWED |
       CO_NMT_ALLOWED ),

    (  CO_PDO_ALLOWED |          /*!< objects in operational mode            */
       CO_SDO_ALLOWED |
      CO_SYNC_ALLOWED |
      CO_TIME_ALLOWED |
      CO_EMCY_ALLOWED |
       CO_NMT_ALLOWED ),

    (  CO_NMT_ALLOWED )          /*!< objects in stop mode                   */
};

/*! \brief NMT MODE ENCODING
*
*    This constant codes are used to encode the NMT state machine mode within
*    heartbeat acc. the standard.
*/
static const uint8_t CONmtModeCode[CO_MODE_NUM] = {
    255,                         /*!< encoding for invalid mode              */
    0,                           /*!< encoding for initialization mode       */
    127,                         /*!< encoding for pre-operational mode      */
    5,                           /*!< encoding for operational mode          */
    4                            /*!< encoding for stopped mode              */
};

/******************************************************************************
* PUBLIC CONSTANTS
******************************************************************************/

const CO_OBJ_TYPE COTNmtHbCons = { 0, 0, COTypeNmtHbConsRead, COTypeNmtHbConsWrite };
const CO_OBJ_TYPE COTNmtHbProd = { 0, 0, 0, COTypeNmtHbProdWrite };

/******************************************************************************
* FUNCTIONS
******************************************************************************/

/*
* see function definition
*/
void CONmtReset(CO_NMT *nmt, CO_NMT_RESET type)
{
    uint8_t nobootup = 1;
#if USE_LSS
    int16_t err;
#endif //USE_LSS

    if (nmt == 0) {
        CONodeFatalError();
        return;
    }
    if (nmt->Mode != CO_INIT) {
        CONmtSetMode(nmt, CO_INIT);
        nobootup = 0;
    }

#if USE_PARAMS
    if (type == CO_RESET_NODE) {
        CONodeParaLoad(nmt->Node, CO_RESET_NODE);
    }
#endif //USE_PARAMS

    if (type <= CO_RESET_COM) {
#if USE_PARAMS
        CONodeParaLoad(nmt->Node, CO_RESET_COM);
#endif //USE_PARAMS
#if USE_LSS
        err = COLssLoad(&nmt->Node->Baudrate, &nmt->Node->NodeId);
        if (err != CO_ERR_NONE) {
            nmt->Node->Error = CO_ERR_LSS_LOAD;
            return;
        }
        COLssInit(&nmt->Node->Lss, nmt->Node);
#endif //USE_LSS
        COTmrClear(&nmt->Node->Tmr);
        CONmtInit(nmt, nmt->Node);
        COSdoInit(nmt->Node->Sdo, nmt->Node);
        COIfCanReset(&nmt->Node->If);
        COEmcyReset(&nmt->Node->Emcy, 1);
        COSyncInit(&nmt->Node->Sync, nmt->Node);
        if (nobootup == 0) {
            CONmtBootup(nmt);
        }
    }
}

/*
* see function definition
*/
void CONmtSetMode(CO_NMT *nmt, CO_MODE mode)
{
    if (nmt == 0) {
        CONodeFatalError();
        return;
    }
    if (nmt->Mode != mode) {
        if (mode == CO_OPERATIONAL) {
            COTPdoInit(nmt->Node->TPdo, nmt->Node);
            CORPdoInit(nmt->Node->RPdo, nmt->Node);
        }
        CONmtModeChange(nmt, mode);
    }
    nmt->Mode    = mode;
    nmt->Allowed = CONmtModeObj[mode];
}

/*
* see function definition
*/
CO_MODE CONmtGetMode(CO_NMT *nmt)
{
    if (nmt == 0) {
        CONodeFatalError();
        return (CO_INVALID);
    }

    return (nmt->Mode);
}

/*
* see function definition
*/
void CONmtSetNodeId(CO_NMT *nmt, uint8_t nodeId)
{
    CO_MODE mode;

    if (nmt == 0) {
        CONodeFatalError();
        return;
    }
    if (nmt->Node == 0) {
        return;
    }
    if (nodeId == 0) {
        nmt->Node->Error = CO_ERR_BAD_ARG;
        return;
    }
    mode = nmt->Mode;
    if (mode != CO_INIT) {
        nmt->Node->Error = CO_ERR_NMT_MODE;
    } else {
        nmt->Node->NodeId = nodeId;
    }
}

/*
* see function definition
*/
uint8_t CONmtGetNodeId(CO_NMT *nmt)
{
    uint8_t result = 0;

    if (nmt == 0) {
        CONodeFatalError();
        return (0);
    }
    if (nmt->Node != 0) {
        result = nmt->Node->NodeId;
    }

    return result;
}

/*
* see function definition
*/
CO_MODE CONmtModeDecode(uint8_t code)
{
    CO_MODE    result = CO_INVALID;
    uint8_t id;

    for (id = 0; id < CO_MODE_NUM; id++) {
        if (CONmtModeCode[id] == code) {
            result = (CO_MODE)id;
        }
    }
    return (result);
}

/*
* see function definition
*/
uint8_t CONmtModeEncode(CO_MODE mode)
{
    uint8_t result = CONmtModeCode[CO_INVALID];

    if (mode < CO_MODE_NUM) {
        result = CONmtModeCode[mode];
    }
    return (result);
}

/*
* see function definition
*/
int16_t CONmtGetHbEvents(CO_NMT *nmt, uint8_t nodeId)
{
    int16_t    result = -1;
    CO_HBCONS *hbc;

    if (nmt == 0) {
        CONodeFatalError();
        return (result);
    }

    hbc = nmt->HbCons;
    while (hbc != 0) {
        if (nodeId == hbc->NodeId) {
            result     = (int16_t)hbc->Event;
            hbc->Event = 0;
        }
        hbc = hbc->Next;
    }

    return (result);
}

/*
* see function definition
*/
CO_MODE CONmtLastHbState(CO_NMT *nmt, uint8_t nodeId)
{
    CO_MODE     result = CO_INVALID;
    CO_HBCONS  *hbc;

    if (nmt == 0) {
        CONodeFatalError();
        return (result);
    }

    hbc = nmt->HbCons;
    while (hbc != 0) {
        if (nodeId == hbc->NodeId) {
            result = hbc->State;
        }
        hbc = hbc->Next;
    }

    return (result);
}

/*
* see function definition
*/
void CONmtHbConsInit(CO_NMT *nmt)
{
    CO_ERR     err;
    CO_ERR     act;
    CO_OBJ    *obj;
    CO_HBCONS *hbc;
    CO_NODE   *node;
    uint8_t    num;

    if (nmt == 0) {
        CONodeFatalError();
        return;
    }
    nmt->HbCons = 0;

    node = nmt->Node;
    obj  = CODictFind(&node->Dict, CO_DEV(0x1016, 0));
    if (obj == 0) {
        node->Error = CO_ERR_NONE;
        return;
    }
    err = COObjRdValue(obj, node, &num, CO_BYTE, 0);
    if ((err != CO_ERR_NONE) || (num < 1)) {
        node->Error = CO_ERR_CFG_1016;
        return;
    }
    while (num > 0) {
        obj = CODictFind(&node->Dict, CO_DEV(0x1016, num));
        if (obj == 0) {
            node->Error = CO_ERR_CFG_1016;
            break;
        }
        hbc = (CO_HBCONS *)obj->Data;
        if (hbc == 0) {
            node->Error = CO_ERR_CFG_1016;
            break;
        }
        act = CONmtHbConsActivate(nmt, hbc, hbc->Time, hbc->NodeId);
        if (act != CO_ERR_NONE) {
            node->Error = act;
        }
        num--;
    }
}

/*
* see function definition
*/
CO_ERR CONmtHbConsActivate(CO_NMT    *nmt,
                            CO_HBCONS *hbc,
                            uint16_t   time,
                            uint8_t    nodeid)
{
    CO_ERR      result  = CO_ERR_NONE;
    int16_t     err;
    CO_HBCONS  *act;
    CO_HBCONS  *prev;
    CO_HBCONS  *found = 0;

    prev = 0;
    act  = nmt->HbCons;
    while (act != 0) {
        if (act->NodeId == nodeid) {
            found = act;
            break;
        }
        prev = act;
        act  = act->Next;
    }

    if (found != 0) {
        if (time > 0) {
            result = CO_ERR_OBJ_INCOMPATIBLE;
        } else {
            if (hbc->Tmr >= 0) {
                err = COTmrDelete(&nmt->Node->Tmr, hbc->Tmr);
                if (err < 0) {
                    result = CO_ERR_TMR_DELETE;
                }
            }
            hbc->Time   = time;
            hbc->NodeId = nodeid;
            hbc->Tmr    = -1;
            hbc->Event  = 0;
            hbc->State  = CO_INVALID;
            hbc->Node   = nmt->Node;
            if (prev == 0) {
                nmt->HbCons = hbc->Next;
            } else {
                prev->Next  = hbc->Next;
            }
            hbc->Next   = 0;
        }
    } else {
        hbc->Time   = time;
        hbc->NodeId = nodeid;
        hbc->Tmr    = -1;
        hbc->Event  = 0;
        hbc->State  = CO_INVALID;
        hbc->Node   = nmt->Node;

        if (time > 0) {
            hbc->Next   = nmt->HbCons;
            nmt->HbCons = hbc;
        } else {
            hbc->Next   = 0;
        }
    }

    return (result);
}

/*
* see function definition
*/
int16_t CONmtHbConsCheck(CO_NMT *nmt, CO_IF_FRM *frm)
{
    CO_HBCONS *hbc;
    CO_TMR    *tmr;
    CO_MODE    state;
    int16_t    result = -1;
    uint32_t   cobid;
    uint32_t   ticks;
    uint8_t    nodeid;

    cobid  = frm->Identifier;
    hbc    = nmt->HbCons;
    if (hbc == 0) {
        return (result);
    }
    if ((cobid >= 1792) &&
        (cobid <= 1792 + 127)) {
        nodeid = (uint8_t)(cobid - 1792);
    } else {
        return (result);
    }
    tmr = &nmt->Node->Tmr;
    while (hbc != 0) {
        if (hbc->NodeId != nodeid) {
            hbc = hbc->Next;
        } else {
            if (hbc->Tmr >= 0) {
                COTmrDelete(tmr, hbc->Tmr);
            }
            ticks = COTmrGetTicks(tmr, hbc->Time, CO_TMR_UNIT_1MS);
            hbc->Tmr = COTmrCreate(tmr,
                ticks,
                0,
                CONmtHbConsMonitor,
                hbc);
            if (hbc->Tmr < 0) {
                nmt->Node->Error = CO_ERR_TMR_CREATE;
            }
            state = CONmtModeDecode(frm->Data[0]);
            if (hbc->State != state) {
                CONmtHbConsChange(nmt, hbc->NodeId, state);
            }
            hbc->State = state;
            result     = (int16_t)hbc->NodeId;
            break;
        }
    }

    return (result);
}

/*
* see function definition
*/
void CONmtHbConsMonitor(void *parg)
{
    CO_NODE   *node;
    CO_HBCONS *hbc;
    CO_TMR    *tmr;
    uint32_t   ticks;

    hbc   = (CO_HBCONS *)parg;
    node  = hbc->Node;
    tmr   = &node->Tmr;
    ticks = COTmrGetTicks(tmr, hbc->Time, CO_TMR_UNIT_1MS);
    hbc->Tmr = COTmrCreate(tmr,
        ticks,
        0,
        CONmtHbConsMonitor,
        hbc);
    if (hbc->Tmr < 0) {
        node->Error = CO_ERR_TMR_CREATE;
    }
    if (hbc->Event < 0xFFu) {
        hbc->Event++;
    }
    CONmtHbConsEvent(&node->Nmt, hbc->NodeId);
}

/*
* see function definition
*/
CO_ERR COTypeNmtHbConsWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t size)
{
    CO_ERR      result = CO_ERR_TYPE_WR;
    CO_HBCONS  *hbc;
    uint32_t    value  = 0;
    uint16_t    time;
    uint8_t     nodeid;

    hbc  = (CO_HBCONS *)obj->Data;
    if (hbc == 0) {
        node->Error = CO_ERR_CFG_1016;
        return (result);
    }
    if (size != CO_LONG) {
        node->Error = CO_ERR_CFG_1016;
        return (result);
    }
    value  = *((uint32_t *)buf);
    time   = (uint16_t)value;
    nodeid = (uint8_t)(value >> 16);
    result = CONmtHbConsActivate(&node->Nmt, hbc, time, nodeid);

    return (result);
}

/*
* see function definition
*/
CO_ERR COTypeNmtHbConsRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t len)
{
    CO_ERR     result = CO_ERR_NONE;
    CO_HBCONS *hbc;
    uint32_t   value;
    uint8_t   *src;
    uint8_t   *dst;
    uint32_t   num;

    (void)node;
    hbc    = (CO_HBCONS *)(obj->Data);
    value  = (uint32_t)(hbc->Time);
    value |= ((uint32_t)(hbc->NodeId)) << 16;
    num    = CO_LONG;
    src    = (uint8_t *)&value;
    dst    = (uint8_t *)buf;
    while ((len > 0) && (num > 0)) {
        *dst = *src;
        src++;
        dst++;
        len--;
        num--;
    }

    return (result);
}

/*
* see function definition
*/
void CONmtHbProdInit(CO_NMT *nmt)
{
    CO_NODE *node;
    CO_TMR  *tmr;
    CO_ERR   err;
    int16_t  tid;
    uint16_t cycTime;
    uint32_t ticks;

    if (nmt == 0) {
        CONodeFatalError();
        return;
    }
    node = nmt->Node;
    tmr  = &node->Tmr;
    err  = CODictRdWord(&node->Dict, CO_DEV(0x1017, 0), &cycTime);
    if (err != CO_ERR_NONE) {
        node->Error = CO_ERR_CFG_1017_0;
    }
    if (nmt->Tmr >= 0) {
        tid = COTmrDelete(tmr, nmt->Tmr);
        if (tid < 0) {
            node->Error = CO_ERR_TMR_DELETE;
        }
    }

    if (cycTime > 0) {
        ticks = COTmrGetTicks(tmr, cycTime, CO_TMR_UNIT_1MS);
        nmt->Tmr = COTmrCreate(tmr,
            ticks,
            ticks,
            CONmtHbProdSend,
            nmt);
        if (nmt->Tmr < 0) {
            node->Error = CO_ERR_TMR_CREATE;
        }
    } else {
        nmt->Tmr = -1;
    }
}

/*
* see function definition
*/
void CONmtHbProdSend(void *parg)
{
    CO_IF_FRM  frm;
    CO_NMT    *nmt;
    uint8_t    state;

    nmt = (CO_NMT *)parg;
    if ((nmt->Allowed & CO_NMT_ALLOWED) == 0) {
        return;
    }

    state = CONmtModeEncode(nmt->Mode);

    CO_SET_ID(&frm, 1792 + nmt->Node->NodeId);
    CO_SET_DLC(&frm, 1);
    CO_SET_BYTE(&frm, state, 0);

    (void)COIfCanSend(&nmt->Node->If, &frm);
}

/*
* see function definition
*/
void CONmtInit(CO_NMT *nmt, CO_NODE *node)
{
    if ((nmt == 0) || (node == 0)) {
        CONodeFatalError();
        return;
    }
    nmt->Node = node;
    CONmtSetMode(nmt, CO_INIT);
    CONmtHbProdInit(nmt);
    CONmtHbConsInit(nmt);
}

/*
* see function definition
*/
void CONmtBootup(CO_NMT *nmt)
{
    CO_IF_FRM frm;

    if (nmt->Mode == CO_INIT) {
        CONmtSetMode(nmt, CO_PREOP);

        CO_SET_ID(&frm, 1792 + nmt->Node->NodeId);
        CO_SET_DLC  (&frm, 1);
        CO_SET_BYTE (&frm, 0, 0);

        (void)COIfCanSend(&nmt->Node->If, &frm);
    }
}

/*
* see function definition
*/
int16_t CONmtCheck(CO_NMT *nmt, CO_IF_FRM *frm)
{
    int16_t result = -1;

    if (frm->Identifier == 0) {
        result = 0;
        if ((frm->Data[1] == nmt->Node->NodeId) || (frm->Data[1] == 0)) {
            switch(frm->Data[0]) {
                case 1:
                    CONmtSetMode(nmt, CO_OPERATIONAL);
                    break;
                case 2:
                    CONmtSetMode(nmt, CO_STOP);
                    break;
                case 128:
                    CONmtSetMode(nmt, CO_PREOP);
                    break;
                case 129:
                    CONmtReset(nmt, CO_RESET_NODE);
                    CONmtResetRequest(nmt, CO_RESET_NODE);
                    break;
                case 130:
                    CONmtReset(nmt, CO_RESET_COM);
                    CONmtResetRequest(nmt, CO_RESET_COM);
                    break;
                default:
                    break;
            }
        }
    }

    return(result);
}

/*
* see function definition
*/
CO_ERR COTypeNmtHbProdWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t size)
{
    CO_ERR    result = CO_ERR_OBJ_ACC;
    CO_TMR   *tmr;
    uint32_t  ticks;
    uint16_t  cycTime;
    int16_t   tid;

    if (size != CO_LONG) {
        return (CO_ERR_BAD_ARG);
    }
    if (CO_GET_IDX(obj->Key) != 0x1017) {
        return (CO_ERR_CFG_1017_0);
    }
    cycTime = (uint16_t)(*(uint32_t *)buf);
    tmr = &node->Tmr;
    if (node->Nmt.Tmr >= 0) {
        tid = COTmrDelete(tmr, node->Nmt.Tmr);
        if (tid < 0) {
            node->Error = CO_ERR_TMR_DELETE;
            return (CO_ERR_TMR_DELETE);
        }
        node->Nmt.Tmr = -1;
    }
    if (cycTime > 0) {
        ticks = COTmrGetTicks(tmr, cycTime, CO_TMR_UNIT_1MS);
        node->Nmt.Tmr = COTmrCreate(tmr,
            ticks,
            ticks,
            CONmtHbProdSend,
            &node->Nmt);
        if (node->Nmt.Tmr < 0) {
            node->Error = CO_ERR_TMR_CREATE;
        }
    } else {
        node->Nmt.Tmr = -1;
    }
    result = COObjWrDirect(obj, (void *)&cycTime, CO_WORD);

    return (result);
}
