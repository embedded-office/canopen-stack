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
*          This test will check that the history contains the correct number of
*          single detected emergency.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_HistNumOnSet)
{
    CO_NODE        node;
    uint8_t     hist_num;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1                        */

    (void)CODictRdByte(&node.Dict,CO_DEV(0x1003,0),&hist_num);
    TS_ASSERT(1 == hist_num);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that the history contains the correct number of
*          same emergency detected 2 times.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_HistNumOnSetRepeat)
{
    CO_NODE        node;
    uint8_t     hist_num;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1                        */
    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1                        */

    (void)CODictRdByte(&node.Dict,CO_DEV(0x1003,0),&hist_num);
    TS_ASSERT(1 == hist_num);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that the history contains the correct number of
*          2 different detected emergencies.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_HistNumOnSetMulti)
{
    CO_NODE        node;
    uint8_t     hist_num;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1                        */
    COEmcySet(&node.Emcy, 2, 0);                      /* register error #1                        */

    (void)CODictRdByte(&node.Dict,CO_DEV(0x1003,0),&hist_num);
    TS_ASSERT(2 == hist_num);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that the history contains the correct number of
*          emergencies after clearing an emergency.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_HistNumOnClr)
{
    CO_NODE        node;
    uint8_t     hist_num;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1                        */
    COEmcySet(&node.Emcy, 2, 0);                      /* register error #1                        */

    COEmcyClr(&node.Emcy, 2);                         /* clear error #1                           */

    (void)CODictRdByte(&node.Dict,CO_DEV(0x1003,0),&hist_num);
    TS_ASSERT(2 == hist_num);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that the history contains the correct number of
*          emergencies after clearing all emergency.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_HistNumOnReset)
{
    CO_NODE        node;
    uint8_t     hist_num;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1                        */
    COEmcySet(&node.Emcy, 2, 0);                      /* register error #1                        */

    COEmcyReset(&node.Emcy, 0);                       /* reset errors                             */

    (void)CODictRdByte(&node.Dict,CO_DEV(0x1003,0),&hist_num);
    TS_ASSERT(2 == hist_num);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that the history contains the correct entry of a
*          single detected emergency.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_HistObjOnSet)
{
    CO_NODE        node;
    uint32_t     hist;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1                        */

    (void)CODictRdLong(&node.Dict,CO_DEV(0x1003,1),&hist);
    TS_ASSERT(0x2000 == hist);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that the history contains the correct number of
*          same emergency detected 2 times.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_HistObjOnSetRepeat)
{
    CO_NODE        node;
    uint32_t     hist;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1                        */
    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1                        */

    (void)CODictRdLong(&node.Dict,CO_DEV(0x1003,2),&hist);
    TS_ASSERT(0 == hist);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that the history contains the correct number of
*          2 different detected emergencies.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_HistObjOnSetMulti)
{
    CO_NODE        node;
    uint32_t     hist;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1                        */
    COEmcySet(&node.Emcy, 2, 0);                      /* register error #2                        */

    (void)CODictRdLong(&node.Dict,CO_DEV(0x1003,1),&hist);
    TS_ASSERT(0x3000 == hist);

    (void)CODictRdLong(&node.Dict,CO_DEV(0x1003,2),&hist);
    TS_ASSERT(0x2000 == hist);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that the history contains the correct number of
*          emergencies after clearing an emergency.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_HistObjOnClr)
{
    CO_NODE        node;
    uint32_t     hist;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1                        */
    COEmcySet(&node.Emcy, 2, 0);                      /* register error #1                        */

    COEmcyClr(&node.Emcy, 2);                         /* clear error #1                           */

    (void)CODictRdLong(&node.Dict,CO_DEV(0x1003,1),&hist);
    TS_ASSERT(0x3000 == hist);

    (void)CODictRdLong(&node.Dict,CO_DEV(0x1003,2),&hist);
    TS_ASSERT(0x2000 == hist);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that the history contains the correct number of
*          emergencies after clearing all emergency.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_HistObjOnReset)
{
    CO_NODE        node;
    uint32_t     hist;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1                        */
    COEmcySet(&node.Emcy, 2, 0);                      /* register error #1                        */

    COEmcyReset(&node.Emcy, 0);                       /* reset errors                             */

    (void)CODictRdLong(&node.Dict,CO_DEV(0x1003,1),&hist);
    TS_ASSERT(0x3000 == hist);

    (void)CODictRdLong(&node.Dict,CO_DEV(0x1003,2),&hist);
    TS_ASSERT(0x2000 == hist);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that the history is cleared via CAN
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_HistClearOk)
{
    CO_NODE        node;
    uint8_t     hist_num;
    uint32_t     hist;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1 without user info      */
    COEmcySet(&node.Emcy, 2, 0);                      /* register error #2 without user info      */
    SimCanRun();

    TS_SDO_SEND (0x2F, 0x1003, 0, 0x00);

    (void)CODictRdByte(&node.Dict,CO_DEV(0x1003,0),&hist_num);
    TS_ASSERT(0 == hist_num);

    (void)CODictRdLong(&node.Dict,CO_DEV(0x1003,1),&hist);
    TS_ASSERT(0 == hist);

    (void)CODictRdLong(&node.Dict,CO_DEV(0x1003,2),&hist);
    TS_ASSERT(0 == hist);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that the history is not cleared if write command via CAN
*          is writing the wrong value (not 0).
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_HistClearBad)
{
    CO_IF_FRM frm;                                    /* Local: virtual CAN frame                 */
    CO_NODE   node;
    uint8_t   hist_num;
    uint32_t  hist;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1 without user info      */
    COEmcySet(&node.Emcy, 2, 0);                      /* register error #2 without user info      */
    SimCanRun();
    CHK_CAN(&frm);                                    /* check for a CAN frame                    */
    CHK_CAN(&frm);                                    /* check for a CAN frame                    */

    TS_SDO_SEND (0x2F, 0x1003, 0, 0x01);
    CHK_CAN(&frm);                                    /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x80);
    CHK_MLTPX(frm, 0x1003, 0);
    CHK_DATA (frm, 0x6090030);

    (void)CODictRdByte(&node.Dict,CO_DEV(0x1003,0),&hist_num);
    TS_ASSERT(2 == hist_num);

    (void)CODictRdLong(&node.Dict,CO_DEV(0x1003,1),&hist);
    TS_ASSERT(0x3000 == hist);

    (void)CODictRdLong(&node.Dict,CO_DEV(0x1003,2),&hist);
    TS_ASSERT(0x2000 == hist);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_EMCY_HIST()
{
    TS_Begin(__FILE__);

//    CanDiagnosticOn(0);

    TS_RUNNER(TS_Emcy_HistNumOnSet);
    TS_RUNNER(TS_Emcy_HistNumOnSetRepeat);
    TS_RUNNER(TS_Emcy_HistNumOnSetMulti);
    TS_RUNNER(TS_Emcy_HistNumOnClr);
    TS_RUNNER(TS_Emcy_HistNumOnReset);

    TS_RUNNER(TS_Emcy_HistObjOnSet);
    TS_RUNNER(TS_Emcy_HistObjOnSetRepeat);
    TS_RUNNER(TS_Emcy_HistObjOnSetMulti);
    TS_RUNNER(TS_Emcy_HistObjOnClr);
    TS_RUNNER(TS_Emcy_HistObjOnReset);
    TS_RUNNER(TS_Emcy_HistClearOk);
    TS_RUNNER(TS_Emcy_HistClearBad);

//    CanDiagnosticOff(0);

    TS_End();
}
