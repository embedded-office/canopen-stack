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

#include "co_nmt.h"

#include "co_core.h"

/******************************************************************************
* PUBLIC CONSTANTS
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

/*! \brief OBJECT TYPE HEARTBEAT CONSUMER
*
*    This object type specializes the general handling of objects for the
*    object directory entry 0x1016. This entries is designed to provide
*    the heartbeat consumer monitor times.
*/
const CO_OBJ_TYPE COTNmtHbCons = {
    0, 0, 0, 0, COTypeNmtHbConsRead, COTypeNmtHbConsWrite
};

/*! \brief OBJECT TYPE HEARTBEAT PRODUCER
*
*    This object type specializes the general handling of objects for the
*    object directory entry 0x1017. This entries is designed to provide
*    the heartbeat producer cycle time.
*/
const CO_OBJ_TYPE COTNmtHbProd = {
    0, 0, 0, 0, 0, COTypeNmtHbProdWrite
};

/******************************************************************************
* FUNCTIONS
******************************************************************************/

/*
* see function definition
*/
void CONmtReset(CO_NMT *nmt, CO_NMT_RESET type)
{
    uint8_t nobootup = 1;
    int16_t err;

    if (nmt == 0) {
        CONodeFatalError();
        return;
    }
    if (nmt->Mode != CO_INIT) {
        CONmtSetMode(nmt, CO_INIT);
        nobootup = 0;
    }

    if (type == CO_RESET_NODE) {
        CONodeParaLoad(nmt->Node, CO_RESET_NODE);     
    }

    if (type <= CO_RESET_COM) {
        CONodeParaLoad(nmt->Node, CO_RESET_COM);      
        err = COLssLoad(&nmt->Node->Baudrate, &nmt->Node->NodeId);
        if (err != CO_ERR_NONE) {
            nmt->Node->Error = CO_ERR_LSS_LOAD;
            return;
        }
        COLssInit(&nmt->Node->Lss, nmt->Node);
        COTmrClear(&nmt->Node->Tmr);
        CONmtInit(nmt, nmt->Node);
        COSdoInit(nmt->Node->Sdo, nmt->Node);
        COIfReset(&nmt->Node->If);
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
    if (mode == CO_OPERATIONAL) {
        COTPdoInit(nmt->Node->TPdo, nmt->Node);
        CORPdoInit(nmt->Node->RPdo, nmt->Node);
    }
    if (nmt->Mode != mode) {
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
    uint8_t    num;
    int16_t    err;
    CO_OBJ    *obj;
    CO_HBCONS *hbc;
    CO_NODE   *node;
    CO_ERR     act;

    if (nmt == 0) {
        CONodeFatalError();
        return;
    }
    nmt->HbCons = 0;

    node = nmt->Node;
    obj  = CODirFind(&node->Dir, CO_DEV(0x1016, 0));
    if (obj == 0) {
        node->Error = CO_ERR_NONE;
        return;
    }
    err = COObjRdValue(obj, &num, CO_BYTE, 0);
    if ((err != CO_ERR_NONE) || (num < 1)) {
        node->Error = CO_ERR_CFG_1016;
        return;
    }
    while (num > 0) {
        obj = CODirFind(&node->Dir, CO_DEV(0x1016, num));
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
    int16_t    result = -1;
    uint32_t   cobid;
    uint8_t    nodeid;
    CO_MODE    state;
    CO_HBCONS *hbc;

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
    while (hbc != 0) {
        if (hbc->NodeId != nodeid) {
            hbc = hbc->Next;
        } else {
            if (hbc->Tmr >= 0) {
                COTmrDelete(&nmt->Node->Tmr, hbc->Tmr);
            } 
            hbc->Tmr = COTmrCreate(&nmt->Node->Tmr,
                CO_TMR_TICKS(hbc->Time),
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

    hbc  = (CO_HBCONS *)parg;
    node = hbc->Node;

    hbc->Tmr = COTmrCreate(&node->Tmr,
        CO_TMR_TICKS(hbc->Time),
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
int16_t COTypeNmtHbConsWrite(CO_OBJ* obj, void *buf, uint32_t size)
{
    CO_NODE    *node;
    CO_DIR     *cod;
    CO_HBCONS  *hbc;
    int16_t     result = CO_ERR_TYPE_WR;
    uint32_t    value  = 0;
    uint16_t    time;
    uint8_t     nodeid;

    cod  = obj->Type->Dir;
    node = cod->Node;
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
int16_t COTypeNmtHbConsRead(CO_OBJ *obj, void *buf, uint32_t len)
{
    CO_HBCONS *hbc;
    int16_t    result = CO_ERR_NONE;
    uint32_t   value;
    uint8_t   *src;
    uint8_t   *dst;
    uint32_t   num;

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
    int16_t  err;
    uint16_t cycTime;

    if (nmt == 0) {
        CONodeFatalError();
        return;
    }
    node = nmt->Node;
    err = CODirRdWord(&node->Dir, CO_DEV(0x1017, 0), &cycTime);
    if (err != CO_ERR_NONE) {
        node->Error = CO_ERR_CFG_1017_0;
    }
    if (nmt->Tmr >= 0) {
        err = COTmrDelete(&node->Tmr, nmt->Tmr);
        if (err < 0) {
            node->Error = CO_ERR_TMR_DELETE;
        }
    }

    if (cycTime > 0) {
        nmt->Tmr = COTmrCreate(&node->Tmr,
            CO_TMR_TICKS(cycTime),
            CO_TMR_TICKS(cycTime),
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

    (void)COIfSend(&nmt->Node->If, &frm);
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

        (void)COIfSend(&nmt->Node->If, &frm);
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
                    break;
                case 130:
                    CONmtReset(nmt, CO_RESET_COM);
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
int16_t COTypeNmtHbProdWrite(CO_OBJ* obj, void *buf, uint32_t size)
{
    CO_NODE  *node;
    uint16_t  cycTime;
    int16_t   result = CO_ERR_OBJ_ACC;

    if (size != CO_LONG) {
        return (CO_ERR_BAD_ARG);
    }
    if (CO_GET_IDX(obj->Key) != 0x1017) {
        return (CO_ERR_CFG_1017_0);
    }
    cycTime = (uint16_t)(*(uint32_t *)buf);
    node    = obj->Type->Dir->Node;

    if (node->Nmt.Tmr >= 0) {
        result = COTmrDelete(&node->Tmr, node->Nmt.Tmr);
        if (result < 0) {
            node->Error = CO_ERR_TMR_DELETE;
            return (CO_ERR_TMR_DELETE);
        }
        node->Nmt.Tmr = -1;
    }
    if (cycTime > 0) {
        node->Nmt.Tmr = COTmrCreate(&node->Tmr,
            CO_TMR_TICKS(cycTime),
            CO_TMR_TICKS(cycTime),
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

void CONmtModeChange(CO_NMT *nmt, CO_MODE mode)
{
    (void)nmt;
    (void)mode;
}

void CONmtHbConsEvent(CO_NMT *nmt, uint8_t nodeId)
{
    (void)nmt;
    (void)nodeId;
}

void CONmtHbConsChange(CO_NMT *nmt, uint8_t nodeId, CO_MODE mode)
{
    (void)nmt;
    (void)nodeId;
    (void)mode;
}
