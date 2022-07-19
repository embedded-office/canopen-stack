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
/*! \brief TC1
*
*          This testcase will check:
*          - no transmission of heartbeat when heartbeat time is 0 (disabled).
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_HBProd_Disable)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  time = 0;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1017, 0, CO_OBJ_____RW), CO_THB_PROD, (CO_DATA)(&time));
    TS_CreateNode(&node,0);
                                                      /*------------------------------------------*/
    TS_Wait(&node, 1000);                             /* wait 1000ms                              */
    CHK_NOCAN(&frm);                                  /* check for no CAN frame                   */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC2
*
*          This testcase will check:
*          - transmission of heartbeat when heartbeat time is 50ms (enabled).
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_HBProd_PreOperational)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  time = 50;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1017, 0, CO_OBJ_____RW), CO_THB_PROD, (CO_DATA)(&time));
    TS_CreateNode(&node,0);
                                                      /*------------------------------------------*/
    TS_Wait(&node, 50);                               /* wait heartbeat time                      */

    CHK_CAN(&frm);                                    /* check for a CAN frame                    */
    CHK_NMT(frm, 1, 127);                             /* check for heartbeat message (1, pre-op)  */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC3
*
*          This testcase will check:
*          - transmission of heartbeat when heartbeat time is 50 (enabled).
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_HBProd_Operational)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  time = 50;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1017, 0, CO_OBJ_____RW), CO_THB_PROD, (CO_DATA)(&time));
    TS_CreateNode(&node,0);
                                                      /*------------------------------------------*/
    TS_NMT_SEND(0x01, 1);                            /* set node-id 0x01 to operational          */
    TS_Wait(&node, 50);                               /* wait heartbeat time                      */

    CHK_CAN(&frm);                                    /* check for a CAN frame                    */
    CHK_NMT(frm, 1, 5);                               /* check for heartbeat message (1, oper)    */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_NMT_HBP()
{
    TS_Begin(__FILE__);
    
//    CanDiagnosticOn(0);

    TS_RUNNER(TS_HBProd_Disable);
    TS_RUNNER(TS_HBProd_PreOperational);
    TS_RUNNER(TS_HBProd_Operational);

//    CanDiagnosticOff(0);

    TS_End();
}
