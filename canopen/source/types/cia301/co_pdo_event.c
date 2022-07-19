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
* PRIVATE DEFINES
******************************************************************************/

#define COT_ENTRY_SIZE    (uint32_t)2

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/* type functions */
static uint32_t COTPdoEventSize (struct CO_OBJ_T *, struct CO_NODE_T *, uint32_t);
static CO_ERR   COTPdoEventRead (struct CO_OBJ_T *, struct CO_NODE_T *, void*, uint32_t);
static CO_ERR   COTPdoEventWrite(struct CO_OBJ_T *, struct CO_NODE_T *, void*, uint32_t);

/******************************************************************************
* PUBLIC GLOBALS
******************************************************************************/

const CO_OBJ_TYPE COTPdoEvent = { COTPdoEventSize, 0, COTPdoEventRead, COTPdoEventWrite };

/******************************************************************************
* PRIVATE TYPE FUNCTIONS
******************************************************************************/

static uint32_t COTPdoEventSize(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    const CO_OBJ_TYPE *uint16 = CO_TUNSIGNED16;
    return uint16->Size(obj, node, width);
}

static CO_ERR COTPdoEventRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *val, uint32_t len)
{
    const CO_OBJ_TYPE *uint16 = CO_TUNSIGNED16;
    return uint16->Read(obj, node, val, len);
}

static CO_ERR COTPdoEventWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *val, uint32_t size)
{
    const CO_OBJ_TYPE *uint16 = CO_TUNSIGNED16;
    CO_DICT  *cod;
    CO_NMT   *nmt;
    CO_TPDO  *pdo;
    CO_TMR   *tmr;
    uint32_t  cobid = 0;
    uint16_t  cycTime;
    uint16_t  num;
    int16_t   tid;
    CO_ERR    err;

    UNUSED(size);

    /* update value in object entry */
    cycTime = (uint16_t)(*(uint16_t *)val);
    err = uint16->Write(obj, node, &cycTime, sizeof(cycTime));
    if (err != CO_ERR_NONE) {
        return (err);
    }

    /* identify the corresponding TPDO */
    num  = CO_GET_IDX(obj->Key);
    num &= 0x1FF;
    pdo  = &node->TPdo[num];

    /* clear already running timer (event and inhibit) */
    tmr = &pdo->Node->Tmr;
    if (pdo->EvTmr >= 0) {
        tid = COTmrDelete(tmr, pdo->EvTmr);
        if (tid < 0) {
            return (CO_ERR_TYPE_WR);
        }
    }
    if (pdo->InTmr >= 0) {
        tid = COTmrDelete(tmr, pdo->InTmr);
        if (tid < 0) {
            return (CO_ERR_TYPE_WR);
        }
    }

    /* start new timer for event when TPDO COB-ID is enabled */
    cod = &node->Dict;
    (void)CODictRdLong(cod, CO_DEV(0x1800+num, 1), &cobid);
    if ((cobid & CO_TPDO_COBID_OFF) == 0) {
        nmt = &node->Nmt;
        if (nmt->Mode == CO_OPERATIONAL) {
            pdo->Event = COTmrGetTicks(tmr, cycTime, CO_TMR_UNIT_1MS);
            if (pdo->Event > 0) {
                pdo->EvTmr = COTmrCreate(tmr,
                                        pdo->Event,
                                        0,
                                        COTPdoTmrEvent,
                                        (void*)pdo);
            }
        }
    }
    return (CO_ERR_NONE);
}
