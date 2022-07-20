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

void TS_ChkBlk(uint32_t start, uint8_t segnum, uint8_t last, uint8_t valid)
{
    CO_IF_FRM frm;
    uint8_t   seg;
    uint32_t  idx;
    uint8_t   num = 7;

    seg = 1;                                          /* initialize sequence counter              */
    idx = start;
    while ((seg <= segnum) && (seg > 0)) {
        if ( (seg >= segnum) &&
             (last != 0     ) ) {
            seg = seg | 0x80;
            num = valid;
        }
        CHK_CAN  (&frm);                              /* check for a CAN frame                    */
        CHK_SDO0 (frm, seg);                          /* check SDO #0 response (Id and DLC)       */
        CHK_SEG  (frm, idx, num);                     /* check SDO response                       */
        idx += 7;
        seg =  (seg + 1) & 0x7F;                      /* calculate next segment counter           */
    }
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block upload of a basic object entry
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_Basic)
{
    CO_IF_FRM  frm;
    CO_NODE    node;
    uint16_t   idx  = 0x2510;
    uint8_t    sub  = 2;
    uint32_t   val  = 0x03020100;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&val));
    TS_CreateNode(&node,0);
                                                      /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    TS_SDO_SEND (0xA0, idx, sub, CO_SDO_BUF_SEG);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_DATA    (frm, 4);                             /* check block size                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);

                                                      /*===== BLOCK UPLOAD =======================*/
    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    TS_ASSERT(0x81 == BYTE(frm,0));
    TS_ASSERT(0x00 == BYTE(frm,1));
    TS_ASSERT(0x01 == BYTE(frm,2));
    TS_ASSERT(0x02 == BYTE(frm,3));
    TS_ASSERT(0x03 == BYTE(frm,4));
    TS_ASSERT(0x00 == BYTE(frm,5));
    TS_ASSERT(0x00 == BYTE(frm,6));
    TS_ASSERT(0x00 == BYTE(frm,7));

                                                      /*===== ACKNOWLEDGE BLOCK ==================*/
    TS_ACKBLK_SEND(0xA2, 1, CO_SDO_BUF_SEG);

                                                      /*===== END BLOCK UPLOAD ===================*/
    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xCD);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    TS_EBLK_SEND(0xA1, 0x00000000);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block upload of an array with size = 42 from  Domainbuffer
*         entry
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_42ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom ;
    uint32_t    size = 42;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 2;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 0);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    TS_SDO_SEND (0xA0, idx, sub, CO_SDO_BUF_SEG);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_DATA    (frm, size);                          /* check block size                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);

                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk(0x00, 6, 1, 7);                         /* check received block                     */

                                                      /*===== ACKNOWLEDGE BLOCK ==================*/
    TS_ACKBLK_SEND(0xA2, 6, CO_SDO_BUF_SEG);

                                                      /*===== END BLOCK UPLOAD ===================*/
    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    TS_EBLK_SEND(0xA1, 0x00000000);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block upload of an array with size = 43 from  Domainbuffer
*         entry
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_43ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom ;
    uint32_t    size = 43;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 3;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 0);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    TS_SDO_SEND (0xA0, idx, sub, CO_SDO_BUF_SEG);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_DATA    (frm, size);                          /* check block size                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);

                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk(0x00, 7, 1, 1);                         /* check received block                     */

                                                      /*===== ACKNOWLEDGE BLOCK ==================*/
    TS_ACKBLK_SEND(0xA2, 7, CO_SDO_BUF_SEG);

                                                      /*===== END BLOCK UPLOAD ===================*/
    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xD9);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    TS_EBLK_SEND(0xA1, 0x00000000);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block upload of an array with size = 41 from  Domainbuffer
*         entry
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_41ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom ;
    uint32_t    size = 41;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 0);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    TS_SDO_SEND (0xA0, idx, sub, CO_SDO_BUF_SEG);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_DATA    (frm, size);                          /* check block size                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);

                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk(0x00, 6, 1, 6);                         /* check received block                     */

                                                      /*===== ACKNOWLEDGE BLOCK ==================*/
    TS_ACKBLK_SEND(0xA2, 6, CO_SDO_BUF_SEG);

                                                      /*===== END BLOCK UPLOAD ===================*/
    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC5);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    TS_EBLK_SEND(0xA1, 0x00000000);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block upload of an array with size = 994 from  Domainbuffer
*         entry
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_994ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom ;
    uint32_t    size = 994;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 6;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 0);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    TS_SDO_SEND (0xA0, idx, sub, CO_SDO_BUF_SEG);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_DATA    (frm, size);                          /* check block size                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);

                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk  (0x00, 127, 0, 7);                     /* check received block                     */
    TS_ACKBLK_SEND(0xA2, 127, CO_SDO_BUF_SEG);

    TS_ChkBlk  (0x79, 15, 1, 7);                      /* check received block                     */
    TS_ACKBLK_SEND(0xA2, 15, CO_SDO_BUF_SEG);

                                                      /*===== END BLOCK UPLOAD ===================*/
    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    TS_EBLK_SEND(0xA1, 0x00000000);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block upload of an array with size = 995 from  Domainbuffer
*         entry
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_995ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom ;
    uint32_t    size = 995;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 6;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 0);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    TS_SDO_SEND (0xA0, idx, sub, CO_SDO_BUF_SEG);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_DATA    (frm, size);                          /* check block size                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);

                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk  (0x00, 127, 0, 7);                     /* check received block                     */
    TS_ACKBLK_SEND(0xA2, 127, CO_SDO_BUF_SEG);

    TS_ChkBlk  (0x79, 16, 1, 1);                      /* check received block                     */
    TS_ACKBLK_SEND(0xA2, 16, CO_SDO_BUF_SEG);

                                                      /*===== END BLOCK UPLOAD ===================*/
    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xD9);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    TS_EBLK_SEND(0xA1, 0x00000000);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block upload of an array with size = 993 from  Domainbuffer
*         entry
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_993ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom ;
    uint32_t    size = 993;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 6;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 0);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    TS_SDO_SEND (0xA0, idx, sub, CO_SDO_BUF_SEG);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_DATA    (frm, size);                          /* check block size                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);

                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk  (0x00, 127, 0, 7);                     /* check received block                     */
    TS_ACKBLK_SEND(0xA2, 127, CO_SDO_BUF_SEG);

    TS_ChkBlk  (0x79, 15, 1, 6);                      /* check received block                     */
    TS_ACKBLK_SEND(0xA2, 15, CO_SDO_BUF_SEG);

                                                      /*===== END BLOCK UPLOAD ===================*/
    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC5);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    TS_EBLK_SEND(0xA1, 0x00000000);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block upload of an array with size = 890 Byte (127*7 + 1)
*         from  Domainbuffer entry
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_890ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom ;
    uint32_t    size = 890;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 6;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 0);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    TS_SDO_SEND (0xA0, idx, sub, CO_SDO_BUF_SEG);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_DATA    (frm, size);                          /* check block size                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);

                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk  (0x00, 127, 0, 7);                     /* check received block                     */
    TS_ACKBLK_SEND(0xA2, 127, CO_SDO_BUF_SEG);

    TS_ChkBlk  (0x79,   1, 1, 1);                     /* check received block                     */
    TS_ACKBLK_SEND(0xA2,   1, CO_SDO_BUF_SEG);

                                                      /*===== END BLOCK UPLOAD ===================*/
    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xD9);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    TS_EBLK_SEND(0xA1, 0x00000000);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block upload of an array with size = 889 Byte (127*7)
*         from  Domainbuffer entry
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_889ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom ;
    uint32_t    size = 889;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 6;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 0);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    TS_SDO_SEND (0xA0, idx, sub, CO_SDO_BUF_SEG);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_DATA    (frm, size);                          /* check block size                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);

                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk  (0x00, 127, 1, 7);                     /* check received block                     */
    TS_ACKBLK_SEND(0xA2, 127, CO_SDO_BUF_SEG);

                                                      /*===== END BLOCK UPLOAD ===================*/
    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    TS_EBLK_SEND(0xA1, 0x00000000);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block upload of an array with size = 888 Byte (127*7-1)
*         from  Domainbuffer entry
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_888ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom ;
    uint32_t    size = 888;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 6;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 0);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    TS_SDO_SEND (0xA0, idx, sub, CO_SDO_BUF_SEG);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_DATA    (frm, size);                          /* check block size                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);

                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk  (0x00, 127, 1, 6);                     /* check received block                     */
    TS_ACKBLK_SEND(0xA2, 127, CO_SDO_BUF_SEG);

                                                      /*===== END BLOCK UPLOAD ===================*/
    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC5);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    TS_EBLK_SEND(0xA1, 0x00000000);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}


/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*          This testcase will check the block upload of an array with size = 994 Bytes and
*          check the failured block upload of loosing one Segment in the middle of the transfer
*          (Go-Back-N ARQ)
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_LostMiddleSeg)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom ;
    uint32_t    size = 26;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 6;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 'a');
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    TS_SDO_SEND (0xA0, idx, sub, 3);                  /* 3 segments per block (21 bytes)          */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_DATA    (frm, size);                          /* check block size                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);

                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk  ('a', 3, 0, 0);                        /* 3 blocks (a-g,h-n,o-u)                   */
    TS_ACKBLK_SEND(0xA2, 2, 3);                       /* ack for segment 1-2 (segment 3: lost)    */

    TS_ChkBlk  ('o', 2, 1, 5);                        /* 2 blocks (o-u,v-z) (last block: 5 bytes) */
    TS_ACKBLK_SEND(0xA2, 2, 3);                       /* ack for segment 3-4                      */

                                                      /*===== END BLOCK UPLOAD ===================*/
    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC9);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    TS_EBLK_SEND(0xA1, 0x00000000);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}


/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*          This testcase will check the block upload of an array with size = 26 Bytes and
*          check the failured block upload of loosing a segment in the middle of the transfer
*          (Go-Back-N ARQ)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_LostLastSeg)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom ;
    uint32_t    size = 26;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 6;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 'a');
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    TS_SDO_SEND (0xA0, idx, sub, CO_SDO_BUF_SEG);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_DATA    (frm, size);                          /* check block size                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);

                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk  ('a', 4, 1, 5);                        /* 4 received blocks (last block: 5 bytes)  */
    TS_ACKBLK_SEND(0xA2, 2, CO_SDO_BUF_SEG);          /* ack for segment 1-2 (segment 3-4: lost)  */

    TS_ChkBlk  ('o', 2, 1, 5);                        /* 2 received blocks (last block: 5 bytes)  */
    TS_ACKBLK_SEND(0xA2, 2, CO_SDO_BUF_SEG);          /* ack for segment 3-4                      */

                                                      /*===== END BLOCK UPLOAD ===================*/
    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC9);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    TS_EBLK_SEND(0xA1, 0x00000000);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*          This testcase will check the block upload of an array with size = 994 Bytes and
*          check the failured block upload of loosing the first Segment  of the transfer
*          (Go-Back-N ARQ)
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_LostFirstSeg)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom ;
    uint32_t    size = 994;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 6;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 0);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    TS_SDO_SEND (0xA0, idx, sub, CO_SDO_BUF_SEG);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_DATA    (frm, size);                          /* check block size                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);

                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk  (0x00, 127, 0, 7);                     /* check received block                     */
    TS_ACKBLK_SEND(0xA2,   0, CO_SDO_BUF_SEG);

    TS_ChkBlk  (0x00, 127, 0, 7);                     /* check received block                     */
    TS_ACKBLK_SEND(0xA2, 127, CO_SDO_BUF_SEG);

    TS_ChkBlk  (0x79,  15, 1, 7);                     /* check received block                     */
    TS_ACKBLK_SEND(0xA2,  15, CO_SDO_BUF_SEG);

                                                      /*===== END BLOCK UPLOAD ===================*/
    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    TS_EBLK_SEND(0xA1, 0x00000000);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*      This testcase will check the Abort code "Client command specifier not valid or unknown"
*                                   Abort code 0x05040001
*
*/
/*-----------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_BadCmd)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  size = 42;
    uint16_t  idx  = 0x2520;
    uint8_t   sub  = 6;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED UPLOAD ==============*/
    TS_SDO_SEND (0xE0, idx, sub, 127);

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
TS_DEF_MAIN(TS_BlkRd_ObjNotExist)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  size = 42;
    uint16_t  idx  = 0x2520;
    uint8_t   sub  = 6;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

    idx++;
                                                      /*===== INIT SEGMENTED UPLOAD ==============*/
    TS_SDO_SEND (0xA0, idx, sub, 127);

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
*      This testcase will check the Abort code " Sub-index does not exist. "
*                                   Abort code 0x06090011
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_SubIdxNotExist)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  size = 42;
    uint16_t  idx  = 0x2520;
    uint8_t   sub  = 0;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

    sub++;

                                                      /*===== INIT SEGMENTED UPLOAD ==============*/
    TS_SDO_SEND (0xA0, idx, sub, size);

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
*      This testcase will check the Abort code " Attempt to read a write only object "
*                                   Abort code 0x06010001
*
*/
/*-----------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_WriteOnly)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  size = 42;
    uint16_t  idx  = 0x2520;
    uint8_t   sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ______W, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED UPLOAD ==============*/
    TS_SDO_SEND (0xA0, idx, sub, 127);

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
*  This testcase will check the Abort code "Data cannot be transferred or stored to the application"
*                               Abort code  0x08000020
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_DomainNullPtr)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  idx  = 0x2520;
    uint8_t   sub  = 9;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TDOMAIN, (CO_DATA)(0));
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED UPLOAD ==============*/
    TS_SDO_SEND (0xA0, idx, sub, 127);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x80);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, 0x08000020);                       /* check abort code                         */

    CHK_ERR(&node, CO_ERR_OBJ_INIT);                  /* check for expected error                 */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*      This testcase will check the Abort code "Invalid block size block mode only"
*                                   Abort code 0x06010000
*
*/
/*-----------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_LenTooHigh)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  size = 42;
    uint16_t  idx  = 0x2520;
    uint8_t   sub  = 6;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED UPLOAD ==============*/
    TS_SDO_SEND (0xA0, idx, sub, 128);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x80);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, 0x05040002);                       /* check abort code                         */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*      This testcase will check the Abort code "Invalid block size block mode only"
*                                   Abort code 0x05040002
*
*/
/*-----------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_LenTooLow)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  size = 42;
    uint16_t  idx  = 0x2520;
    uint8_t   sub  = 6;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED UPLOAD ==============*/
    TS_SDO_SEND (0xA0, idx, sub, 0);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x80);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, 0x05040002);                       /* check abort code                         */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*      This testcase will check the Abort code "Invalid block size, block mode only",
*      after a Go-Back-N ARQ  with blksize = 0
*                                   Abort code 0x06010000
*
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_LenTooLowAfterRestart)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom ;
    uint32_t    size = 994;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 6;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 0);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    TS_SDO_SEND (0xA0, idx, sub, CO_SDO_BUF_SEG);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_DATA    (frm, size);                          /* check block size                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);

                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk  (0x00, 127, 0, 7);                     /* check received block                     */
    TS_ACKBLK_SEND(0xA2,   0, CO_SDO_BUF_SEG);

    TS_ChkBlk  (0x00, 127, 0, 7);                     /* check received block                     */
    TS_ACKBLK_SEND(0xA2, 127, 0);

                                                      /*===== ABORT BLOCK UPLOAD =================*/
    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x80);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, 0x05040002);                       /* check abort code                         */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}


/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*      This testcase will check the Abort code "Invalid block size, block mode only",
*      after a Go-Back-N ARQ  with blksize = 128
*                                   Abort code 0x06010000
*
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_LenTooHighAfterRestart)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom ;
    uint32_t    size = 994;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 6;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 0);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    TS_SDO_SEND (0xA0, idx, sub, CO_SDO_BUF_SEG);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_DATA    (frm, size);                          /* check block size                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);

                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk  (0x00, 127, 0, 7);                     /* check received block                     */
    TS_ACKBLK_SEND(0xA2,   0, CO_SDO_BUF_SEG);

    TS_ChkBlk  (0x00, 127, 0, 7);                     /* check received block                     */
    TS_ACKBLK_SEND(0xA2, 127, 128);

                                                      /*===== ABORT BLOCK UPLOAD =================*/
    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x80);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, 0x05040002);                       /* check abort code                         */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}


/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*      This testcase will check the Abort code "Invalid sequence number, block mode only",
*      after a Go-Back-N ARQ  with ackseq = 128
*                                   Abort code 0x05040003
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_BadSeqNbrAfterRestart)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom ;
    uint32_t    size = 994;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 6;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 0);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    TS_SDO_SEND (0xA0, idx, sub, CO_SDO_BUF_SEG);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_DATA    (frm, size);                          /* check block size                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);

                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk  (0x00, 127, 0, 7);                     /* check received block                     */
    TS_ACKBLK_SEND(0xA2,   0, CO_SDO_BUF_SEG);

    TS_ChkBlk  (0x00, 127, 0, 7);                     /* check received block                     */
    TS_ACKBLK_SEND(0xA2, 128, CO_SDO_BUF_SEG);

                                                      /*===== ABORT BLOCK UPLOAD =================*/
    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x80);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, 0x05040003);                       /* check abort code                         */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}


/*------------------------------------------------------------------------------------------------*/
/*!
* \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block upload of an array with size = 42 from Domainbuffer
*         entry and afterwards the block upload of an array with size = 43 from Domainbuffer
*         entry
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_TwoDomains)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom1  = 0;
    CO_OBJ_DOM *dom2  = 0;
    uint32_t    size1 = 42;
    uint32_t    size2 = 43;
    uint16_t    idx   = 0x2520;
    uint8_t     sub1  = 2;
    uint8_t     sub2  = 3;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom1 = DomCreate(idx, sub1, CO_OBJ_____RW, size1);
    dom2 = DomCreate(idx, sub2, CO_OBJ_____RW, size2);
    DomFill(dom1, 0);
    DomFill(dom2, 0);
    TS_CreateNode(&node,0);

                                                      /*===== FIRST BLOCK UPLOAD =================*/

                                                      /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    TS_SDO_SEND (0xA0, idx, sub1, CO_SDO_BUF_SEG);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub1);                     /* check multiplexer                        */
    CHK_DATA    (frm, size1);                         /* check block size                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);

                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk(0x00, 6, 1, 7);                         /* check received block                     */

                                                      /*===== ACKNOWLEDGE BLOCK ==================*/
    TS_ACKBLK_SEND(0xA2, 6, CO_SDO_BUF_SEG);

                                                      /*===== END BLOCK UPLOAD ===================*/
    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    TS_EBLK_SEND(0xA1, 0x00000000);

    CHK_NO_ERR  (&node);                              /* check error free stack execution         */


                                                      /*===== SECOND BLOCK UPLOAD ================*/

                                                      /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    TS_SDO_SEND (0xA0, idx, sub2, CO_SDO_BUF_SEG);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub2);                     /* check multiplexer                        */
    CHK_DATA    (frm, size2);                         /* check block size                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);

                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk(0x00, 7, 1, 1);                         /* check received block                     */

                                                      /*===== ACKNOWLEDGE BLOCK ==================*/
    TS_ACKBLK_SEND(0xA2, 7, CO_SDO_BUF_SEG);

                                                      /*===== END BLOCK UPLOAD ===================*/
    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xD9);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    TS_EBLK_SEND(0xA1, 0x00000000);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}


/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block upload of an array with size = 42 from  Domainbuffer
*         entry
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_BlkWr_BlkRd)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom ;
    uint32_t    size = 10;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 2;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 'a');
    TS_CreateNode(&node,0);

    TS_SDO_SEND (0xA0, idx, sub, CO_SDO_BUF_SEG);     /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    CHK_CAN     (&frm);                               /* consume response frame                   */
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);                 /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk('a', 2, 1, 3);                          /* block (a-g, h-j) (last block: 3 bytes)   */
    TS_ACKBLK_SEND(0xA2, 2, CO_SDO_BUF_SEG);          /*===== ACKNOWLEDGE BLOCK ==================*/
    CHK_CAN     (&frm);                               /* consume response frame                   */
    TS_EBLK_SEND(0xA1, 0x00000000);                   /*===== END BLOCK UPLOAD ===================*/

    TS_SDO_SEND (0xC2, idx, sub, size);               /*===== INIT BLOCK DOWNLOAD ================*/
    CHK_CAN     (&frm);                               /* consume response frame                   */
                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 2, 1, 3);                        /* block (0-6, 7-9) (last block: 3 bytes)   */
    CHK_CAN     (&frm);                               /* consume response frame                   */
    TS_EBLK_SEND(0xC1, 0x00000000);                   /*===== END BLOCK DOWNLOAD =================*/
    CHK_CAN     (&frm);                               /* consume response frame                   */

    TS_SDO_SEND (0xA0, idx, sub, CO_SDO_BUF_SEG);     /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    CHK_CAN     (&frm);                               /* consume response frame                   */
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);                 /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk(0x00, 2, 1, 3);                         /* block (0-6, 7-9) (last block: 3 bytes)   */
    TS_ACKBLK_SEND(0xA2, 2, CO_SDO_BUF_SEG);          /*===== ACKNOWLEDGE BLOCK ==================*/
    CHK_CAN     (&frm);                               /* consume response frame                   */
    TS_EBLK_SEND(0xA1, 0x00000000);                   /*===== END BLOCK UPLOAD ===================*/

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}


/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block upload restart within a block upload transfer
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkRd_Restart_BlkRd)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom ;
    uint32_t    size = 994;
    uint16_t    idx  = 0x2520;
    uint8_t     sub  = 6;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    DomFill(dom, 0);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    TS_SDO_SEND (0xA0, idx, sub, CO_SDO_BUF_SEG);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_DATA    (frm, size);                          /* check block size                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);

                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk  (0x00, 127, 0, 7);                     /* check received block                     */

                                                      /*===== BAD INIT BLOCK UPLOAD (PHASE I) ====*/
    TS_SDO_SEND (0xA0, idx, sub, CO_SDO_BUF_SEG);
    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0x80);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_DATA    (frm, 0x05040001);                    /* check abort code                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE I) ========*/
    TS_SDO_SEND (0xA0, idx, sub, CO_SDO_BUF_SEG);
    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_DATA    (frm, size);                          /* check block size                         */

                                                      /*===== INIT BLOCK UPLOAD (PHASE II) =======*/
    TS_SDO_SEND (0xA3, 0x0000, 0, 0);
                                                      /*===== BLOCK UPLOAD =======================*/
    TS_ChkBlk  (0x00, 127, 0, 7);                     /* check received block                     */
    TS_ACKBLK_SEND(0xA2, 127, CO_SDO_BUF_SEG);

    TS_ChkBlk  (0x79, 15, 1, 7);                      /* check received block                     */
    TS_ACKBLK_SEND(0xA2, 15, CO_SDO_BUF_SEG);

                                                      /*===== END BLOCK UPLOAD ===================*/
    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xC1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    TS_EBLK_SEND(0xA1, 0x00000000);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}


/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_BLK_UP()
{
    TS_Begin(__FILE__);

//    CanDiagnosticOn(0);

    TS_RUNNER(TS_BlkRd_Basic);
    TS_RUNNER(TS_BlkRd_42ByteDomain);
    TS_RUNNER(TS_BlkRd_43ByteDomain);
    TS_RUNNER(TS_BlkRd_41ByteDomain);
    TS_RUNNER(TS_BlkRd_994ByteDomain);
    TS_RUNNER(TS_BlkRd_995ByteDomain);
    TS_RUNNER(TS_BlkRd_993ByteDomain);
    TS_RUNNER(TS_BlkRd_890ByteDomain);
    TS_RUNNER(TS_BlkRd_889ByteDomain);
    TS_RUNNER(TS_BlkRd_888ByteDomain);
    TS_RUNNER(TS_BlkRd_LostMiddleSeg);
    TS_RUNNER(TS_BlkRd_LostLastSeg);
    TS_RUNNER(TS_BlkRd_LostFirstSeg);
    TS_RUNNER(TS_BlkRd_BadCmd);
    TS_RUNNER(TS_BlkRd_ObjNotExist);
    TS_RUNNER(TS_BlkRd_SubIdxNotExist);
    TS_RUNNER(TS_BlkRd_WriteOnly);
    TS_RUNNER(TS_BlkRd_DomainNullPtr);
    TS_RUNNER(TS_BlkRd_LenTooHigh);
    TS_RUNNER(TS_BlkRd_LenTooLow);
    TS_RUNNER(TS_BlkRd_LenTooLowAfterRestart);
    TS_RUNNER(TS_BlkRd_LenTooHighAfterRestart);
    TS_RUNNER(TS_BlkRd_BadSeqNbrAfterRestart);
    TS_RUNNER(TS_BlkRd_TwoDomains);
    TS_RUNNER(TS_BlkRd_BlkWr_BlkRd);
    TS_RUNNER(TS_BlkRd_Restart_BlkRd);

//    CanDiagnosticOff(0);

    TS_End();
}
