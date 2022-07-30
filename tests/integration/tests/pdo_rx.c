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
* PRIVATE VARIABLES
******************************************************************************/

static TS_CALLBACK CORpdoWriteDataCb;

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC1
*
*          This testcase will check the principle reception of:
*          - PDO #0 (8 separate bytes in content)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_RPdo_8x1Byte)
{
    CO_NODE  node;
    uint32_t rpdo_id     = 0x40000200;
    uint32_t rpdo_map[8] = { 0x25000B08, 0x25000C08, 0x25000D08, 0x25000E08,
                             0x25000F08, 0x25001008, 0x25001108, 0x25001208 };
    uint8_t  rpdo_type   = 1;
    uint8_t  rpdo_len    = 8;
    uint8_t  data[8]     = { 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98};

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &rpdo_id,     &rpdo_type);
    TS_CreateRPdoMap(0, &rpdo_map[0], &rpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ_____RW), CO_TUNSIGNED8, (CO_DATA)(&data[0]));
    TS_ODAdd(CO_KEY(0x2500, 0x0C, CO_OBJ_____RW), CO_TUNSIGNED8, (CO_DATA)(&data[1]));
    TS_ODAdd(CO_KEY(0x2500, 0x0D, CO_OBJ_____RW), CO_TUNSIGNED8, (CO_DATA)(&data[2]));
    TS_ODAdd(CO_KEY(0x2500, 0x0E, CO_OBJ_____RW), CO_TUNSIGNED8, (CO_DATA)(&data[3]));
    TS_ODAdd(CO_KEY(0x2500, 0x0F, CO_OBJ_____RW), CO_TUNSIGNED8, (CO_DATA)(&data[4]));
    TS_ODAdd(CO_KEY(0x2500, 0x10, CO_OBJ_____RW), CO_TUNSIGNED8, (CO_DATA)(&data[5]));
    TS_ODAdd(CO_KEY(0x2500, 0x11, CO_OBJ_____RW), CO_TUNSIGNED8, (CO_DATA)(&data[6]));
    TS_ODAdd(CO_KEY(0x2500, 0x12, CO_OBJ_____RW), CO_TUNSIGNED8, (CO_DATA)(&data[7]));
    TS_CreateNodeAutoStart(&node);

    TS_PDO_SEND(0x201, 0x51);

    /* check signals to be unchanged */
    TS_ASSERT(0x91 == data[0]);            
    TS_ASSERT(0x92 == data[1]);
    TS_ASSERT(0x93 == data[2]);
    TS_ASSERT(0x94 == data[3]);
    TS_ASSERT(0x95 == data[4]);
    TS_ASSERT(0x96 == data[5]);
    TS_ASSERT(0x97 == data[6]);
    TS_ASSERT(0x98 == data[7]);

    TS_SYNC_SEND();

    /* check signals to be changed */
    TS_ASSERT(0x51 == data[0]);            
    TS_ASSERT(0x52 == data[1]);
    TS_ASSERT(0x53 == data[2]);
    TS_ASSERT(0x54 == data[3]);
    TS_ASSERT(0x55 == data[4]);
    TS_ASSERT(0x56 == data[5]);
    TS_ASSERT(0x57 == data[6]);
    TS_ASSERT(0x58 == data[7]);

    /* check error free stack execution         */
    CHK_NO_ERR(&node);                                
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC2
*
*          This testcase will check the principle reception of:
*          - PDO #0 (4 separate words in content)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_RPdo_4x2Byte)
{
    CO_NODE  node;
    uint32_t rpdo_id     = 0x40000200;
    uint32_t rpdo_map[4] = { 0x25001510, 0x25001610, 0x25001710, 0x25001810 };
    uint8_t  rpdo_type   = 1;
    uint8_t  rpdo_len    = 4;
    uint16_t data[4]     = { 0x8182, 0x8384, 0x8586, 0x8788};

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &rpdo_id,     &rpdo_type);
    TS_CreateRPdoMap(0, &rpdo_map[0], &rpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x15, CO_OBJ_____RW), CO_TUNSIGNED16, (CO_DATA)(&data[0]));
    TS_ODAdd(CO_KEY(0x2500, 0x16, CO_OBJ_____RW), CO_TUNSIGNED16, (CO_DATA)(&data[1]));
    TS_ODAdd(CO_KEY(0x2500, 0x17, CO_OBJ_____RW), CO_TUNSIGNED16, (CO_DATA)(&data[2]));
    TS_ODAdd(CO_KEY(0x2500, 0x18, CO_OBJ_____RW), CO_TUNSIGNED16, (CO_DATA)(&data[3]));
    TS_CreateNodeAutoStart(&node);

    TS_PDO_SEND(0x201, 0x41);

    /* check signals to be unchanged */
    TS_ASSERT(0x8182 == data[0]);
    TS_ASSERT(0x8384 == data[1]);
    TS_ASSERT(0x8586 == data[2]);
    TS_ASSERT(0x8788 == data[3]);

    TS_SYNC_SEND();

    /* check signals to be changed */
    TS_ASSERT(0x4241 == data[0]);
    TS_ASSERT(0x4443 == data[1]);
    TS_ASSERT(0x4645 == data[2]);
    TS_ASSERT(0x4847 == data[3]);

    /* check error free stack execution */
    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC3
*
*          This testcase will check the principle reception of:
*          - PDO #0 (2 separate longs in content)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_RPdo_2x4Byte)
{
    CO_NODE  node;
    uint32_t rpdo_id     = 0x40000200;
    uint32_t rpdo_map[2] = { 0x25001F20, 0x25002020 };
    uint8_t  rpdo_type   = 1;
    uint8_t  rpdo_len    = 2;
    uint32_t data[2]     = { 0x71727374, 0x75767778};

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &rpdo_id,     &rpdo_type);
    TS_CreateRPdoMap(0, &rpdo_map[0], &rpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x1F, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&data[0]));
    TS_ODAdd(CO_KEY(0x2500, 0x20, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&data[1]));
    TS_CreateNodeAutoStart(&node);

    TS_PDO_SEND(0x201, 0x31);

    /* check signals to be unchanged */
    TS_ASSERT(0x71727374 == data[0]);
    TS_ASSERT(0x75767778 == data[1]);

    TS_SYNC_SEND();

    /* check signals to be changed */
    TS_ASSERT(0x34333231 == data[0]);
    TS_ASSERT(0x38373635 == data[1]);

    /* check error free stack execution */
    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC4
*
*          This testcase will check the principle reception of:
*          - PDO #0 (different data with bad alignments in content)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_RPdo_1_2_4Byte)
{
    CO_NODE  node;
    uint32_t rpdo_id     = 0x40000200;
    uint32_t rpdo_map[3] = { 0x25000B08, 0x25001510, 0x25001F20 };
    uint8_t  rpdo_type   = 1;
    uint8_t  rpdo_len    = 3;
    uint8_t  data8       = 0x91;
    uint16_t data16      = 0x8182;
    uint32_t data32      = 0x71727374;

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &rpdo_id,     &rpdo_type);
    TS_CreateRPdoMap(0, &rpdo_map[0], &rpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ_____RW), CO_TUNSIGNED8,  (CO_DATA)(&data8));
    TS_ODAdd(CO_KEY(0x2500, 0x15, CO_OBJ_____RW), CO_TUNSIGNED16, (CO_DATA)(&data16));
    TS_ODAdd(CO_KEY(0x2500, 0x1F, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&data32));
    TS_CreateNodeAutoStart(&node);

    TS_PDO_SEND(0x201, 0x21);

    /* check signals to be unchanged */
    TS_ASSERT(0x91 == data8);
    TS_ASSERT(0x8182 == data16);
    TS_ASSERT(0x71727374 == data32);

    TS_SYNC_SEND();

    /* check signals to be unchanged */
    TS_ASSERT(0x21 == data8);
    TS_ASSERT(0x2322 == data16);
    TS_ASSERT(0x27262524 == data32);

    /* check error free stack execution */
    CHK_NO_ERR(&node);
}

TS_DEF_MAIN(TS_RPdo_24Bit)
{
    CO_NODE  node;
    uint32_t rpdo_id     = 0x40000200;
    uint32_t rpdo_map[2] = { 0x25000B18, 0x25001520 };
    uint8_t  rpdo_type   = 1;
    uint8_t  rpdo_len    = 2;
    uint32_t data24      = 0x91929394;
    uint32_t data32      = 0x71727374;

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &rpdo_id,     &rpdo_type);
    TS_CreateRPdoMap(0, &rpdo_map[0], &rpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&data24));
    TS_ODAdd(CO_KEY(0x2500, 0x15, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&data32));
    TS_CreateNodeAutoStart(&node);

    TS_PDO_SEND(0x201, 0x21);

    /* check signals to be unchanged */
    TS_ASSERT(0x91929394 == data24);
    TS_ASSERT(0x71727374 == data32);

    TS_SYNC_SEND();

    /* check signals to be unchanged */
    TS_ASSERT(0x00232221 == data24);
    TS_ASSERT(0x27262524 == data32);

    /* check error free stack execution */
    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC5
*
*          This testcase will check the principle reception of:
*          - PDO #0 (no data in content)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_RPdo_NoData)
{
    CO_NODE  node;
    uint32_t rpdo_id   = 0x40000200;
    uint8_t  rpdo_type = 1;
    uint8_t  rpdo_len  = 0;

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &rpdo_id, &rpdo_type);
    TS_CreateRPdoMap(0,        0, &rpdo_len);
    TS_CreateNodeAutoStart(&node);

    TS_PDO_SEND(0x201, 0x11);

    TS_SYNC_SEND();

    /* check error free stack execution */
    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC6
*
*          This testcase will check the principle reception of:
*          - PDO #0 (data update after next SYNC, even if type is 3)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_RPdo_UpdateAfterSync)
{
    CO_NODE  node;
    uint32_t rpdo_id   = 0x40000200;
    uint32_t rpdo_map  = 0x25000B08;
    uint8_t  rpdo_type = 3;
    uint8_t  rpdo_len  = 1;
    uint8_t  data      = 0x91;

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &rpdo_id,  &rpdo_type);
    TS_CreateRPdoMap(0, &rpdo_map, &rpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ_____RW), CO_TUNSIGNED8, (CO_DATA)(&data));
    TS_CreateNodeAutoStart(&node);

    TS_PDO_SEND(0x201, 0x11);

    /* check signals to be unchanged */
    TS_ASSERT(0x91 == data);

    TS_SYNC_SEND();

    /* check signals to be changed */
    TS_ASSERT(0x11 == data);

    /* check error free stack execution */
    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC7
*
*          This testcase will check the principle reception of:
*          - PDO #0 (data update after reception, even if type is 254)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_RPdo_UpdateType254)
{
    CO_NODE  node;
    uint32_t rpdo_id   = 0x40000200;
    uint32_t rpdo_map  = 0x25000B08;
    uint8_t  rpdo_type = 254;
    uint8_t  rpdo_len  = 1;
    uint8_t  data      = 0x91;

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &rpdo_id,  &rpdo_type);
    TS_CreateRPdoMap(0, &rpdo_map, &rpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ_____RW), CO_TUNSIGNED8, (CO_DATA)(&data));
    TS_CreateNodeAutoStart(&node);

    TS_PDO_SEND(0x201, 0x31);

    /* check signals to be changed */
    TS_ASSERT(0x31 == data);

    /* check error free stack execution */
    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC8
*
*          This testcase will check the principle reception of:
*          - PDO #0 (data update after reception, even if type is 255)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_RPdo_UpdateType255)
{
    CO_NODE  node;
    uint32_t rpdo_id   = 0x40000200;
    uint32_t rpdo_map  = 0x25000B08;
    uint8_t  rpdo_type = 255;
    uint8_t  rpdo_len  = 1;
    uint8_t  data      = 0x91;

    TS_CreateMandatoryDir();
    TS_CreateRPdoCom(0, &rpdo_id,  &rpdo_type);
    TS_CreateRPdoMap(0, &rpdo_map, &rpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ_____RW), CO_TUNSIGNED8, (CO_DATA)(&data));
    TS_CreateNodeAutoStart(&node);

    TS_PDO_SEND(0x201, 0xE1);

    /* check signals to be changed */
    TS_ASSERT(0xE1 == data);

    /* check error free stack execution */
    CHK_NO_ERR(&node);
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

static void PdoRxSetup(void)
{
    TS_CallbackInit(&CORpdoWriteDataCb);
}

static void PdoRxCleanup(void)
{
    TS_CallbackDeInit();
}

SUITE_PDO_RX()
{
    TS_Begin(__FILE__);
    TS_SetupCase(PdoRxSetup, PdoRxCleanup);

    TS_RUNNER(TS_RPdo_8x1Byte);
    TS_RUNNER(TS_RPdo_4x2Byte);
    TS_RUNNER(TS_RPdo_2x4Byte);
    TS_RUNNER(TS_RPdo_1_2_4Byte);
    TS_RUNNER(TS_RPdo_24Bit);

    TS_RUNNER(TS_RPdo_NoData);
    TS_RUNNER(TS_RPdo_UpdateAfterSync);
    TS_RUNNER(TS_RPdo_UpdateType254);
    TS_RUNNER(TS_RPdo_UpdateType255);

    TS_End();
}
