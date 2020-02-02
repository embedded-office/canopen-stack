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
void TS_PrepareTmrLists(CO_NODE *node, uint32_t t1, uint32_t t2, uint32_t t3)
{
    COTmrReset(&node->Tmr);

    a11 = COTmrCreate(&node->Tmr, 0, t1+3, TS_TmrFunc, 0);
    a12 = COTmrCreate(&node->Tmr, 0, t1+3, TS_TmrFunc, 0);
    a13 = COTmrCreate(&node->Tmr, 0, t1+3, TS_TmrFunc, 0);

    a21 = COTmrCreate(&node->Tmr, 0, t2+2, TS_TmrFunc, 0);
    a22 = COTmrCreate(&node->Tmr, 0, t2+2, TS_TmrFunc, 0);
    a23 = COTmrCreate(&node->Tmr, 0, t2+2, TS_TmrFunc, 0);

    a31 = COTmrCreate(&node->Tmr, 0, t3+1, TS_TmrFunc, 0);
    a32 = COTmrCreate(&node->Tmr, 0, t3+1, TS_TmrFunc, 0);
    a33 = COTmrCreate(&node->Tmr, 0, t3+1, TS_TmrFunc, 0);

    COTmrService(&node->Tmr);
    COTmrService(&node->Tmr);
    COTmrService(&node->Tmr);
}


TEST_DEF(TS_Tmr_DelA11)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    TS_PrepareTmrLists(&node, 1000, 2000, 3000);

    val = COTmrDelete(&node.Tmr, a11);

    /* check success */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

TEST_DEF(TS_Tmr_DelA12)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    TS_PrepareTmrLists(&node, 1000, 2000, 3000);

    val = COTmrDelete(&node.Tmr, a12);

    /* check success */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

TEST_DEF(TS_Tmr_DelA13)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    TS_PrepareTmrLists(&node, 1000, 2000, 3000);

    val = COTmrDelete(&node.Tmr, a13);

    /* check success */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

TEST_DEF(TS_Tmr_DelA21)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    TS_PrepareTmrLists(&node, 1000, 2000, 3000);

    val = COTmrDelete(&node.Tmr, a21);

    /* check success */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

TEST_DEF(TS_Tmr_DelA22)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    TS_PrepareTmrLists(&node, 1000, 2000, 3000);

    val = COTmrDelete(&node.Tmr, a22);

    /* check success */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

TEST_DEF(TS_Tmr_DelA23)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    TS_PrepareTmrLists(&node, 1000, 2000, 3000);

    val = COTmrDelete(&node.Tmr, a23);

    /* check success */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

TEST_DEF(TS_Tmr_DelA31)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    TS_PrepareTmrLists(&node, 1000, 2000, 3000);

    val = COTmrDelete(&node.Tmr, a31);

    /* check success */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

TEST_DEF(TS_Tmr_DelA32)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    TS_PrepareTmrLists(&node, 1000, 2000, 3000);

    val = COTmrDelete(&node.Tmr, a32);

    /* check success */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

TEST_DEF(TS_Tmr_DelA33)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    TS_PrepareTmrLists(&node, 1000, 2000, 3000);

    val = COTmrDelete(&node.Tmr, a33);

    /* check success */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

TEST_DEF(TS_Tmr_DelA41)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    TS_PrepareTmrLists(&node, 0, 0, 0);

    val = COTmrDelete(&node.Tmr, a11);

    /* check success */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

TEST_DEF(TS_Tmr_DelA42)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    TS_PrepareTmrLists(&node, 0, 0, 0);

    val = COTmrDelete(&node.Tmr, a12);

    /* check success */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

TEST_DEF(TS_Tmr_DelA43)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    TS_PrepareTmrLists(&node, 0, 0, 0);

    val = COTmrDelete(&node.Tmr, a13);

    /* check success */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

TEST_DEF(TS_Tmr_DelA51)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    TS_PrepareTmrLists(&node, 0, 0, 0);

    val = COTmrDelete(&node.Tmr, a21);

    /* check success */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

TEST_DEF(TS_Tmr_DelA52)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    TS_PrepareTmrLists(&node, 0, 0, 0);

    val = COTmrDelete(&node.Tmr, a22);

    /* check success */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

TEST_DEF(TS_Tmr_DelA53)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    TS_PrepareTmrLists(&node, 0, 0, 0);

    val = COTmrDelete(&node.Tmr, a23);

    /* check success */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

TEST_DEF(TS_Tmr_DelA61)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    TS_PrepareTmrLists(&node, 0, 0, 0);

    val = COTmrDelete(&node.Tmr, a31);

    /* check success */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

TEST_DEF(TS_Tmr_DelA62)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    TS_PrepareTmrLists(&node, 0, 0, 0);

    val = COTmrDelete(&node.Tmr, a32);

    /* check success */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

TEST_DEF(TS_Tmr_DelA63)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    TS_PrepareTmrLists(&node, 0, 0, 0);

    val = COTmrDelete(&node.Tmr, a33);

    /* check success */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------------*/
TEST_DEF(TS_Tmr_1Tmr_MaxActs)
{
    uint32_t  num;
    int16_t   val;
    CO_NODE   node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    COTmrReset(&node.Tmr);

    /* create actions */
    for (num = 1; num <= node.Tmr.Max; num++) {
        val = COTmrCreate(&node.Tmr,
                          0, 1000, TS_TmrFunc, 0);
        TS_ASSERT(val >= 0);
    }
    val = COTmrCreate(&node.Tmr,
                      0, 1000,
                      TS_TmrFunc, 0);
    TS_ASSERT(val < 0);
    TS_ASSERT(CO_ERR_TMR_NO_ACT == CONodeGetErr(&node));

    /* delete all actions */
    val = COTmrDelete(&node.Tmr, 1);            /* delete middle action */
    TS_ASSERT(val == 0);

    val = COTmrDelete(&node.Tmr, 0);            /* delete first action  */
    TS_ASSERT(val == 0);

    val = COTmrDelete(&node.Tmr, 1);            /* already deleted action */
    TS_ASSERT(val < 0);

    val = COTmrDelete(&node.Tmr,                /* delete last action */
                      (int16_t)node.Tmr.Max-1);
    TS_ASSERT(val == 0);

    for (num = 2; num < (node.Tmr.Max-1); num++) { /* delete rest of actions */
        val = COTmrDelete(&node.Tmr, (int16_t)num);
        TS_ASSERT(val == 0);
    }

    val = COTmrDelete(&node.Tmr, 1);            /* delete in empty list */
    TS_ASSERT(val < 0);
    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------------*/
TEST_DEF(TS_Tmr_MaxTmrs_1Act)
{
    uint32_t  num;
    int16_t   val;
    CO_NODE   node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);
    COTmrReset(&node.Tmr);

    /* create timers */
    for (num = 1; num <= node.Tmr.Max; num++) {
        val = COTmrCreate(&node.Tmr,
                          0, num * 1000,
                          TS_TmrFunc, 0);
        TS_ASSERT(val >= 0);
    }
    val = COTmrCreate(&node.Tmr,
                      0, num * 1000,
                      TS_TmrFunc, 0);
    TS_ASSERT(val < 0);
    TS_ASSERT(CO_ERR_TMR_NO_ACT == CONodeGetErr(&node));

    /* delete timers */
    val = COTmrDelete(&node.Tmr, 1);            /* delete middle timer */
    TS_ASSERT(val == 0);

    val = COTmrDelete(&node.Tmr, 0);            /* delete first timer */
    TS_ASSERT(val == 0);

    val = COTmrDelete(&node.Tmr, 1);            /* already deleted timer */
    TS_ASSERT(val < 0);

    val = COTmrDelete(&node.Tmr,                /* delete last timer */
                      (int16_t)node.Tmr.Max-1);
    TS_ASSERT(val == 0);

    for (num=2; num < node.Tmr.Max-1; num++) {  /* delete rest of timers */
        val = COTmrDelete(&node.Tmr, (int16_t)num);
        TS_ASSERT(val == 0);
    }

    val = COTmrDelete(&node.Tmr, 1);            /* delete in empty list */
    TS_ASSERT(val < 0);
    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC3
*
*          This testcase will check:
*          - creation of 100ms periodic timer is calling callback function every 100ms
*/
/*------------------------------------------------------------------------------------------------*/
TEST_DEF(TS_Tmr_Period100ms)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);
    COTmrReset(&node.Tmr);

    /* create actions */
    val = COTmrCreate(&node.Tmr,
                      CO_TMR_TICKS(0),
                      CO_TMR_TICKS(100),
                      TS_TmrFunc, 0);
    TS_ASSERT(val >= 0);

    SET_TMR_CNT(0);                       /* clear timer callback calling counter */
    TS_Wait(&node, 50);
    CHK_TMR_CALL(0);                      /* not called after 50ms                */
    TS_Wait(&node, 100);
    CHK_TMR_CALL(1);                      /* 1 time called after 150ms            */
    TS_Wait(&node, 100);
    CHK_TMR_CALL(2);                      /* 2 times called after 250ms           */
    TS_Wait(&node, 100);
    CHK_TMR_CALL(3);                      /* 3 times called after 350ms           */

    val = COTmrDelete(&node.Tmr, val);    /* delete periodic timer callback       */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC4
*
*          This testcase will check:
*          - creation of 100ms oneshot timer is calling callback function once after 100ms
*/
/*------------------------------------------------------------------------------------------------*/
TEST_DEF(TS_Tmr_OneShot100ms)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);
    COTmrReset(&node.Tmr);

    /* create timer */
    val = COTmrCreate(&node.Tmr,
                      CO_TMR_TICKS(100),
                      CO_TMR_TICKS(0),
                      TS_TmrFunc, 0);
    TS_ASSERT(val >= 0);

    SET_TMR_CNT(0);                       /* clear timer callback calling counter     */
    TS_Wait(&node, 50);
    CHK_TMR_CALL(0);                      /* not called after 50ms                    */
    TS_Wait(&node, 100);
    CHK_TMR_CALL(1);                      /* 1 time called after 150ms                */
    TS_Wait(&node, 100);
    CHK_TMR_CALL(1);                      /* 1 time called after 250ms                */
    TS_Wait(&node, 100);
    CHK_TMR_CALL(1);                      /* 1 time called after 350ms                */

    val = COTmrDelete(&node.Tmr, val);    /* delete periodic timer callback           */
    TS_ASSERT(val < 0);
    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC5
*
*          This testcase will check:
*          - creation of 100ms periodic timer with start delay of 200ms is calling callback 
*            function after 200ms and then every 100ms
*/
/*------------------------------------------------------------------------------------------------*/
TEST_DEF(TS_Tmr_StartDelay)
{
    int16_t  val;
    CO_NODE  node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);
    COTmrReset(&node.Tmr);

    /* create timer */
    val = COTmrCreate(&node.Tmr,
                      CO_TMR_TICKS(200),
                      CO_TMR_TICKS(100),
                      TS_TmrFunc, 0);
    TS_ASSERT(val >= 0);

    SET_TMR_CNT(0);                                   /* clear timer callback calling counter     */
    TS_Wait(&node, 50);
    CHK_TMR_CALL(0);                                  /* not called after 50ms                    */
    TS_Wait(&node, 100);
    CHK_TMR_CALL(0);                                  /* not called after 150ms                   */
    TS_Wait(&node, 100);
    CHK_TMR_CALL(1);                                  /* 1 time called after 250ms                */
    TS_Wait(&node, 100);
    CHK_TMR_CALL(2);                                  /* 2 times called after 350ms               */
    TS_Wait(&node, 100);
    CHK_TMR_CALL(3);                                  /* 3 times called after 450ms               */

    val = COTmrDelete(&node.Tmr, val);                /* delete periodic timer callback           */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC6
*
*          This testcase will check:
*          - application periodic timer is executed, even if NMT modes are changed.
*/
/*------------------------------------------------------------------------------------------------*/
TEST_DEF(TS_Tmr_AppTmrAfterNodeReset)
{
    CO_IF_FRM  frm;
    int16_t    val;
    CO_NODE    node;

    TS_CreateMandatoryDir();
    TS_CreateNode(&node);
    COTmrReset(&node.Tmr);

    /* create actions */
    val = COTmrCreate(&node.Tmr,
                      CO_TMR_TICKS(0),
                      CO_TMR_TICKS(100),
                      TS_TmrFunc, 0);
    TS_ASSERT(val >= 0);

    SET_TMR_CNT(0);                                   /* clear timer callback calling counter     */
    TS_Wait(&node, 50);
    CHK_TMR_CALL(0);                                  /* not called after 50ms                    */
    TS_Wait(&node, 100);
    CHK_TMR_CALL(1);                                  /* 1 time called after 150ms (in PRE-OP)    */

    TS_NMT_SEND(0x01, 1);                            /* set node-id 0x01 to operational          */

    TS_Wait(&node, 100);
    CHK_TMR_CALL(2);                                  /* 2 times called after 250ms (in OP)       */

    TS_NMT_SEND(0x82, 1);                            /* perform reset communication on Node #1   */
    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */

    TS_Wait(&node, 100);
    CHK_TMR_CALL(3);                                  /* 3 times called after 350ms (in PRE-OP)   */

    val = COTmrDelete(&node.Tmr, val);                /* delete periodic timer callback           */
    TS_ASSERT(val == 0);
    CHK_NO_ERR(&node);
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_CORE_TMR()
{
    TS_Begin(__FILE__);
    
//    CanDiagnosticOn(0);

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

//    CanDiagnosticOff(0);

    TS_End();
}
