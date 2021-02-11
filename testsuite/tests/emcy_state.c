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

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/*---------------------------------------------------------------------------*/
/*
*  Check that EMCY message is sent after first registration of an error.
*/
TS_DEF_MAIN(TS_Emcy_TxOnSet)
{
    CO_IF_FRM frm;
    CO_NODE   node;

    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    /* register EMCY-ID #1 without user info */
    COEmcySet(&node.Emcy, 1, 0);
    SimCanRun();

    /* check for a CAN frame */
    CHK_CAN  (&frm);
    /* check EMCY (Id and DLC) */
    CHK_EMCY (frm);
    CHK_WORD (frm, 0, 0x2000);
    /* check error code */
    CHK_BYTE (frm, 2, 0x03);
    /* check manufacturer specific info field */
    CHK_BYTE (frm, 3, 0x00);
    CHK_BYTE (frm, 4, 0x00);
    CHK_BYTE (frm, 5, 0x00);
    CHK_BYTE (frm, 6, 0x00);
    CHK_BYTE (frm, 7, 0x00);

    /* check error free stack execution */
    CHK_NO_ERR(&node);
}

/*---------------------------------------------------------------------------*/
/*
*  This test will check that no EMCY message is sent after the second
*  registration of an error.
*/
TS_DEF_MAIN(TS_Emcy_NoTxOnSetRepeat)
{
    CO_IF_FRM frm;
    CO_NODE   node;

    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    /* register EMCY-ID #2 without user info */
    COEmcySet(&node.Emcy, 2, 0);
    SimCanRun();
    SimCanFlush();

    /* register EMCY-ID #2 a second time */
    COEmcySet(&node.Emcy, 2, 0);
    SimCanRun();

    /* check for no CAN frame */
    CHK_NOCAN(&frm);            

    /* check error free stack execution */
    CHK_NO_ERR(&node);
}

/*---------------------------------------------------------------------------*/
/*
*  This test will check that an EMCY message is sent after clearing
*  a registered error
*/
TS_DEF_MAIN(TS_Emcy_TxOnClr)
{
    CO_IF_FRM frm;
    CO_NODE   node;

    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    /* register EMCY-ID #1 without user info */
    COEmcySet(&node.Emcy, 1, 0);
    SimCanRun();
    SimCanFlush();

    /* clear EMCY-ID #1 */
    COEmcyClr(&node.Emcy, 1);
    SimCanRun();

    /* check for a CAN frame */
    CHK_CAN  (&frm);
    /* check EMCY (Id and DLC) */
    CHK_EMCY (frm);
    /* check error code */
    CHK_WORD (frm, 0, 0x0000);
    /* check error register */
    CHK_BYTE (frm, 2, 0x00);
    /* check manufacturer specific info field */
    CHK_BYTE (frm, 3, 0x00);
    CHK_BYTE (frm, 4, 0x00);
    CHK_BYTE (frm, 5, 0x00);
    CHK_BYTE (frm, 6, 0x00);
    CHK_BYTE (frm, 7, 0x00);

    /* check error free stack execution */
    CHK_NO_ERR(&node);
}

/*---------------------------------------------------------------------------*/
/*
*  This test will check that no EMCY message is sent after clearing
*  a not registered error.
*/
TS_DEF_MAIN(TS_Emcy_NoTxOnClrRepeat)
{
    CO_IF_FRM frm;
    CO_NODE   node;

    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    /* clear not existing EMCY-ID #2 */
    COEmcyClr(&node.Emcy, 2);
    SimCanRun();

    /* check for no CAN frame */
    CHK_NOCAN(&frm);

    /* check error free stack execution */
    CHK_NO_ERR(&node);
}

/*---------------------------------------------------------------------------*/
/*
*  This test will check an EMCY message for is sent after resetting all errors.
*/
TS_DEF_MAIN(TS_Emcy_TxOnReset)
{
    CO_IF_FRM frm;
    CO_NODE   node;

    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    /* register EMCY-ID #1 and #3 without user info */
    COEmcySet(&node.Emcy, 1, 0);
    COEmcySet(&node.Emcy, 3, 0);
    SimCanRun();
    SimCanFlush();

    /* reset all errors */
    COEmcyReset(&node.Emcy, 0);
    SimCanRun();

    /* check for 1st CAN frame */
    CHK_CAN  (&frm);
    /* check EMCY (Id and DLC) */
    CHK_EMCY (frm);
    /* check error code */
    CHK_WORD (frm, 0, 0x0000);
    /* check error register */
    CHK_BYTE (frm, 2, 0x09);
    /* check manufacturer specific info field */
    CHK_BYTE (frm, 3, 0x00);
    CHK_BYTE (frm, 4, 0x00);
    CHK_BYTE (frm, 5, 0x00);
    CHK_BYTE (frm, 6, 0x00);
    CHK_BYTE (frm, 7, 0x00);

    /* check for 2nd CAN frame */
    CHK_CAN  (&frm);
    /* check EMCY (Id and DLC) */
    CHK_EMCY (frm);
    /* check error code */
    CHK_WORD (frm, 0, 0x0000);
    /* check error register */
    CHK_BYTE (frm, 2, 0x00);
    /* check manufacturer specific info field */
    CHK_BYTE (frm, 3, 0x00);
    CHK_BYTE (frm, 4, 0x00);
    CHK_BYTE (frm, 5, 0x00);
    CHK_BYTE (frm, 6, 0x00);
    CHK_BYTE (frm, 7, 0x00);

    /* check error free stack execution */
    CHK_NO_ERR(&node);
}

/*---------------------------------------------------------------------------*/
/*
*  This test will check that no EMCY message is sent after resetting with
*  no active error.
*/
TS_DEF_MAIN(TS_Emcy_NoTxOnResetRepeat)
{
    CO_IF_FRM frm;
    CO_NODE   node;

    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    /* reset all errors */
    COEmcyReset(&node.Emcy, 0);
    SimCanRun();

    /* check for no CAN frame */
    CHK_NOCAN(&frm);

    /* check error free stack execution */
    CHK_NO_ERR(&node);
}

/*---------------------------------------------------------------------------*/
/*
*  This test will check that no EMCY message is sent after resetting via NMT
*  with active error.
*/
TS_DEF_MAIN(TS_Emcy_NoTxOnNmtReset)
{
    CO_IF_FRM frm;
    CO_NODE   node;

    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    /* register EMCY-ID #1 without user info */
    COEmcySet(&node.Emcy, 1, 0);
    SimCanRun();
    SimCanFlush();

    /* node reset (and bootup message) */
    TS_NMT_SEND(129, 0);
    CHK_CAN(&frm);

    /* check for no EMCY CAN frame */
    CHK_NOCAN(&frm);

    /* check error free stack execution */
    CHK_NO_ERR(&node);
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_EMCY_STATE()
{
    TS_Begin(__FILE__);

    TS_RUNNER(TS_Emcy_TxOnSet);
    TS_RUNNER(TS_Emcy_NoTxOnSetRepeat);
    TS_RUNNER(TS_Emcy_TxOnClr);
    TS_RUNNER(TS_Emcy_NoTxOnClrRepeat);
    TS_RUNNER(TS_Emcy_TxOnReset);
    TS_RUNNER(TS_Emcy_NoTxOnResetRepeat);
    TS_RUNNER(TS_Emcy_NoTxOnNmtReset);

    TS_End();
}
