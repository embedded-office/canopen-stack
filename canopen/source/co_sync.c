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

#include "co_sync.h"

#include "co_core.h"

/******************************************************************************
* FUNCTIONS
******************************************************************************/

/*
* see function definition
*/
void COSyncInit(CO_SYNC *sync, struct CO_NODE_T *node)
{
    int16_t err;
    uint8_t   i;

    if ((sync == 0) || (node == 0)) {
        CONodeFatalError();
        return;
    }

    sync->Node = node;
    for (i = 0; i < CO_TPDO_N; i++) {
        sync->TSync[i] = 0;
        sync->TPdo[i]  = (CO_TPDO *)0;
        sync->TNum[i]  = 0;
    }
    for (i = 0; i < CO_RPDO_N; i++) {
        sync->RPdo[i]  = (CO_RPDO *)0;
    }
    err = CODictRdLong(&node->Dict, CO_DEV(0x1005, 0), &sync->CobId);
    if (err != CO_ERR_NONE) {
        node->Error = CO_ERR_CFG_1005_0;
        sync->CobId = 0;
    }
}

/*
* see function definition
*/
void COSyncHandler (CO_SYNC *sync)
{
    uint8_t i;

    for (i = 0; i < CO_TPDO_N; i++) {
        if (sync->TPdo[i] != 0) {
            if (sync->TNum[i] == 0) {
                COTPdoTx(sync->TPdo[i]);
            } else {
                if (sync->TSync[i] == sync->TNum[i]) {
                    COTPdoTx(sync->TPdo[i]);
                    sync->TSync[i] = 0;
                }
            }
        }
    }

    for (i = 0; i < CO_RPDO_N; i++) {
        if (sync->RPdo[i] != 0) {
            CORPdoWrite(sync->RPdo[i], &sync->RFrm[i]);
            COPdoSyncUpdate(sync->RPdo[i]);
        }
    }
}

/*
* see function definition
*/
void COSyncAdd (CO_SYNC *sync, uint16_t num, uint8_t msgType, uint8_t txtype)
{
    /* transmit pdo */
    if (msgType == CO_SYNC_FLG_TX) {
        if (sync->TPdo[num] == 0) {
            sync->TPdo[num] = &sync->Node->TPdo[num];
        }
        sync->TNum[num]  = txtype;
        sync->TSync[num] = 0;
    }
    /* receive pdo */
    if (msgType == CO_SYNC_FLG_RX) {
        if (sync->RPdo[num] == 0) {
            sync->RPdo[num] = &sync->Node->RPdo[num];
        }
    }
}

/*
* see function definition
*/
void COSyncRemove (CO_SYNC *sync, uint16_t num, uint8_t msgType)
{
    /* transmit pdo */
    if (msgType == CO_SYNC_FLG_TX) {
        sync->TPdo[num]  = 0;
        sync->TNum[num]  = 0;
        sync->TSync[num] = 0;
    }
    /* receive pdo */
    if (msgType == CO_SYNC_FLG_RX) {
        sync->RPdo[num]  = 0;
    }
}

/*
* see function definition
*/
void COSyncRx(CO_SYNC *sync, CO_IF_FRM *frm)
{
    int16_t i;
    int16_t n;

    for (i = 0; i < CO_RPDO_N; i++) {
        if (sync->RPdo[i]->Identifier == frm->Identifier) {
            for (n=0; n < 8; n++) {
                sync->RFrm[i].Data[n] = frm->Data[n];
            }
            sync->RFrm[i].DLC = frm->DLC;
            break;
        }
    }
}

/*
* see function definition
*/
int16_t COSyncUpdate(CO_SYNC *sync, CO_IF_FRM *frm)
{
    int16_t result = -1;
    uint8_t i;

    if (frm->Identifier == sync->CobId) {
        for (i = 0; i < CO_TPDO_N; i++) {
            if (sync->TPdo[i] != 0) {
                sync->TSync[i]++;
            }
        }
        result = 0;
    }

    return (result);
}

/*
* see function definition
*/
void COSyncRestart(CO_SYNC *sync)
{
    uint8_t i;

    for (i = 0; i < CO_TPDO_N; i++) {
        if (sync->TPdo[i] != 0) {
            sync->TSync[i] = 0;
        }
    }
}
