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

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that the correct EMCY message sent after first
*          registration of error #1 (error code 2000h, no user data)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_TxOnSet)
{
    CO_IF_FRM frm;
    CO_NODE        node;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1 without user info      */
    RunSimCan(0, 0);                                  /* run simulated CAN                        */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_EMCY (frm);                                   /* check EMCY (Id and DLC)                  */
    CHK_WORD (frm, 0, 0x2000);                        /* check error code                         */

    CHK_BYTE (frm, 2, 0x03);                          /* check error register                     */

    CHK_BYTE (frm, 3, 0x00);                          /* check manufacturer specific info field   */
    CHK_BYTE (frm, 4, 0x00);
    CHK_BYTE (frm, 5, 0x00);
    CHK_BYTE (frm, 6, 0x00);
    CHK_BYTE (frm, 7, 0x00);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that no EMCY message is sent after the second
*          registration of error #2
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_NoTxOnSetRepeat)
{
    CO_IF_FRM frm;
    CO_NODE        node;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    COEmcySet(&node.Emcy, 2, 0);                      /* register error #2 without user info      */
    RunSimCan(0, 0);                                  /* run simulated CAN                        */
    SimCanFlush(0);                                   /* remove all simulated CAN messages        */

    COEmcySet(&node.Emcy, 2, 0);                      /* register error #2 a second time          */
    RunSimCan(0, 0);                                  /* run simulated CAN                        */

    CHK_NOCAN(&frm);                                  /* check for no CAN frame                   */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that an EMCY message is sent after clearing
*          a registered error
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_TxOnClr)
{
    CO_IF_FRM frm;
    CO_NODE        node;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1 without user info      */
    RunSimCan(0, 0);                                  /* run simulated CAN                        */
    SimCanFlush(0);                                   /* remove all simulated CAN messages        */

    COEmcyClr(&node.Emcy, 1);                         /* clear error #1                           */
    RunSimCan(0, 0);                                  /* run simulated CAN                        */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_EMCY (frm);                                   /* check EMCY (Id and DLC)                  */
    CHK_WORD (frm, 0, 0x0000);                        /* check error code                         */
    CHK_BYTE (frm, 2, 0x00);                          /* check error register                     */
    CHK_BYTE (frm, 3, 0x00);                          /* check manufacturer specific info field   */
    CHK_BYTE (frm, 4, 0x00);
    CHK_BYTE (frm, 5, 0x00);
    CHK_BYTE (frm, 6, 0x00);
    CHK_BYTE (frm, 7, 0x00);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that no EMCY message is sent after the clearing
*          a not registered error.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_NoTxOnClrRepeat)
{
    CO_IF_FRM frm;
    CO_NODE        node;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    COEmcyClr(&node.Emcy, 2);                         /* clear not existing error #2              */
    RunSimCan(0, 0);                                  /* run simulated CAN                        */

    CHK_NOCAN(&frm);                                  /* check for no CAN frame                   */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check an EMCY message for is sent after resetting all errors.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_TxOnReset)
{
    CO_IF_FRM frm;
    CO_NODE        node;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1 without user info      */
    COEmcySet(&node.Emcy, 3, 0);                      /* register error #3 without user info      */
    RunSimCan(0, 0);                                  /* run simulated CAN                        */
    SimCanFlush(0);                                   /* remove all simulated CAN messages        */

    COEmcyReset(&node.Emcy, 0);                       /* reset all errors                         */
    RunSimCan(0, 0);                                  /* run simulated CAN                        */

    CHK_CAN  (&frm);                                  /* check for 1st CAN frame                  */
    CHK_EMCY (frm);                                   /* check EMCY (Id and DLC)                  */
    CHK_WORD (frm, 0, 0x0000);                        /* check error code                         */
    CHK_BYTE (frm, 2, 0x09);                          /* check error register                     */
    CHK_BYTE (frm, 3, 0x00);                          /* check manufacturer specific info field   */
    CHK_BYTE (frm, 4, 0x00);
    CHK_BYTE (frm, 5, 0x00);
    CHK_BYTE (frm, 6, 0x00);
    CHK_BYTE (frm, 7, 0x00);

    CHK_CAN  (&frm);                                  /* check for 2nd CAN frame                  */
    CHK_EMCY (frm);                                   /* check EMCY (Id and DLC)                  */
    CHK_WORD (frm, 0, 0x0000);                        /* check error code                         */
    CHK_BYTE (frm, 2, 0x00);                          /* check error register                     */
    CHK_BYTE (frm, 3, 0x00);                          /* check manufacturer specific info field   */
    CHK_BYTE (frm, 4, 0x00);
    CHK_BYTE (frm, 5, 0x00);
    CHK_BYTE (frm, 6, 0x00);
    CHK_BYTE (frm, 7, 0x00);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that no EMCY message is sent after resetting with no active error.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_NoTxOnResetRepeat)
{
    CO_IF_FRM frm;
    CO_NODE        node;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateNode(&node);

    COEmcyReset(&node.Emcy, 0);                       /* reset all errors                         */
    RunSimCan(0, 0);                                  /* run simulated CAN                        */

    CHK_NOCAN(&frm);                                  /* check for no CAN frame                   */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_EMCY_STATE()
{
    TS_Begin(__FILE__);

//    CanDiagnosticOn(0);

    TS_RUNNER(TS_Emcy_TxOnSet);
    TS_RUNNER(TS_Emcy_NoTxOnSetRepeat);
    TS_RUNNER(TS_Emcy_TxOnClr);
    TS_RUNNER(TS_Emcy_NoTxOnClrRepeat);
    TS_RUNNER(TS_Emcy_TxOnReset);
    TS_RUNNER(TS_Emcy_NoTxOnResetRepeat);

//    CanDiagnosticOff(0);

    TS_End();
}
