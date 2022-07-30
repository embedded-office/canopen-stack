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
* \addtogroup sdos_seg_up
* \details    This test suite checks the protocol for Segmented SDO Upload (e.g. Master read data
*             from Slave).
*
* #### Interface
*
* + obj: object entry type
*   - dom    : object is a domain
*   - badidx : index of the object is not existing in object dictionary
*   - badsub : subindex of the object is not existing in object dictionary
* + size: object entry size
*   - 6seg   : object value contains 6 segments (=6 x 7 bytes)
*   - 6seg+1 : object value contains 6 segments plus 1 byte (=6 x 7 + 1 bytes)
*   - 6seg-1 : object value contains 6 segments minus 1 byte (=6 x 7 - 1 bytes)
*   - 3seg+2 : object value contains 3 segments plus 2 bytes (=3 x 7 + 2 bytes)
*   - no-data: object data reference is invalid (NULL)
* + prop: object entry property
*   - none   : no special property set
*   - wr-only: object entry is write-only
* + req: SDO request
*   - ok     : correct SDO request
*   - badcmd : bad SDO command in request
*   - tgl1   : bad first toggle bit
*   - tgl2   : bad second toggle bit
*
* #### Test Definition
*
* test-function                 | obj    | size    | prop    | req    | type
* ----------------------------- | ------ | ------- | ------- | ------ | ----
* \ref TS_SegRd_42ByteDomain    | dom    | 6seg    | none    | ok     | F
* \ref TS_SegRd_43ByteDomain    | dom    | 6seg+1  | none    | ok     | F
* \ref TS_SegRd_41ByteDomain    | dom    | 6seg-1  | none    | ok     | F
* \ref TS_SegRd_23ByteString    | dom    | 3seg+2  | none    | ok     | F
* \ref TS_SegRd_BadCmd          | --     | --      | --      | badcmd | R
* \ref TS_SegRd_ObjNotExist     | badidx | --      | --      | ok     | R
* \ref TS_SegRd_SubIdxNotExist  | badsub | --      | --      | ok     | R
* \ref TS_SegRd_WriteOnly       | dom    | 6seg    | wr-only | ok     | R
* \ref TS_SegRd_DomainNullPtr   | dom    | no-data | none    | ok     | R
* \ref TS_SegRd_Bad1stToggleBit | dom    | 6seg    | none    | tgl1   | R
* \ref TS_SegRd_Bad2ndToggleBit | dom    | 6seg    | none    | tgl2   | R
*
* Type Legend: R = Robustness Test, F = Functional Test
* @{
*/

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "def_suite.h"

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the segmented upload of an array with size = 42 from the Domainbuffer
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegRd_42ByteDomain)
{
    CO_IF_FRM   frm;
    uint8_t     tgl;
    uint32_t    id;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 42;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 2;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 0);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED UPLOAD ==============*/
    TS_SDO_SEND (0x40, idx, sub, size);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x41);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, size);                             /* check data area                          */

                                                      /*===== SEGMENTED UPLOAD ===================*/
    tgl = 0x00;                                       /* start with toggle bit 0                  */
    for (id = 0; id < (size-7); id += 7) {
        TS_SDO_SEND((0x60 | tgl), 0, 0, 0);

        CHK_CAN  (&frm);                              /* check for a CAN frame                    */
        CHK_SDO0 (frm, tgl);                          /* check SDO #0 response (Id and DLC)       */
        CHK_SEG  (frm, id, 7);                        /* check segment data                       */
                                                      /*------------------------------------------*/
        tgl ^= 0x10;                                  /* prepare the toggle bit for next request  */
    }
                                                      /*===== LAST SEGMENTED UPLOAD ==============*/
    TS_SDO_SEND((0x60 | tgl), 0, 0, 0);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, (0x01 | tgl));                     /* check SDO #0 response (Id and DLC)       */
    CHK_SEG  (frm, id, 7);                            /* check segment data                       */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*          This testcase will check the segmented upload of an array with size = 43 from the Domainbuffer
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegRd_43ByteDomain)
{
    CO_IF_FRM   frm;
    uint8_t     tgl;
    uint32_t    id;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 43;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 3;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 0);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED UPLOAD ==============*/
    TS_SDO_SEND (0x40, idx, sub, size);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x41);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, size);                             /* check data area                          */

                                                      /*===== SEGMENTED UPLOAD ===================*/
    tgl = 0x00;                                       /* start with toggle bit 0                  */
    for (id = 0; id < (size-7); id += 7) {
        TS_SDO_SEND((0x60 | tgl), 0, 0, 0);

        CHK_CAN  (&frm);                              /* check for a CAN frame                    */
        CHK_SDO0 (frm, tgl);                          /* check SDO #0 response (Id and DLC)       */
        CHK_SEG  (frm, id, 7);                        /* check segment data                       */
                                                      /*------------------------------------------*/
        tgl ^= 0x10;                                  /* prepare the toggle bit for next request  */
    }
                                                      /*===== LAST SEGMENTED UPLOAD ==============*/
    TS_SDO_SEND((0x60 | tgl), 0, 0, 0);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, (0x0D | tgl));                     /* check SDO #0 response (Id and DLC)       */
    CHK_SEG  (frm, id, 1);                            /* check segment data                       */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*          This testcase will check the segmented upload of an array with size = 41 from the Domainbuffer
*
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegRd_41ByteDomain)
{
    CO_IF_FRM   frm;
    uint8_t     tgl;
    uint32_t    id;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 41;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 0);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED UPLOAD ==============*/
    TS_SDO_SEND (0x40, idx, sub, size);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x41);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, size);                             /* check data area                          */

                                                      /*===== SEGMENTED UPLOAD ===================*/
    tgl = 0x00;                                       /* start with toggle bit 0                  */
    for (id = 0; id < (size-7); id += 7) {
        TS_SDO_SEND((0x60 | tgl), 0, 0, 0);

        CHK_CAN  (&frm);                              /* check for a CAN frame                    */
        CHK_SDO0 (frm, tgl);                          /* check SDO #0 response (Id and DLC)       */
        CHK_SEG  (frm, id, 7);                        /* check segment data                       */
                                                      /*------------------------------------------*/
        tgl ^= 0x10;                                  /* prepare the toggle bit for next request  */
    }
                                                      /*===== LAST SEGMENTED UPLOAD ==============*/
    TS_SDO_SEND((0x60 | tgl), 0, 0, 0);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, (0x03 | tgl));                     /* check SDO #0 response (Id and DLC)       */
    CHK_SEG  (frm, id, 6);                            /* check segment data                       */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*          This testcase will check the segmented upload of an 23 byte STRING
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegRd_23ByteString)
{
    CO_IF_FRM   frm;
    uint8_t     tgl;
    CO_NODE     node;
    uint32_t    size = 23;
    uint16_t    idx  = 0x2530;
    uint8_t     sub  = 2;
    CO_OBJ_STR  Str23;
    char       *strPtr = "abcdefghijklmnopqrstuvw";

    Str23.Offset = 0;
    Str23.Start  = (uint8_t *)strPtr;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____R_), CO_TSTRING, (CO_DATA)(&Str23));
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED UPLOAD ==============*/
    TS_SDO_SEND (0x40, idx, sub, size);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x41);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, size);                             /* check data area                          */

                                                      /*===== FIRST SEGMENTED UPLOAD =============*/
    tgl = 0x00;                                       /* start with toggle bit 0                  */
    TS_SDO_SEND((0x60 | tgl), 0, 0, 0);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, tgl);                              /* check SDO #0 response (Id and DLC)       */
    CHK_SEG  (frm, 'a', 7);                           /* check segment data                       */

                                                      /*===== SECOND SEGMENTED UPLOAD ============*/
    tgl ^= 0x10;                                      /* update toggle bit                        */
    TS_SDO_SEND((0x60 | tgl), 0, 0, 0);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, tgl);                              /* check SDO #0 response (Id and DLC)       */
    CHK_SEG  (frm, 'h', 7);                           /* check segment data                       */

                                                      /*===== THIRD SEGMENTED UPLOAD =============*/
    tgl ^= 0x10;                                      /* update toggle bit                        */
    TS_SDO_SEND((0x60 | tgl), 0, 0, 0);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, tgl);                              /* check SDO #0 response (Id and DLC)       */
    CHK_SEG  (frm, 'o', 7);                           /* check segment data                       */

                                                      /*===== LAST SEGMENTED UPLOAD ==============*/
    tgl ^= 0x10;                                      /* update toggle bit                        */
    TS_SDO_SEND((0x60 | tgl), 0, 0, 0);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, (0x0B | tgl));                     /* check SDO #0 response (Id and DLC)       */
    CHK_SEG  (frm, 'v', 2);                           /* check segment data                       */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*      This testcase will check the Abort code "Client/server command specifier not valid or unknown"
*                                   Abort code 0x05040001
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegRd_BadCmd)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  size = 41;
    uint16_t  idx  = 0x2520;
    uint8_t   sub  = 6;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0xF0, idx, sub, 0);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x80);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, 0x05040001);                       /* check abort code                         */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*      This testcase will check the Abort code " Object does not exist in the object dictionary."
*                                   Abort code 0x06020000
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegRd_ObjNotExist)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  size = 41;
    uint16_t  idx  = 0x2520;
    uint8_t   sub  = 0;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

    idx++;
                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0x40, idx, sub, 0);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x80);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, 0x06020000);                       /* check abort code                         */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*      This testcase will check the Abort code "Sub-index does not exist"
*                                   Abort code 0x06090011
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegRd_SubIdxNotExist)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  size = 41;
    uint16_t  idx  = 0x2520;
    uint8_t   sub  = 0;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

    sub++;
                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0x40, idx, sub, 0);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x80);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, 0x06090011);                       /* check abort code                         */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*      This testcase will check the Abort code " Attempt to read a write only object"
*                                   Abort code 0x06010001
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegRd_WriteOnly)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  size = 42;
    uint16_t  idx  = 0x2520;
    uint8_t   sub  = 8;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ______W, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0x40, idx, sub, 0);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x80);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, 0x06010001);                       /* check abort code                         */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*  This testcase will check the Abort code " Data cannot be transferred or stored to the application "
*                               Abort code  0x08000020
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegRd_DomainNullPtr)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  idx  = 0x2520;
    uint8_t   sub  = 9;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TUNSIGNED8, (CO_DATA)(0));
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0x40, idx, sub, 0);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x80);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, 0x08000020);                       /* check abort code                         */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*          This testcase will check the Abort code " Toggle bit not alternated "
*                                   Abort code 0x05030000
*                                   Failed toggle bit on request first segment
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegRd_Bad1stToggleBit)
{
    CO_IF_FRM   frm;
    uint8_t     tgl;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 42;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 2;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 0);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED UPLOAD ==============*/
    TS_SDO_SEND (0x40, idx, sub, size);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x41);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, size);                             /* check data area                          */

                                                      /*===== SEGMENTED UPLOAD ===================*/
    tgl = 0x10;                                       /* start with bad toggle bit 1              */
    TS_SDO_SEND((0x60 | tgl), 0, 0, 0);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x80);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, 0x05030000);                       /* check abort code                         */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*          This testcase will check the Abort code " Toggle bit not alternated "
*                                   Abort code 0x05030000
*                                   Failed toggle bit on request second segment
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegRd_Bad2ndToggleBit)
{
    CO_IF_FRM   frm;
    uint8_t     tgl;
    CO_NODE     node;
    uint32_t    size   = 23;
    uint16_t    idx    = 0x2530;
    uint8_t     sub    = 2;
    CO_OBJ_STR  Str23;
    char       *strPtr = "abcdefghijklmnopqrstuvw";

    Str23.Offset = 0;
    Str23.Start  = (uint8_t *)strPtr;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____R_), CO_TSTRING, (CO_DATA)(&Str23));
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED UPLOAD ==============*/
    TS_SDO_SEND (0x40, idx, sub, size);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x41);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, size);                             /* check data area                          */

                                                      /*===== FIRST SEGMENTED UPLOAD =============*/
    tgl = 0x00;                                       /* start with toggle bit 0                  */
    TS_SDO_SEND((0x60 | tgl), 0, 0, 0);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, tgl);                              /* check SDO #0 response (Id and DLC)       */
    CHK_SEG  (frm, 'a', 7);                           /* check segment data                       */

                                                      /*===== SECOND SEGMENTED UPLOAD ============*/
    /* tgl ^= 0x10; */                                /* do not toggle bit for second upload      */

    TS_SDO_SEND((0x60 | tgl), 0, 0, 0);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x80);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, 0x05030000);                       /* check abort code                         */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_SEG_UP()
{
    TS_Begin(__FILE__);

//    CanDiagnosticOn(0);

    TS_RUNNER(TS_SegRd_42ByteDomain);
    TS_RUNNER(TS_SegRd_43ByteDomain);
    TS_RUNNER(TS_SegRd_41ByteDomain);
    TS_RUNNER(TS_SegRd_23ByteString);
    TS_RUNNER(TS_SegRd_BadCmd);
    TS_RUNNER(TS_SegRd_ObjNotExist);
    TS_RUNNER(TS_SegRd_SubIdxNotExist);
    TS_RUNNER(TS_SegRd_WriteOnly);
    TS_RUNNER(TS_SegRd_DomainNullPtr);
    TS_RUNNER(TS_SegRd_Bad1stToggleBit);
    TS_RUNNER(TS_SegRd_Bad2ndToggleBit);

//    CanDiagnosticOff(0);

    TS_End();
}

/*! @} */
