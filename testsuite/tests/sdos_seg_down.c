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
*         This testcase will check the segmented download of a basic data type
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegWr_Basic)
{
    CO_IF_FRM  frm;
    CO_NODE    node;
    uint16_t   idx  = 0x2510;
    uint8_t    sub  = 1;
    uint32_t   val  = 0;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&val));
    TS_CreateNode(&node,0);
                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0x21, idx, sub, 4);

    CHK_SDO0_OK(idx, sub);
                                                      /*===== LAST SEGMENTED DOWNLOAD  ===========*/
    TS_SEG_SEND(0x01, 0);

    CHK_CAN  (&frm);
    CHK_SDO0 (frm, 0x20);
    CHK_ZERO (frm);

    TS_ASSERT(0x03020100 == val);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}


/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the aborted segmented download when last transfer is not
*         marked correctly.
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegWr_Basic_Bad)
{
    CO_NODE    node;
    uint16_t   idx  = 0x2510;
    uint8_t    sub  = 1;
    uint32_t   val  = 0;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&val));
    TS_CreateNode(&node,0);
                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0x21, idx, sub, 4);

    CHK_SDO0_OK(idx, sub);
                                                      /*===== SEGMENTED DOWNLOAD  ================*/
    TS_SEG_SEND(0x00, 0);
    CHK_SDO0_ERR(idx, sub, 0x08000000);
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the segmented download of an array with size = 42 to the Domainbuffer
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegWr_42ByteDomain)
{
    CO_IF_FRM   frm;
    uint8_t     tgl;
    uint32_t    id;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 42;
    uint16_t    idx  = 0x2300;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0x21, idx, sub, size);

    CHK_SDO0_OK(idx, sub);

                                                      /*===== SEGMENTED DOWNLOAD  ================*/
    tgl = 0x00;                                       /* start with toggle bit 0                  */
    for (id = 0; id < (size-7); id += 7) {
        TS_SEG_SEND(tgl, id);

        CHK_CAN  (&frm);                              /* check for a CAN frame                    */
        CHK_SDO0 (frm, (0x20 | tgl));                 /* check SDO #0 response (Id and DLC)       */
        CHK_ZERO (frm);                               /* check data area                          */
                                                      /*------------------------------------------*/
        tgl ^= 0x10;                                  /* prepare the toggle bit for next request  */
    }
                                                      /*===== LAST SEGMENTED DOWNLOAD  ===========*/
    TS_SEG_SEND((0x01 | tgl), id);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, (0x20 | tgl));                     /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO (frm);                                   /* check data area                          */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*          This testcase will check the segmented download of an array with size = 43 to the Domainbuffer
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegWr_43ByteDomain)
{
    CO_IF_FRM   frm;
    uint8_t     tgl;
    uint32_t    id;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 43;
    uint16_t    idx  = 0x2300;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0x21, idx, sub, size);

    CHK_SDO0_OK(idx, sub);

                                                      /*===== SEGMENTED DOWNLOAD  ================*/
    tgl = 0x00;                                       /* start with toggle bit 0                  */
    for (id = 0; id < (size-7); id += 7) {
        TS_SEG_SEND(tgl, id);

        CHK_CAN  (&frm);                              /* check for a CAN frame                    */
        CHK_SDO0 (frm, (0x20 | tgl));                 /* check SDO #0 response (Id and DLC)       */
        CHK_ZERO (frm);                               /* check data area                          */
                                                      /*------------------------------------------*/
        tgl ^= 0x10;                                  /* prepare the toggle bit for next request  */
    }
                                                      /*===== LAST SEGMENTED DOWNLOAD  ===========*/
    TS_SEG_SEND((0x01 | tgl), id);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, (0x20 | tgl));                     /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO (frm);                                   /* check data area                          */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */

}


/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*          This testcase will check the segmented download of an array with size = 41 to the Domainbuffer
*
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegWr_41ByteDomain)
{
    CO_IF_FRM   frm;
    uint8_t     tgl;
    uint32_t    id;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 41;
    uint16_t    idx  = 0x2300;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0x21, idx, sub, size);

    CHK_SDO0_OK(idx, sub);

                                                      /*===== SEGMENTED DOWNLOAD  ================*/
    tgl = 0x00;                                       /* start with toggle bit 0                  */
    for (id = 0; id < (size-7); id += 7) {
        TS_SEG_SEND(tgl, id);

        CHK_CAN  (&frm);                              /* check for a CAN frame                    */
        CHK_SDO0 (frm, (0x20 | tgl));                 /* check SDO #0 response (Id and DLC)       */
        CHK_ZERO (frm);                               /* check data area                          */
                                                      /*------------------------------------------*/
        tgl ^= 0x10;                                  /* prepare the toggle bit for next request  */
    }
                                                      /*===== LAST SEGMENTED DOWNLOAD  ===========*/
    TS_SEG_SEND((0x01 | tgl), id);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, (0x20 | tgl));                     /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO (frm);                                   /* check data area                          */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}


/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*     This testcase will check the segmented download of an array with size = 46 to the Domainbuffer
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegWr_46ByteDomain)
{
    CO_IF_FRM   frm;
    uint8_t     tgl;
    uint32_t    id;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 46;
    uint16_t    idx  = 0x2300;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0x21, idx, sub, size);

    CHK_SDO0_OK(idx, sub);

                                                      /*===== SEGMENTED DOWNLOAD  ================*/
    tgl = 0x00;                                       /* start with toggle bit 0                  */
    for (id = 0; id < (size-7); id += 7) {
        TS_SEG_SEND(tgl, id);

        CHK_CAN  (&frm);                              /* check for a CAN frame                    */
        CHK_SDO0 (frm, (0x20 | tgl));                 /* check SDO #0 response (Id and DLC)       */
        CHK_ZERO (frm);                               /* check data area                          */
                                                      /*------------------------------------------*/
        tgl ^= 0x10;                                  /* prepare the toggle bit for next request  */
    }
                                                      /*===== LAST SEGMENTED DOWNLOAD  ===========*/
    TS_SEG_SEND((0x01 | tgl), id);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, (0x20 | tgl));                     /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO (frm);                                   /* check data area                          */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}


/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*          This testcase will check the segmented download of an array with size = 44 to the Domainbuffer
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegWr_44ByteDomain)
{
    CO_IF_FRM   frm;
    uint8_t     tgl;
    uint32_t    id;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 44;
    uint16_t    idx  = 0x2300;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0x21, idx, sub, size);

    CHK_SDO0_OK(idx, sub);

                                                      /*===== SEGMENTED DOWNLOAD  ================*/
    tgl = 0x00;                                       /* start with toggle bit 0                  */
    for (id = 0; id < (size-7); id += 7) {
        TS_SEG_SEND(tgl, id);

        CHK_CAN  (&frm);                              /* check for a CAN frame                    */
        CHK_SDO0 (frm, (0x20 | tgl));                 /* check SDO #0 response (Id and DLC)       */
        CHK_ZERO (frm);                               /* check data area                          */
                                                      /*------------------------------------------*/
        tgl ^= 0x10;                                  /* prepare the toggle bit for next request  */
    }
                                                      /*===== LAST SEGMENTED DOWNLOAD  ===========*/
    TS_SEG_SEND((0x01 | tgl), id);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, (0x20 | tgl));                     /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO (frm);                                   /* check data area                          */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

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
TS_DEF_MAIN(TS_SegWr_BadCmd)
{
    CO_NODE  node;
    uint32_t size = 42;
    uint16_t idx  = 0x2300;
    uint8_t  sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0xE0, idx, sub, 0);

    CHK_SDO0_ERR(idx, sub, 0x05040001);

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
TS_DEF_MAIN(TS_SegWr_ObjNotExist)
{
    CO_NODE  node;
    uint32_t size = 42;
    uint16_t idx  = 0x2100;
    uint8_t  sub  = 0;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

    idx++;
                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0x21, idx, sub, size);

    CHK_SDO0_ERR(idx, sub, 0x06020000);

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
TS_DEF_MAIN(TS_SegWr_SubIdxNotExist)
{
    CO_NODE  node;
    uint32_t size = 42;
    uint16_t idx  = 0x2100;
    uint8_t  sub  = 0;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

    sub++;
                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0x21, idx, sub, size);

    CHK_SDO0_ERR(idx, sub, 0x06090011);

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
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegWr_ReadOnly)
{
    CO_NODE  node;
    uint32_t size = 42;
    uint16_t idx  = 0x2100;
    uint8_t  sub  = 0;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ_____R_, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0x21, idx, sub, size);

    CHK_SDO0_ERR(idx, sub, 0x06010002);

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
TS_DEF_MAIN(TS_SegWr_LenTooHigh)
{
    CO_NODE  node;
    uint32_t size = 42;
    uint16_t idx  = 0x2100;
    uint8_t  sub  = 0;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

    size++;
                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0x21, idx, sub, size);

    CHK_SDO0_ERR(idx, sub, 0x06070012);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*         This testcase will check the segmented download of an array with size = 41 to the
*         Domainbuffer with size = 42
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegWr_42ByteDomain_41Byte)
{
    CO_IF_FRM   frm;
    uint8_t     tgl;
    uint32_t    id;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 42;
    uint16_t    idx  = 0x2300;
    uint8_t     sub  = 1;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

    size--;
                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0x21, idx, sub, size);

    CHK_SDO0_OK(idx, sub);

                                                      /*===== SEGMENTED DOWNLOAD  ================*/
    tgl = 0x00;                                       /* start with toggle bit 0                  */
    for (id = 0; id < (size-7); id += 7) {
        TS_SEG_SEND(tgl, id);

        CHK_CAN  (&frm);                              /* check for a CAN frame                    */
        CHK_SDO0 (frm, (0x20 | tgl));                 /* check SDO #0 response (Id and DLC)       */
        CHK_ZERO (frm);                               /* check data area                          */
                                                      /*------------------------------------------*/
        tgl ^= 0x10;                                  /* prepare the toggle bit for next request  */
    }
                                                      /*===== LAST SEGMENTED DOWNLOAD  ===========*/
    TS_SEG_SEND((0x01 | tgl), id);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, (0x20 | tgl));                     /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO (frm);                                   /* check data area                          */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

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
TS_DEF_MAIN(TS_SegWr_DomainNullPtr)
{
    CO_NODE  node;
    uint32_t size = 42;
    uint16_t idx  = 0x2100;
    uint8_t  sub  = 0;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TDOMAIN, (CO_DATA)(0));
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0x21, idx, sub, size);

    CHK_SDO0_ERR(idx, sub, 0x08000020);

    CHK_ERR(&node, CO_ERR_OBJ_INIT);                  /* check for expected error                 */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*          This testcase will check the Abort code " Toggle bit not alternated "
*                                   Abort code 0x05030000
*
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegWr_BadToggleBit)
{
    CO_IF_FRM   frm;
    uint8_t     tgl;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 42;
    uint16_t    idx  = 0x2300;
    uint8_t     sub  = 1;
    uint8_t     id;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0x21, idx, sub, size);

    CHK_SDO0_OK(idx, sub);

                                                      /*===== SEGMENTED DOWNLOAD  ================*/
    tgl = 0x00;                                       /* start with toggle bit 0                  */
    for (id = 0; id < (size-7); id += 7) {
        TS_SEG_SEND(tgl, id);

        CHK_CAN  (&frm);                              /* check for a CAN frame                    */
        CHK_SDO0 (frm, (0x20 | tgl));                 /* check SDO #0 response (Id and DLC)       */
        CHK_ZERO (frm);                               /* check data area                          */
                                                      /*------------------------------------------*/
        tgl ^= 0x10;                                  /* prepare the toggle bit for next request  */
    }

    tgl ^= 0x10;                                      /* insert toggle bit failure                */

                                                      /*===== LAST SEGMENTED DOWNLOAD  ===========*/
    TS_SEG_SEND((0x01 | tgl), id);

    CHK_SDO0_ERR(idx, sub, 0x05030000);

    CHK_DOM_PART(dom, 0, 7);                          /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TESTCASE DESCRIPTION
*
* \ingroup TS_CO
*
*        This testcase will check the abort request sending from client to server segmented download
*        and after them the client starts a second transfer to server.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_SegWr_RestartTransfer)
{
    CO_IF_FRM   frm;
    uint8_t     tgl;
    CO_NODE     node;
    CO_OBJ_DOM *dom;
    uint32_t    size = 42;
    uint16_t    idx  = 0x2300;
    uint8_t     sub  = 1;
    uint8_t     id;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    dom = DomCreate(idx, sub, CO_OBJ_____RW, size);
    TS_CreateNode(&node,0);

                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0x21, idx, sub, size);

    CHK_SDO0_OK(idx, sub);

                                                      /*===== SEGMENTED DOWNLOAD  ================*/
    tgl = 0x00;                                       /* start with toggle bit 0                  */
    for (id = 0; id < (size-21); id += 7) {
        TS_SEG_SEND(tgl, id);

        CHK_CAN  (&frm);                              /* check for a CAN frame                    */
        CHK_SDO0 (frm, (0x20 | tgl));                 /* check SDO #0 response (Id and DLC)       */
        CHK_ZERO (frm);                               /* check data area                          */
                                                      /*------------------------------------------*/
        tgl ^= 0x10;                                  /* prepare the toggle bit for next request  */
    }
                                                      /*===== ABORT REQUEST ======================*/
    TS_SDO_SEND (0x80, 0x0000, 0, 0x00000000);
    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */

                                                      /*===== INIT SEGMENTED DOWNLOAD  ===========*/
    TS_SDO_SEND (0x21, idx, sub, size);

    CHK_SDO0_OK(idx, sub);

                                                      /*===== SEGMENTED DOWNLOAD  ================*/
    tgl = 0x00;                                       /* start with toggle bit 0                  */
    for (id = 0; id < (size-7); id += 7) {
        TS_SEG_SEND(tgl, id);

        CHK_CAN  (&frm);                              /* check for a CAN frame                    */
        CHK_SDO0 (frm, (0x20 | tgl));                 /* check SDO #0 response (Id and DLC)       */
        CHK_ZERO (frm);                               /* check data area                          */
                                                      /*------------------------------------------*/
        tgl ^= 0x10;                                  /* prepare the toggle bit for next request  */
    }
                                                      /*===== LAST SEGMENTED DOWNLOAD  ===========*/
    TS_SEG_SEND((0x01 | tgl), id);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, (0x20 | tgl));                     /* check SDO #0 response (Id and DLC)       */
    CHK_ZERO (frm);                                   /* check data area                          */

    CHK_DOM_FULL(dom, 0);                             /* check content of domain                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_SEG_DOWN()
{
    TS_Begin(__FILE__);

//    CanDiagnosticOn(0);

    TS_RUNNER(TS_SegWr_Basic);
    TS_RUNNER(TS_SegWr_Basic_Bad);
    TS_RUNNER(TS_SegWr_42ByteDomain);
    TS_RUNNER(TS_SegWr_43ByteDomain);
    TS_RUNNER(TS_SegWr_41ByteDomain);
    TS_RUNNER(TS_SegWr_46ByteDomain);
    TS_RUNNER(TS_SegWr_44ByteDomain);
    TS_RUNNER(TS_SegWr_BadCmd);
    TS_RUNNER(TS_SegWr_ObjNotExist);
    TS_RUNNER(TS_SegWr_SubIdxNotExist);
    TS_RUNNER(TS_SegWr_ReadOnly);
    TS_RUNNER(TS_SegWr_LenTooHigh);
    TS_RUNNER(TS_SegWr_42ByteDomain_41Byte);
    TS_RUNNER(TS_SegWr_DomainNullPtr);
    TS_RUNNER(TS_SegWr_BadToggleBit);
    TS_RUNNER(TS_SegWr_RestartTransfer);

//    CanDiagnosticOff(0);

    TS_End();
}
