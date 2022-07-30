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

static TS_CALLBACK NmtLssCb;

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC1
*
*          This testcase will check:
*          - The LSS request is correctly detected and resumed.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Lss_MsgConsumed)
{
    CO_IF_FRM frm;
    CO_NODE        node;

    /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateNode(&node,0);
    /*------------------------------------------*/
    TS_LSS_SEND( 4, 0, 0);

    CHK_NOCAN(&frm);
    CHK_CB_IF_RECEIVE(&NmtLssCb, 0);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC1
*
*          This testcase will check:
*          - The switching with global switch and configuring the nodeId is working.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Lss_SelGlobal_ChgNodeId)
{
    CO_NODE node;

    /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateNode(&node,0);
    /*------------------------------------------*/

    TS_LSS_SEND( 4, 1, 0);                            /* global switch to configuration mode      */
    TS_LSS_SEND(17,11, 0);                            /* configure node ID                        */
    TS_LSS_SEND(23, 0, 0);                            /* store configuration                      */

    CHK_CB_LSS_ARG_NODE_ID(&NmtLssCb, 11);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC1
*
*          This testcase will check:
*          - The switching with global switch and configuring the nodeId is working.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Lss_SelIdent_WrongVendor)
{
    CO_NODE   node;
    CO_IF_FRM frm;
    uint32_t  vendor = 0x11000011;
    uint32_t  product = 0x22000022;
    uint32_t  revision = 0x33000033;
    uint32_t  serial = 0x44000044;

    /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1018, 1, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&vendor));
    TS_ODAdd(CO_KEY(0x1018, 2, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&product));
    TS_ODAdd(CO_KEY(0x1018, 3, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&revision));
    TS_ODAdd(CO_KEY(0x1018, 4, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&serial));
    TS_CreateNode(&node,0);
    /*------------------------------------------*/

    TS_LSS_SEND_L(64, vendor + 1);
    TS_LSS_SEND_L(65, product);
    TS_LSS_SEND_L(66, revision);
    TS_LSS_SEND_L(67, serial);

    CHK_NOCAN(&frm);
    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC1
*
*          This testcase will check:
*          - The switching with global switch and configuring the nodeId is working.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Lss_SelIdent_WrongProduct)
{
    CO_NODE   node;
    CO_IF_FRM frm;
    uint32_t  vendor = 0x11000011;
    uint32_t  product = 0x22000022;
    uint32_t  revision = 0x33000033;
    uint32_t  serial = 0x44000044;

    /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1018, 1, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&vendor));
    TS_ODAdd(CO_KEY(0x1018, 2, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&product));
    TS_ODAdd(CO_KEY(0x1018, 3, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&revision));
    TS_ODAdd(CO_KEY(0x1018, 4, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&serial));
    TS_CreateNode(&node,0);
    /*------------------------------------------*/

    TS_LSS_SEND_L(64, vendor);
    TS_LSS_SEND_L(65, product + 1);
    TS_LSS_SEND_L(66, revision);
    TS_LSS_SEND_L(67, serial);

    CHK_NOCAN(&frm);
    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC1
*
*          This testcase will check:
*          - The switching with global switch and configuring the nodeId is working.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Lss_SelIdent_WrongRevision)
{
    CO_NODE   node;
    CO_IF_FRM frm;
    uint32_t  vendor = 0x11000011;
    uint32_t  product = 0x22000022;
    uint32_t  revision = 0x33000033;
    uint32_t  serial = 0x44000044;

    /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1018, 1, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&vendor));
    TS_ODAdd(CO_KEY(0x1018, 2, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&product));
    TS_ODAdd(CO_KEY(0x1018, 3, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&revision));
    TS_ODAdd(CO_KEY(0x1018, 4, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&serial));
    TS_CreateNode(&node,0);
    /*------------------------------------------*/

    TS_LSS_SEND_L(64, vendor);
    TS_LSS_SEND_L(65, product);
    TS_LSS_SEND_L(66, revision + 1);
    TS_LSS_SEND_L(67, serial);

    CHK_NOCAN(&frm);
    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC1
*
*          This testcase will check:
*          - The switching with global switch and configuring the nodeId is working.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Lss_SelIdent_WrongSerial)
{
    CO_NODE   node;
    CO_IF_FRM frm;
    uint32_t  vendor = 0x11000011;
    uint32_t  product = 0x22000022;
    uint32_t  revision = 0x33000033;
    uint32_t  serial = 0x44000044;

    /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1018, 1, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&vendor));
    TS_ODAdd(CO_KEY(0x1018, 2, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&product));
    TS_ODAdd(CO_KEY(0x1018, 3, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&revision));
    TS_ODAdd(CO_KEY(0x1018, 4, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&serial));
    TS_CreateNode(&node,0);
    /*------------------------------------------*/

    TS_LSS_SEND_L(64, vendor);
    TS_LSS_SEND_L(65, product);
    TS_LSS_SEND_L(66, revision);
    TS_LSS_SEND_L(67, serial + 1);

    CHK_NOCAN(&frm);
    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC1
*
*          This testcase will check:
*          - The switching with global switch and configuring the nodeId is working.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Lss_SelIdent_Ok)
{
    CO_NODE  node;
    uint32_t vendor = 0x11000011;
    uint32_t product = 0x22000022;
    uint32_t revision = 0x33000033;
    uint32_t serial = 0x44000044;

    /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1018, 1, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&vendor));
    TS_ODAdd(CO_KEY(0x1018, 2, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&product));
    TS_ODAdd(CO_KEY(0x1018, 3, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&revision));
    TS_ODAdd(CO_KEY(0x1018, 4, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&serial));
    TS_CreateNode(&node,0);
    /*------------------------------------------*/

    TS_LSS_SEND_L(64, vendor);
    TS_LSS_SEND_L(65, product);
    TS_LSS_SEND_L(66, revision);
    TS_LSS_SEND_L(67, serial);

    CHK_LSS_OK(68);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

static void NmtLssSetup(void)
{
    TS_CallbackInit(&NmtLssCb);
}

static void NmtLssCleanup(void)
{
    TS_CallbackDeInit();
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_NMT_LSS()
{
    TS_Begin(__FILE__);
    TS_SetupCase(NmtLssSetup, NmtLssCleanup);

    //    CanDiagnosticOn(0);

    TS_RUNNER(TS_Lss_MsgConsumed);
    TS_RUNNER(TS_Lss_SelGlobal_ChgNodeId);
    TS_RUNNER(TS_Lss_SelIdent_WrongVendor);
    TS_RUNNER(TS_Lss_SelIdent_WrongProduct);
    TS_RUNNER(TS_Lss_SelIdent_WrongRevision);
    TS_RUNNER(TS_Lss_SelIdent_WrongSerial);
    TS_RUNNER(TS_Lss_SelIdent_Ok);

    //    CanDiagnosticOff(0);

    TS_End();
}
