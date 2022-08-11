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
#define COT_OBJECT        (uint16_t)0x1017
#define COT_COB_ID        (uint32_t)0x00000700

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/* type functions */
static uint32_t COTNmtHbProdSize (struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width);
static CO_ERR   COTNmtHbProdRead (struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTNmtHbProdWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTNmtHbProdInit (struct CO_OBJ_T *obj, struct CO_NODE_T *node);

/* helper functions */
static void CONmtHbProdSend(void *parg);

/******************************************************************************
* PUBLIC GLOBALS
******************************************************************************/

const CO_OBJ_TYPE COTNmtHbProd = { COTNmtHbProdSize, COTNmtHbProdInit, COTNmtHbProdRead, COTNmtHbProdWrite, 0 };

/******************************************************************************
* PRIVATE TYPE FUNCTIONS
******************************************************************************/

static uint32_t COTNmtHbProdSize(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    uint32_t result = (uint32_t)0;

    CO_UNUSED(node);
    CO_UNUSED(width);

    /* check for valid reference */
    if ((obj->Data) != (CO_DATA)0) {
        result = COT_ENTRY_SIZE;
    }
    return (result);
}

static CO_ERR COTNmtHbProdRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *uint16 = CO_TUNSIGNED16;
    return uint16->Read(obj, node, buffer, size);
}

static CO_ERR COTNmtHbProdWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *uint16 = CO_TUNSIGNED16;
    CO_ERR    result = CO_ERR_TYPE_WR;
    CO_TMR   *tmr;
    CO_NMT   *nmt;
    uint32_t  ticks;
    uint16_t  cycTime;
    int16_t   tid;

    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(buffer, CO_ERR_BAD_ARG);
    ASSERT_EQU_ERR(size, COT_ENTRY_SIZE, CO_ERR_BAD_ARG);

    if (CO_GET_IDX(obj->Key) == COT_OBJECT) {
        /* get new cycle time */
        cycTime = *((uint16_t *)buffer);

        /* get active heartbeat producer timer */
        nmt = &node->Nmt;
        tmr = &node->Tmr;
        if (node->Nmt.Tmr >= 0) {
            /* stop timer if active */
            tid = COTmrDelete(tmr, nmt->Tmr);
            if (tid < 0) {
                return (CO_ERR_TMR_DELETE);
            }
            nmt->Tmr = -1;
        }

        /* start time for given cycletime */
        if (cycTime > 0) {
            ticks = COTmrGetTicks(tmr, cycTime, CO_TMR_UNIT_1MS);
            nmt->Tmr = COTmrCreate(tmr, ticks, ticks, CONmtHbProdSend, nmt);
            if (nmt->Tmr < 0) {
                return (CO_ERR_TMR_CREATE);
            }
        } else {
            nmt->Tmr = -1;
        }

        /* use basic type to update the object entry */
        result = uint16->Write(obj, node, &cycTime, sizeof(cycTime));
    }
    return (result);
}

static CO_ERR COTNmtHbProdInit(struct CO_OBJ_T *obj, struct CO_NODE_T *node)
{
    const CO_OBJ_TYPE *uint16 = CO_TUNSIGNED16;
    CO_ERR   result = CO_ERR_TYPE_INIT;
    CO_NMT  *nmt;
    CO_TMR  *tmr;
    int16_t  tid;
    uint16_t cycTime;
    uint32_t ticks;

    ASSERT_PTR_ERR(node, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);

    /* check for heartbeat producer object */
    if (CO_DEV(COT_OBJECT, 0) == CO_GET_DEV(obj->Key)) {
        /* stop timer if heartbeat producer is already active */
        nmt  = &node->Nmt;
        tmr  = &node->Tmr;
        if (nmt->Tmr >= 0) {
            tid = COTmrDelete(tmr, nmt->Tmr);
            if (tid < 0) {
                node->Error = CO_ERR_TMR_DELETE;
                return (result);
            }
        }

        /* start heartbeat producer timer with given cycletime */
        (void)uint16->Read(obj, node, &cycTime, sizeof(cycTime));
        if (cycTime > 0) {
            ticks = COTmrGetTicks(tmr, cycTime, CO_TMR_UNIT_1MS);
            nmt->Tmr = COTmrCreate(tmr,
                ticks,
                ticks,
                CONmtHbProdSend,
                nmt);
            if (nmt->Tmr < 0) {
                node->Error = CO_ERR_TMR_CREATE;
                return (result);
            }
        } else {
            nmt->Tmr = -1;
        }
        result = CO_ERR_NONE;
    }
    return (result);
}

/******************************************************************************
* PRIVATE HELPER FUNCTIONS
******************************************************************************/

static void CONmtHbProdSend(void *parg)
{
    CO_IF_FRM  frm;
    CO_NMT    *nmt;
    uint8_t    state;

    nmt = (CO_NMT *)parg;
    if ((nmt->Allowed & CO_NMT_ALLOWED) == 0) {
        return;
    }

    state = CONmtModeEncode(nmt->Mode);

    CO_SET_ID(&frm, COT_COB_ID + nmt->Node->NodeId);
    CO_SET_DLC(&frm, 1);
    CO_SET_BYTE(&frm, state, 0);

    (void)COIfCanSend(&nmt->Node->If, &frm);
}
