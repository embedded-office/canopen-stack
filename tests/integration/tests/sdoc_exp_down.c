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

/*---------------------------------------------------------------------------*/
/*!
* \addtogroup csdo_exp_down
* \details    This test suite checks the protocol for expedited SDO download
*             (e.g. SDO client reads data from SDO server).
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

static TS_CALLBACK CSdoExpDownCb;

/*---------------------------------------------------------------------------*/
/*!
* \brief    Use SDO client to write a byte to SDO server
*
*           The SDO client #0 is used on testing node with Node-Id 1 to send
*           a byte to the SDO server #0 on device with Node-Id 5.
*/
/*---------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_CSdoWr_1Byte)
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
    err = COCSdoRequestDownload(csdo,
                                CO_DEV(idx, sub),
                                &val, sizeof(val),
                                TS_AppCSdoCallback,
                                timeout);
    TS_ASSERT(err == CO_ERR_NONE);

    /* -- CHECK REQUEST -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x2F);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, val);

    /* -- SIMULATE RESPONSE -- */
    TS_SDO5_SEND (0x60, idx, sub, 0x00000000);

    /* -- CHECK TRANSFER FINISHED -- */
    CHK_CB_CSDO_FINISHED(&CSdoExpDownCb, 1);
    CHK_CB_CSDO_CODE(&CSdoExpDownCb, 0);

    CHK_NO_ERR(&node);
}

/*---------------------------------------------------------------------------*/
/*!
* \brief    Use SDO client to write a word to SDO server
*
*           The SDO client #0 is used on testing node with Node-Id 1 to send
*           a word to the SDO server #0 on device with Node-Id 5.
*/
/*---------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_CSdoWr_1Word)
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
    err = COCSdoRequestDownload(csdo,
                                CO_DEV(idx, sub),
                                (uint8_t *)&val, sizeof(val),
                                TS_AppCSdoCallback,
                                timeout);
    TS_ASSERT(err == CO_ERR_NONE);

    /* -- CHECK REQUEST -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x2B);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, val);

    /* -- SIMULATE RESPONSE -- */
    TS_SDO5_SEND (0x60, idx, sub, 0x00000000);

    /* -- CHECK TRANSFER FINISHED -- */
    CHK_CB_CSDO_FINISHED(&CSdoExpDownCb, 1);
    CHK_CB_CSDO_CODE(&CSdoExpDownCb, 0);

    CHK_NO_ERR(&node);
}

/*---------------------------------------------------------------------------*/
/*!
* \brief    Use SDO client to write a long to SDO server
*
*           The SDO client #0 is used on testing node with Node-Id 1 to send
*           a long to the SDO server #0 on device with Node-Id 5.
*/
/*---------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_CSdoWr_1Long)
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
    err = COCSdoRequestDownload(csdo,
                                CO_DEV(idx, sub),
                                (uint8_t *)&val, sizeof(val),
                                TS_AppCSdoCallback,
                                timeout);
    TS_ASSERT(err == CO_ERR_NONE);

    /* -- CHECK REQUEST -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x23);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, val);

    /* -- SIMULATE RESPONSE -- */
    TS_SDO5_SEND (0x60, idx, sub, 0x00000000);

    /* -- CHECK TRANSFER FINISHED -- */
    CHK_CB_CSDO_FINISHED(&CSdoExpDownCb, 1);
    CHK_CB_CSDO_CODE(&CSdoExpDownCb, 0);

    CHK_NO_ERR(&node);
}

/*---------------------------------------------------------------------------*/
/*!
* \brief    Use SDO client to handle a timeout
*
*           The SDO client #0 is used on testing node with Node-Id 1 to try
*           to send a byte to the SDO server #0 on device with Node-Id 5,
*           but no response within timeout is sent.
*/
/*---------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_CSdoWr_Timeout)
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
    err = COCSdoRequestDownload(csdo,
                                CO_DEV(idx, sub),
                                &val, sizeof(val),
                                TS_AppCSdoCallback,
                                timeout);
    TS_ASSERT(err == CO_ERR_NONE);

    /* -- CHECK REQUEST -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x2F);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, val);

    /* -- SIMULATE RESPONSE -- */
    TS_Wait(&node, 50);
    CHK_CB_CSDO_FINISHED(&CSdoExpDownCb, 0);

    TS_Wait(&node, 100);
    CHK_CB_CSDO_FINISHED(&CSdoExpDownCb, 1);

    /* -- CHECK TRANSFER FINISHED -- */
    CHK_CB_CSDO_FINISHED(&CSdoExpDownCb, 1);
    CHK_CB_CSDO_CODE(&CSdoExpDownCb, 0x05040000);

    CHK_NO_ERR(&node);
}

/*---------------------------------------------------------------------------*/
/*!
* \brief    Use SDO client to handle a abort
*
*           The SDO client #0 is used on testing node with Node-Id 1 to try
*           to send a byte to the SDO server #0 on device with Node-Id 5,
*           but an abort response is sent.
*/
/*---------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_CSdoWr_About)
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
    err = COCSdoRequestDownload(csdo,
                                CO_DEV(idx, sub),
                                &val, sizeof(val),
                                TS_AppCSdoCallback,
                                timeout);
    TS_ASSERT(err == CO_ERR_NONE);

    /* -- CHECK REQUEST -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x2F);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, val);

    /* -- SIMULATE RESPONSE -- */
    TS_SDO5_SEND (0x80, idx, sub, 0x06020000);

    /* -- CHECK TRANSFER FINISHED -- */
    CHK_CB_CSDO_FINISHED(&CSdoExpDownCb, 1);
    CHK_CB_CSDO_CODE(&CSdoExpDownCb, 0x06020000);
    CHK_NO_ERR(&node);
}

static void CSdoExpDownSetup(void)
{
    TS_CallbackInit(&CSdoExpDownCb);
}

static void CSdoExpDownCleanup(void)
{
    TS_CallbackDeInit();
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_CSDO_EXP_DOWN()
{
    TS_Begin(__FILE__);
    TS_SetupCase(CSdoExpDownSetup, CSdoExpDownCleanup);

    TS_RUNNER(TS_CSdoWr_1Byte);
    TS_RUNNER(TS_CSdoWr_1Word);
    TS_RUNNER(TS_CSdoWr_1Long);

    TS_RUNNER(TS_CSdoWr_Timeout);
    TS_RUNNER(TS_CSdoWr_About);

    TS_End();
}

#endif

/*! @} */
