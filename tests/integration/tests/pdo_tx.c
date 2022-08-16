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
*          This testcase will check the principle transmission of:
*          - PDO #0 (8 separate bytes in content)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_8x1Byte)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map[8]  = { 0x25000B08, 0x25000C08, 0x25000D08, 0x25000E08,
                               0x25000F08, 0x25001008, 0x25001108, 0x25001208 };
    uint8_t   tpdo_type    = 1;
    uint16_t  tpdo_inhibit = 0;
    uint16_t  tpdo_evtime  = 0;
    uint8_t   tpdo_len     = 8;
    uint8_t   data[8]      = { 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98};

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(0, &tpdo_map[0], &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data[0]));
    TS_ODAdd(CO_KEY(0x2500, 0x0C, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data[1]));
    TS_ODAdd(CO_KEY(0x2500, 0x0D, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data[2]));
    TS_ODAdd(CO_KEY(0x2500, 0x0E, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data[3]));
    TS_ODAdd(CO_KEY(0x2500, 0x0F, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data[4]));
    TS_ODAdd(CO_KEY(0x2500, 0x10, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data[5]));
    TS_ODAdd(CO_KEY(0x2500, 0x11, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data[6]));
    TS_ODAdd(CO_KEY(0x2500, 0x12, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data[7]));
    TS_CreateNodeAutoStart(&node);

    TS_SYNC_SEND();

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 8);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);
    CHK_BYTE (frm, 1, 0x92);
    CHK_BYTE (frm, 2, 0x93);
    CHK_BYTE (frm, 3, 0x94);
    CHK_BYTE (frm, 4, 0x95);
    CHK_BYTE (frm, 5, 0x96);
    CHK_BYTE (frm, 6, 0x97);
    CHK_BYTE (frm, 7, 0x98);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC2
*
*          This testcase will check the principle transmission of:
*          - PDO #0 (4 separate words in content)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_4x2Byte)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map[4]  = { 0x25001510, 0x25001610, 0x25001710, 0x25001810 };
    uint8_t   tpdo_type    = 1;
    uint16_t  tpdo_inhibit = 0;
    uint16_t  tpdo_evtime  = 0;
    uint8_t   tpdo_len     = 4;
    uint16_t  data[4]      = { 0x8182, 0x8384, 0x8586, 0x8788 };

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(0, &tpdo_map[0], &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x15, CO_OBJ____PRW), CO_TUNSIGNED16, (CO_DATA)(&data[0]));
    TS_ODAdd(CO_KEY(0x2500, 0x16, CO_OBJ____PRW), CO_TUNSIGNED16, (CO_DATA)(&data[1]));
    TS_ODAdd(CO_KEY(0x2500, 0x17, CO_OBJ____PRW), CO_TUNSIGNED16, (CO_DATA)(&data[2]));
    TS_ODAdd(CO_KEY(0x2500, 0x18, CO_OBJ____PRW), CO_TUNSIGNED16, (CO_DATA)(&data[3]));
    TS_CreateNodeAutoStart(&node);

    TS_SYNC_SEND();

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 8);                         /* check PDO #0 (Id and DLC)                */
    CHK_WORD (frm, 0, 0x8182);
    CHK_WORD (frm, 2, 0x8384);
    CHK_WORD (frm, 4, 0x8586);
    CHK_WORD (frm, 6, 0x8788);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC3
*
*          This testcase will check the principle transmission of:
*          - PDO #0 (2 separate longs in content)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_2x4Byte)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map[2]  = { 0x25001F20, 0x25002020 };
    uint8_t   tpdo_type    = 1;
    uint16_t  tpdo_inhibit = 0;
    uint16_t  tpdo_evtime  = 0;
    uint8_t   tpdo_len     = 2;
    uint32_t  data[2]      = { 0x71727374, 0x75767778 };

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(0, &tpdo_map[0], &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x1F, CO_OBJ____PRW), CO_TUNSIGNED32, (CO_DATA)(&data[0]));
    TS_ODAdd(CO_KEY(0x2500, 0x20, CO_OBJ____PRW), CO_TUNSIGNED32, (CO_DATA)(&data[1]));
    TS_CreateNodeAutoStart(&node);

    TS_SYNC_SEND();

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 8);                         /* check PDO #0 (Id and DLC)                */
    CHK_LONG (frm, 0, 0x71727374);
    CHK_LONG (frm, 4, 0x75767778);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC4
*
*          This testcase will check the principle transmission of:
*          - PDO #0 (different data with bad alignments in content)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_1_2_4Byte)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map[3]  = { 0x25000B08, 0x25001510, 0x25001F20 };
    uint8_t   tpdo_type    = 1;
    uint16_t  tpdo_inhibit = 0;
    uint16_t  tpdo_evtime  = 0;
    uint8_t   tpdo_len     = 3;
    uint8_t   data8        = 0x91;
    uint16_t  data16       = 0x8182;
    uint32_t  data32       = 0x71727374;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(0, &tpdo_map[0], &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ____PRW), CO_TUNSIGNED8,  (CO_DATA)(&data8));
    TS_ODAdd(CO_KEY(0x2500, 0x15, CO_OBJ____PRW), CO_TUNSIGNED16, (CO_DATA)(&data16));
    TS_ODAdd(CO_KEY(0x2500, 0x1F, CO_OBJ____PRW), CO_TUNSIGNED32, (CO_DATA)(&data32));
    TS_CreateNodeAutoStart(&node);

    TS_SYNC_SEND();

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 7);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);
    CHK_WORD (frm, 1, 0x8182);
    CHK_LONG (frm, 3, 0x71727374);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

TS_DEF_MAIN(TS_TPdo_24Bit)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map[2]  = { 0x25000B18, 0x25001520 };
    uint8_t   tpdo_type    = 0xfe;
    uint16_t  tpdo_inhibit = 0;
    uint16_t  tpdo_evtime  = 0;
    uint8_t   tpdo_len     = 2;
    uint32_t  data24       = 0x00123456;
    uint32_t  data32       = 0x789abcde;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(0, &tpdo_map[0], &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ____PRW), CO_TUNSIGNED32, (CO_DATA)(&data24));
    TS_ODAdd(CO_KEY(0x2500, 0x15, CO_OBJ____PRW), CO_TUNSIGNED32, (CO_DATA)(&data32));
    TS_CreateNodeAutoStart(&node);

    COTPdoTrigPdo(node.TPdo, 0);                      /* send PDO #0                              */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 7);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x56);
    CHK_BYTE (frm, 1, 0x34);
    CHK_BYTE (frm, 2, 0x12);
    CHK_LONG (frm, 3, 0x789abcde);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC5
*
*          This testcase will check the principle transmission of:
*          - PDO #0 (no data in content)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_NoData)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint8_t   tpdo_type    = 1;
    uint16_t  tpdo_inhibit = 0;
    uint16_t  tpdo_evtime  = 0;
    uint8_t   tpdo_len     = 0;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(0, 0, &tpdo_len);
    TS_CreateNodeAutoStart(&node);

    TS_SYNC_SEND();

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 0);                         /* check PDO #0 (Id and DLC)                */

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC6
*
*          This testcase will check the principle transmission of:
*          - PDO #0 (transmission after 3 SYNC messages)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_After3Sync)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map     = 0x25000B08;
    uint8_t   tpdo_type    = 3;
    uint16_t  tpdo_inhibit = 0;
    uint16_t  tpdo_evtime  = 0;
    uint8_t   tpdo_len     = 1;
    uint8_t   data8        = 0x91;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(0, &tpdo_map, &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data8));
    TS_CreateNodeAutoStart(&node);

    TS_SYNC_SEND();

    CHK_NOCAN(&frm);                                  /* check for no CAN frame                   */

    TS_SYNC_SEND();

    CHK_NOCAN(&frm);                                  /* check for no CAN frame                   */

    TS_SYNC_SEND();

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC8
*
*          This testcase will check the principle transmission of:
*          - PDO #0 (transmission after 240 SYNC messages)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_After240Sync)
{
    uint8_t   n;
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map     = 0x25000B08;
    uint8_t   tpdo_type    = 240;
    uint16_t  tpdo_inhibit = 0;
    uint16_t  tpdo_evtime  = 0;
    uint8_t   tpdo_len     = 1;
    uint8_t   data8        = 0x91;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(0, &tpdo_map, &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data8));
    TS_CreateNodeAutoStart(&node);

    for (n=0; n < 239; n++) {
        TS_SYNC_SEND();

        CHK_NOCAN(&frm);                              /* check for no CAN frame                   */
    }
    TS_SYNC_SEND();

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC9
*
*          This testcase will check the principle transmission of:
*          - PDO #0 (transmission after manufacturer specific event (type=254) via Object
*            reference)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_Type254ViaObj)
{
    CO_OBJ   *obj;
    uint16_t  n;
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map     = 0x25000B08;
    uint8_t   tpdo_type    = 254;
    uint16_t  tpdo_inhibit = 0;
    uint16_t  tpdo_evtime  = 0;
    uint8_t   tpdo_len     = 1;
    uint8_t   data8        = 0x91;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(0, &tpdo_map, &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data8));
    TS_CreateNodeAutoStart(&node);

    for (n = 0; n < 256; n++) {
        TS_SYNC_SEND();

        CHK_NOCAN(&frm);                              /* check for no CAN frame                   */
    }

    obj = CODictFind(&node.Dict, CO_DEV(0x2500,0x0B));  /* trigger PDO via object reference         */
    COTPdoTrigObj(node.TPdo, obj);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC10
*
*          This testcase will check the principle transmission of:
*          - PDO #0 (transmission after device profile specific event (type=255) via Object
*            reference)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_Type255ViaObj)
{
    CO_OBJ   *obj;
    uint16_t  n;
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map     = 0x25000B08;
    uint8_t   tpdo_type    = 255;
    uint16_t  tpdo_inhibit = 0;
    uint16_t  tpdo_evtime  = 0;
    uint8_t   tpdo_len     = 1;
    uint8_t   data8        = 0x91;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(0, &tpdo_map, &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data8));
    TS_CreateNodeAutoStart(&node);

    for (n = 0; n < 256; n++) {
        TS_SYNC_SEND();

        CHK_NOCAN(&frm);                              /* check for no CAN frame                   */
    }

    obj = CODictFind(&node.Dict, CO_DEV(0x2500,0x0B));  /* trigger PDO via object reference         */
    COTPdoTrigObj(node.TPdo, obj);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC11
*
*          This testcase will check the principle transmission of:
*          - PDO #0 (transmission after manufacturer specific event (type=254) via PDO number)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_Type254ViaNum)
{
    uint16_t  n;
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map     = 0x25000B08;
    uint8_t   tpdo_type    = 254;
    uint16_t  tpdo_inhibit = 0;
    uint16_t  tpdo_evtime  = 0;
    uint8_t   tpdo_len     = 1;
    uint8_t   data8        = 0x91;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(0, &tpdo_map, &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data8));
    TS_CreateNodeAutoStart(&node);

    for (n = 0; n < 256; n++) {
        TS_SYNC_SEND();

        CHK_NOCAN(&frm);                              /* check for no CAN frame                   */
    }

    COTPdoTrigPdo(node.TPdo, 0);                      /* trigger PDO via PDO number               */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC12
*
*          This testcase will check the principle transmission of:
*          - PDO #0 (transmission after device profile specific event (type=255) via PDO number)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_Type255ViaNum)
{
    uint16_t  n;
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map     = 0x25000B08;
    uint8_t   tpdo_type    = 255;
    uint16_t  tpdo_inhibit = 0;
    uint16_t  tpdo_evtime  = 0;
    uint8_t   tpdo_len     = 1;
    uint8_t   data8        = 0x91;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(0, &tpdo_map, &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data8));
    TS_CreateNodeAutoStart(&node);

    for (n = 0; n < 256; n++) {
        TS_SYNC_SEND();
        CHK_NOCAN(&frm);                              /* check for no CAN frame                   */
    }

    COTPdoTrigPdo(node.TPdo, 0);                      /* trigger PDO via PDO number               */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC13
*
*          This testcase will check the principle transmission of:
*          - PDO #0 (transmission after manufacturer specific event (type=255) via event timer 200ms)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_Tmr)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map     = 0x25000B08;
    uint8_t   tpdo_type    = 255;
    uint16_t  tpdo_inhibit = 0;
    uint16_t  tpdo_evtime  = 200;
    uint8_t   tpdo_len     = 1;
    uint8_t   data8        = 0x91;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(0, &tpdo_map, &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data8));
    TS_CreateNode(&node,0);

                                                      /* wait more than 1 event timer time        */
    TS_Wait(&node, 300);                              /* wait 300ms                               */

    TS_NMT_SEND(0x01, 1);                             /* set node-id 0x01 to operational          */

    TS_Wait(&node, 200);                              /* wait 200ms                               */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    TS_Wait(&node, 200);                              /* wait 200ms                               */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC14
*
*          This testcase will check the principle transmission of:
*          - PDO #0 (transmission after manufacturer specific event (type=255) via event timer
*            200ms and application event after 50ms)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_TmrAndNum)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map     = 0x25000B08;
    uint8_t   tpdo_type    = 255;
    uint16_t  tpdo_inhibit = 0;
    uint16_t  tpdo_evtime  = 200;
    uint8_t   tpdo_len     = 1;
    uint8_t   data8        = 0x91;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(0, &tpdo_map, &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data8));
    TS_CreateNodeAutoStart(&node);

    TS_Wait(&node, 200);                              /* wait 200ms                               */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    TS_Wait(&node, 200);                              /* wait 200ms                               */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    TS_Wait(&node, 50);                               /* wait 50ms                                */

    COTPdoTrigPdo(node.TPdo, 0);                      /* trigger PDO via PDO number               */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    TS_Wait(&node, 150);                              /* wait 150ms                               */

    CHK_NOCAN(&frm);                                  /* check for no CAN frame                   */

    TS_Wait(&node, 50);                               /* wait 50ms                                */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC15
*
*          This testcase will check the principle transmission of:
*          - PDO #0 (transmission after manufacturer specific event (type=255) via event timer
*            200ms, an inhibit time of 100ms and application event after 50ms)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_Inhibit)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map     = 0x25000B08;
    uint8_t   tpdo_type    = 255;
    uint16_t  tpdo_inhibit = 1000;
    uint16_t  tpdo_evtime  = 200;
    uint8_t   tpdo_len     = 1;
    uint8_t   data8        = 0x91;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(0, &tpdo_map, &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data8));
    TS_CreateNodeAutoStart(&node);

    TS_Wait(&node, 200);                              /* wait 200ms                               */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    TS_Wait(&node, 200);                              /* wait 200ms                               */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    TS_Wait(&node, 50);                               /* wait 50ms                                */

    COTPdoTrigPdo(node.TPdo, 0);                      /* trigger PDO via PDO number               */
    CHK_NOCAN(&frm);                                  /* check for no CAN frame                   */

    TS_Wait(&node, 50);                               /* wait 50ms                                */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC20
*
*          This testcase will check the principle transmission of:
*          - PDO #0 (transmission after manufacturer specific event (type=255) via event timer
*            60ms and an inhibit time of 100ms)
*
*            E.....E.....E.....E.....E        [Event Timer Elapsed]
*            |     |     |     |     |
*            |<---I--->|<---I--->|<---I--->|  [Inhibit Timer Active]
*            |         |         |
*            V         V         V            [Transmission Event & Inhibit Timer Reset]
*
*            .x.....x.....x.....x.....x       [x = checks in test]
*             1     0     1     0     1       [expected: 1 = message, 0 = no message]
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_TmrAndInhibit)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map     = 0x25000B08;
    uint8_t   tpdo_type    = 255;
    uint16_t  tpdo_inhibit = 1000;
    uint16_t  tpdo_evtime  = 60;
    uint8_t   tpdo_len     = 1;
    uint8_t   data8        = 0x91;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(0, &tpdo_map, &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data8));
    TS_CreateNodeAutoStart(&node);

    TS_Wait(&node, 70);                               /* wait 70ms                                */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    TS_Wait(&node, 60);                               /* wait 60ms                               */

    CHK_NOCAN(&frm);                                  /* check for no CAN frame                   */

    TS_Wait(&node, 60);                               /* wait 60ms                                */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    TS_Wait(&node, 60);                               /* wait 60ms                               */

    CHK_NOCAN(&frm);                                  /* check for no CAN frame                   */

    TS_Wait(&node, 60);                               /* wait 60ms                                */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC23
*
*          This testcase will check the change of the PDO event timer via SDO access with:
*          - NMT in PRE-OPERATIONAL mode
*          - PDO #0 event timer disabled, inhibit time 100ms
*          - Set PDO #0 event timer via SDO to 200ms
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_ChangeTmr)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map     = 0x25000B08;
    uint8_t   tpdo_type    = 255;
    uint16_t  tpdo_inhibit = 1000;
    uint16_t  tpdo_evtime  = 0;
    uint8_t   tpdo_len     = 1;
    uint8_t   data8        = 0x91;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(0, &tpdo_map, &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data8));
    TS_CreateNode(&node,0);

    TS_SDO_SEND (0x22, 0x1800, 5, 200);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x60);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, 0x1800, 5);                        /* check multiplexer                        */
    CHK_DATA (frm, 0x00000000);                       /* check data area                          */

    TS_NMT_SEND(0x01, 1);                            /* set node-id 0x01 to operational          */

    TS_Wait(&node, 210);                              /* wait 210ms                               */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    TS_Wait(&node, 200);                              /* wait 200ms                               */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC24
*
*          This testcase will check the change of the PDO event timer via SDO access with:
*          - NMT in OPERATIONAL mode
*          - PDO #0 event timer disabled, inhibit time 100ms
*          - Set PDO #0 event timer via SDO to 1ms
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_TmrFastest)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map     = 0x25000B08;
    uint8_t   tpdo_type    = 255;
    uint16_t  tpdo_inhibit = 1000;
    uint16_t  tpdo_evtime  = 0;
    uint8_t   tpdo_len     = 1;
    uint8_t   data8        = 0x91;
    uint16_t  min_time;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(0, &tpdo_map, &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data8));
    TS_CreateNodeAutoStart(&node);

    TS_Wait(&node, 210);                              /* wait 210ms                               */

    CHK_NOCAN(&frm);                                  /* check no PDO is sent                     */

    min_time = COTmrGetMinTime(&node.Tmr, CO_TMR_UNIT_1MS);
    TS_SDO_SEND (0x22, 0x1800, 5, min_time);

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_SDO0 (frm, 0x60);                             /* check SDO #0 response (Id and DLC)       */
    CHK_MLTPX(frm, 0x1800, 5);                        /* check multiplexer                        */
    CHK_DATA (frm, 0x00000000);                       /* check data area                          */

    TS_Wait(&node, 10);                               /* wait 10ms                                */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    TS_Wait(&node, 50);                               /* wait 50ms                                */

    CHK_NOCAN(&frm);                                  /* check no PDO is sent                     */

    TS_Wait(&node, 50);                               /* wait 50ms                                */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    TS_Wait(&node, 50);                               /* wait 50ms                                */

    CHK_NOCAN(&frm);                                  /* check no PDO is sent                     */

    TS_Wait(&node, 50);                               /* wait 50ms                                */

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC25
*
*          This testcase will check the asynchronous transmission of:
*          - PDO #0 (2 separate longs in content)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_Async)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map[2]  = { 0x25002920, 0x25002A20 };
    uint8_t   tpdo_type    = 1;
    uint16_t  tpdo_inhibit = 0;
    uint16_t  tpdo_evtime  = 0;
    uint8_t   tpdo_len     = 2;
    uint32_t  data[2]      = { 0x71727374, 0x75767778 };

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(0, &tpdo_map[0], &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x29, CO_OBJ___APRW), CO_TUNSIGNED32, (CO_DATA)(&data[0]));
    TS_ODAdd(CO_KEY(0x2500, 0x2A, CO_OBJ____PRW), CO_TUNSIGNED32, (CO_DATA)(&data[1]));
    TS_CreateNodeAutoStart(&node);

    CODictWrLong(&node.Dict,CO_DEV(0x2500,0x29),0L);  /* write to asynchronous object entry       */
    SimCanRun();

    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 8);                         /* check PDO #0 (Id and DLC)                */
    CHK_LONG (frm, 0, 0x00000000);
    CHK_LONG (frm, 4, 0x75767778);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TC25
*
*          This testcase will check the PDO timing in the case, that operational mode
*          is requested while we are already in operational mode.
*          - PDO #0 (2 separate longs in content)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_SwitchToOpWhileInOp)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map     = 0x25000B08;
    uint8_t   tpdo_type    = 255;
    uint16_t  tpdo_inhibit = 0;
    uint16_t  tpdo_evtime  = 200;
    uint8_t   tpdo_len     = 1;
    uint8_t   data8        = 0x91;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(0, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(0, &tpdo_map, &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data8));
    TS_CreateNode(&node,0);

                                                      /* wait more than 1 event timer time        */
    TS_Wait(&node, 300);                              /* wait 300ms                               */
    TS_NMT_SEND(0x01, 1);                             /* set node-id 0x01 to operational          */

    TS_Wait(&node, 200);                              /* wait 200ms                               */
    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    TS_Wait(&node, 190);                              /* wait 190ms                               */
    CHK_NOCAN(&frm)                                   /* check for a single CAN frame             */

    TS_Wait(&node, 10);                               /* wait 10ms                                */
    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    TS_NMT_SEND(0x01, 1);                             /* set node-id 0x01 to operational again    */

    TS_Wait(&node, 190);                              /* wait 190ms                               */
    CHK_NOCAN(&frm)                                   /* check for a single CAN frame             */

    TS_Wait(&node, 10);                               /* wait 10ms                                */
    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    TS_Wait(&node, 190);                              /* wait 190ms                               */
    CHK_NOCAN(&frm)                                   /* check for a single CAN frame             */

    TS_Wait(&node, 10);                               /* wait 10ms                                */
    CHK_CAN  (&frm);                                  /* check for a CAN frame                    */
    CHK_PDO0 (frm, 0x181, 1);                         /* check PDO #0 (Id and DLC)                */
    CHK_BYTE (frm, 0, 0x91);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This testcase will check issue #118 (case 1)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_ChangeEventTime)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map     = 0x25000B08;
    uint8_t   tpdo_type    = 254;
    uint16_t  tpdo_inhibit = 0;
    uint16_t  tpdo_evtime  = 0;
    uint8_t   tpdo_len     = 1;
    uint8_t   data8        = 0x91;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(1, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(1, &tpdo_map, &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ___APRW), CO_TUNSIGNED8, (CO_DATA)(&data8));
    TS_CreateNode(&node,0);
    
    TS_NMT_SEND(1, 1);                                       /* set node-id 1 to operational      */

    CODictWrByte(&node.Dict, CO_DEV(0x2500, 0x0B), 0x11);    /* change object value               */
    CHK_CAN  (&frm);                                         /* check for a CAN frame             */

    TS_SDO_SEND(0x2B, 0x1801, 5, 1000);                      /* write to 1801:05, value 1000      */
    CHK_SDO0_OK(0x1801, 5);                                  /* check for success                 */

    TS_Wait(&node, 1100);                                    /* wait for 1000ms +10%              */
    CHK_CAN(&frm);                                           /* check for a CAN frame             */

    TS_NMT_SEND(128, 1);                                     /* set node-id 1 to pre-operational  */

    CODictWrByte(&node.Dict, CO_DEV(0x2500, 0x0B), 0x22);    /* change object value               */
    CHK_NOCAN(&frm)                                          /* check for no CAN frame            */

    TS_NMT_SEND(130, 1);                                     /* communication reset on Node 1     */
    CHK_CAN(&frm);                                           /* check for a CAN frame (bootup)    */
    CHK_BOOTUP(frm, 1);                                      /* BootUp message of Node-ID #1      */

    CODictWrByte(&node.Dict, CO_DEV(0x2500, 0x0B), 0x33);    /* change object value               */
    CHK_NOCAN(&frm)                                          /* check for no CAN frame            */

    TS_Wait(&node, 1100);                                    /* wait for 1000ms +10%              */
    CHK_NOCAN(&frm);                                         /* check for no CAN frame            */

    CHK_NO_ERR(&node);                                       /* check error free stack execution  */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This testcase will check issue #118 (case 2)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_SetEventTime)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map     = 0x25000B08;
    uint8_t   tpdo_type    = 254;
    uint16_t  tpdo_inhibit = 0;
    uint16_t  tpdo_evtime  = 0;
    uint8_t   tpdo_len     = 1;
    uint8_t   data8        = 0x91;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(1, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(1, &tpdo_map, &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ___APRW), CO_TUNSIGNED8, (CO_DATA)(&data8));
    TS_CreateNode(&node,0);
    
    TS_NMT_SEND(128, 1);                                     /* set node-id 1 to pre-operational  */

    TS_SDO_SEND(0x2B, 0x1801, 5, 1000);                      /* write to 1801:05, value 1000      */
    CHK_SDO0_OK(0x1801, 5);                                  /* check for success                 */

    TS_NMT_SEND(1, 1);                                       /* set node-id 1 to operational      */

    TS_Wait(&node, 1100);                                    /* wait for 1000ms +10%              */
    CHK_CAN(&frm);                                           /* check for a CAN frame             */

    CODictWrByte(&node.Dict, CO_DEV(0x2500, 0x0B), 0x22);    /* change object value               */
    CHK_CAN(&frm)                                            /* check for a CAN frame             */

    CHK_NO_ERR(&node);                                       /* check error free stack execution  */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This testcase will check issue #118 (case 3)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_SetEventTimeAndReset)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map     = 0x25000B08;
    uint8_t   tpdo_type    = 254;
    uint16_t  tpdo_inhibit = 0;
    uint16_t  tpdo_evtime  = 0;
    uint8_t   tpdo_len     = 1;
    uint8_t   data8        = 0x91;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(1, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(1, &tpdo_map, &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ___APRW), CO_TUNSIGNED8, (CO_DATA)(&data8));
    TS_CreateNode(&node,0);
    
    TS_NMT_SEND(128, 1);                                     /* set node-id 1 to pre-operational  */

    TS_SDO_SEND(0x2B, 0x1801, 5, 1000);                      /* write to 1801:05, value 1000      */
    CHK_SDO0_OK(0x1801, 5);                                  /* check for success                 */

    TS_NMT_SEND(130, 1);                                     /* reset communication node-id 1     */
    CHK_CAN(&frm);                                           /* check for a CAN frame (bootup)    */

    TS_NMT_SEND(1, 1);                                       /* set node-id 1 to operational      */

    TS_Wait(&node, 1100);                                    /* wait for 1000ms +10%              */
    CHK_CAN(&frm);                                           /* check for a CAN frame             */

    CODictWrByte(&node.Dict, CO_DEV(0x2500, 0x0B), 0x22);    /* change object value               */
    CHK_CAN(&frm)                                            /* check for a CAN frame             */

    CHK_NO_ERR(&node);                                       /* check error free stack execution  */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This testcase will check issue #118 (change object entry property)
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_TPdo_ChangeAsyncProperty)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint32_t  tpdo_id      = 0x40000180;
    uint32_t  tpdo_map     = 0x25000B08;
    uint8_t   tpdo_type    = 254;
    uint16_t  tpdo_inhibit = 0;
    uint16_t  tpdo_evtime  = 0;
    uint8_t   tpdo_len     = 1;
    uint8_t   data8        = 0x91;

    TS_CreateMandatoryDir();
    TS_CreateTPdoCom(1, &tpdo_id, &tpdo_type, &tpdo_inhibit, &tpdo_evtime);
    TS_CreateTPdoMap(1, &tpdo_map, &tpdo_len);
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ___APRW), CO_TUNSIGNED8, (CO_DATA)(&data8));
    TS_CreateNode(&node,0);
    
    TS_NMT_SEND(1, 1);                                       /* set node-id 1 to operational      */

    TS_SDO_SEND(0x2B, 0x1801, 5, 1000);                      /* write to 1801:05, value 1000      */
    CHK_SDO0_OK(0x1801, 5);                                  /* check for success                 */

                                                             /* change object entry property -A   */
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data8));

    TS_Wait(&node, 1100);                                    /* wait for 1000ms +10%              */
    CHK_CAN(&frm);                                           /* check for a CAN frame             */

    CODictWrByte(&node.Dict, CO_DEV(0x2500, 0x0B), 0x22);    /* change object value               */
    CHK_NOCAN(&frm);                                         /* check for no CAN frame            */

    TS_SDO_SEND(0x2B, 0x1801, 5, 0);                         /* write to 1801:05, value 0         */
    CHK_SDO0_OK(0x1801, 5);                                  /* check for success                 */

                                                             /* change object entry property +A   */
    TS_ODAdd(CO_KEY(0x2500, 0x0B, CO_OBJ___APRW), CO_TUNSIGNED8, (CO_DATA)(&data8));

    TS_Wait(&node, 1100);                                    /* wait for 1000ms +10%              */
    CHK_NOCAN(&frm);                                         /* check for no timer CAN frame      */

    CODictWrByte(&node.Dict, CO_DEV(0x2500, 0x0B), 0x33);    /* change object value               */
    CHK_CAN(&frm);                                           /* check for a CAN frame             */

    CHK_NO_ERR(&node);                                       /* check error free stack execution  */
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_PDO_TX()
{
    TS_Begin(__FILE__);

    TS_RUNNER(TS_TPdo_8x1Byte);
    TS_RUNNER(TS_TPdo_4x2Byte);
    TS_RUNNER(TS_TPdo_2x4Byte);
    TS_RUNNER(TS_TPdo_1_2_4Byte);
    TS_RUNNER(TS_TPdo_24Bit);
    TS_RUNNER(TS_TPdo_NoData);
    TS_RUNNER(TS_TPdo_After3Sync);
    TS_RUNNER(TS_TPdo_After240Sync);
    TS_RUNNER(TS_TPdo_Type254ViaObj);
    TS_RUNNER(TS_TPdo_Type255ViaObj);
    TS_RUNNER(TS_TPdo_Type254ViaNum);
    TS_RUNNER(TS_TPdo_Type255ViaNum);
    TS_RUNNER(TS_TPdo_Tmr);
    TS_RUNNER(TS_TPdo_TmrAndNum);
    TS_RUNNER(TS_TPdo_Inhibit);
    TS_RUNNER(TS_TPdo_TmrAndInhibit);
    TS_RUNNER(TS_TPdo_ChangeTmr);
    TS_RUNNER(TS_TPdo_TmrFastest);
    TS_RUNNER(TS_TPdo_Async);
    TS_RUNNER(TS_TPdo_SwitchToOpWhileInOp);
    TS_RUNNER(TS_TPdo_ChangeEventTime);
    TS_RUNNER(TS_TPdo_SetEventTime);
    TS_RUNNER(TS_TPdo_SetEventTimeAndReset);
    TS_RUNNER(TS_TPdo_ChangeAsyncProperty);

    TS_End();
}
