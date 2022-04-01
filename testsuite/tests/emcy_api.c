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
/*
*          This test will check that EMCY message sent with given user data bytes.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_TxUserData)
{
    CO_IF_FRM frm;                               /* Local: virtual CAN frame                 */
    CO_EMCY_USR    usr;                               /* Local: user data for EMCY message        */
    CO_NODE        node;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    usr.Emcy[0] = 0x11;
    usr.Emcy[1] = 0x12;
    usr.Emcy[2] = 0x13;
    usr.Emcy[3] = 0x14;
    usr.Emcy[4] = 0x15;

    COEmcySet(&node.Emcy, 1, &usr);                   /* register error #1 without user info      */
    SimCanRun();

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_EMCY (frm);                                   /* check EMCY (Id and DLC)                  */
    CHK_WORD (frm, 0, 0x2000);                        /* check error code                         */
    CHK_BYTE (frm, 2, 0x03);                          /* check error register                     */

    CHK_BYTE (frm, 3, 0x11);                          /* check manufacturer specific info field   */
    CHK_BYTE (frm, 4, 0x12);
    CHK_BYTE (frm, 5, 0x13);
    CHK_BYTE (frm, 6, 0x14);
    CHK_BYTE (frm, 7, 0x15);


    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that the EMCY message is sent with changed COBID.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_TxChangedId)
{
    CO_IF_FRM frm;                               /* Local: virtual CAN frame                 */
    CO_NODE   node;
    uint32_t  emcy_id = 0x00000101;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    (void)CODictWrLong(&node.Dict,CO_DEV(0x1014,0),0x81L | (1L << 31));    /* disable EMCY COB-ID */
    (void)CODictWrLong(&node.Dict,CO_DEV(0x1014,0),emcy_id);               /* set new value       */

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1 without user info      */
    SimCanRun();

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    TS_ASSERT(0x101 == frm.Identifier);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that a change of EMCY COB-ID with value < 0x81h is aborted.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_IdTooLow)
{
    CO_IF_FRM frm;                                    /* Local: virtual CAN frame                 */
    CO_NODE   node;
    uint32_t  emcy_id = 0x00000070;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    TS_SDO_SEND(0x23, 0x1014, 0, emcy_id);            /* try to change with a low value           */
    CHK_CAN(&frm);                                    /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x80);
    CHK_MLTPX(frm, 0x1014, 0);
    CHK_DATA (frm, 0x6090030);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1 without user info      */
    SimCanRun();
    CHK_CAN(&frm);                                    /* check for a CAN frame                    */
    TS_ASSERT(0x81 == frm.Identifier);                /* unchanged Id                             */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that getting error free EMCY error status is working.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_GetStatusNoError)
{
    int16_t     err;                               /* Local: status of EMCY error              */
    CO_NODE        node;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    err = COEmcyGet(&node.Emcy, 1);                   /* get status of EMCY error #1              */
    TS_ASSERT(0 == err);                              /* check status 'no error'                  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that getting error free EMCY error status is working.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_GetStatusError)
{
    int16_t     err;                               /* Local: status of EMCY error              */
    CO_NODE        node;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1 without user info      */

    err = COEmcyGet(&node.Emcy, 1);                   /* get status of EMCY error #1              */
    TS_ASSERT(1 == err);                              /* check status 'error detected'            */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that the total number of detected EMCY errors is returned
*          correctly.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_TotalNumNoError)
{
    int16_t     num;                               /* Local: total number of EMCY errors       */
    CO_NODE        node;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    num = COEmcyCnt(&node.Emcy);                      /* get total number of EMCY errors          */
    TS_ASSERT(0 == num);                              /* check total number                       */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that the total number of detected EMCY errors is returned
*          correctly.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_TotalNumError)
{
    int16_t     num;                               /* Local: total number of EMCY errors       */
    CO_NODE        node;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1 without user info      */
    COEmcySet(&node.Emcy, 2, 0);                      /* register error #2 without user info      */
    COEmcySet(&node.Emcy, 1, 0);                      /* (re-)register error #1 without user info */

    num = COEmcyCnt(&node.Emcy);                      /* get total number of EMCY errors          */
    TS_ASSERT(2 == num);                              /* check total number                       */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_EMCY_API()
{
    TS_Begin(__FILE__);

//    CanDiagnosticOn(0);

    TS_RUNNER(TS_Emcy_TxUserData);
    TS_RUNNER(TS_Emcy_TxChangedId);
    TS_RUNNER(TS_Emcy_IdTooLow);
    TS_RUNNER(TS_Emcy_GetStatusNoError);
    TS_RUNNER(TS_Emcy_GetStatusError);
    TS_RUNNER(TS_Emcy_TotalNumNoError);
    TS_RUNNER(TS_Emcy_TotalNumError);

//    CanDiagnosticOff(0);

    TS_End();
}
