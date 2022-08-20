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

/******************************************************************************
* FUNCTIONS
******************************************************************************/

/*
* see function definition
*/
void CONodeInit(CO_NODE *node, CO_NODE_SPEC *spec)
{
    int16_t num;
    CO_ERR  err;

    node->If.Drv   = spec->Drv;
    node->SdoBuf   = spec->SdoBuf;
    node->Baudrate = spec->Baudrate;
    node->NodeId   = spec->NodeId;
    node->Error    = CO_ERR_NONE;
    node->Nmt.Tmr  = -1;
#if USE_LSS
    err = COLssLoad(&node->Baudrate, &node->NodeId);
    if (err != CO_ERR_NONE) {
        node->Error = CO_ERR_LSS_LOAD;
    }
#endif //USE_LSS
    COIfInit(&node->If, node, spec->TmrFreq);
    COTmrInit(&node->Tmr, node, spec->TmrMem, spec->TmrNum, spec->TmrFreq);
    num = CODictInit(&node->Dict, node, spec->Dict, spec->DictLen);
    if (num < 0) {
        node->Error = CO_ERR_DICT_INIT;
    } else {
        CONmtInit(&node->Nmt, node);
        COSdoInit(node->Sdo, node);
    #if USE_CSDO
        COCSdoInit(node->CSdo, node);
    #endif
        COTPdoClear(node->TPdo, node);
        CORPdoClear(node->RPdo, node);
        COEmcyInit(&node->Emcy, node, spec->EmcyCode);
        COSyncInit(&node->Sync, node);
    #if USE_LSS
        COLssInit(&node->Lss, node);
    #endif //USE_LSS
        err = CODictObjInit(&node->Dict, node);
        if (err != CO_ERR_NONE) {
            node->Error = CO_ERR_OBJ_INIT;
        }
        COIfCanEnable(&node->If, node->Baudrate);
    }
}

/*
* see function definition
*/
void CONodeStart(CO_NODE *node)
{
    CO_MODE mode;

    mode = CONmtGetMode(&node->Nmt);
    if (mode == CO_INIT) {
        CONmtBootup(&node->Nmt);
    }
}

/*
* see function definition
*/
void CONodeStop(CO_NODE *node)
{
    COTmrClear(&node->Tmr);
    CONmtSetMode(&node->Nmt, CO_INVALID);
    COIfCanClose(&node->If);
}

/*
* see function definition
*/
CO_ERR CONodeGetErr(CO_NODE *node)
{
    CO_ERR result;

    result      = node->Error;
    node->Error = CO_ERR_NONE;

    return (result);
}

/*
* see function definition
*/
void CONodeProcess(CO_NODE *node)
{
    CO_IF_FRM frm;
    CO_ERR    err;
    CO_SDO   *srv;
#if USE_CSDO
    CO_CSDO  *csdo;
#endif
    CO_RPDO  *rpdo;
    int16_t   result;
    uint8_t   allowed;

    result = COIfCanRead(&node->If, &frm);
    if (result <= 0) {
        allowed = 0;
    } else {
        allowed = node->Nmt.Allowed;
#if USE_LSS
        result  = COLssCheck(&node->Lss, &frm);
        if (result != 0) {
            if (result > 0) {
                (void)COIfCanSend(&node->If, &frm);
            }
            allowed = 0;
        }
#endif //USE_LSS
    }

    if ((allowed & CO_SDO_ALLOWED) != (uint8_t)0) {
        srv = COSdoCheck(node->Sdo, &frm);
        if (srv != NULL) {
            err = COSdoResponse(srv);
            if ((err == CO_ERR_NONE     ) ||
                (err == CO_ERR_SDO_ABORT)) {
                (void)COIfCanSend(&node->If, &frm);
            }
            allowed = 0;
#if USE_CSDO
        } else {
            csdo = COCSdoCheck(node->CSdo, &frm);
            if (csdo != NULL) {
                err = COCSdoResponse(csdo);
                if ((err == CO_ERR_NONE) ||
                    (err == CO_ERR_SDO_ABORT)) {
                    (void)COIfCanSend(&node->If, &frm);
                }
                allowed = 0;
            }
#endif
        }
    }

    if ((allowed & CO_NMT_ALLOWED) != (uint8_t)0) {
        if (CONmtCheck(&node->Nmt, &frm) >= 0) {
            allowed = 0;
        }
        if (CONmtHbConsCheck(&node->Nmt, &frm) >= 0) {
            allowed = 0;
        }
    }

    if ((allowed & CO_PDO_ALLOWED) != (uint8_t)0) {
        rpdo = CORPdoCheck(node->RPdo, &frm);
        if (rpdo != NULL) {
            CORPdoRx(rpdo, &frm);
            allowed = 0;
        }
    }

    if ((allowed & CO_SYNC_ALLOWED) != (uint8_t)0) {
        result = COSyncUpdate(&node->Sync, &frm);
        if (result >= 0) {
            COSyncHandler(&node->Sync);
            allowed = 0;
        }
    }

    if (allowed != (uint8_t)0) {
        COIfCanReceive(&frm);
    }
}
