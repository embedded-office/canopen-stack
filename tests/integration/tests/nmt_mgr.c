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
* PRIVATE VARIABLES
******************************************************************************/

static TS_CALLBACK NmtMgrCb;

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC1
*
*          This testcase will check:
*          - transmission of BootUp message during standard initialization path.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Nmt_Bootup)
{
    CO_IF_FRM frm;
    CO_NODE        node;
    CO_NODE_SPEC   spec;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateSpec(&node, &spec, 0);
                                                      /*------------------------------------------*/
    CONodeInit(&node, &spec);                         /* Init with Node-ID in Specification       */
    CHK_NOCAN (&frm);                                 /* check, that no BootUp message is sent    */

    CONodeStart(&node);                               /* Start communication                      */
    CHK_CAN   (&frm);                                 /* check for a CAN frame                    */
    CHK_BOOTUP(frm, 1);                               /* check for BootUp message of Node-ID #1   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC2
*
*          This testcase will check:
*          - transmission of BootUp message after changing the value during initialization path.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Nmt_BootupChangedNodeId)
{
    CO_IF_FRM frm;
    CO_NODE        node;
    CO_NODE_SPEC   spec;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateSpec(&node, &spec, 0);

    CONodeInit(&node, &spec);                         /* Init with Node-ID in Specification       */
    CHK_NOCAN (&node);                                /* check, that no BootUp message is sent    */

    CONmtSetNodeId(&node.Nmt, 0x55);                  /* Change Node-ID in CANopen node           */

    CONodeStart(&node);                               /* Start communication                      */
    CHK_CAN   (&frm);                                 /* check for a CAN frame                    */
    CHK_BOOTUP(frm, 0x55);                            /* check for BootUp message of Node-ID #1   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC3
*
*          This testcase will check:
*          - no transmission of BootUp message after resetting node during initialization path.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Nmt_NoBootupInInitResetNode)
{
    CO_IF_FRM frm;
    CO_NODE        node;
    CO_NODE_SPEC   spec;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateSpec(&node, &spec, 0);

    CONodeInit(&node, &spec);                         /* Init with Node-ID in Specification       */
    CHK_NOCAN (&frm);                                 /* check, that no BootUp message is sent    */

    CONmtReset(&node.Nmt, CO_RESET_NODE);             /* Reset node                               */
    CHK_NOCAN (&frm);                                 /* check, that no BootUp message is sent    */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC4
*
*          This testcase will check:
*          - no transmission of BootUp message after resetting communication during
*            initialization path.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Nmt_NoBootupInInitResetCom)
{
    CO_IF_FRM frm;
    CO_NODE        node;
    CO_NODE_SPEC   spec;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateSpec(&node, &spec, 0);

    CONodeInit(&node, &spec);                         /* Init with Node-ID in Specification       */
    CHK_NOCAN (&frm);                                 /* check, that no BootUp message is sent    */

    CONmtReset(&node.Nmt, CO_RESET_COM);              /* Reset communication                      */
    CHK_NOCAN (&frm);                                 /* check, that no BootUp message is sent    */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC5
*
*          This testcase will check:
*          - transmission of BootUp message after resetting node in pre-operational mode.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Nmt_TxBootupInPreopResetNode)
{
    CO_IF_FRM frm;
    CO_NODE        node;
    CO_NODE_SPEC   spec;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateSpec(&node, &spec, 0);

    CONodeInit(&node, &spec);                         /* Init with Node-ID in Specification       */
    CHK_NOCAN (&frm);                                 /* check, that no BootUp message is sent    */

    CONodeStart(&node);                               /* Start communication                      */
    CHK_CAN   (&frm);                                 /* check for a CAN frame                    */
    CHK_BOOTUP(frm, 1);                               /* check for BootUp message of Node-ID #1   */

    CONmtReset(&node.Nmt, CO_RESET_NODE);             /* Reset node                               */
    CHK_CAN   (&frm);                                 /* check for a CAN frame                    */
    CHK_BOOTUP(frm, 1);                               /* check for BootUp message of Node-ID #1   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC6
*
*          This testcase will check:
*          - transmission of BootUp message after resetting communication in pre-operational mode.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Nmt_TxBootupInPreopResetCom)
{
    CO_IF_FRM frm;
    CO_NODE        node;
    CO_NODE_SPEC   spec;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateSpec(&node, &spec, 0);

    CONodeInit(&node, &spec);                         /* Init with Node-ID in Specification       */
    CHK_NOCAN (&frm);                                 /* check, that no BootUp message is sent    */

    CONodeStart(&node);                               /* Start communication                      */
    CHK_CAN   (&frm);                                 /* check for a CAN frame                    */
    CHK_BOOTUP(frm, 1);                               /* check for BootUp message of Node-ID #1   */

    CONmtReset(&node.Nmt, CO_RESET_COM);              /* Reset communication                      */
    CHK_CAN   (&frm);                                 /* check for a CAN frame                    */
    CHK_BOOTUP(frm, 1);                               /* check for BootUp message of Node-ID #1   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC7
*
*          This testcase will check:
*          - that no SDO request is accepted during INIT mode
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Nmt_NoSdoInInit)
{
    CO_IF_FRM frm;
    CO_NODE        node;
    CO_NODE_SPEC   spec;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateSpec(&node, &spec, 0);

    CONodeInit(&node, &spec);                         /* Init with Node-ID in Specification       */
    CHK_NOCAN (&frm);                                 /* check, that no BootUp message is sent    */

    TS_SDO_SEND (0x2F, 0x1017, 0, 0x20);

    CHK_NOCAN (&frm);                                 /* check, that no response is sent          */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC8
*
*          This testcase will check:
*          - that no EMCY message is sent during INIT mode
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Nmt_NoEmcyInInit)
{
    CO_IF_FRM frm;
    CO_NODE        node;
    CO_NODE_SPEC   spec;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateSpec(&node, &spec, 0);

    CONodeInit(&node, &spec);                         /* Init with Node-ID in Specification       */
    CHK_NOCAN (&frm);                                 /* check, that no BootUp message is sent    */

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1 without user info      */
    SimCanRun();
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC9
*
*          This testcase will check:
*          - that no NMT message is accepted during INIT mode
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Nmt_NoNmtInInit)
{
    CO_IF_FRM frm;
    CO_NODE        node;
    CO_NODE_SPEC   spec;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateSpec(&node, &spec, 0);

    CONodeInit(&node, &spec);                         /* Init with Node-ID in Specification       */
    CHK_NOCAN (&frm);                                 /* check, that no BootUp message is sent    */

    TS_NMT_SEND(0x01, 1);                            /* request operational mode on Node #1      */
    CHK_NOCAN (&frm);                                 /* check, that no response is sent          */

    CHK_MODE(&node.Nmt, CO_INIT);                     /* check, that mode is unchanged            */
}


/*------------------------------------------------------------------------------------------------*/
/*! \brief TC10
*
*          This testcase will check:
*          - that NMT reset callback is called after application reset
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Nmt_ResetNode)
{
    CO_IF_FRM frm;
    CO_NODE        node;
    CO_NODE_SPEC   spec;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateSpec(&node, &spec, 0);

    CONodeInit(&node, &spec);                         /* Init with Node-ID in Specification       */
    CHK_NOCAN (&frm);                                 /* check, that no BootUp message is sent    */

    CONodeStart(&node);                               /* Start communication                      */
    CHK_CAN   (&frm);                                 /* check for a CAN frame                    */
    CHK_BOOTUP(frm, 1);                               /* check for BootUp message of Node-ID #1   */

    TS_NMT_SEND(129, 1);                              /* request application reset on Node #1     */
    CHK_CAN   (&frm);                                 /* check for a CAN frame                    */
    CHK_BOOTUP(frm, 1);                               /* check for BootUp message of Node-ID #1   */

    CHK_CB_NMT_RESET_REQUEST(&NmtMgrCb, 1);
    CHK_MODE(&node.Nmt, CO_PREOP);                    /* check, that mode is unchanged            */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC10
*
*          This testcase will check:
*          - that NMT reset callback is called after communication reset
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Nmt_ResetCom)
{
    CO_IF_FRM frm;
    CO_NODE        node;
    CO_NODE_SPEC   spec;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateSpec(&node, &spec, 0);

    CONodeInit(&node, &spec);                         /* Init with Node-ID in Specification       */
    CHK_NOCAN (&frm);                                 /* check, that no BootUp message is sent    */

    CONodeStart(&node);                               /* Start communication                      */
    CHK_CAN   (&frm);                                 /* check for a CAN frame                    */
    CHK_BOOTUP(frm, 1);                               /* check for BootUp message of Node-ID #1   */

    TS_NMT_SEND(130, 1);                              /* request communication reset on Node #1   */
    CHK_CAN   (&frm);                                 /* check for a CAN frame                    */
    CHK_BOOTUP(frm, 1);                               /* check for BootUp message of Node-ID #1   */

    CHK_CB_NMT_RESET_REQUEST(&NmtMgrCb, 1);
    CHK_MODE(&node.Nmt, CO_PREOP);                    /* check, that mode is unchanged            */
}

static void NmtMgrSetup(void)
{
    TS_CallbackInit(&NmtMgrCb);
}

static void NmtMgrCleanup(void)
{
    TS_CallbackDeInit();
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_NMT_MGR()
{
    TS_Begin(__FILE__);
    TS_SetupCase(NmtMgrSetup, NmtMgrCleanup);

//    CanDiagnosticOn(0);

    TS_RUNNER(TS_Nmt_Bootup);
    TS_RUNNER(TS_Nmt_BootupChangedNodeId);
    TS_RUNNER(TS_Nmt_NoBootupInInitResetNode);
    TS_RUNNER(TS_Nmt_NoBootupInInitResetCom);
    TS_RUNNER(TS_Nmt_TxBootupInPreopResetNode);
    TS_RUNNER(TS_Nmt_TxBootupInPreopResetCom);
    TS_RUNNER(TS_Nmt_NoSdoInInit);
    TS_RUNNER(TS_Nmt_NoEmcyInInit);
    TS_RUNNER(TS_Nmt_NoNmtInInit);
    TS_RUNNER(TS_Nmt_ResetNode);
    TS_RUNNER(TS_Nmt_ResetCom);

//    CanDiagnosticOff(0);

    TS_End();
}
