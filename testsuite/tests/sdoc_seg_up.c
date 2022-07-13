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
* \addtogroup csdo_seg_up
* \details    This test suite checks the protocol for segmented SDO upload
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

static TS_CALLBACK CSdoSegUpCb;

/*---------------------------------------------------------------------------*/
/*!
* \brief    Use SDO client to read a 16 byte domain from the SDO server
*
*           The SDO client #0 is used on testing node with Node-Id 1 to send
*           a byte to the SDO server #0 on device with Node-Id 5.
*/
/*---------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_CSdoRd_Seg16ByteDomain)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    CO_CSDO  *csdo;
    uint8_t   serverId = 5;
    uint32_t  idx = 0x2000;
    uint8_t   sub = 0x01;
    uint32_t  timeout = 1000;
    uint8_t   val[16] = { 0 };
    CO_ERR    err;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_CreateCSdoCom(0, &serverId);
    TS_CreateNode(&node, 0);
    csdo = COCSdoFind(&node, 0);

    /* -- TEST -- */
    err = COCSdoRequestUpload(csdo,
                              CO_DEV(idx, sub),
                              &val[0], 16,
                              TS_AppCSdoCallback,
                              timeout);
    TS_ASSERT(err == CO_ERR_NONE);

    /* -- CHECK SDO INIT REQUEST -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x40);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, 0);

    /* -- SERVER INIT RESPONSE: OK -- */
    TS_SDO5_SEND (0x41, idx, sub, 16);

    /* -- CHECK SDO UPLOAD REQUEST #1 -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x60);
    CHK_BYTE (frm, 1, 0);
    CHK_BYTE (frm, 2, 0);
    CHK_BYTE (frm, 3, 0);
    CHK_BYTE (frm, 4, 0);
    CHK_BYTE (frm, 5, 0);
    CHK_BYTE (frm, 6, 0);
    CHK_BYTE (frm, 7, 0);

    /* -- SERVER UPLOAD RESPONSE #1: FIRST DATA SEGMENT -- */
    TS_SEG5_SEND (0x00, 0x04030201, 0x070605);

    /* -- CHECK SDO UPLOAD REQUEST #2 -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x70);
    CHK_BYTE (frm, 1, 0);
    CHK_BYTE (frm, 2, 0);
    CHK_BYTE (frm, 3, 0);
    CHK_BYTE (frm, 4, 0);
    CHK_BYTE (frm, 5, 0);
    CHK_BYTE (frm, 6, 0);
    CHK_BYTE (frm, 7, 0);

    /* -- SERVER UPLOAD RESPONSE #2: SECOND DATA SEGMENT -- */
    TS_SEG5_SEND (0x10, 0x0b0a0908, 0x0e0d0c);

    /* -- CHECK SDO UPLOAD REQUEST #3 -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x60);
    CHK_BYTE (frm, 1, 0);
    CHK_BYTE (frm, 2, 0);
    CHK_BYTE (frm, 3, 0);
    CHK_BYTE (frm, 4, 0);
    CHK_BYTE (frm, 5, 0);
    CHK_BYTE (frm, 6, 0);
    CHK_BYTE (frm, 7, 0);

    /* -- SERVER UPLOAD RESPONSE #2: LAST DATA SEGMENT -- */
    TS_SEG5_SEND (0x0b, 0x0000100f, 0x000000);

    /* -- CHECK TRANSFER FINISHED -- */
    CHK_CB_CSDO_FINISHED(&CSdoSegUpCb, 1);
    CHK_CB_CSDO_CODE(&CSdoSegUpCb, 0);
    TS_ASSERT(val[0] == 1);
    TS_ASSERT(val[1] == 2);
    TS_ASSERT(val[2] == 3);
    TS_ASSERT(val[3] == 4);
    TS_ASSERT(val[4] == 5);
    TS_ASSERT(val[5] == 6);
    TS_ASSERT(val[6] == 7);
    TS_ASSERT(val[7] == 8);
    TS_ASSERT(val[8] == 9);
    TS_ASSERT(val[9] == 10);
    TS_ASSERT(val[10] == 11);
    TS_ASSERT(val[11] == 12);
    TS_ASSERT(val[12] == 13);
    TS_ASSERT(val[13] == 14);
    TS_ASSERT(val[14] == 15);
    TS_ASSERT(val[15] == 16);

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
TS_DEF_MAIN(TS_CSdoRd_SegTimeout)
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
    csdo = COCSdoFind(&node, 0);

    /* -- TEST -- */
    err = COCSdoRequestUpload(csdo,
                              CO_DEV(idx, sub),
                              &val, sizeof(val),
                              TS_AppCSdoCallback,
                              timeout);
    TS_ASSERT(err == CO_ERR_NONE);

    /* -- CHECK SDO INIT REQUEST -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x40);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, 0);

    /* -- NO SERVER INIT RESPONSE -- */
    TS_Wait(&node, 50);
    CHK_CB_CSDO_FINISHED(&CSdoSegUpCb, 0);

    TS_Wait(&node, 100);
    CHK_CB_CSDO_FINISHED(&CSdoSegUpCb, 1);

    /* -- CHECK TRANSFER FINISHED -- */
    CHK_CB_CSDO_FINISHED(&CSdoSegUpCb, 1);
    CHK_CB_CSDO_CODE(&CSdoSegUpCb, 0x05040000);

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
TS_DEF_MAIN(TS_CSdoRd_SegAbout)
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
    csdo = COCSdoFind(&node, 0);

    /* -- TEST -- */
    err = COCSdoRequestUpload(csdo,
                              CO_DEV(idx, sub),
                              &val, sizeof(val),
                              TS_AppCSdoCallback,
                              timeout);
    TS_ASSERT(err == CO_ERR_NONE);

    /* -- CHECK SDO INIT REQUEST -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x40);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, 0);

    /* -- SERVER INIT RESPONSE: ABORT CODE -- */
    TS_SDO5_SEND (0x80, idx, sub, 0x06020000);

    /* -- CHECK TRANSFER FINISHED -- */
    CHK_CB_CSDO_FINISHED(&CSdoSegUpCb, 1);
    CHK_CB_CSDO_CODE(&CSdoSegUpCb, 0x06020000);
    CHK_NO_ERR(&node);
}

/*---------------------------------------------------------------------------*/
/*!
* \brief    Use SDO client to handle toggle bit error
*
*           The SDO client #0 is used on testing node with Node-Id 1 to try
*           to send a byte to the SDO server #0 on device with Node-Id 5,
*           but an toggle bit error occurs.
*/
/*---------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_CSdoRd_SegBadToggle)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    CO_CSDO  *csdo;
    uint8_t   serverId = 5;
    uint32_t  idx = 0x2000;
    uint8_t   sub = 0x01;
    uint32_t  timeout = 1000;
    uint8_t   val[16] = { 0 };
    CO_ERR    err;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_CreateCSdoCom(0, &serverId);
    TS_CreateNode(&node, 0);
    csdo = COCSdoFind(&node, 0);

    /* -- TEST -- */
    err = COCSdoRequestUpload(csdo,
                              CO_DEV(idx, sub),
                              &val[0], 16,
                              TS_AppCSdoCallback,
                              timeout);
    TS_ASSERT(err == CO_ERR_NONE);

    /* -- CHECK SDO INIT REQUEST -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x40);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, 0);

    /* -- SERVER INIT RESPONSE: OK -- */
    TS_SDO5_SEND (0x41, idx, sub, 16);

    /* -- CHECK SDO UPLOAD REQUEST #1 -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x60);
    CHK_BYTE (frm, 1, 0);
    CHK_BYTE (frm, 2, 0);
    CHK_BYTE (frm, 3, 0);
    CHK_BYTE (frm, 4, 0);
    CHK_BYTE (frm, 5, 0);
    CHK_BYTE (frm, 6, 0);
    CHK_BYTE (frm, 7, 0);

    /* -- SERVER UPLOAD RESPONSE #1: FIRST DATA SEGMENT -- */
    TS_SEG5_SEND (0x00, 0x04030201, 0x070605);

    /* -- CHECK SDO UPLOAD REQUEST #2 -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x70);
    CHK_BYTE (frm, 1, 0);
    CHK_BYTE (frm, 2, 0);
    CHK_BYTE (frm, 3, 0);
    CHK_BYTE (frm, 4, 0);
    CHK_BYTE (frm, 5, 0);
    CHK_BYTE (frm, 6, 0);
    CHK_BYTE (frm, 7, 0);

    /* -- SERVER UPLOAD RESPONSE #2: SECOND DATA SEGMENT -- */
    TS_SEG5_SEND (0x00, 0x0b0a0908, 0x0e0d0c);

    /* -- CHECK TRANSFER FINISHED -- */
    CHK_CB_CSDO_FINISHED(&CSdoSegUpCb, 1);
    CHK_CB_CSDO_CODE(&CSdoSegUpCb, 0x5030000);

    CHK_NO_ERR(&node);
}

/*---------------------------------------------------------------------------*/
/*!
* \brief    Use SDO client to handle a bad buffer size
*
*           The SDO client #0 is used on testing node with Node-Id 1 to try
*           to send a byte to the SDO server #0 on device with Node-Id 5,
*           but a bad size is detected.
*/
/*---------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_CSdoRd_SegBadSize)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    CO_CSDO  *csdo;
    uint8_t   serverId = 5;
    uint32_t  idx = 0x2000;
    uint8_t   sub = 0x01;
    uint32_t  timeout = 1000;
    uint8_t   val[16] = { 0 };
    CO_ERR    err;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_CreateCSdoCom(0, &serverId);
    TS_CreateNode(&node, 0);
    csdo = COCSdoFind(&node, 0);

    /* -- TEST -- */
    err = COCSdoRequestUpload(csdo,
                              CO_DEV(idx, sub),
                              &val[0], 16,
                              TS_AppCSdoCallback,
                              timeout);
    TS_ASSERT(err == CO_ERR_NONE);

    /* -- CHECK SDO INIT REQUEST -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x40);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, 0);

    /* -- SERVER INIT RESPONSE: WITH A SIZE UNEQUAL BUFFER SIZE -- */
    TS_SDO5_SEND (0x41, idx, sub, 32);

    /* -- CHECK TRANSFER FINISHED -- */
    CHK_CB_CSDO_FINISHED(&CSdoSegUpCb, 1);
    CHK_CB_CSDO_CODE(&CSdoSegUpCb, 0x06070010);

    CHK_NO_ERR(&node);
}

static void CSdoSegUpSetup(void)
{
    TS_CallbackInit(&CSdoSegUpCb);
}

static void CSdoSegUpCleanup(void)
{
    TS_CallbackDeInit();
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_CSDO_SEG_UP()
{
    TS_Begin(__FILE__);
    TS_SetupCase(CSdoSegUpSetup, CSdoSegUpCleanup);

    TS_RUNNER(TS_CSdoRd_Seg16ByteDomain);

    TS_RUNNER(TS_CSdoRd_SegTimeout);
    TS_RUNNER(TS_CSdoRd_SegAbout);
    TS_RUNNER(TS_CSdoRd_SegBadToggle);
    TS_RUNNER(TS_CSdoRd_SegBadSize);

    TS_End();
}

#endif

/*! @} */
