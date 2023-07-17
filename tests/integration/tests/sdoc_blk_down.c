/******************************************************************************
* INCLUDES
******************************************************************************/

#include "def_suite.h"

#if USE_CSDO

/*****************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

static TS_CALLBACK CSdoBlkDownCb;

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

    CO_OBJ_DOM *dom;
    uint32_t size = 42;
    uint32_t  idx = 0x2000;
    uint8_t   sub = 0x01;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_CreateCSdoCom(0, &serverId);
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 0);
    TS_CreateNode(&node, 0);
    csdo = COCSdoFind(&node, 0);

    /* -- TEST -- */
    err = COCSdoRequestDownloadBlock(csdo,
                                CO_DEV(idx, sub),
                                &val[0], 16,
                                TS_AppCSdoCallback,
                                timeout);
    TS_ASSERT(err == CO_ERR_NONE);

    /* -- CHECK SDO INIT REQUEST -- */
    CHK_CAN  (&frm);
    //CHK_SDO5 (frm, 0x21);
    CHK_SDO5 (frm, (6<<5|1<<1));
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, size);

    /* -- Simulate SERVER INIT RESPONSE: OK -- */
    //TS_SDO5_SEND (0x60, idx, sub, 0x00000000);
    uint32_t blkSize = sizeof(val)/7;
    if( blkSize > 0xFF )
        blkSize = 0xFF;
    TS_SDO5_SEND ((5<<5), idx, sub, blkSize);

    // client send the sub-blocks

    /* -- Simulate SERVER ACKBLK RESPONSE: OK -- */

    /* -- CHECK SDO BLK END FRM REQUEST -- */

    /* -- Simulate SERVER END RESPONSE: OK -- */

    ///* -- CHECK SDO DOWNLOAD REQUEST #1 -- */
    //CHK_CAN  (&frm);
    //CHK_SDO5 (frm, 0x00);
    //CHK_BYTE (frm, 1, 1);
    //CHK_BYTE (frm, 2, 2);
    //CHK_BYTE (frm, 3, 3);
    //CHK_BYTE (frm, 4, 4);
    //CHK_BYTE (frm, 5, 5);
    //CHK_BYTE (frm, 6, 6);
    //CHK_BYTE (frm, 7, 7);

    ///* -- SERVER DOWNLOAD RESPONSE #1: OK -- */
    //TS_SEG5_SEND (0x20, 0x00000000, 0x000000);

    ///* -- CHECK SDO DOWNLOAD REQUEST #2 -- */
    //CHK_CAN  (&frm);
    //CHK_SDO5 (frm, 0x10);
    //CHK_BYTE (frm, 1, 8);
    //CHK_BYTE (frm, 2, 9);
    //CHK_BYTE (frm, 3, 10);
    //CHK_BYTE (frm, 4, 11);
    //CHK_BYTE (frm, 5, 12);
    //CHK_BYTE (frm, 6, 13);
    //CHK_BYTE (frm, 7, 14);

    ///* -- SERVER DOWNLOAD RESPONSE #2: OK -- */
    //TS_SEG5_SEND (0x30, 0x00000000, 0x000000);

    ///* -- CHECK SDO DOWNLOAD REQUEST #3 -- */
    //CHK_CAN  (&frm);
    //CHK_SDO5 (frm, 0x0B);
    //CHK_BYTE (frm, 1, 15);
    //CHK_BYTE (frm, 2, 16);
    //CHK_BYTE (frm, 3, 0);
    //CHK_BYTE (frm, 4, 0);
    //CHK_BYTE (frm, 5, 0);
    //CHK_BYTE (frm, 6, 0);
    //CHK_BYTE (frm, 7, 0);

    ///* -- SERVER DOWNLOAD RESPONSE #2: OK -- */
    //TS_SEG5_SEND (0x20, 0x00000000, 0x000000);

    /* -- CHECK TRANSFER FINISHED -- */
    CHK_CB_CSDO_FINISHED(&CSdoSegDownCb, 1);
    CHK_CB_CSDO_CODE(&CSdoSegDownCb, 0);

    CHK_NO_ERR(&node);
}


/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_CSDO_BLK_DOWN()
{
    TS_BEGIN(__FILE__);
    TS_SetupCase(CSdoBlkDownSetup, CSdoBlkDownCleanup);

    // TS_RUNNER(<test_name>)
    
    TS_End();
}
#endif 
