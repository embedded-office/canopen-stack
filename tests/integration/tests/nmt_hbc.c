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
*          This testcase will check that the read access gets the correct encoded value.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_HBCons_RdEntry)
{
    CO_NODE   node;
    CO_HBCONS data = { 0 };
    uint32_t  value;

    data.NodeId = 10;
    data.Time   = 50;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1016, 0, CO_OBJ_D___R_), CO_THB_CONS, (CO_DATA)(1));
    TS_ODAdd(CO_KEY(0x1016, 1, CO_OBJ_____R_), CO_THB_CONS, (CO_DATA)(&data));
    TS_CreateNode(&node,0);
                                                      /*------------------------------------------*/
    CODictRdLong(&node.Dict, CO_DEV(0x1016, 1), &value);
    TS_ASSERT(0x000A0032 == value);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC2
*
*          This testcase will focus on the situation:
*            HBP : --------------------O- (O: heartbeat)
*            HBC : ....................|. (!: heartbeat consumer check/event)
*          Check that the consumer waits for first heartbeat before activating the monitoring.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_HBCons_WaitForHb)
{
    CO_NODE   node;
    CO_HBCONS data = { 0 };
    int16_t   events;

    data.NodeId = 10;
    data.Time   = 50;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1016, 0, CO_OBJ_D___R_), CO_THB_CONS, (CO_DATA)(1));
    TS_ODAdd(CO_KEY(0x1016, 1, CO_OBJ_____R_), CO_THB_CONS, (CO_DATA)(&data));
    TS_CreateNode(&node,0);
                                                      /*------------------------------------------*/
    TS_Wait(&node, 200);

    events = CONmtGetHbEvents(&node.Nmt, 10);
    TS_ASSERT(0 == events);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC3
*
*          This testcase will focus on the situation:
*            HBP : O----?-- (O: heartbeat)
*            HBC : |.....!. (!: heartbeat consumer check/event)
*          Check that the consumer returns first state correctly.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_HBCons_CheckState)
{
    CO_NODE   node;
    CO_HBCONS data = { 0 };
    CO_MODE   state;

    data.NodeId = 10;
    data.Time   = 50;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1016, 0, CO_OBJ_D___R_), CO_THB_CONS, (CO_DATA)(1));
    TS_ODAdd(CO_KEY(0x1016, 1, CO_OBJ_____R_), CO_THB_CONS, (CO_DATA)(&data));
    TS_CreateNode(&node,0);
                                                      /*------------------------------------------*/
    TS_HB_SEND(10, 5);

    state = CONmtLastHbState(&node.Nmt, 10);
    TS_ASSERT(CO_OPERATIONAL == state);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC4
*
*          This testcase will focus on the situation:
*            HBP : O------- (O: heartbeat)
*            HBC : |.....!. (!: heartbeat consumer check/event)
*          Check that the consumer detects a heartbeat if heartbeat is missing.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_HBCons_SingleEvent)
{
    CO_NODE   node;
    CO_HBCONS data = { 0 };
    int16_t   events;

    data.NodeId = 10;
    data.Time   = 50;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1016, 0, CO_OBJ_D___R_), CO_THB_CONS, (CO_DATA)(1));
    TS_ODAdd(CO_KEY(0x1016, 1, CO_OBJ_____R_), CO_THB_CONS, (CO_DATA)(&data));
    TS_CreateNode(&node,0);
                                                      /*------------------------------------------*/
    TS_HB_SEND(10, 5);
    TS_Wait(&node, 60);

    events = CONmtGetHbEvents(&node.Nmt, 10);
    TS_ASSERT(1 == events);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC5
*
*          This testcase will focus on the situation:
*            HBP : O--------------------------------------------------- (O: heartbeat)
*            HBC : |....!....!....!....!....!....!....!....!....!....!. (!: heartbeat consumer check/event)
*          Check that the consumer detects multiple heartbeat events.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_HBCons_MultiEvent)
{
    CO_NODE   node;
    CO_HBCONS data = { 0 };
    int16_t   events;

    data.NodeId = 10;
    data.Time   = 50;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1016, 0, CO_OBJ_D___R_), CO_THB_CONS, (CO_DATA)(1));
    TS_ODAdd(CO_KEY(0x1016, 1, CO_OBJ_____R_), CO_THB_CONS, (CO_DATA)(&data));
    TS_CreateNode(&node,0);
                                                      /*------------------------------------------*/
    TS_HB_SEND(10, 5);
    TS_Wait(&node, 500);

    events = CONmtGetHbEvents(&node.Nmt, 10);
    TS_ASSERT(10 == events);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC6
*
*          This testcase will focus on the situation:
*            HBP : O---O-- (O: heartbeat)
*            HBC : |....!. (!: heartbeat consumer check/event)
*          Check that the consumer accepts heartbeats with correct timing.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_HBCons_NoEvent)
{
    CO_NODE   node;
    CO_HBCONS data = { 0 };
    int16_t   events;

    data.NodeId = 10;
    data.Time   = 50;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1016, 0, CO_OBJ_D___R_), CO_THB_CONS, (CO_DATA)(1));
    TS_ODAdd(CO_KEY(0x1016, 1, CO_OBJ_____R_), CO_THB_CONS, (CO_DATA)(&data));
    TS_CreateNode(&node,0);
                                                      /*------------------------------------------*/
    TS_HB_SEND(10, 5);
    TS_Wait(&node, 40);
    TS_HB_SEND(10, 5);

    events = CONmtGetHbEvents(&node.Nmt, 10);
    TS_ASSERT(0 == events);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC7
*
*          This testcase will focus on the situation:
*            HBP : O-O-O----O-- (O: heartbeat)
*            HBC : |..!.!....!. (!: heartbeat consumer check/event)
*          Check that the consumer monitor timing is restarted with a consumed heartbeat.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_HBCons_Jitter)
{
    CO_NODE   node;
    CO_HBCONS data = { 0 };
    int16_t   events;

    data.NodeId = 10;
    data.Time   = 60;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1016, 0, CO_OBJ_D___R_), CO_THB_CONS, (CO_DATA)(1));
    TS_ODAdd(CO_KEY(0x1016, 1, CO_OBJ_____R_), CO_THB_CONS, (CO_DATA)(&data));
    TS_CreateNode(&node,0);
                                                      /*------------------------------------------*/
    TS_HB_SEND(10, 5);
    TS_Wait(&node, 20);
    TS_HB_SEND(10, 5);
    TS_Wait(&node, 20);
    TS_HB_SEND(10, 5);
    TS_Wait(&node, 50);
    TS_HB_SEND(10, 5);

    events = CONmtGetHbEvents(&node.Nmt, 10);
    TS_ASSERT(0 == events);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC8
*
*          This testcase will focus on the situation:
*            HBP : O---O---O-- (O: heartbeat)
*            HBC : |....!...!. (!: heartbeat consumer check/event)
*          Check that the consumer monitor timing is accepted from Node-ID 0 (Master Node)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_HBCons_MasterId)
{
    CO_NODE   node;
    CO_HBCONS data = { 0 };
    int16_t   events;

    data.NodeId = 0;
    data.Time   = 50;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1016, 0, CO_OBJ_D___R_), CO_THB_CONS, (CO_DATA)(1));
    TS_ODAdd(CO_KEY(0x1016, 1, CO_OBJ_____R_), CO_THB_CONS, (CO_DATA)(&data));
    TS_CreateNode(&node,0);
                                                      /*------------------------------------------*/
    TS_HB_SEND(0, 5);
    TS_Wait(&node, 40);
    TS_HB_SEND(0, 5);
    TS_Wait(&node, 40);
    TS_HB_SEND(0, 5);
    TS_Wait(&node, 40);
    TS_HB_SEND(0, 5);

    events = CONmtGetHbEvents(&node.Nmt, 0);
    TS_ASSERT(0 == events);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC9
*
*          This testcase will check that no consumer monitor is possible.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_HBCons_NoConsumer)
{
    CO_NODE node;
    CO_MODE state;
    int16_t events;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateNode(&node,0);
                                                      /*------------------------------------------*/
    events = CONmtGetHbEvents(&node.Nmt, 10);
    TS_ASSERT(-1 == events);

    state = CONmtLastHbState(&node.Nmt, 10);
    TS_ASSERT(CO_INVALID == state);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC10
*
*          This testcase will focus on the situation:
*            HBP : O---O-- (O: heartbeat)
*            HBC : |....!. (!: heartbeat consumer check/event)
*          Check that asking for un-monitored consumer is detected while having active consumers.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_HBCons_OtherConsumer)
{
    CO_NODE   node;
    CO_HBCONS data = { 0 };
    int16_t   events;

    data.NodeId = 10;
    data.Time   = 50;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1016, 0, CO_OBJ_D___R_), CO_THB_CONS, (CO_DATA)(1));
    TS_ODAdd(CO_KEY(0x1016, 1, CO_OBJ_____R_), CO_THB_CONS, (CO_DATA)(&data));
    TS_CreateNode(&node,0);
                                                      /*------------------------------------------*/
    TS_HB_SEND(10, 5);
    TS_Wait(&node, 40);
    TS_HB_SEND(10, 5);

    events = CONmtGetHbEvents(&node.Nmt, 42);
    TS_ASSERT(-1 == events);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC11
*
*          This testcase will focus on the situation:
*            HBP1 : O---O---O---- (O: heartbeat)
*            HBC1 : |....!...!... (!: heartbeat consumer check/event)
*            HBP2 : O----O----O-- (O: heartbeat)
*            HBC2 : |.....!....!. (!: heartbeat consumer check/event)
*          Check that multiple consumer monitors are managed correctly.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_HBCons_MultiConsumer)
{
    CO_NODE   node;
    CO_HBCONS data1 = { 0 };
    CO_HBCONS data2 = { 0 };
    int16_t   events;

    data1.NodeId = 10;
    data1.Time   = 50;

    data2.NodeId = 20;
    data2.Time   = 70;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1016, 0, CO_OBJ_D___R_), CO_THB_CONS, (CO_DATA)(2));
    TS_ODAdd(CO_KEY(0x1016, 1, CO_OBJ_____R_), CO_THB_CONS, (CO_DATA)(&data1));
    TS_ODAdd(CO_KEY(0x1016, 2, CO_OBJ_____R_), CO_THB_CONS, (CO_DATA)(&data2));
    TS_CreateNode(&node,0);
                                                      /*------------------------------------------*/
    TS_HB_SEND(10, 5);
    TS_HB_SEND(20, 5);
    TS_Wait(&node, 40);
    TS_HB_SEND(10, 5);
    TS_Wait(&node, 10);
    TS_HB_SEND(20, 5);
    TS_Wait(&node, 30);
    TS_HB_SEND(10, 5);
    TS_Wait(&node, 20);
    TS_HB_SEND(20, 5);

    events = CONmtGetHbEvents(&node.Nmt, 10);
    TS_ASSERT(0 == events);

    events = CONmtGetHbEvents(&node.Nmt, 20);
    TS_ASSERT(0 == events);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC12
*
*          This testcase will check that the write access sets the correct values.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_HBCons_WrEntry)
{
    CO_NODE   node;
    CO_HBCONS data  = { 0 };
    uint32_t  value = 0x000A0032;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1016, 0, CO_OBJ_D___R_), CO_THB_CONS, (CO_DATA)(1));
    TS_ODAdd(CO_KEY(0x1016, 1, CO_OBJ_____RW), CO_THB_CONS, (CO_DATA)(&data));
    TS_CreateNode(&node,0);
                                                      /*------------------------------------------*/
    CODictWrLong(&node.Dict, CO_DEV(0x1016, 1), value);
    TS_ASSERT(10 == data.NodeId);
    TS_ASSERT(50 == data.Time);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC13
*
*          This testcase will focus on the situation:
*            HBP : O------- (O: heartbeat)
*            HBC : |.....!. (!: heartbeat consumer check/event)
*          Check that the dynamic consumer detects a heartbeat if heartbeat is missing.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_HBCons_DynEvent)
{
    CO_NODE   node;
    CO_HBCONS data  = { 0 };
    uint32_t  value = 0x000A0032;
    int16_t   events;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1016, 0, CO_OBJ_D___R_), CO_THB_CONS, (CO_DATA)(1));
    TS_ODAdd(CO_KEY(0x1016, 1, CO_OBJ_____RW), CO_THB_CONS, (CO_DATA)(&data));
    TS_CreateNode(&node,0);
                                                      /*------------------------------------------*/
    CODictWrLong(&node.Dict, CO_DEV(0x1016, 1), value);

    TS_HB_SEND(10, 5);
    TS_Wait(&node, 60);

    events = CONmtGetHbEvents(&node.Nmt, 10);
    TS_ASSERT(1 == events);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC14
*
*          Check that the SDO abort is generated correctly, if a heartbeat consumer
*          is already active.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_HBCons_DynSdoError)
{
    CO_NODE   node;
    CO_HBCONS data  = { 0 };
    int16_t   events;

    data.NodeId = 10;
    data.Time   = 50;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1016, 0, CO_OBJ_D___R_), CO_THB_CONS, (CO_DATA)(1));
    TS_ODAdd(CO_KEY(0x1016, 1, CO_OBJ_____RW), CO_THB_CONS, (CO_DATA)(&data));
    TS_CreateNode(&node,0);
                                                      /*------------------------------------------*/
    TS_HB_SEND(10, 5);
    TS_Wait(&node, 30);

    TS_SDO_SEND (0x23, 0x1016, 1, 0x000A0100);

    TS_Wait(&node, 30);
    TS_HB_SEND(10, 5);

    CHK_SDO0_ERR(0x1016, 1, 0x06040043);

    events = CONmtGetHbEvents(&node.Nmt, 10);
    TS_ASSERT(1 == events);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC15
*
*          Check that the consumer can be disabled via SDO transfer correctly, if a consumer
*          is enabled.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_HBCons_DynSdoDisable)
{
    CO_NODE   node;
    CO_HBCONS data  = { 0 };
    int16_t   events;

    data.NodeId = 10;
    data.Time   = 50;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x1016, 0, CO_OBJ_D___R_), CO_THB_CONS, (CO_DATA)(1));
    TS_ODAdd(CO_KEY(0x1016, 1, CO_OBJ_____RW), CO_THB_CONS, (CO_DATA)(&data));
    TS_CreateNode(&node,0);
                                                      /*------------------------------------------*/
    TS_HB_SEND(10, 5);
    TS_Wait(&node, 30);

    TS_SDO_SEND (0x23, 0x1016, 1, 0x000A0000);

    CHK_SDO0_OK(0x1016, 1);

    TS_Wait(&node, 30);

    events = CONmtGetHbEvents(&node.Nmt, 10);
    TS_ASSERT(-1 == events);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_NMT_HBC()
{
    TS_Begin(__FILE__);
    
//    CanDiagnosticOn(0);

    TS_RUNNER(TS_HBCons_RdEntry);
    TS_RUNNER(TS_HBCons_WaitForHb);
    TS_RUNNER(TS_HBCons_CheckState);
    TS_RUNNER(TS_HBCons_SingleEvent);
    TS_RUNNER(TS_HBCons_MultiEvent);
    TS_RUNNER(TS_HBCons_NoEvent);
    TS_RUNNER(TS_HBCons_Jitter);
    TS_RUNNER(TS_HBCons_MasterId);
    TS_RUNNER(TS_HBCons_NoConsumer);
    TS_RUNNER(TS_HBCons_OtherConsumer);
    TS_RUNNER(TS_HBCons_MultiConsumer);
    TS_RUNNER(TS_HBCons_WrEntry);
    TS_RUNNER(TS_HBCons_DynEvent);
    TS_RUNNER(TS_HBCons_DynSdoError);
    TS_RUNNER(TS_HBCons_DynSdoDisable);

//    CanDiagnosticOff(0);

    TS_End();
}
