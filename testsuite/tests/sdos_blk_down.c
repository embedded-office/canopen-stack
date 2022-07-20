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
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block download of a basic data type
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_Basic)
{
    CO_IF_FRM  frm;
    CO_NODE    node;
    uint16_t   idx  = 0x2510;
    uint8_t    sub  = 3;
    uint32_t   val  = 0;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC2, idx, sub, 4);                  /* ccs=6, cc=0, s=1, cs=0                   */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 1, 1, 0);                        /* transmit segment in (last) block         */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 1);                             /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xCD, 0x00000000);                   /* ccs=6, n=3, cs=1                         */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    TS_ASSERT(0x03020100 == val);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block download of an array with size = 42 to the Domainbuffer
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_42ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 42;
    uint16_t    idx  = 0x2100;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);
                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC2, idx, sub, size);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 6, 1, 0);                        /* transmit segments in (last) block        */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 6);                             /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xC1, 0x00000000);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*          This testcase will check the block download of an array with size = 43 to the Domainbuffer
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_43ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 43;
    uint16_t    idx  = 0x2100;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC2, idx, sub, size);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 7, 1, 0);                        /* transmit segments in (last) block        */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 7);                             /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xD9, 0x00000000);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*          This testcase will check the block download of an array with size = 41 to the Domainbuffer
*
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_41ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 41;
    uint16_t    idx  = 0x2100;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC2, idx, sub, size);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 6, 1, 0);                        /* transmit segments in (last) block        */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 6);                             /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xC5, 0x00000000);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block download of an array with size = 994 to the Domainbuffer
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_994ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 994;
    uint16_t    idx  = 0x2100;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC2, idx, sub, size);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 127, 0, 0);                      /* transmit segments in block               */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 127);                           /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

    TS_SendBlk(0x79, 15, 1, 0);                       /* cont. transmit segments to (last) block  */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 15);                            /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xC1, 0x00000000);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block download of an array with size = 995 to the Domainbuffer
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_995ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 995;
    uint16_t    idx  = 0x2100;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC2, idx, sub, size);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 127, 0, 0);                      /* transmit segments in block               */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 127);                           /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

    TS_SendBlk(0x79, 16, 1, 0);                       /* cont. transmit segments to (last) block  */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 16);                            /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xD9, 0x00000000);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block download of an array with size = 993 to the Domainbuffer
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_993ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 993;
    uint16_t    idx  = 0x2100;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC2, idx, sub, size);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 127, 0, 0);                      /* transmit segments in block               */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 127);                           /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

    TS_SendBlk(0x79, 15, 1, 0);                       /* cont. transmit segments to (last) block  */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 15);                            /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xC5, 0x00000000);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block download of an array with max size from the Domainbuffer
*         data set size is not indicated (size indicator = 0)
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_42ByteDomain_NoLen)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 42;
    uint16_t    idx  = 0x2100;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC0, idx, sub, 0);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 6, 1, 0);                        /* transmit segments in (last) block        */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 6);                             /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xC1, 0x00000000);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}


/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block download of an array with max size from the Domainbuffer
*         data set size is not indicated (size indicator = 0) in request message
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_43ByteDomain_NoLen)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 43;
    uint16_t    idx  = 0x2100;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC0, idx, sub, 0);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 7, 1, 0);                        /* transmit segments in (last) block        */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 7);                             /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xD9, 0x00000000);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block download of an array with max size from the Domainbuffer
*         data set size is not indicated (size indicator = 0) in request message
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_41ByteDomain_NoLen)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 41;
    uint16_t    idx  = 0x2100;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC0, idx, sub, 0);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 6, 1, 0);                        /* transmit segments in (last) block        */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 6);                             /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xC5, 0x00000000);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block download of an max array size = 1000 to the Domainbuffer
*         with data set is not indicated (size indicator = 0)
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_1000ByteDomain_NoLen)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 1000;
    uint16_t    idx  = 0x2100;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC0, idx, sub, 0);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 127, 0, 0);                      /* transmit segments in block               */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 127);                           /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

    TS_SendBlk(0x79, 16, 1, 0);                       /* cont. transmit segments to (last) block  */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 16);                            /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xC5, 0x00000000);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}


/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*          This testcase will check the failured block download of loosing one Segment in the middle
*          of the transfer
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_LostSeg)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 42;
    uint16_t    idx  = 0x2100;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC0, idx, sub, size);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 6, 1, 3);                        /* transmit segment 1 and 2; 3 is lost      */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 2);                             /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== RE-SEND FAILED BLOCK DOWNLOAD ======*/
    TS_SendBlk(0x0E, 4, 1, 0);                        /* transmit segment 1 to 4                  */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 4);                             /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xC1, 0x00000000);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */
    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block download of an array with size = 890 to the Domainbuffer
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_890ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 890;
    uint16_t    idx  = 0x2100;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC2, idx, sub, 890);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 127, 1, 0);                      /* transmit segments in block               */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 127);                           /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xC1, 0x00000000);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}


/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block download of an array with size = 889 to the Domainbuffer
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_889ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 889;
    uint16_t    idx  = 0x2100;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC2, idx, sub, size);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 127, 1, 0);                      /* transmit segments in block               */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 127);                           /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xC5, 0x00000000);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}


/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*          This testcase will check the block download of an array with size = 46 to the Domainbuffer
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_46ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 46;
    uint16_t    idx  = 0x2100;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC2, idx, sub, size);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 7, 1, 0);                        /* transmit segments in (last) block        */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 7);                             /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xCD, 0x00000000);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*          This testcase will check the block download of an array with size = 44 to the Domainbuffer
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_44ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 44;
    uint16_t    idx  = 0x2100;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC2, idx, sub, size);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 7, 1, 0);                        /* transmit segments in (last) block        */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 7);                             /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xD5, 0x00000000);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

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
TS_DEF_MAIN(TS_BlkWr_BadCmd)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  size = 44;
    uint16_t  idx  = 0x2100;
    uint8_t   sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0xE0, idx, sub, size);

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
TS_DEF_MAIN(TS_BlkWr_ObjNotExist)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  size = 42;
    uint16_t  idx  = 0x2100;
    uint8_t   sub  = 0;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

    idx++;
                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0xC2, idx, sub, size);

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
TS_DEF_MAIN(TS_BlkWr_SubIdxNotExist)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  size = 42;
    uint16_t  idx  = 0x2100;
    uint8_t   sub  = 0;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

    sub++;
                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0xC2, idx, sub, size);

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
*      This testcase will check the Abort code " Attempt to write a read only object "
*                                   Abort code 0x06010002
*
*/
/*-----------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_ReadOnly)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  size = 42;
    uint16_t  idx  = 0x2100;
    uint8_t   sub  = 0;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ_____R_, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0xC2, idx, sub, size);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x80);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, 0x06010002);                       /* check abort code                         */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*          This testcase will check the Abort code " Data type doesn't match, length too high.  "
*                                   Abort code 0x06070012
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_LenTooHigh)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  size = 42;
    uint16_t  idx  = 0x2100;
    uint8_t   sub  = 0;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

    size++;
                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0xC2, idx, sub, size);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x80);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, 0x06070012);                       /* check abort code                         */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*      This testcase will check the Abort code "Data cannot be transferred or stored to the application"
*                                   Abort code 0x08000020
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_DomainNullPtr)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  idx  = 0x2100;
    uint8_t   sub  = 0;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TDOMAIN, (CO_DATA)(0));
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0xC2, idx, sub, 0);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x80);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, 0x08000020);                       /* check abort code                         */

    CHK_ERR(&node, CO_ERR_OBJ_INIT);                  /* check for expected error                 */
}


/*------------------------------------------------------------------------------------------------*/
/*!
* \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block download of an array with size = 42 to the Domainbuffer
*         and afterwards the block download of an array with size = 43 to the Domainbuffer
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_TwoDomains)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom1  = 0;
    CO_OBJ_DOM *dom2  = 0;
    uint32_t    size1 = 42;
    uint32_t    size2 = 43;
    uint16_t    idx   = 0x2100;
    uint8_t     sub1  = 0;
    uint8_t     sub2  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom1 = DomCreate(idx, sub1, CO_OBJ_____RW, size1);
    dom2 = DomCreate(idx, sub2, CO_OBJ_____RW, size2);
    TS_CreateNode(&node,0);

                                                      /*===== FIRST BLOCK DOWNLOAD ===============*/

                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC2, idx, sub1, size1);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub1);                     /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 6, 1, 0);                        /* transmit segments in (last) block        */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 6);                             /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xC1, 0x00000000);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    CHK_DOM_FULL(dom1, 0);                            /* check content of domain                  */
    CHK_DOM_PART(dom2, 0, dom2->Size);                /* check content of domain                  */
    DomClear    (dom1);                               /* reset domain with default content (0xFF) */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */

                                                      /*===== SECOND BLOCK DOWNLOAD ==============*/

                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC2, idx, sub2, size2);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub2);                     /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 7, 1, 0);                        /* transmit segments in (last) block        */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 7);                             /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xD9, 0x00000000);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    CHK_DOM_FULL(dom2, 0);                            /* check content of domain                  */
    CHK_DOM_PART(dom1, 0, dom1->Size);                /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block download of an array with size = 455 to the
*         Domainbuffer. size = 455 will cause 65 (0x41) segments and this will cause the same
*         command for the last block as for end block transfer. 0x41 | 0x80 => 0xC1
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_455ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 455;
    uint16_t    idx  = 0x2100;
    uint8_t     sub  = 0;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC2, idx, sub, size);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00,  65, 1, 0);                      /* transmit segments in block               */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 65);                            /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */


                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xC1, 0x00000000);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}



/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block download of an array with size = 35 to the
*         Domainbuffer with size = 42
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_42ByteDomain_35Byte)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 42;
    uint16_t    idx  = 0x2100;
    uint8_t     sub  = 0;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

    size = 35;
                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC2, idx, sub, size);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 5, 1, 0);                        /* transmit segments in (last) block        */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 5);                             /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xC1, 0x00000000);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    CHK_DOM_PART(dom, 0, 7);                          /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}


/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block download of an array with size = 49 to the
*         Domainbuffer with size = 42, with size indicated
*         This testcase will check the Abort code " Data type doesn't match, length too high.  "
*                                   Abort code 0x06070012
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_42ByteDomain_49Byte)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  size = 42;
    uint16_t  idx  = 0x2100;
    uint8_t   sub  = 0;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

    size = 49;
                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC2, idx, sub, size);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x80);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, 0x06070012);                       /* check abort code                         */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the block download of an array with size = 49 to the
*         Domainbuffer with size = 42, without size indicated
*         This testcase will check the Abort code " Data type doesn't match, length too high.  "
*                                   Abort code 0x06070012
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_42ByteDomain_49Byte_NoLen)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  size = 42;
    uint16_t  idx  = 0x2100;
    uint8_t   sub  = 0;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC2, idx, sub, 0);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 7, 1, 0);                        /* transmit segments in (last) block        */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x80);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx, sub);                         /* check multiplexer                        */
    CHK_DATA (frm, 0x06070012);                       /* check abort code                         */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief   TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*          This testcase will check the block download of an array with size = 43 to the
*          Domainbuffer and afterwards executes an expedited transfer to check if all sdo info
*          table entries are cleared at end of block transfer.
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_BlkWr_ExpWrAfter43ByteDomain)
{
    CO_IF_FRM   frm;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 43;
    uint16_t    idx  = 0x2100;
    uint8_t     sub  = 0;
    uint32_t    val  = 0;
    uint16_t    idx2 = 0x2200;
    uint8_t     sub2 = 0;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_ODAdd(CO_KEY(idx2, sub2, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

                                                      /*===== INIT BLOCK DOWNLOAD ================*/
    TS_SDO_SEND (0xC2, idx, sub, size);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA0);                          /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX   (frm, idx, sub);                      /* check multiplexer                        */
    CHK_BLKSIZE (frm, CO_SDO_BUF_SEG);                /* check block size                         */

                                                      /*===== BLOCK DOWNLOAD =====================*/
    TS_SendBlk(0x00, 7, 1, 0);                        /* transmit segments in (last) block        */

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA2);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ACKSEQ  (frm, 7);                             /* check acknowledged sequence number       */
    CHK_NEXTBLK (frm, CO_SDO_BUF_SEG);                /* check next block size                    */

                                                      /*===== END BLOCK DOWNLOAD =================*/
    TS_EBLK_SEND(0xD9, 0x00000000);

    CHK_CAN     (&frm);                               /* check for a CAN frame                    */
    CHK_SDO0    (frm, 0xA1);                          /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO    (frm);                                /* check cleared data area                  */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    TS_SDO_SEND (0x23, idx2, sub2, 0x44434241);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x60);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, idx2, sub2);                       /* check multiplexer                        */
    CHK_DATA (frm, 0x00000000);                       /* check data area                          */
                                                      /*------------------------------------------*/
    TS_ASSERT(0x44434241 == val);                     /* check parameter in object dictionary      */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_BLK_DOWN()
{
    TS_Begin(__FILE__);

//    CanDiagnosticOn(0);

    TS_RUNNER(TS_BlkWr_Basic);
    TS_RUNNER(TS_BlkWr_42ByteDomain);
    TS_RUNNER(TS_BlkWr_43ByteDomain);
    TS_RUNNER(TS_BlkWr_41ByteDomain);
    TS_RUNNER(TS_BlkWr_994ByteDomain);
    TS_RUNNER(TS_BlkWr_995ByteDomain);
    TS_RUNNER(TS_BlkWr_993ByteDomain);
    TS_RUNNER(TS_BlkWr_42ByteDomain_NoLen);
    TS_RUNNER(TS_BlkWr_43ByteDomain_NoLen);
    TS_RUNNER(TS_BlkWr_41ByteDomain_NoLen);
    TS_RUNNER(TS_BlkWr_1000ByteDomain_NoLen);
    TS_RUNNER(TS_BlkWr_LostSeg);
    TS_RUNNER(TS_BlkWr_890ByteDomain);
    TS_RUNNER(TS_BlkWr_889ByteDomain);
    TS_RUNNER(TS_BlkWr_46ByteDomain);
    TS_RUNNER(TS_BlkWr_44ByteDomain);
    TS_RUNNER(TS_BlkWr_BadCmd);
    TS_RUNNER(TS_BlkWr_ObjNotExist);
    TS_RUNNER(TS_BlkWr_SubIdxNotExist);
    TS_RUNNER(TS_BlkWr_ReadOnly);
    TS_RUNNER(TS_BlkWr_LenTooHigh);
    TS_RUNNER(TS_BlkWr_DomainNullPtr);
    TS_RUNNER(TS_BlkWr_TwoDomains);
    TS_RUNNER(TS_BlkWr_455ByteDomain);
    TS_RUNNER(TS_BlkWr_42ByteDomain_35Byte);
    TS_RUNNER(TS_BlkWr_42ByteDomain_49Byte);
    TS_RUNNER(TS_BlkWr_42ByteDomain_49Byte_NoLen);
    TS_RUNNER(TS_BlkWr_ExpWrAfter43ByteDomain);

//    CanDiagnosticOff(0);

    TS_End();
}
