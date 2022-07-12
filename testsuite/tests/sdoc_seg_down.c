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
* \addtogroup csdo_seg_down
* \details    This test suite checks the protocol for segmented SDO download
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

static TS_CALLBACK CSdoSegDownCb;

/*---------------------------------------------------------------------------*/
/*!
* \brief    Use SDO client to write a 16 byte domain to SDO server
*
*           The SDO client #0 is used on testing node with Node-Id 1 to send
*           a byte to the SDO server #0 on device with Node-Id 5.
*/
/*---------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_CSdoWr_Seg16ByteDomain)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    CO_CSDO  *csdo;
    uint8_t   serverId = 5;
    uint32_t  idx = 0x2000;
    uint8_t   sub = 0x01;
    uint32_t  timeout = 1000;
    uint8_t   val[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
    CO_ERR    err;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_CreateCSdoCom(0, &serverId);
    TS_CreateNode(&node, 0);
    csdo = COCSdoFind(&node, 0);

    /* -- TEST -- */
    err = COCSdoRequestDownload(csdo,
                                CO_DEV(idx, sub),
                                &val[0], 16,
                                TS_AppCSdoCallback,
                                timeout);
    TS_ASSERT(err == CO_ERR_NONE);

    /* -- CHECK SDO INIT REQUEST -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x21);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, 16);

    /* -- SERVER INIT RESPONSE: OK -- */
    TS_SDO5_SEND (0x60, idx, sub, 0x00000000);

    /* -- CHECK SDO DOWNLOAD REQUEST #1 -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x00);
    CHK_BYTE (frm, 1, 1);
    CHK_BYTE (frm, 2, 2);
    CHK_BYTE (frm, 3, 3);
    CHK_BYTE (frm, 4, 4);
    CHK_BYTE (frm, 5, 5);
    CHK_BYTE (frm, 6, 6);
    CHK_BYTE (frm, 7, 7);

    /* -- SERVER DOWNLOAD RESPONSE #1: OK -- */
    TS_SEG5_SEND (0x20, 0x00000000, 0x000000);

    /* -- CHECK SDO DOWNLOAD REQUEST #2 -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x10);
    CHK_BYTE (frm, 1, 8);
    CHK_BYTE (frm, 2, 9);
    CHK_BYTE (frm, 3, 10);
    CHK_BYTE (frm, 4, 11);
    CHK_BYTE (frm, 5, 12);
    CHK_BYTE (frm, 6, 13);
    CHK_BYTE (frm, 7, 14);

    /* -- SERVER DOWNLOAD RESPONSE #2: OK -- */
    TS_SEG5_SEND (0x30, 0x00000000, 0x000000);

    /* -- CHECK SDO DOWNLOAD REQUEST #3 -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x0B);
    CHK_BYTE (frm, 1, 15);
    CHK_BYTE (frm, 2, 16);
    CHK_BYTE (frm, 3, 0);
    CHK_BYTE (frm, 4, 0);
    CHK_BYTE (frm, 5, 0);
    CHK_BYTE (frm, 6, 0);
    CHK_BYTE (frm, 7, 0);

    /* -- SERVER DOWNLOAD RESPONSE #2: OK -- */
    TS_SEG5_SEND (0x20, 0x00000000, 0x000000);

    /* -- CHECK TRANSFER FINISHED -- */
    CHK_CB_CSDO_FINISHED(&CSdoSegDownCb, 1);
    CHK_CB_CSDO_CODE(&CSdoSegDownCb, 0);

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
TS_DEF_MAIN(TS_CSdoWr_SegTimeout)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    CO_CSDO  *csdo;
    uint8_t   serverId = 5;
    uint32_t  idx = 0x2000;
    uint8_t   sub = 0x01;
    uint32_t  timeout = 100;
    uint8_t   val[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
    CO_ERR    err;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_CreateCSdoCom(0, &serverId);
    TS_CreateNode(&node,0);
    csdo = COCSdoFind(&node, 0);

    /* -- TEST -- */
    err = COCSdoRequestDownload(csdo,
                                CO_DEV(idx, sub),
                                &val[0], 16,
                                TS_AppCSdoCallback,
                                timeout);
    TS_ASSERT(err == CO_ERR_NONE);

    /* -- CHECK SDO INIT REQUEST -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x21);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, sizeof(val));

    /* -- NO SERVER INIT RESPONSE -- */
    TS_Wait(&node, 50);
    CHK_CB_CSDO_FINISHED(&CSdoSegDownCb, 0);

    TS_Wait(&node, 100);
    CHK_CB_CSDO_FINISHED(&CSdoSegDownCb, 1);

    /* -- CHECK TRANSFER FINISHED -- */
    CHK_CB_CSDO_FINISHED(&CSdoSegDownCb, 1);
    CHK_CB_CSDO_CODE(&CSdoSegDownCb, 0x05040000);

    CHK_NO_ERR(&node);
}

/*---------------------------------------------------------------------------*/
/*!
* \brief    Use SDO client to handle an abort response
*
*           The SDO client #0 is used on testing node with Node-Id 1 to try
*           to send a byte to the SDO server #0 on device with Node-Id 5,
*           but an abort response is sent.
*/
/*---------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_CSdoWr_SegAbout)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    CO_CSDO  *csdo;
    uint8_t   serverId = 5;
    uint32_t  idx = 0x2000;
    uint8_t   sub = 0x01;
    uint32_t  timeout = 1000;
    uint8_t   val[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
    CO_ERR    err;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_CreateCSdoCom(0, &serverId);
    TS_CreateNode(&node,0);
    csdo = COCSdoFind(&node, 0);

    /* -- TEST -- */
    err = COCSdoRequestDownload(csdo,
                                CO_DEV(idx, sub),
                                &val[0], 16,
                                TS_AppCSdoCallback,
                                timeout);
    TS_ASSERT(err == CO_ERR_NONE);

    /* -- CHECK SDO INIT REQUEST -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x21);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, sizeof(val));

    /* -- SERVER INIT RESPONSE: ABORT CODE -- */
    TS_SDO5_SEND (0x80, idx, sub, 0x06020000);

    /* -- CHECK TRANSFER FINISHED -- */
    CHK_CB_CSDO_FINISHED(&CSdoSegDownCb, 1);
    CHK_CB_CSDO_CODE(&CSdoSegDownCb, 0x06020000);
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
TS_DEF_MAIN(TS_CSdoWr_SegBadToggle)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    CO_CSDO  *csdo;
    uint8_t   serverId = 5;
    uint32_t  idx = 0x2000;
    uint8_t   sub = 0x01;
    uint32_t  timeout = 1000;
    uint8_t   val[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
    CO_ERR    err;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_CreateCSdoCom(0, &serverId);
    TS_CreateNode(&node, 0);
    csdo = COCSdoFind(&node, 0);

    /* -- TEST -- */
    err = COCSdoRequestDownload(csdo,
                                CO_DEV(idx, sub),
                                &val[0], 16,
                                TS_AppCSdoCallback,
                                timeout);
    TS_ASSERT(err == CO_ERR_NONE);

    /* -- CHECK SDO INIT REQUEST -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x21);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, 16);

    /* -- SERVER INIT RESPONSE: OK -- */
    TS_SDO5_SEND (0x60, idx, sub, 0x00000000);

    /* -- CHECK SDO DOWNLOAD REQUEST #1 -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x00);
    CHK_BYTE (frm, 1, 1);
    CHK_BYTE (frm, 2, 2);
    CHK_BYTE (frm, 3, 3);
    CHK_BYTE (frm, 4, 4);
    CHK_BYTE (frm, 5, 5);
    CHK_BYTE (frm, 6, 6);
    CHK_BYTE (frm, 7, 7);

    /* -- SERVER DOWNLOAD RESPONSE #1: OK -- */
    TS_SEG5_SEND (0x20, 0x00000000, 0x000000);

    /* -- CHECK SDO DOWNLOAD REQUEST #2 -- */
    CHK_CAN  (&frm);
    CHK_SDO5 (frm, 0x10);
    CHK_BYTE (frm, 1, 8);
    CHK_BYTE (frm, 2, 9);
    CHK_BYTE (frm, 3, 10);
    CHK_BYTE (frm, 4, 11);
    CHK_BYTE (frm, 5, 12);
    CHK_BYTE (frm, 6, 13);
    CHK_BYTE (frm, 7, 14);

    /* -- SERVER DOWNLOAD RESPONSE #2: BAD TOGGLE -- */
    TS_SEG5_SEND (0x20, 0x00000000, 0x000000);

    /* -- CHECK TRANSFER FINISHED -- */
    CHK_CB_CSDO_FINISHED(&CSdoSegDownCb, 1);
    CHK_CB_CSDO_CODE(&CSdoSegDownCb, 0x5030000);

    CHK_NO_ERR(&node);
}

static void CSdoSegDownSetup(void)
{
    TS_CallbackInit(&CSdoSegDownCb);
}

static void CSdoSegDownCleanup(void)
{
    TS_CallbackDeInit();
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_CSDO_SEG_DOWN()
{
    TS_Begin(__FILE__);
    TS_SetupCase(CSdoSegDownSetup, CSdoSegDownCleanup);

    TS_RUNNER(TS_CSdoWr_Seg16ByteDomain);

    TS_RUNNER(TS_CSdoWr_SegTimeout);
    TS_RUNNER(TS_CSdoWr_SegAbout);
    TS_RUNNER(TS_CSdoWr_SegBadToggle);

    TS_End();
}

#endif

/*! @} */
