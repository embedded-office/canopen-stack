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
*          This testcase will check the transmission of SYNC messages with cycle period 1s
*          in PRE-OPERATIONAL mode.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Sync_1s)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  id     = CO_SYNC_COBID_ON | 0x80L;
    uint32_t  period = 1000L * 1000L;

    TS_CreateMandatoryDir();
    TS_CreateSyncPeriod(&id, &period);
    TS_CreateNode(&node, 0);

    TS_Wait(&node, 500);

    CHK_NOCAN(&frm);                                  /* check for no CAN frame                   */

    TS_Wait(&node, 1000);

    CHK_CAN(&frm);                                    /* check for a CAN frame                    */
    CHK_SYNC(frm, 0x80);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC2
*
*          This testcase will check the transmission of SYNC messages with cycle period 100ms
*          in OPERATIONAL mode.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Sync_100ms)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  id     = CO_SYNC_COBID_ON | 0x80L;
    uint32_t  period = 100L * 1000L;

    TS_CreateMandatoryDir();
    TS_CreateSyncPeriod(&id, &period);
    TS_CreateNodeAutoStart(&node);

    TS_Wait(&node, 50);

    CHK_NOCAN(&frm);                                  /* check for no CAN frame                   */

    TS_Wait(&node, 100);

    CHK_CAN(&frm);                                    /* check for a CAN frame                    */
    CHK_SYNC(frm, 0x80);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC3
*
*          This testcase will check that nothing is transmitted when producer is disabled.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Sync_disabled)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  id     = 0x80L;
    uint32_t  period = 100L * 1000L;

    TS_CreateMandatoryDir();
    TS_CreateSyncPeriod(&id, &period);
    TS_CreateNodeAutoStart(&node);

    TS_Wait(&node, 50);

    CHK_NOCAN(&frm);                                  /* check for no CAN frame                   */

    TS_Wait(&node, 100);

    CHK_NOCAN(&frm);                                  /* check for no CAN frame                   */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC4
*
*          This testcase will check that nothing is transmitted when NMT mode is STOP.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Sync_stop_mode)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  id     = CO_SYNC_COBID_ON | 0x80L;
    uint32_t  period = 100L * 1000L;

    TS_CreateMandatoryDir();
    TS_CreateSyncPeriod(&id, &period);
    TS_CreateNodeAutoStart(&node);
    CONmtSetMode(&node.Nmt, CO_STOP);

    TS_Wait(&node, 50);

    CHK_NOCAN(&frm);                                  /* check for no CAN frame                   */

    TS_Wait(&node, 100);

    CHK_NOCAN(&frm);                                  /* check for no CAN frame                   */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC5
*
*          This testcase will check that SYNC COB-ID is changeable via CAN.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Sync_change_id)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  id     = CO_SYNC_COBID_ON | 0x80L;
    uint32_t  period = 100L * 1000L;

    TS_CreateMandatoryDir();
    TS_CreateSyncPeriod(&id, &period);
    TS_CreateNode(&node,0);

    TS_Wait(&node, 50);

    CHK_NOCAN(&frm);                                  /* check for no CAN frame                   */

    TS_Wait(&node, 100);

    CHK_CAN(&frm);                                    /* check for a CAN frame                    */
    CHK_SYNC(frm, 0x80);

    TS_SDO_SEND(0x23, 0x1005, 0, 0x80);
    CHK_CAN(&frm);
    TS_SDO_SEND(0x23, 0x1005, 0, CO_SYNC_COBID_ON | 0x88);
    CHK_CAN(&frm);

    TS_Wait(&node, 100);

    CHK_CAN(&frm);                                    /* check for a CAN frame                    */
    CHK_SYNC(frm, 0x88);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC6
*
*          This testcase will check that SYNC COB-ID is frozen when active.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Sync_freeze_id)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  id     = CO_SYNC_COBID_ON | 0x80L;
    uint32_t  period = 100L * 1000L;

    TS_CreateMandatoryDir();
    TS_CreateSyncPeriod(&id, &period);
    TS_CreateNode(&node,0);

    TS_Wait(&node, 50);

    CHK_NOCAN(&frm);                                  /* check for no CAN frame                   */

    TS_Wait(&node, 100);

    CHK_CAN(&frm);                                    /* check for a CAN frame                    */
    CHK_SYNC(frm, 0x80);

    TS_SDO_SEND(0x23, 0x1005, 0, CO_SYNC_COBID_ON | 0x88);
    CHK_SDO0_ERR(0x1005, 0, 0x06090030);

    TS_Wait(&node, 100);

    CHK_CAN(&frm);                                    /* check for a CAN frame                    */
    CHK_SYNC(frm, 0x80);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC7
*
*          This testcase will check that SYNC period is changeable via CAN.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Sync_change_period)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  id     = CO_SYNC_COBID_ON | 0x80L;
    uint32_t  period = 200L * 1000L;

    TS_CreateMandatoryDir();
    TS_CreateSyncPeriod(&id, &period);
    TS_CreateNode(&node,0);

    TS_Wait(&node, 50);

    CHK_NOCAN(&frm);                                  /* check for no CAN frame                   */

    TS_Wait(&node, 200);

    CHK_CAN(&frm);                                    /* check for a CAN frame                    */
    CHK_SYNC(frm, 0x80);

    TS_SDO_SEND(0x23, 0x1006, 0, 100L * 1000L);
    CHK_CAN(&frm);

    TS_Wait(&node, 50);

    CHK_NOCAN(&frm);                                  /* check for no CAN frame                   */

    TS_Wait(&node, 100);

    CHK_CAN(&frm);                                    /* check for a CAN frame                    */
    CHK_SYNC(frm, 0x80);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_SYNC_PROD()
{
    TS_Begin(__FILE__);

    TS_RUNNER(TS_Sync_1s);
    TS_RUNNER(TS_Sync_100ms);
    TS_RUNNER(TS_Sync_disabled);
    TS_RUNNER(TS_Sync_stop_mode);
    TS_RUNNER(TS_Sync_change_id);
    TS_RUNNER(TS_Sync_freeze_id);
    TS_RUNNER(TS_Sync_change_period);

    TS_End();
}
