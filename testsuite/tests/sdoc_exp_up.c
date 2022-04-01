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

/*------------------------------------------------------------------------------------------------*/
/*!
* \addtogroup csdo_exp_up
* \details    This test suite checks the protocol for expedited SDO upload
*             (e.g. SDO client writes data to SDO server).
* @{
*/

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "def_suite.h"

#if USE_CSDO

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

static TS_CALLBACK CSdoExpUpCb;

/*---------------------------------------------------------------------------*/
/*!
* \brief    Use SDO client to read a byte from SDO server
*
*           The SDO client #0 is used on testing node with Node-Id 1 to get
*           a byte from the SDO server #0 on device with Node-Id 5.
*/
/*---------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_CSdoRd_1Byte)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    CO_CSDO  *csdo;
    uint8_t   serverId = 5;
    uint32_t  idx = 0x2000;
    uint8_t   sub = 0x01;
    uint32_t  timeout = 1000;
    uint8_t   val = 0x33;
    CO_ERR    err;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_CreateCSdoCom(0, &serverId);
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    csdo = COCSdoFind(&node, 0);
    err = COCSdoRequestUpload(csdo,
                              CO_DEV(idx, sub),
                              &val, sizeof(val),
                              TS_AppCSdoCallback,
                              timeout);
    TS_ASSERT(err == CO_ERR_NONE);

    /* -- CHECK REQUEST -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x40);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, 0);

    /* -- SIMULATE RESPONSE -- */
    TS_SDO5_SEND (0x4F, idx, sub, 0x00000011);

    /* -- CHECK TRANSFER FINISHED -- */
    CHK_CB_CSDO_FINISHED(&CSdoExpUpCb, 1);
    CHK_CB_CSDO_CODE(&CSdoExpUpCb, 0);
    TS_ASSERT((val) == 0x11);

    CHK_NO_ERR(&node);
}

/*---------------------------------------------------------------------------*/
/*!
* \brief    Use SDO client to read a word from SDO server
*
*           The SDO client #0 is used on testing node with Node-Id 1 to get
*           a word from the SDO server #0 on device with Node-Id 5.
*/
/*---------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_CSdoRd_1Word)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    CO_CSDO  *csdo;
    uint8_t   serverId = 5;
    uint32_t  idx = 0x2000;
    uint8_t   sub = 0x01;
    uint32_t  timeout = 1000;
    uint16_t  val = 0x4455;
    CO_ERR    err;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_CreateCSdoCom(0, &serverId);
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    csdo = COCSdoFind(&node, 0);
    err = COCSdoRequestUpload(csdo,
                              CO_DEV(idx, sub),
                              (uint8_t *)&val, sizeof(val),
                              TS_AppCSdoCallback,
                              timeout);
    TS_ASSERT(err == CO_ERR_NONE);

    /* -- CHECK REQUEST -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x40);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, 0);

    /* -- SIMULATE RESPONSE -- */
    TS_SDO5_SEND (0x4B, idx, sub, 0x00002233);

    /* -- CHECK TRANSFER FINISHED -- */
    CHK_CB_CSDO_FINISHED(&CSdoExpUpCb, 1);
    CHK_CB_CSDO_CODE(&CSdoExpUpCb, 0);
    TS_ASSERT((val) == 0x2233);

    CHK_NO_ERR(&node);
}

/*---------------------------------------------------------------------------*/
/*!
* \brief    Use SDO client to read a long from SDO server
*
*           The SDO client #0 is used on testing node with Node-Id 1 to get
*           a long from the SDO server #0 on device with Node-Id 5.
*/
/*---------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_CSdoRd_1Long)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    CO_CSDO  *csdo;
    uint8_t   serverId = 5;
    uint32_t  idx = 0x2000;
    uint8_t   sub = 0x01;
    uint32_t  timeout = 1000;
    uint32_t  val = 0x66778899;
    CO_ERR    err;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_CreateCSdoCom(0, &serverId);
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    csdo = COCSdoFind(&node, 0);
    err = COCSdoRequestUpload(csdo,
                              CO_DEV(idx, sub),
                              (uint8_t *)&val, sizeof(val),
                              TS_AppCSdoCallback,
                              timeout);
    TS_ASSERT(err == CO_ERR_NONE);

    /* -- CHECK REQUEST -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x40);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, 0);

    /* -- SIMULATE RESPONSE -- */
    TS_SDO5_SEND (0x43, idx, sub, 0x44556677);

    /* -- CHECK TRANSFER FINISHED -- */
    CHK_CB_CSDO_FINISHED(&CSdoExpUpCb, 1);
    CHK_CB_CSDO_CODE(&CSdoExpUpCb, 0);
    TS_ASSERT((val) == 0x44556677);

    CHK_NO_ERR(&node);
}

/*---------------------------------------------------------------------------*/
/*!
* \brief    Use SDO client to handle a timeout
*
*           The SDO client #0 is used on testing node with Node-Id 1 to try
*           to get a byte from the SDO server #0 on device with Node-Id 5,
*           but no response within timeout is sent.
*/
/*---------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_CSdoRd_Timeout)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    CO_CSDO  *csdo;
    uint8_t   serverId = 5;
    uint32_t  idx = 0x2000;
    uint8_t   sub = 0x01;
    uint32_t  timeout = 100;
    uint8_t   val = 0x33;
    CO_ERR    err;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_CreateCSdoCom(0, &serverId);
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    csdo = COCSdoFind(&node, 0);
    err = COCSdoRequestUpload(csdo,
                              CO_DEV(idx, sub),
                              &val, sizeof(val),
                              TS_AppCSdoCallback,
                              timeout);
    TS_ASSERT(err == CO_ERR_NONE);

    /* -- CHECK REQUEST -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x40);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, 0);

    /* -- SIMULATE RESPONSE -- */
    TS_Wait(&node, 50);
    CHK_CB_CSDO_FINISHED(&CSdoExpUpCb, 0);

    TS_Wait(&node, 100);
    CHK_CB_CSDO_FINISHED(&CSdoExpUpCb, 1);

    /* -- CHECK TRANSFER FINISHED -- */
    CHK_CB_CSDO_FINISHED(&CSdoExpUpCb, 1);
    CHK_CB_CSDO_CODE(&CSdoExpUpCb, 0x05040000);
    TS_ASSERT((val) == 0x33);

    CHK_NO_ERR(&node);
}

/*---------------------------------------------------------------------------*/
/*!
* \brief    Use SDO client to handle a abort
*
*           The SDO client #0 is used on testing node with Node-Id 1 to try
*           to get a byte from the SDO server #0 on device with Node-Id 5,
*           but an abort response is sent.
*/
/*---------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_CSdoRd_Abort)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    CO_CSDO  *csdo;
    uint8_t   serverId = 5;
    uint32_t  idx = 0x2000;
    uint8_t   sub = 0x01;
    uint32_t  timeout = 100;
    uint8_t   val = 0x33;
    CO_ERR    err;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_CreateCSdoCom(0, &serverId);
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    csdo = COCSdoFind(&node, 0);
    err = COCSdoRequestUpload(csdo,
                              CO_DEV(idx, sub),
                              &val, sizeof(val),
                              TS_AppCSdoCallback,
                              timeout);
    TS_ASSERT(err == CO_ERR_NONE);

    /* -- CHECK REQUEST -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x40);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, 0);

    /* -- SIMULATE RESPONSE -- */
    TS_SDO5_SEND (0x80, idx, sub, 0x06020000);

    /* -- CHECK TRANSFER FINISHED -- */
    CHK_CB_CSDO_FINISHED(&CSdoExpUpCb, 1);
    CHK_CB_CSDO_CODE(&CSdoExpUpCb, 0x06020000);
    TS_ASSERT((val) == 0x33);

    CHK_NO_ERR(&node);
}

static void CSdoExpUpSetup(void)
{
    TS_CallbackInit(&CSdoExpUpCb);
}

static void CSdoExpUpCleanup(void)
{
    TS_CallbackDeInit();
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_CSDO_EXP_UP()
{
    TS_Begin(__FILE__);
    TS_SetupCase(CSdoExpUpSetup, CSdoExpUpCleanup);

    TS_RUNNER(TS_CSdoRd_1Byte);
    TS_RUNNER(TS_CSdoRd_1Word);
    TS_RUNNER(TS_CSdoRd_1Long);

    TS_RUNNER(TS_CSdoRd_Timeout);
    TS_RUNNER(TS_CSdoRd_Abort);

    TS_End();
}

#endif

/*! @} */
