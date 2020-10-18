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

#include "def_suite.h"
#include "co_core.h"

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

static CO_NODE  TsNode;
static CO_TMR  *TsTmr;
static int16_t  a11, a12, a13;
static int16_t  a21, a22, a23;
static int16_t  a31, a32, a33;

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/*
*       root -> T10 -> A11 -> A12 -> A13 -> NULL    [dt=t1]
*                |
*               T20 -> A21 -> A22 -> A23 -> NULL    [dt=t2]
*                |
*               T30 -> A31 -> A32 -> A33 -> NULL    [dt=t3]
*/
void TS_PrepareTmrLists(uint32_t t1, uint32_t t2, uint32_t t3)
{
    a11 = COTmrCreate(TsTmr, 0, t1+3, TS_TmrFunc, 0);
    a12 = COTmrCreate(TsTmr, 0, t1+3, TS_TmrFunc, 0);
    a13 = COTmrCreate(TsTmr, 0, t1+3, TS_TmrFunc, 0);

    a21 = COTmrCreate(TsTmr, 0, t2+2, TS_TmrFunc, 0);
    a22 = COTmrCreate(TsTmr, 0, t2+2, TS_TmrFunc, 0);
    a23 = COTmrCreate(TsTmr, 0, t2+2, TS_TmrFunc, 0);

    a31 = COTmrCreate(TsTmr, 0, t3+1, TS_TmrFunc, 0);
    a32 = COTmrCreate(TsTmr, 0, t3+1, TS_TmrFunc, 0);
    a33 = COTmrCreate(TsTmr, 0, t3+1, TS_TmrFunc, 0);

    COTmrService(TsTmr);
    COTmrService(TsTmr);
    COTmrService(TsTmr);
}


TEST_DEF(TS_Tmr_DelA11)
{
    int16_t  val;

    TS_PrepareTmrLists(1000, 2000, 3000);

    val = COTmrDelete(TsTmr, a11);
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

TEST_DEF(TS_Tmr_DelA12)
{
    int16_t  val;

    TS_PrepareTmrLists(1000, 2000, 3000);

    val = COTmrDelete(TsTmr, a12);
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

TEST_DEF(TS_Tmr_DelA13)
{
    int16_t  val;

    TS_PrepareTmrLists(1000, 2000, 3000);

    val = COTmrDelete(TsTmr, a13);
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

TEST_DEF(TS_Tmr_DelA21)
{
    int16_t  val;

    TS_PrepareTmrLists(1000, 2000, 3000);

    val = COTmrDelete(TsTmr, a21);
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

TEST_DEF(TS_Tmr_DelA22)
{
    int16_t  val;

    TS_PrepareTmrLists(1000, 2000, 3000);

    val = COTmrDelete(TsTmr, a22);
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

TEST_DEF(TS_Tmr_DelA23)
{
    int16_t  val;

    TS_PrepareTmrLists(1000, 2000, 3000);

    val = COTmrDelete(TsTmr, a23);
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

TEST_DEF(TS_Tmr_DelA31)
{
    int16_t  val;

    TS_PrepareTmrLists(1000, 2000, 3000);

    val = COTmrDelete(TsTmr, a31);
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

TEST_DEF(TS_Tmr_DelA32)
{
    int16_t  val;

    TS_PrepareTmrLists(1000, 2000, 3000);

    val = COTmrDelete(TsTmr, a32);
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

TEST_DEF(TS_Tmr_DelA33)
{
    int16_t  val;

    TS_PrepareTmrLists(1000, 2000, 3000);

    val = COTmrDelete(TsTmr, a33);
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

TEST_DEF(TS_Tmr_DelA41)
{
    int16_t  val;

    TS_PrepareTmrLists(0, 0, 0);

    val = COTmrDelete(TsTmr, a11);
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

TEST_DEF(TS_Tmr_DelA42)
{
    int16_t  val;

    TS_PrepareTmrLists(0, 0, 0);

    val = COTmrDelete(TsTmr, a12);
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

TEST_DEF(TS_Tmr_DelA43)
{
    int16_t  val;

    TS_PrepareTmrLists(0, 0, 0);

    val = COTmrDelete(TsTmr, a13);
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

TEST_DEF(TS_Tmr_DelA51)
{
    int16_t  val;

    TS_PrepareTmrLists(0, 0, 0);

    val = COTmrDelete(TsTmr, a21);
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

TEST_DEF(TS_Tmr_DelA52)
{
    int16_t  val;

    TS_PrepareTmrLists(0, 0, 0);

    val = COTmrDelete(TsTmr, a22);
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

TEST_DEF(TS_Tmr_DelA53)
{
    int16_t  val;

    TS_PrepareTmrLists(0, 0, 0);

    val = COTmrDelete(TsTmr, a23);
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

TEST_DEF(TS_Tmr_DelA61)
{
    int16_t  val;

    TS_PrepareTmrLists(0, 0, 0);

    val = COTmrDelete(TsTmr, a31);
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

TEST_DEF(TS_Tmr_DelA62)
{
    int16_t  val;

    TS_PrepareTmrLists(0, 0, 0);

    val = COTmrDelete(TsTmr, a32);
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

TEST_DEF(TS_Tmr_DelA63)
{
    int16_t  val;

    TS_PrepareTmrLists(0, 0, 0);

    val = COTmrDelete(TsTmr, a33);
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

/*---------------------------------------------------------------------------*/
/*! \brief TC1
*
*          This testcase will check:
*          - creation of maximal actions in single timer
*          - deletion of all actions
*            a) action in middle of list
*            b) first action in list
*            c) already deleted action
*            d) last action in list
*            e) delete action on empty list
*/
/*---------------------------------------------------------------------------*/
TEST_DEF(TS_Tmr_1Tmr_MaxActs)
{
    uint32_t num;
    int16_t  val;

    /* create actions */
    for (num = 1; num <= TsTmr->Max; num++) {
        val = COTmrCreate(TsTmr, 0, 1000, TS_TmrFunc, 0);
        TS_ASSERT(val >= 0);
    }
    val = COTmrCreate(TsTmr, 0, 1000, TS_TmrFunc, 0);
    TS_ASSERT(val < 0);
    TS_ASSERT(CO_ERR_TMR_NO_ACT == CONodeGetErr(&TsNode));

    /* delete all actions */
    val = COTmrDelete(TsTmr, 1);                     /* delete middle action */
    TS_ASSERT(val == 0);

    val = COTmrDelete(TsTmr, 0);                     /* delete first action */
    TS_ASSERT(val == 0);

    val = COTmrDelete(TsTmr, 1);                   /* already deleted action */
    TS_ASSERT(val < 0);

    val = COTmrDelete(TsTmr,                           /* delete last action */
                      (int16_t)TsTmr->Max-1);
    TS_ASSERT(val == 0);

    for (num = 2; num < (TsTmr->Max-1); num++) {   /* delete rest of actions */
        val = COTmrDelete(TsTmr, (int16_t)num);
        TS_ASSERT(val == 0);
    }

    val = COTmrDelete(TsTmr, 1);                     /* delete in empty list */
    TS_ASSERT(val < 0);
    CHK_NO_ERR(&TsNode);
}

/*---------------------------------------------------------------------------*/
/*! \brief TC2
*
*          This testcase will check:
*          - creation of maximal actions in separate timers
*          - deletion of all timers
*            a) timer in middle of list
*            b) first timer in list
*            c) already deleted timer
*            d) last timer in list
*            e) delete timer on empty list
*/
/*---------------------------------------------------------------------------*/
TEST_DEF(TS_Tmr_MaxTmrs_1Act)
{
    uint32_t  num;
    int16_t   val;

    /* create timers */
    for (num = 1; num <= TsTmr->Max; num++) {
        val = COTmrCreate(TsTmr, 0, num * 1000, TS_TmrFunc, 0);
        TS_ASSERT(val >= 0);
    }
    val = COTmrCreate(TsTmr, 0, num * 1000, TS_TmrFunc, 0);
    TS_ASSERT(val < 0);
    TS_ASSERT(CO_ERR_TMR_NO_ACT == CONodeGetErr(&TsNode));

    /* delete timers */
    val = COTmrDelete(TsTmr, 1);                      /* delete middle timer */
    TS_ASSERT(val == 0);

    val = COTmrDelete(TsTmr, 0);                       /* delete first timer */
    TS_ASSERT(val == 0);

    val = COTmrDelete(TsTmr, 1);                    /* already deleted timer */
    TS_ASSERT(val < 0);

    val = COTmrDelete(TsTmr,                            /* delete last timer */
                      (int16_t)TsTmr->Max-1);
    TS_ASSERT(val == 0);

    for (num=2; num < TsTmr->Max-1; num++) {        /* delete rest of timers */
        val = COTmrDelete(TsTmr, (int16_t)num);
        TS_ASSERT(val == 0);
    }

    val = COTmrDelete(TsTmr, 1);                     /* delete in empty list */
    TS_ASSERT(val < 0);
    CHK_NO_ERR(&TsNode);
}

/*---------------------------------------------------------------------------*/
/*! \brief TC3
*
*          This testcase will check:
*          - creation of 100ms periodic timer is calling callback
*            function every 100ms
*/
/*---------------------------------------------------------------------------*/
TEST_DEF(TS_Tmr_Period100ms)
{
    int16_t  val;
    uint32_t start;
    uint32_t cycle;

    /* create actions */
    start = COTmrGetTicks(TsTmr, 0, CO_TMR_UNIT_1MS);
    cycle = COTmrGetTicks(TsTmr, 100, CO_TMR_UNIT_1MS);
    val   = COTmrCreate(TsTmr, start, cycle, TS_TmrFunc, 0);
    TS_ASSERT(val >= 0);

    SET_TMR_CNT(0);                  /* clear timer callback calling counter */
    TS_Wait(&TsNode, 50);
    CHK_TMR_CALL(0);                                /* not called after 50ms */
    TS_Wait(&TsNode, 100);
    CHK_TMR_CALL(1);                            /* 1 time called after 150ms */
    TS_Wait(&TsNode, 100);
    CHK_TMR_CALL(2);                           /* 2 times called after 250ms */
    TS_Wait(&TsNode, 100);
    CHK_TMR_CALL(3);                           /* 3 times called after 350ms */

    val = COTmrDelete(TsTmr, val);         /* delete periodic timer callback */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

/*---------------------------------------------------------------------------*/
/*! \brief TC4
*
*          This testcase will check:
*          - creation of 100ms oneshot timer is calling callback 
*            function once after 100ms
*/
/*---------------------------------------------------------------------------*/
TEST_DEF(TS_Tmr_OneShot100ms)
{
    int16_t  val;
    uint32_t start;
    uint32_t cycle;

    /* create timer */
    start = COTmrGetTicks(TsTmr, 100, CO_TMR_UNIT_1MS);
    cycle = COTmrGetTicks(TsTmr, 0, CO_TMR_UNIT_1MS);
    val   = COTmrCreate(TsTmr, start, cycle, TS_TmrFunc, 0);
    TS_ASSERT(val >= 0);

    SET_TMR_CNT(0);                  /* clear timer callback calling counter */
    TS_Wait(&TsNode, 50);
    CHK_TMR_CALL(0);                                /* not called after 50ms */
    TS_Wait(&TsNode, 100);
    CHK_TMR_CALL(1);                            /* 1 time called after 150ms */
    TS_Wait(&TsNode, 100);
    CHK_TMR_CALL(1);                            /* 1 time called after 250ms */
    TS_Wait(&TsNode, 100);
    CHK_TMR_CALL(1);                            /* 1 time called after 350ms */

    val = COTmrDelete(TsTmr, val);         /* delete periodic timer callback */
    TS_ASSERT(val < 0);
    CHK_NO_ERR(&TsNode);
}

/*---------------------------------------------------------------------------*/
/*! \brief TC5
*
*          This testcase will check:
*          - creation of 100ms periodic timer with start delay of 200ms
*            is calling callback function after 200ms and then every 100ms
*/
/*---------------------------------------------------------------------------*/
TEST_DEF(TS_Tmr_StartDelay)
{
    int16_t  val;
    uint32_t start;
    uint32_t cycle;

    /* create timer */
    start = COTmrGetTicks(TsTmr, 200, CO_TMR_UNIT_1MS);
    cycle = COTmrGetTicks(TsTmr, 100, CO_TMR_UNIT_1MS);
    val   = COTmrCreate(TsTmr, start, cycle, TS_TmrFunc, 0);
    TS_ASSERT(val >= 0);

    SET_TMR_CNT(0);                  /* clear timer callback calling counter */
    TS_Wait(&TsNode, 50);
    CHK_TMR_CALL(0);                                /* not called after 50ms */
    TS_Wait(&TsNode, 100);
    CHK_TMR_CALL(0);                               /* not called after 150ms */
    TS_Wait(&TsNode, 100);
    CHK_TMR_CALL(1);                            /* 1 time called after 250ms */
    TS_Wait(&TsNode, 100);
    CHK_TMR_CALL(2);                           /* 2 times called after 350ms */
    TS_Wait(&TsNode, 100);
    CHK_TMR_CALL(3);                           /* 3 times called after 450ms */

    val = COTmrDelete(TsTmr, val);         /* delete periodic timer callback */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

/*---------------------------------------------------------------------------*/
/*! \brief TC6
*
*          This testcase will check:
*          - application periodic timer is executed, even if NMT modes
*            are changed.
*/
/*---------------------------------------------------------------------------*/
TEST_DEF(TS_Tmr_AppTmrAfterNodeReset)
{
    CO_IF_FRM  frm;
    int16_t    val;
    uint32_t   start;
    uint32_t   cycle;

    /* create actions */
    start = COTmrGetTicks(TsTmr, 0, CO_TMR_UNIT_1MS);
    cycle = COTmrGetTicks(TsTmr, 100, CO_TMR_UNIT_1MS);
    val   = COTmrCreate(TsTmr, start, cycle, TS_TmrFunc, 0);
    TS_ASSERT(val >= 0);

    SET_TMR_CNT(0);                  /* clear timer callback calling counter */
    TS_Wait(&TsNode, 50);
    CHK_TMR_CALL(0);                                /* not called after 50ms */
    TS_Wait(&TsNode, 100);
    CHK_TMR_CALL(1);                /* 1 time called after 150ms (in PRE-OP) */

    TS_NMT_SEND(0x01, 1);                 /* set node-id 0x01 to operational */

    TS_Wait(&TsNode, 100);
    CHK_TMR_CALL(2);                   /* 2 times called after 250ms (in OP) */

    TS_NMT_SEND(0x82, 1);          /* perform reset communication on Node #1 */
    CHK_CAN  (&frm);                                /* check for a CAN frame */

    TS_Wait(&TsNode, 100);
    CHK_TMR_CALL(3);               /* 3 times called after 350ms (in PRE-OP) */

    val = COTmrDelete(TsTmr, val);         /* delete periodic timer callback */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&TsNode);
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

static void Setup(void)
{
    TS_CreateMandatoryDir();
    TS_CreateNode(&TsNode, 0);

    TsTmr = &TsNode.Tmr;
}

SUITE_CORE_TMR()
{
    TS_Begin(__FILE__);
    TS_SetupCase(Setup, NULL);
    
    TS_RUNNER(TS_Tmr_DelA11);
    TS_RUNNER(TS_Tmr_DelA12);
    TS_RUNNER(TS_Tmr_DelA13);
    TS_RUNNER(TS_Tmr_DelA21);
    TS_RUNNER(TS_Tmr_DelA22);
    TS_RUNNER(TS_Tmr_DelA23);
    TS_RUNNER(TS_Tmr_DelA31);
    TS_RUNNER(TS_Tmr_DelA32);
    TS_RUNNER(TS_Tmr_DelA33);

    TS_RUNNER(TS_Tmr_DelA41);
    TS_RUNNER(TS_Tmr_DelA42);
    TS_RUNNER(TS_Tmr_DelA43);
    TS_RUNNER(TS_Tmr_DelA51);
    TS_RUNNER(TS_Tmr_DelA52);
    TS_RUNNER(TS_Tmr_DelA53);
    TS_RUNNER(TS_Tmr_DelA61);
    TS_RUNNER(TS_Tmr_DelA62);
    TS_RUNNER(TS_Tmr_DelA63);

    TS_RUNNER(TS_Tmr_1Tmr_MaxActs);
    TS_RUNNER(TS_Tmr_MaxTmrs_1Act);
    TS_RUNNER(TS_Tmr_Period100ms);
    TS_RUNNER(TS_Tmr_OneShot100ms);
    TS_RUNNER(TS_Tmr_StartDelay);
    TS_RUNNER(TS_Tmr_AppTmrAfterNodeReset);

    TS_End();
}
