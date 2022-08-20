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

/*------------------------------------------------------------------------------------------------*/
/*!
* \addtogroup sdos_exp_up
* \details    This test suite checks the protocol for Expedited SDO Upload (e.g. Master read data
*             from Slave).
*
* #### Interface
*
* + obj: object entry type
*   - para   : object is a parameter
*   - var    : object is a variable
*   - const  : object is a constant
*   - badidx : index of the object is not existing in object dictionary
*   - badsub : subindex of the object is not existing in object dictionary
* + size: object entry size
*   - byte   : object value is a byte
*   - word   : object value is a word
*   - long   : object value is a long
*   - nodata : object data reference is invalid (NULL)
* + prop: object entry property
*   - none   : no special property set
*   - node-id: property 'Node-ID' is set
*   - wr-only: object entry is write-only
* + req: SDO request
*   - ok     : correct SDO request
*   - badcmd : bad SDO command in request
*
* #### Test Definition
*
* test-function                  | obj    | size    | prop    | req    | type
* ------------------------------ | ------ | ------- | ------- | ------ | ----
* \ref TS_ExpRd_1BytePara        | para   | byte    | none    | ok     | F
* \ref TS_ExpRd_2BytePara        | para   | word    | none    | ok     | F
* \ref TS_ExpRd_4BytePara        | para   | long    | none    | ok     | F
* \ref TS_ExpRd_1ByteVar         | var    | byte    | none    | ok     | F
* \ref TS_ExpRd_2ByteVar         | var    | word    | none    | ok     | F
* \ref TS_ExpRd_4ByteVar         | var    | long    | none    | ok     | F
* \ref TS_ExpRd_4ByteConst       | const  | long    | none    | ok     | F
* \ref TS_ExpRd_4ByteConstNodeId | const  | long    | node-id | ok     | F
* \ref TS_ExpRd_2ByteParaNodeId  | para   | word    | node-id | ok     | F
* \ref TS_ExpRd_BadCmd           | --     | --      | --      | badcmd | R
* \ref TS_ExpRd_ObjNotExist      | badidx | --      | --      | ok     | R
* \ref TS_ExpRd_SubIdxNotExist   | badsub | --      | --      | ok     | R
* \ref TS_ExpRd_WriteOnly        | var    | word    | wr-only | ok     | R
* \ref TS_ExpRd_DataNullPtr      | var    | no-data | --      | ok     | R
*
* Type Legend: R = Robustness Test, F = Functional Test
* @{
*/

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "def_suite.h"

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

#define MY_TYPE  ((CO_OBJ_TYPE *)&MyType)
static uint32_t MyTypeSize(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    CO_UNUSED(obj);
    CO_UNUSED(node);
    CO_UNUSED(width);

    return (1u);
}
static CO_ERR MyTypeReadErr(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t size)
{
    CO_ERR err = CO_ERR_TYPE_RD;
    CO_UNUSED(buf);
    CO_UNUSED(size);
    COObjTypeUserSDOAbort(obj, node, 0x11223344);

    return (err);
}
static const CO_OBJ_TYPE MyType = { MyTypeSize, 0, MyTypeReadErr, 0 };

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Read a parameter byte from object dictionary
*
* \details  This test checks, that the expedited Upload of "UNSIGNED8/RW" parameter
*           is working.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a parameter of type UNSIGNED8/RW.
*
* ####      Test Steps
*           1. Send SDO expedited upload request
*
* ####      Test Checks
*           1. Check, that SDO server send a response
*           2. Check, that SDO server response is correct
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpRd_1BytePara)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  idx  = 0x2510;
    uint8_t   sub  = 1;
    uint8_t   val  = 0x11;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TUNSIGNED8, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x40, idx, sub, 0x00000000);

    /* -- CHECK -- */
    CHK_CAN  (&frm);

    CHK_SDO0 (frm, 0x4F);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, val);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Read a parameter word from object dictionary
*
* \details  This test checks, that the expedited Upload of "UNSIGNED16/RW" parameter
*           is working.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a parameter of type UNSIGNED16/RW.
*
* ####      Test Steps
*           1. Send SDO expedited upload request
*
* ####      Test Checks
*           1. Check, that SDO server send a response
*           2. Check, that SDO server response is correct
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpRd_2BytePara)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  idx  = 0x2510;
    uint8_t   sub  = 2;
    uint16_t  val  = 0x2221;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TUNSIGNED16, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x40, idx, sub, 0x00000000);

    /* -- CHECK -- */
    CHK_CAN  (&frm);

    CHK_SDO0 (frm, 0x4B);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, val);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Read a parameter long from object dictionary
*
* \details  This test checks, that the expedited Upload of "UNSIGNED32/RW" parameter
*           is working.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a parameter of type UNSIGNED32/RW.
*
* ####      Test Steps
*           1. Send SDO expedited upload request
*
* ####      Test Checks
*           1. Check, that SDO server send a response
*           2. Check, that SDO server response is correct
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpRd_4BytePara)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  idx  = 0x2510;
    uint8_t   sub  = 3;
    uint32_t  val  = 0x44434241;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x40, idx, sub, 0x00000000);

    /* -- CHECK -- */
    CHK_CAN  (&frm);

    CHK_SDO0 (frm, 0x43);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, val);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Read a variable byte from object dictionary
*
* \details  This test checks, that the expedited Upload of "UNSIGNED8/RW" variable
*           is working.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a variable of type UNSIGNED8/RW.
*
* ####      Test Steps
*           1. Send SDO expedited upload request
*
* ####      Test Checks
*           1. Check, that SDO server send a response
*           2. Check, that SDO server response is correct
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpRd_1ByteVar)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  idx  = 0x2500;
    uint8_t   sub  = 1;
    uint8_t   val  = 0x11;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x40, idx, sub, 0x00000000);

    /* -- CHECK -- */
    CHK_CAN  (&frm);

    CHK_SDO0 (frm, 0x4F);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, val);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Read a variable word from object dictionary
*
* \details  This test checks, that the expedited Upload of "UNSIGNED16/RW" variable
*           is working.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a variable of type UNSIGNED16/RW.
*
* ####      Test Steps
*           1. Send SDO expedited upload request
*
* ####      Test Checks
*           1. Check, that SDO server send a response
*           2. Check, that SDO server response is correct
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpRd_2ByteVar)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  idx  = 0x2500;
    uint8_t   sub  = 2;
    uint16_t  val  = 0x2221;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ____PRW), CO_TUNSIGNED16, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x40, idx, sub, 0x00000000);

    /* -- CHECK -- */
    CHK_CAN  (&frm);
    CHK_SDO0 (frm, 0x4B);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, val);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Read a variable long from object dictionary
*
* \details  This test checks, that the expedited Upload of "UNSIGNED32/RW" variable
*           is working.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a variable of type UNSIGNED32/RW.
*
* ####      Test Steps
*           1. Send SDO expedited upload request
*
* ####      Test Checks
*           1. Check, that SDO server send a response
*           2. Check, that SDO server response is correct
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpRd_4ByteVar)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  idx  = 0x2500;
    uint8_t   sub  = 3;
    uint32_t  val  = 0x44434241;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ____PRW), CO_TUNSIGNED32, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x40, idx, sub, 0x00000000);

    /* -- CHECK -- */
    CHK_CAN  (&frm);

    CHK_SDO0 (frm, 0x43);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, val);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Read a constant long from object dictionary
*
* \details  This test checks, that the expedited Upload of "UNSIGNED32/RO" constant
*           is working.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a constant of type UNSIGNED32/RO.
*
* ####      Test Steps
*           1. Send SDO expedited upload request
*
* ####      Test Checks
*           1. Check, that SDO server send a response
*           2. Check, that SDO server response is correct
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpRd_4ByteConst)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  idx  = 0x2505;
    uint8_t   sub  = 3;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_D___R_), CO_TUNSIGNED32, (CO_DATA)(0x44434241));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x40, idx, sub, 0x00000000);

    /* -- CHECK -- */
    CHK_CAN  (&frm);

    CHK_SDO0 (frm, 0x43);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, 0x44434241);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Read a constant long with property 'Node-ID' from object dictionary
*
* \details  This test checks, that the expedited Upload of "UNSIGNED32/RO" constant
*           with enabled property 'Node-ID' is working.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a constant of type UNSIGNED32/RO and
*              enabled property 'Node-ID'.
*
* ####      Test Steps
*           1. Send SDO expedited upload request
*
* ####      Test Checks
*           1. Check, that SDO server send a response
*           2. Check, that SDO server response is correct
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpRd_4ByteConstNodeId)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  idx  = 0x2505;
    uint8_t   sub  = 3;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_DN__R_),  CO_TUNSIGNED32, (CO_DATA)(0x44434241));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x40, idx, sub, 0x00000000);

    /* -- CHECK -- */
    CHK_CAN  (&frm);

    CHK_SDO0(frm, 0x43);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA(frm, 0x44434241 + node.NodeId);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Read a parameter word with property 'Node-ID' from object dictionary
*
* \details  This test checks, that the expedited Upload of "UNSIGNED16/RW" parameter
*           with enabled property 'Node-ID' is working.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a constant of type UNSIGNED16/RW and
*              enabled property 'Node-ID'.
*
* ####      Test Steps
*           1. Send SDO expedited upload request
*
* ####      Test Checks
*           1. Check, that SDO server send a response
*           2. Check, that SDO server response is correct
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpRd_2ByteParaNodeId)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  idx  = 0x2510;
    uint8_t   sub  = 6;
    uint16_t  val  = 0x2221;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ__N_PRW), CO_TUNSIGNED16, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x40, idx, sub, 0x00000000);

    /* -- CHECK -- */
    CHK_CAN  (&frm);

    CHK_SDO0 (frm, 0x4B);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, val + node.NodeId);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Response to a bad SDO expedited upload request command
*
* \details  This test will check, that the Abort code "Client/server command specifier not valid
*           or unknown" (abort code 0x05040001) is sent to a SDO request with a bad command byte.
*
* ####      Test Preparation
*           none
*
* ####      Test Steps
*           1. Send SDO expedited upload request with bad command
*
* ####      Test Checks
*           1. Check, that SDO server send a response
*           2. Check, that SDO server aborts the request
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpRd_BadCmd)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  idx  = 0x2510;
    uint8_t   sub  = 3;
    uint16_t  val  = 0x2221;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TUNSIGNED16, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0xF0, idx, sub, 0x00000000);

    /* -- CHECK -- */
    CHK_CAN  (&frm);

    CHK_SDO0 (frm, 0x80);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, 0x05040001);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Response to a SDO expedited upload request addressing a not existing object entry
*
* \details  This test will check, that the Abort code "Object does not exist in the object
*           dictionary." (abort code 0x06020000) is sent to a SDO request addressing a not
*           existing object entry.
*
* ####      Test Preparation
*           none
*
* ####      Test Steps
*           1. Send SDO expedited upload request addressing a non existing object entry
*
* ####      Test Checks
*           1. Check, that SDO server send a response
*           2. Check, that SDO server aborts the request
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpRd_ObjNotExist)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  idx  = 0x2180;
    uint8_t   sub  = 0;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x40, idx, sub, 0x00000000);

    /* -- CHECK -- */
    CHK_CAN  (&frm);

    CHK_SDO0 (frm, 0x80);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, 0x06020000);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Response to a SDO expedited upload request addressing a not existing subindex
*
* \details  This test will check, that the Abort code "Sub-index does not exist." (abort
*           code 0x06090011) is sent to a SDO request addressing a not existing subindex.
*
* ####      Test Preparation
*           none
*
* ####      Test Steps
*           1. Send SDO expedited upload request addressing a non existing subindex
*
* ####      Test Checks
*           1. Check, that SDO server send a response
*           2. Check, that SDO server aborts the request
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpRd_SubIdxNotExist)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  idx  = 0x2505;
    uint8_t   sub  = 0;
    uint16_t  val  = 0x2221;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TUNSIGNED16, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    sub++;
    TS_SDO_SEND (0x40, idx, sub, 0x00000000);

    /* -- CHECK -- */
    CHK_CAN  (&frm);

    CHK_SDO0 (frm, 0x80);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, 0x06090011);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Response to a SDO expedited upload request addressing a write-only object
*
* \details  This test will check, that the Abort code "Attempt to read a write only object."
*           (abort code 0x06010001) is sent to a SDO request addressing a write-only object.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a write-only object
*
* ####      Test Steps
*           1. Send SDO expedited upload request addressing a write-only object
*
* ####      Test Checks
*           1. Check, that SDO server send a response
*           2. Check, that SDO server aborts the request
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpRd_WriteOnly)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  idx  = 0x2505;
    uint8_t   sub  = 5;
    uint16_t  val  = 0x2221;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ______W), CO_TUNSIGNED16, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x40, idx, sub, 0x00000000);

    /* -- CHECK -- */
    CHK_CAN  (&frm);

    CHK_SDO0 (frm, 0x80);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, 0x06010001);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Response to a SDO expedited upload request addressing a write-only object
*
* \details  This test will check, that the Abort code "Data cannot be transferred or stored to
*           the application." (abort code 0x08000020) is sent to a SDO request addressing an
*           object with a miss-configured data reference.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a miss-configured object (data reference is NULL)
*
* ####      Test Steps
*           1. Send SDO expedited upload request addressing the miss-configured object
*
* ####      Test Checks
*           1. Check, that SDO server send a response
*           2. Check, that SDO server aborts the request
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpRd_DataNullPtr)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  idx  = 0x2500;
    uint8_t   sub  = 4;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TUNSIGNED16, (CO_DATA)(0));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x40, idx, sub, 0x00000000);

    /* -- CHECK -- */
    CHK_CAN  (&frm);

    CHK_SDO0 (frm, 0x80);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, 0x08000020);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Response to a bad SDO expedited upload request command
*
* \details  This test will check, that the Abort code "Client/server command specifier not valid
*           or unknown" (abort code 0x05040001) is sent to a SDO request with a bad command byte.
*
* ####      Test Preparation
*           none
*
* ####      Test Steps
*           1. Send SDO expedited upload request with bad command
*
* ####      Test Checks
*           1. Check, that SDO server send a response
*           2. Check, that SDO server aborts the request
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpRd_UserReadErr)
{
    CO_IF_FRM frm;
    CO_NODE   node;
    uint16_t  idx  = 0x2510;
    uint8_t   sub  = 3;
    uint16_t  val  = 0x1234;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), MY_TYPE, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x40, idx, sub, 0);

    /* -- CHECK -- */
    CHK_CAN  (&frm);

    CHK_SDO0 (frm, 0x80);
    CHK_MLTPX(frm, idx, sub);
    CHK_DATA (frm, 0x11223344);

    CHK_NO_ERR(&node);
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_EXP_UP()
{
    TS_Begin(__FILE__);

//    CanDiagnosticOn(0);

    TS_RUNNER(TS_ExpRd_1BytePara);
    TS_RUNNER(TS_ExpRd_2BytePara);
    TS_RUNNER(TS_ExpRd_4BytePara);
    TS_RUNNER(TS_ExpRd_1ByteVar);
    TS_RUNNER(TS_ExpRd_2ByteVar);
    TS_RUNNER(TS_ExpRd_4ByteVar);
    TS_RUNNER(TS_ExpRd_4ByteConst);
    TS_RUNNER(TS_ExpRd_4ByteConstNodeId);
    TS_RUNNER(TS_ExpRd_2ByteParaNodeId);

    TS_RUNNER(TS_ExpRd_BadCmd);
    TS_RUNNER(TS_ExpRd_ObjNotExist);
    TS_RUNNER(TS_ExpRd_SubIdxNotExist);
    TS_RUNNER(TS_ExpRd_WriteOnly);
    TS_RUNNER(TS_ExpRd_DataNullPtr);

    TS_RUNNER(TS_ExpRd_UserReadErr);

//    CanDiagnosticOff(0);

    TS_End();
}

/*! @} */
