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

#include "co_para.h"

/******************************************************************************
* FUNCTIONS
******************************************************************************/

/*
* see function definition
*/
void CONodeInit(CO_NODE *node, CO_NODE_SPEC *spec)
{
    int16_t  err;

    node->If.Drv   = spec->CanDrv;
    node->SdoBuf   = spec->SdoBuf;
    node->Baudrate = spec->Baudrate;
    node->NodeId   = spec->NodeId;
    node->Error    = CO_ERR_NONE;
    node->Nmt.Tmr  = -1;
    err = COLssLoad(&node->Baudrate, &node->NodeId);
    if (err != CO_ERR_NONE) {
        node->Error = CO_ERR_LSS_LOAD;
        return;
    }
    COTmrInit(&node->Tmr, node, spec->TmrMem, spec->TmrNum);
    COIfInit(&node->If, node);
    COIfEnable(&node->If, node->Baudrate);
    err = CODirInit(&node->Dir, node, spec->Dir, spec->DirLen);
    if (err < 0) {
        return;
    }
    CONodeParaLoad(node, CO_RESET_COM);
    CONodeParaLoad(node, CO_RESET_NODE);
    CONmtInit(&node->Nmt, node);
    COSdoInit(node->Sdo, node);
    COTPdoClear(node->TPdo, node);
    CORPdoClear(node->RPdo, node);
    if (spec->EmcyCode != 0) {
        COEmcyInit(&node->Emcy, node, spec->EmcyCode); 
    }
    COSyncInit(&node->Sync, node);
    COLssInit(&node->Lss, node);
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
    COIfClose(&node->If);
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
int16_t CONodeParaLoad(CO_NODE *node, CO_NMT_RESET type)
{
    CO_DIR  *cod;
    CO_OBJ  *obj;
    CO_PARA *pg;
    int16_t  err;
    int16_t  result = 0;
    uint8_t  num    = 0;
    uint8_t  sub;

    cod = &node->Dir;
    err = CODirRdByte(cod, CO_DEV(0x1010, 0), &num);
    if (err != CO_ERR_NONE) {
        node->Error = CO_ERR_NONE;
        return (result);
    }

    for (sub = 1; sub <= num; sub++) {
        obj = CODirFind(cod, CO_DEV(0x1010, sub));
        if (obj != 0) {
            pg = (CO_PARA *)obj->Data;
            if (pg->Type == type) {
                err = COParaLoad(pg);
                if (err != CO_ERR_NONE) {
                    node->Error = CO_ERR_PARA_LOAD;
                    result      = -1;
                }
            }
        }
    }

    return (result);
}

/*
* see function definition
*/
void CONodeProcess(CO_NODE *node)
{
    CO_IF_FRM frm;
    CO_SDO   *srv;
    int16_t   err;
    uint8_t   allowed;
    int16_t   num;

    err = COIfRead(&node->If, &frm);
    if (err < 0) {
        allowed = 0;
    } else {
        allowed = node->Nmt.Allowed;
    }

    err = COLssCheck(&node->Lss, &frm);
    if (err != 0) {
        if (err > 0) {
            (void)COIfSend(&node->If, &frm);
        }
        allowed = 0;
    }

    if ((allowed & CO_SDO_ALLOWED) != 0) {
        srv = COSdoCheck(node->Sdo, &frm);
        if (srv != 0) {
            err = COSdoResponse(srv);
            if (err >= -1) {
                (void)COIfSend(&node->If, &frm);
            }
            allowed = 0;
        }
    }

    if ((allowed & CO_NMT_ALLOWED) != 0) {
        if (CONmtCheck(&node->Nmt, &frm) >= 0) {
            allowed = 0;
        } else if (CONmtHbConsCheck(&node->Nmt, &frm) >= 0) {
            allowed = 0;
        }
    }

    if ((allowed & CO_PDO_ALLOWED) != 0) {
        num = CORPdoCheck(node->RPdo, &frm);
        if (num >= 0) {
            CORPdoRx(node->RPdo, num, &frm);
            allowed = 0;
        }
    }

    if ((allowed & CO_SYNC_ALLOWED) != 0) {
        err = COSyncUpdate(&node->Sync, &frm);
        if (err >= 0) {
            COSyncHandler(&node->Sync);
            allowed = 0;
        }
    }

    if (allowed != 0) {
        COIfReceive(&frm);
    }
}
