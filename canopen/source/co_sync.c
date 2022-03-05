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
* GLOBAL CONSTANTS
******************************************************************************/

const CO_OBJ_TYPE COTSyncId = { 0, 0, 0, COTypeSyncIdWrite };

const CO_OBJ_TYPE COTSyncCycle = { 0, 0, 0, COTypeSyncCycleWrite };

/******************************************************************************
* FUNCTIONS
******************************************************************************/

/*
* see function definition
*/
void COSyncInit(CO_SYNC *sync, struct CO_NODE_T *node)
{
    CO_ERR  err;
    uint8_t i;

    if ((sync == 0) || (node == 0)) {
        CONodeFatalError();
        return;
    }

    sync->Node  = node;
    sync->Tmr   = -1;
    sync->Cycle =  0;

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
        sync->CobId = 0;
    }

    COSyncProdActivate(sync);
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

    if (frm->Identifier == (sync->CobId & CO_SYNC_COBID_MASK)) {
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

/*
* see function definition
*/
void COSyncProdActivate(CO_SYNC *sync) {
    uint32_t ticks, time;
    CO_ERR   err;
    CO_NODE *node;
    int16_t  tid;

    node = sync->Node;

    if ((sync->CobId & CO_SYNC_COBID_ON) == 0) {
        /* 
         * SYNC producer is disabled, activation is skipped 
         * and system is accepted as is.
         */
        return;
    }

    err = CODictRdLong(&node->Dict, CO_DEV(0x1006, 0), &sync->Cycle);
    if (err != CO_ERR_NONE) {
        /*
         * In case of enabled SYNC producer, entry 1006h is mandatory
         * and invalid read operation results in configuration error.
         */
        node->Error = CO_ERR_CFG_1006_0;
        sync->Cycle = 0;
        return;
    }

    time = COTmrGetMinTime(&node->Tmr, CO_TMR_UNIT_100US);
    if ((time * 100) > sync->Cycle) {
        /* 
         * Provided timer driver has small resolution for configured 
         * value, it is not possible to handle SYNCs on requested 
         * communication cycle.
         * 
         * TODO: refactor SYNC producer to use highest possible 
         * resolution of COTmr API (which is currently 100 us).
         */
        node->Error = CO_ERR_SYNC_RES;
        return;
    }

    if (sync->Tmr >= 0) {
        tid = COTmrDelete(&node->Tmr, sync->Tmr);
        if (tid < 0) {
            node->Error = CO_ERR_TMR_DELETE;
        }
    }

    time = (sync->Cycle / 100);
    if (time > 0) {
        ticks = COTmrGetTicks(&node->Tmr, time, CO_TMR_UNIT_100US);
        sync->Tmr = COTmrCreate(&node->Tmr,
            ticks,
            ticks,
            COSyncProdSend,
            sync);
        if (sync->Tmr < 0) {
            node->Error = CO_ERR_TMR_CREATE;
        }
    } else {
        sync->Tmr = -1;
    }
}

/*
* see function definition
*/
void COSyncProdDeactivate(CO_SYNC *sync) {
    int16_t tid;

    if (sync->Tmr >= 0) {
        tid       = COTmrDelete(&sync->Node->Tmr, sync->Tmr);
        sync->Tmr = -1;
        if (tid < 0) {
            sync->Node->Error = CO_ERR_TMR_DELETE;
        }
    }
}

/*
* see function definition
*/
void COSyncProdSend(void *parg) {
    CO_IF_FRM  frm;
    CO_SYNC   *sync;
    CO_NMT    *nmt; 

    sync = (CO_SYNC *) parg;
    nmt  = (CO_NMT  *) &sync->Node->Nmt;

    /* Do not transmit SYNC frames in case of NMT mode that does now allow it */
    if ((nmt->Allowed & CO_SYNC_ALLOWED) == 0) {
        return;
    }

    CO_SET_ID(&frm, (sync->CobId & CO_SYNC_COBID_MASK));
    CO_SET_DLC(&frm, 0);

    (void)COIfCanSend(&sync->Node->If, &frm);
}

/*
* see function definition
*/
CO_ERR COTypeSyncIdWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t len) {
    CO_ERR result = CO_ERR_NONE;
    CO_SYNC *sync;
    uint32_t nid;
    uint32_t oid;

    (void) len;

    sync = &node->Sync;
    nid = *(uint32_t*) buf;
    (void) COObjRdDirect(obj, &oid, CO_LONG);

    /* when current entry is generating SYNCs, bits 0 to 29 shall not be changed */
    if ((oid & CO_SYNC_COBID_ON) != (uint32_t) 0) {
        if ((nid & CO_SYNC_COBID_MASK) != (oid & CO_SYNC_COBID_MASK)) {
            result = CO_ERR_OBJ_RANGE;
        } else {
            /* SYNC producer deactivation */
            if ((nid & CO_SYNC_COBID_ON) == 0) {
                COSyncProdDeactivate(sync);
            }
            sync->CobId = nid;
            result = COObjWrDirect(obj, &nid, CO_LONG);
            if (result != CO_ERR_NONE) {
                result = CO_ERR_OBJ_RANGE;
            }
        }
    } else {
        /* SYNC producer activation */
        if (((nid & CO_SYNC_COBID_ON) != 0)) {
            sync->CobId = nid;
            COSyncProdActivate(sync);
            if (node->Error == CO_ERR_SYNC_RES) {
                /*
                 * Unable to start timer, return back
                 * the old COB-ID and report error
                 */
                sync->CobId = oid;
                result      = CO_ERR_OBJ_RANGE;
                return (result);
            }
        }
        sync->CobId = nid;
        result = COObjWrDirect(obj, &nid, CO_LONG);
        if (result != CO_ERR_NONE) {
            result = CO_ERR_OBJ_RANGE;
        }
    }

    return (result);
}

CO_ERR COTypeSyncCycleWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t len) {
    CO_ERR   result;
    CO_SYNC *sync;
    uint32_t nus, ous;


    (void) len;
    result  = CO_ERR_NONE;
    sync    = &node->Sync;
    nus     = *(uint32_t *) buf;
    ous     = 0;

    /*
     * Fetch old settings (will be restored in
     * case producer reactivation went wrong
     */
    (void) COObjRdDirect(obj, &ous, CO_LONG);

    result = COObjWrDirect(obj, &nus, CO_LONG);
    if (result != CO_ERR_NONE) {
        /* Oops, write access went wrong */
        return CO_ERR_OBJ_RANGE;
    }

    /* Reactivate sync producer with new cycle value */
    if ((sync->CobId & CO_SYNC_COBID_ON) != 0) {
        COSyncProdActivate(sync);
        if (node->Error == CO_ERR_SYNC_RES) {
            /*
             * Restore old SYNC cycle value because used timer has
             * resolution that is not able to produce SYNCs with new
             * cycle.
             *
             * Object write access was successful once already,
             * no result check is needed.
             */
            (void) COObjWrDirect(obj, &ous, CO_LONG);
            sync->Cycle = ous;
            result      = CO_ERR_OBJ_RANGE;
        }
    }

    return (result);
}
