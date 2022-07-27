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
*          This testcase will check the basic path
*          - RPDO #0 COB-ID from valid to invalid
*          - alternate path invalid to valid
*          - alternate path not operational
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_RPdo_IdOnOff_PreOp)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, 0, &pdo_len);
    TS_CreateNode(&node,0);

    /* valid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xC0000201);
    TS_ASSERT(CO_ERR_NONE == result);

    /* invalid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0x40000201);
    TS_ASSERT(CO_ERR_NONE == result);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

TS_DEF_MAIN(TS_RPdo_IdOnOff_Op)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, 0, &pdo_len);
    TS_CreateNode(&node,0);

    TS_NMT_SEND(0x01, 1);                            /* set node-id 0x01 to operational          */

    /* valid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xC0000201);
    TS_ASSERT(CO_ERR_NONE == result);

    /* invalid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0x40000201);
    TS_ASSERT(CO_ERR_NONE == result);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC2
*
*          This testcase will check the alternate path
*          - TPDO #0 COB-ID from valid to invalid
*          - and alternate path invalid to valid
*          - alternate path not operational
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_IdOnOff_PreOp)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint16_t pdo_inhibit = 0;
    uint16_t pdo_evtimer = 0;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &pdo_id, &pdo_type, &pdo_inhibit, &pdo_evtimer);
    TS_CreateTPdoMap(0, 0, &pdo_len);
    TS_CreateNode(&node,0);

    /* valid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0xC0000181);
    TS_ASSERT(CO_ERR_NONE == result);

    /* invalid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0x40000181);
    TS_ASSERT(CO_ERR_NONE == result);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

TS_DEF_MAIN(TS_TPdo_IdOnOff_Op)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint16_t pdo_inhibit = 0;
    uint16_t pdo_evtimer = 0;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &pdo_id, &pdo_type, &pdo_inhibit, &pdo_evtimer);
    TS_CreateTPdoMap(0, 0, &pdo_len);
    TS_CreateNode(&node,0);

    TS_NMT_SEND(0x01, 1);                            /* set node-id 0x01 to operational          */

    /* valid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0xC0000181);
    TS_ASSERT(CO_ERR_NONE == result);

    /* invalid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0x40000181);
    TS_ASSERT(CO_ERR_NONE == result);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC3
*
*          This testcase will check the exception path
*          - extended COB-ID for RPDO/TPDO once with change COB-ID sequence and once directly
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_RPdo_ExtId)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, 0, &pdo_len);
    TS_CreateNode(&node,0);

    /* extended COB-ID RPDO */
    /* valid to invalid */
    CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xC0000201);
    
    /* set extended flag */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xE0000201);
    TS_ASSERT(CO_ERR_OBJ_RANGE == result);

    /* invalid to valid */
    CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0x40000201);

    /* direct access with extended cob-id */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xE0000201);
    TS_ASSERT(CO_ERR_OBJ_RANGE == result);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

TS_DEF_MAIN(TS_TPdo_ExtId)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint16_t pdo_inhibit = 0;
    uint16_t pdo_evtimer = 0;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &pdo_id, &pdo_type, &pdo_inhibit, &pdo_evtimer);
    TS_CreateTPdoMap(0, 0, &pdo_len);
    TS_CreateNode(&node,0);

    /* extended COB-ID TPDO */
    /* valid to invalid */
    CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0xC0000181);

    /* set extended flag */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0xE0000181);
    TS_ASSERT(CO_ERR_OBJ_RANGE == result);

    /* invalid to valid */
    CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0x40000181);

    /* direct access with extended cob-id */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0xE0000181);
    TS_ASSERT(CO_ERR_OBJ_RANGE == result);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC4
*
*          This testcase will check the exception path
*          - COB-ID for RPDO/TPDO from valid to valid
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_RPdo_ChangeActiveId)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, 0, &pdo_len);
    TS_CreateNode(&node,0);

    /* COB-ID RPDO */
    /* valid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0x40000211);
    TS_ASSERT(CO_ERR_OBJ_RANGE == result);

    CHK_ERR(&node, CO_ERR_NONE);
}

TS_DEF_MAIN(TS_TPdo_ChangeActiveId)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint16_t pdo_inhibit = 0;
    uint16_t pdo_evtimer = 0;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &pdo_id, &pdo_type, &pdo_inhibit, &pdo_evtimer);
    TS_CreateTPdoMap(0, 0, &pdo_len);
    TS_CreateNode(&node,0);

    /* COB-ID TPDO */
    /* valid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0x40000281);
    TS_ASSERT(CO_ERR_OBJ_RANGE == result);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC5
*
*          This testcase will check the exception path
*          - RTR
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_RemoteFrame)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint16_t pdo_inhibit = 0;
    uint16_t pdo_evtimer = 0;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &pdo_id, &pdo_type, &pdo_inhibit, &pdo_evtimer);
    TS_CreateTPdoMap(0, 0, &pdo_len);
    TS_CreateNode(&node,0);

    /* RTR COB-ID TPDO */
    /* valid to invalid */
    CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0xC0000181);

    /* clear RTR flag */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0x80000181);
    TS_ASSERT(CO_ERR_OBJ_RANGE == result);

    /* invalid to valid */
    CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0x40000181);

    /* direct access with RTR cob-id */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0x00000181);
    TS_ASSERT(CO_ERR_OBJ_RANGE == result);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC6
*
*          This testcase will check the alternate path
*          - TPDO/RPDO COB-ID from valid to invalid
*          - and alternate path invalid to invalid
*          - and alternate path invalid to valid
*          - alternate path not operational
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_RPdo_IdChange_PreOp)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, 0, &pdo_len);
    TS_CreateNode(&node,0);

    /* COB-ID RPDO */
    /* valid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xC0000201);
    TS_ASSERT(CO_ERR_NONE == result);

    /* invalid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xC0000211);
    TS_ASSERT(CO_ERR_NONE == result);

    /* invalid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xC0000201);
    TS_ASSERT(CO_ERR_NONE == result);

    /* invalid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0x40000201);
    TS_ASSERT(CO_ERR_NONE == result);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

TS_DEF_MAIN(TS_TPdo_IdChange_PreOp)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint16_t pdo_inhibit = 0;
    uint16_t pdo_evtimer = 0;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &pdo_id, &pdo_type, &pdo_inhibit, &pdo_evtimer);
    TS_CreateTPdoMap(0, 0, &pdo_len);
    TS_CreateNode(&node,0);

    /* COB-ID TPDO */
    /* valid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0xC0000181);
    TS_ASSERT(CO_ERR_NONE == result);

    /* invalid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0xC0000191);
    TS_ASSERT(CO_ERR_NONE == result);

    /* invalid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0xC0000181);
    TS_ASSERT(CO_ERR_NONE == result);

    /* invalid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0x40000181);
    TS_ASSERT(CO_ERR_NONE == result);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

TS_DEF_MAIN(TS_RPdo_IdChange_Op)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, 0, &pdo_len);
    TS_CreateNode(&node,0);

    TS_NMT_SEND(0x01, 1);                            /* set node-id 0x01 to operational          */

    /* COB-ID RPDO */
    /* valid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xC0000201);
    TS_ASSERT(CO_ERR_NONE == result);

    /* invalid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xC0000211);
    TS_ASSERT(CO_ERR_NONE == result);

    /* invalid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xC0000201);
    TS_ASSERT(CO_ERR_NONE == result);

    /* invalid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0x40000201);
    TS_ASSERT(CO_ERR_NONE == result);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

TS_DEF_MAIN(TS_TPdo_IdChange_Op)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint16_t pdo_inhibit = 0;
    uint16_t pdo_evtimer = 0;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &pdo_id, &pdo_type, &pdo_inhibit, &pdo_evtimer);
    TS_CreateTPdoMap(0, 0, &pdo_len);
    TS_CreateNode(&node,0);

    TS_NMT_SEND(0x01, 1);                            /* set node-id 0x01 to operational          */

    /* COB-ID TPDO */
    /* valid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0xC0000181);
    TS_ASSERT(CO_ERR_NONE == result);

    /* invalid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0xC0000191);
    TS_ASSERT(CO_ERR_NONE == result);

    /* invalid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0xC0000181);
    TS_ASSERT(CO_ERR_NONE == result);

    /* invalid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0x40000181);
    TS_ASSERT(CO_ERR_NONE == result);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC7
*
*          This testcase will check the exception path
*          - no PDO COB-ID sub-index
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_RPdo_BadIdSubIdxCfg)
{
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, 0, &pdo_len);
    TS_ODAdd(CO_KEY(0x1400, 3, CO_OBJ__N__RW), CO_TPDO_ID, (CO_DATA)(&pdo_id));
    TS_CreateNode(&node,0);

    CHK_ERR(&node, CO_ERR_OBJ_INIT);                  /* check for expected error                 */
}

TS_DEF_MAIN(TS_TPdo_BadIdSubIdxCfg)
{
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint16_t pdo_inhibit = 0;
    uint16_t pdo_evtimer = 0;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &pdo_id, &pdo_type, &pdo_inhibit, &pdo_evtimer);
    TS_ODAdd(CO_KEY(0x1800, 6, CO_OBJ__N__RW), CO_TPDO_ID, (CO_DATA)(&pdo_id));
    TS_CreateTPdoMap(0, 0, &pdo_len);
    TS_CreateNode(&node,0);

    CHK_ERR(&node, CO_ERR_OBJ_INIT);                  /* check for expected error                 */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC8
*
*          This testcase will check the exception path
*          - no PDO communication parameter index
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_RPdo_BadIdIdxCfg)
{
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, 0, &pdo_len);
    TS_ODAdd(CO_KEY(0x2502, 1, CO_OBJ__N__RW), CO_TPDO_ID, (CO_DATA)(&pdo_id));
    TS_CreateNode(&node,0);

    CHK_ERR(&node, CO_ERR_OBJ_INIT);                  /* check for expected error                 */
}

TS_DEF_MAIN(TS_TPdo_BadIdIdxCfg)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint16_t pdo_inhibit = 0;
    uint16_t pdo_evtimer = 0;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &pdo_id, &pdo_type, &pdo_inhibit, &pdo_evtimer);
    TS_ODAdd(CO_KEY(0x2501, 1, CO_OBJ__N__RW), CO_TPDO_ID, (CO_DATA)(&pdo_id));
    TS_CreateTPdoMap(0, 0, &pdo_len);
    TS_CreateNode(&node,0);

    CHK_ERR(&node, CO_ERR_OBJ_INIT);                  /* check for expected error                 */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC9
*
*          This testcase will check the basic path
*          - change PDO mapping number
*          - write PDO mapping
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_RPdo_MapNumChange)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 3;
    uint32_t pdo_map[3]  = { 0, 0, 0 };
    uint32_t data[2]     = { 0, 0 };

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, &pdo_map[0], &pdo_len);
    TS_ODAdd(CO_KEY(0x2500, 31, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data[0]));
    TS_ODAdd(CO_KEY(0x2500, 32, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data[1]));
    TS_CreateNode(&node,0);

    /* PDO valid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xC0000201);
    TS_ASSERT(CO_ERR_NONE == result);

    /* set mapping to 0 */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1600,0), 0x0);
    TS_ASSERT(CO_ERR_NONE == result);

    /* write mapping */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1600,1), CO_LINK(0x2500,31,32));
    TS_ASSERT(CO_ERR_NONE == result);

    result = CODictWrLong(&node.Dict, CO_DEV(0x1600,2), CO_LINK(0x2500,32,32));
    TS_ASSERT(CO_ERR_NONE == result);

    /* set mapping to 2 */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1600,0), 0x2);
    TS_ASSERT(CO_ERR_NONE == result);

    /* PDO invalid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0x40000201);
    TS_ASSERT(CO_ERR_NONE == result);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC10
*
*          This testcase will check the exception path
*          - change PDO mapping number with valid COB-ID
*          - write PDO mapping with valid COB-ID
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_RPdo_ChangeActiveMap)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 3;
    uint32_t pdo_map[3]  = { 0, 0, 0 };
    uint32_t data[2]     = { 0, 0 };

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, &pdo_map[0], &pdo_len);
    TS_ODAdd(CO_KEY(0x2500, 31, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data[0]));
    TS_ODAdd(CO_KEY(0x2500, 32, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data[1]));
    TS_CreateNode(&node,0);

    /* set mapping to 0 */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1600,0), 0x0);
    TS_ASSERT(CO_ERR_OBJ_ACC == result);

    /* write mapping */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1600,1), CO_LINK(0x2500,31,32));
    TS_ASSERT(CO_ERR_OBJ_ACC == result);
    result = CODictWrLong(&node.Dict, CO_DEV(0x1600,2), CO_LINK(0x2500,32,32));
    TS_ASSERT(CO_ERR_OBJ_ACC == result);

    /* set mapping to 2 */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1600,0), 0x2);
    TS_ASSERT(CO_ERR_OBJ_ACC == result);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC11
*
*          This testcase will check the exception path
*          - change PDO mapping number to > supported mappings
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_RPdo_MapNumTooHigh)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id     = 0x40000180;
    uint8_t  pdo_type   = 1;
    uint8_t  pdo_len    = 3;
    uint32_t pdo_map[3] = { 0, 0, 0 };
    uint32_t data[2]    = { 0, 0 };

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, &pdo_map[0], &pdo_len);
    TS_ODAdd(CO_KEY(0x2500, 31, CO_OBJ____PRW), CO_TUNSIGNED32, (CO_DATA)(&data[0]));
    TS_ODAdd(CO_KEY(0x2500, 32, CO_OBJ____PRW), CO_TUNSIGNED32, (CO_DATA)(&data[1]));
    TS_CreateNode(&node,0);

    /* PDO valid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xC0000201);
    TS_ASSERT(CO_ERR_NONE == result);
    /* set mapping to 0 */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1600,0), 0x0);
    TS_ASSERT(CO_ERR_NONE == result);

    /* write mapping */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1600,1), CO_LINK(0x2500,31,32));
    TS_ASSERT(CO_ERR_NONE == result);

    result = CODictWrLong(&node.Dict, CO_DEV(0x1600,2), CO_LINK(0x2500,32,32));
    TS_ASSERT(CO_ERR_NONE == result);

    /* set mapping to 9 */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1600,0), 0x9);
    TS_ASSERT(CO_ERR_OBJ_MAP_LEN == result);

    /* set mapping to 2 */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1600,0), 0x2);
    TS_ASSERT(CO_ERR_NONE == result);

    /* PDO invalid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0x40000201);
    TS_ASSERT(CO_ERR_NONE == result);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC12
*
*          This testcase will check the exception path
*          - change PDO mapping number to more than 8 bytes
*          - write PDO mapping to more than 8 bytes
*/
/*------------------------------------------------------------------------------------------------*/
#if DISABLED
TS_DEF_MAIN(TS_RPdo_MapLenTooHigh)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 0;
    uint32_t pdo_map[3]  = { 0, 0, 0 };

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, &pdo_map[0], &pdo_len);
    TS_CreateNode(&node,0);

    /* PDO valid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xC0000201);
    TS_ASSERT(CO_ERR_NONE == result);

    /* set mapping to 0 */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1600,0), 0x0);
    TS_ASSERT(CO_ERR_NONE == result);

    /* write mapping */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1600,1), CO_KEY(0x2500,31, CO_UNSIGNED32));
    TS_ASSERT(CO_ERR_NONE == result);

    result = CODictWrLong(&node.Dict, CO_DEV(0x1600,2), CO_KEY(0x2500,32, CO_UNSIGNED32));
    TS_ASSERT(CO_ERR_NONE == result);

    result = CODictWrLong(&node.Dict, CO_DEV(0x1600,3), CO_KEY(0x2500,13, CO_UNSIGNED8));
    TS_ASSERT(CO_ERR_NONE == result);

    /* set mapping to 3 (too many bytes) */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1600,0), 0x3);
    TS_ASSERT(CO_ERR_OBJ_MAP_LEN == result);

    /* set mapping to 2 (valid number of bytes) */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1600,0), 0x2);
    TS_ASSERT(CO_ERR_NONE == result);

    /* PDO invalid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0x40000201);
    TS_ASSERT(CO_ERR_NONE == result);

    CHK_ERR(&node, CO_ERR_OBJ_WRITE);                  /* check for expected error                 */
}
#endif

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC13
*
*          This testcase will check the exception path
*          - no PDO mapping number sub-index
*/
/*------------------------------------------------------------------------------------------------*/
#if DISABLED
TS_DEF_MAIN(TS_RPdo_BadMapNumSubIdxCfg)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 0;
    uint32_t pdo_map[3]  = { 0, 0, 0 };

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, &pdo_map[0], &pdo_len);
    TS_CreateNode(&node,0);

    /* no PDO mapping number sub-index */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1600, 9), 0x0);
    TS_ASSERT(CO_ERR_TPDO_MAP_OBJ == result);

    CHK_ERR(&node, CO_ERR_OBJ_WRITE);                 /* check for expected error                 */
}
#endif

#if DISABLED
TS_DEF_MAIN(TS_TPdo_BadMapNumSubIdxCfg)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint16_t pdo_inhibit = 0;
    uint16_t pdo_evtimer = 0;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &pdo_id, &pdo_type, &pdo_inhibit, &pdo_evtimer);
    TS_CreateTPdoMap(0, 0, &pdo_len);
    TS_ODAdd(CO_KEY(0x1A00, 9, CO_OBJ_____RW), CO_TPDO_NUM, (CO_DATA)(&pdo_len));
    TS_CreateNode(&node,0);

    /* no PDO mapping number sub-index */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1A00, 9), 0x0);
    TS_ASSERT(CO_ERR_TPDO_MAP_OBJ == result);

    CHK_ERR(&node, CO_ERR_OBJ_WRITE);                 /* check for expected error                 */
}
#endif

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC14
*
*          This testcase will check the exception path
*          - no PDO mapping index
*/
/*------------------------------------------------------------------------------------------------*/
#if DISABLED
TS_DEF_MAIN(TS_RPdo_BadMapNumIdxCfg)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 0;
    uint32_t pdo_map[3]  = { 0, 0, 0 };

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, &pdo_map[0], &pdo_len);
    TS_ODAdd(CO_KEY(0x2503, 0, CO_OBJ_____RW), CO_TPDO_NUM, (CO_DATA)(&pdo_len));
    TS_CreateNode(&node,0);

    /* no PDO mapping index */
    result = CODictWrByte(&node.Dict, CO_DEV(0x2503, 0), 0x0);
    TS_ASSERT(CO_ERR_TPDO_MAP_OBJ == result);

    CHK_ERR(&node, CO_ERR_OBJ_WRITE);                 /* check for expected error                 */
}
#endif

#if DISABLED
TS_DEF_MAIN(TS_TPdo_BadMapNumIdxCfg)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint16_t pdo_inhibit = 0;
    uint16_t pdo_evtimer = 0;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &pdo_id, &pdo_type, &pdo_inhibit, &pdo_evtimer);
    TS_CreateTPdoMap(0, 0, &pdo_len);
    TS_ODAdd(CO_KEY(0x2504, 0, CO_OBJ_____RW), CO_TPDO_NUM, (CO_DATA)(&pdo_len));
    TS_CreateNode(&node,0);

    /* no PDO mapping index */
    result = CODictWrByte(&node.Dict, CO_DEV(0x2504, 0), 0x0);
    TS_ASSERT(CO_ERR_TPDO_MAP_OBJ == result);

    CHK_ERR(&node, CO_ERR_OBJ_WRITE);                 /* check for expected error                 */
}
#endif

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC15
*
*          This testcase will check the alternate path
*          - change TPDO mapping number
*          - write TPDO mapping
*/
/*------------------------------------------------------------------------------------------------*/
#if DISABLED
TS_DEF_MAIN(TS_TPdo_MapNumChange)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint16_t pdo_inhibit = 0;
    uint16_t pdo_evtimer = 0;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &pdo_id, &pdo_type, &pdo_inhibit, &pdo_evtimer);
    TS_CreateTPdoMap(0, 0, &pdo_len);
    TS_CreateNode(&node,0);

    /* PDO valid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0xC0000181);
    TS_ASSERT(CO_ERR_NONE == result);

    /* set mapping to 0 */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1A00,0), 0x0);
    TS_ASSERT(CO_ERR_NONE == result);

    /* write mapping */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1A00,1), CO_KEY(0x2500,31, CO_UNSIGNED32));
    TS_ASSERT(CO_ERR_NONE == result);

    result = CODictWrLong(&node.Dict, CO_DEV(0x1A00,2), CO_KEY(0x2500,32, CO_UNSIGNED32));
    TS_ASSERT(CO_ERR_NONE == result);

    /* set mapping to 2 */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1A00,0), 0x2);
    TS_ASSERT(CO_ERR_NONE == result);

    /* PDO invalid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0x40000181);
    TS_ASSERT(CO_ERR_NONE == result);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}
#endif

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC18
*
*          This testcase will check the exception path
*          - mapping activated
*/
/*------------------------------------------------------------------------------------------------*/
#if DISABLED
TS_DEF_MAIN(TS_TPdo_ChangeActiveMap)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint16_t pdo_inhibit = 0;
    uint16_t pdo_evtimer = 0;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &pdo_id, &pdo_type, &pdo_inhibit, &pdo_evtimer);
    TS_CreateTPdoMap(0, 0, &pdo_len);
    TS_CreateNode(&node,0);

    /* set mapping to 0 */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1A00,0), 0x0);
    TS_ASSERT(CO_ERR_OBJ_ACC == result);

    /* write mapping */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1A00,1), CO_KEY(0x2500,31, CO_UNSIGNED32));
    TS_ASSERT(CO_ERR_OBJ_ACC == result);

    result = CODictWrLong(&node.Dict, CO_DEV(0x1A00,2), CO_KEY(0x2500,32, CO_UNSIGNED32));
    TS_ASSERT(CO_ERR_OBJ_ACC == result);

    /* set mapping to 2 */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1A00,0), 0x2);
    TS_ASSERT(CO_ERR_OBJ_ACC == result);

    CHK_ERR(&node, CO_ERR_OBJ_WRITE);                 /* check for expected error                 */
}
#endif

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC19
*
*          This testcase will check the exception path
*          - mapping entry not writeable
*/
/*------------------------------------------------------------------------------------------------*/
#if DISABLED
TS_DEF_MAIN(TS_RPdo_MapReadOnlyObj)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 0;
    uint32_t pdo_map[3]  = { 0, 0, 0 };

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, &pdo_map[0], &pdo_len);
    TS_CreateNode(&node,0);

    /* PDO valid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xC0000201);
    TS_ASSERT(CO_ERR_NONE == result);

    /* set mapping to 0 */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1600,0), 0x0);
    TS_ASSERT(CO_ERR_NONE == result);

    /* write mapping */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1600,1), CO_KEY(0x2501, 4, CO_UNSIGNED32));
    TS_ASSERT(CO_ERR_OBJ_MAP_TYPE == result);

    result = CODictWrLong(&node.Dict, CO_DEV(0x1600,2), CO_KEY(0x2501, 5, CO_UNSIGNED32));
    TS_ASSERT(CO_ERR_OBJ_MAP_TYPE == result);

    /* write correct mapping */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1600,1), CO_KEY(0x2500,31, CO_UNSIGNED32));
    TS_ASSERT(CO_ERR_NONE == result);

    result = CODictWrLong(&node.Dict, CO_DEV(0x1600,2), CO_KEY(0x2500,32, CO_UNSIGNED32));
    TS_ASSERT(CO_ERR_NONE == result);

    /* set mapping to 2 */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1600,0), 0x2);
    TS_ASSERT(CO_ERR_NONE == result);

    /* PDO invalid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0x40000201);
    TS_ASSERT(CO_ERR_NONE == result);

    CHK_ERR(&node, CO_ERR_OBJ_WRITE);                 /* check for expected error                 */
}
#endif

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC20
*
*          This testcase will check the exception path
*          - mapping entry not readable
*/
/*------------------------------------------------------------------------------------------------*/
#if DISABLED
TS_DEF_MAIN(TS_TPdo_MapWriteOnlyObj)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint16_t pdo_inhibit = 0;
    uint16_t pdo_evtimer = 0;
    uint8_t  pdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &pdo_id, &pdo_type, &pdo_inhibit, &pdo_evtimer);
    TS_CreateTPdoMap(0, 0, &pdo_len);
    TS_CreateNode(&node,0);

    /* PDO valid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0xC0000181);
    TS_ASSERT(CO_ERR_NONE == result);

    /* set mapping to 0 */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1A00,0), 0x0);
    TS_ASSERT(CO_ERR_NONE == result);

    /* write mapping */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1A00,1), CO_KEY(0x2501, 6, CO_UNSIGNED32));
    TS_ASSERT(CO_ERR_OBJ_MAP_TYPE == result);

    result = CODictWrLong(&node.Dict, CO_DEV(0x1A00,2), CO_KEY(0x2501, 7, CO_UNSIGNED32));
    TS_ASSERT(CO_ERR_OBJ_MAP_TYPE == result);

    /* write correct mapping */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1A00,1), CO_KEY(0x2500,31, CO_UNSIGNED32));
    TS_ASSERT(CO_ERR_NONE == result);

    result = CODictWrLong(&node.Dict, CO_DEV(0x1A00,2), CO_KEY(0x2500,32, CO_UNSIGNED32));
    TS_ASSERT(CO_ERR_NONE == result);

    /* set mapping to 2 */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1A00,0), 0x2);
    TS_ASSERT(CO_ERR_NONE == result);

    /* PDO invalid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1800,1), 0x40000181);
    TS_ASSERT(CO_ERR_NONE == result);

    CHK_ERR(&node, CO_ERR_OBJ_WRITE);                 /* check for expected error                 */
}
#endif

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC21
*
*          This testcase will check the exception path
*          - mapping entry not mapable
*/
/*------------------------------------------------------------------------------------------------*/
#if DISABLED
TS_DEF_MAIN(TS_RPdo_MapNotPdoMappableObj)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 0;
    uint32_t pdo_map[3]  = { 0, 0, 0 };

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, &pdo_map[0], &pdo_len);
    TS_CreateNode(&node,0);

    /* PDO valid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xC0000201);
    TS_ASSERT(CO_ERR_NONE == result);

    /* set mapping to 0 */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1600,0), 0x0);
    TS_ASSERT(CO_ERR_NONE == result);

    /* write mapping not mapable */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1600,1), CO_KEY(0x2510, 3, CO_UNSIGNED32));
    TS_ASSERT(CO_ERR_OBJ_MAP_TYPE == result);

    /* write correct mapping */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1600,1), CO_KEY(0x2500,31, CO_UNSIGNED32));
    TS_ASSERT(CO_ERR_NONE == result);

    result = CODictWrLong(&node.Dict, CO_DEV(0x1600,2), CO_KEY(0x2500,32, CO_UNSIGNED32));
    TS_ASSERT(CO_ERR_NONE == result);

    /* set mapping to 2 */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1600,0), 0x2);
    TS_ASSERT(CO_ERR_NONE == result);

    /* PDO invalid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0x40000201);
    TS_ASSERT(CO_ERR_NONE == result);

    CHK_ERR(&node, CO_ERR_OBJ_WRITE);                 /* check for expected error                 */
}
#endif

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC22
*
*          This testcase will check the exception path
*          - not found
*/
/*------------------------------------------------------------------------------------------------*/
#if DISABLED
TS_DEF_MAIN(TS_RPdo_MapObjNotExist)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 0;
    uint32_t pdo_map[3]  = { 0, 0, 0 };

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, &pdo_map[0], &pdo_len);
    TS_CreateNode(&node,0);

    /* PDO valid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xC0000201);
    TS_ASSERT(CO_ERR_NONE == result);

    /* set mapping to 0 */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1600,0), 0x0);
    TS_ASSERT(CO_ERR_NONE == result);

    /* write mapping not found */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1600,1), CO_KEY(0x2700, 3, CO_UNSIGNED32));
    TS_ASSERT(CO_ERR_OBJ_MAP_TYPE == result);

    /* write correct mapping */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1600,1), CO_KEY(0x2500,31, CO_UNSIGNED32));
    TS_ASSERT(CO_ERR_NONE == result);

    result = CODictWrLong(&node.Dict, CO_DEV(0x1600,2), CO_KEY(0x2500,32, CO_UNSIGNED32));
    TS_ASSERT(CO_ERR_NONE == result);

    /* set mapping to 2 */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1600,0), 0x2);
    TS_ASSERT(CO_ERR_NONE == result);

    /* PDO invalid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0x40000201);
    TS_ASSERT(CO_ERR_NONE == result);

    CHK_ERR(&node, CO_ERR_OBJ_WRITE);                 /* check for expected error                 */
}
#endif

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC23
*
*          This testcase will check the basic path
*          - write pdo type
*/
/*------------------------------------------------------------------------------------------------*/
#if DISABLED
TS_DEF_MAIN(TS_RPdo_ComTypeChange)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 0;
    uint32_t pdo_map[3]  = { 0, 0, 0 };

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, &pdo_map[0], &pdo_len);
    TS_CreateNode(&node,0);

    /* PDO valid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xC0000201);
    TS_ASSERT(CO_ERR_NONE == result);

    /* write type */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1400,2), 0x0);
    TS_ASSERT(CO_ERR_NONE == result);

    /* PDO invalid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0x40000201);
    TS_ASSERT(CO_ERR_NONE == result);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}
#endif

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC24
*
*          This testcase will check the exception path
*          - COB-ID is valid
*/
/*------------------------------------------------------------------------------------------------*/
#if DISABLED
TS_DEF_MAIN(TS_RPdo_ActiveComTypeChange)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 0;
    uint32_t pdo_map[3]  = { 0, 0, 0 };

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, &pdo_map[0], &pdo_len);
    TS_CreateNode(&node,0);

    /* write type */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1400,2), 0x0);
    TS_ASSERT(CO_ERR_OBJ_RANGE == result);

    CHK_ERR(&node, CO_ERR_OBJ_WRITE);                  /* check for expected error                 */
}
#endif

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC25
*
*          This testcase will check the exception path
*          - Object is no communication parameter index
*/
/*------------------------------------------------------------------------------------------------*/
#if DISABLED
TS_DEF_MAIN(TS_RPdo_BadComTypeIdxCfg)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 0;
    uint32_t pdo_map[3]  = { 0, 0, 0 };

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, &pdo_map[0], &pdo_len);
    TS_CreateNode(&node,0);

    /* PDO valid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xC0000201);
    TS_ASSERT(CO_ERR_NONE == result);

    /* write type */
    result = CODictWrByte(&node.Dict, CO_DEV(0x2504, 2), 0x0);
    TS_ASSERT(CO_ERR_PARA_IDX == result);

    /* PDO invalid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0x40000201);
    TS_ASSERT(CO_ERR_NONE == result);

    CHK_ERR(&node, CO_ERR_OBJ_WRITE);                 /* check for expected error                 */
}
#endif

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC26
*
*          This testcase will check the exception path
*          - Object is no communication type sub-index
*/
/*------------------------------------------------------------------------------------------------*/
#if DISABLED
TS_DEF_MAIN(TS_RPdo_BadComTypeSubIdxCfg)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 0;
    uint32_t pdo_map[3]  = { 0, 0, 0 };

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, &pdo_map[0], &pdo_len);
    TS_CreateNode(&node,0);

    /* PDO valid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xC0000201);
    TS_ASSERT(CO_ERR_NONE == result);

    /* write type */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1400, 4), 0x0);
    TS_ASSERT(CO_ERR_PARA_IDX == result);

    /* PDO invalid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0x40000201);
    TS_ASSERT(CO_ERR_NONE == result);

    CHK_ERR(&node, CO_ERR_OBJ_WRITE);                 /* check for expected error                 */
}
#endif

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC27
*
*          This testcase will check the exception path
*          - Errorcode is not CO_ERR_NONE
*/
/*------------------------------------------------------------------------------------------------*/
#if DISABLED
TS_DEF_MAIN(TS_RPdo_ComTypeNullPtr)
{
    int16_t  result;
    CO_NODE  node;
    uint32_t pdo_id      = 0x40000180;
    uint8_t  pdo_type    = 1;
    uint8_t  pdo_len     = 0;
    uint32_t pdo_map[3]  = { 0, 0, 0 };

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &pdo_id, &pdo_type);
    TS_CreateRPdoMap(0, &pdo_map[0], &pdo_len);
    TS_CreateNode(&node,0);

    /* PDO valid to invalid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1401,1), 0xC0000201);
    TS_ASSERT(CO_ERR_NONE == result);

    /* write type */
    result = CODictWrByte(&node.Dict, CO_DEV(0x1401,2), 0x0);
    TS_ASSERT(CO_ERR_OBJ_ACC == result);

    /* PDO invalid to valid */
    result = CODictWrLong(&node.Dict, CO_DEV(0x1401,1), 0x40000201);
    TS_ASSERT(CO_ERR_NONE == result);

    CHK_ERR(&node, CO_ERR_OBJ_WRITE);                 /* check for expected error                 */
}
#endif

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_PDO_DYN()
{
    TS_Begin(__FILE__);
    
//    CanDiagnosticOn(0);

    TS_RUNNER(TS_TPdo_IdOnOff_PreOp);
    TS_RUNNER(TS_TPdo_IdOnOff_Op);
    TS_RUNNER(TS_TPdo_ExtId);
    TS_RUNNER(TS_TPdo_ChangeActiveId);
    TS_RUNNER(TS_TPdo_RemoteFrame);
    TS_RUNNER(TS_TPdo_IdChange_PreOp);
    TS_RUNNER(TS_TPdo_IdChange_Op);

    TS_RUNNER(TS_TPdo_BadIdSubIdxCfg);
    TS_RUNNER(TS_TPdo_BadIdIdxCfg);
    // TS_RUNNER(TS_TPdo_BadMapNumSubIdxCfg);
    // TS_RUNNER(TS_TPdo_BadMapNumIdxCfg);
    // TS_RUNNER(TS_TPdo_MapNumChange);
    // TS_RUNNER(TS_TPdo_ChangeActiveMap);
    // TS_RUNNER(TS_TPdo_MapWriteOnlyObj);

    TS_RUNNER(TS_RPdo_IdOnOff_PreOp);
    TS_RUNNER(TS_RPdo_IdOnOff_Op);
    TS_RUNNER(TS_RPdo_ExtId);
    TS_RUNNER(TS_RPdo_ChangeActiveId);
    TS_RUNNER(TS_RPdo_IdChange_PreOp);
    TS_RUNNER(TS_RPdo_IdChange_Op);
    TS_RUNNER(TS_RPdo_BadIdSubIdxCfg);
    TS_RUNNER(TS_RPdo_BadIdIdxCfg);
    TS_RUNNER(TS_RPdo_MapNumChange);
    TS_RUNNER(TS_RPdo_ChangeActiveMap);

    TS_RUNNER(TS_RPdo_MapNumTooHigh);
    // TS_RUNNER(TS_RPdo_MapLenTooHigh);
    // TS_RUNNER(TS_RPdo_BadMapNumSubIdxCfg);
    // TS_RUNNER(TS_RPdo_BadMapNumIdxCfg);
    // TS_RUNNER(TS_RPdo_MapReadOnlyObj);
    // TS_RUNNER(TS_RPdo_MapNotPdoMappableObj);
    // TS_RUNNER(TS_RPdo_MapObjNotExist);
    // TS_RUNNER(TS_RPdo_ComTypeChange);
    // TS_RUNNER(TS_RPdo_ActiveComTypeChange);
    // TS_RUNNER(TS_RPdo_BadComTypeIdxCfg);
    // TS_RUNNER(TS_RPdo_BadComTypeSubIdxCfg);
    // TS_RUNNER(TS_RPdo_ComTypeNullPtr);

//    CanDiagnosticOff(0);

    TS_End();
}
