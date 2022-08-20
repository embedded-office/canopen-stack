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
* \addtogroup sdos_exp_down
* \details    This test suite checks the protocol for Expedited SDO Download (e.g. Master writes
*             data to Slave).
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
*   - no-data: object data reference is invalid (NULL)
*   - small  : written size is smaller than object size
*   - large  : written size is larger than object size
* + prop: object entry property
*   - none   : no special property set
*   - rd-only: object entry is read-only
* + req: SDO request
*   - ok     : correct SDO request
*   - badcmd : bad SDO command in request
*
* #### Test Definition
*
* test-function                  | obj    | size    | prop    | req    | type
* ------------------------------ | ------ | ------- | ------- | ------ | ----
* \ref TS_ExpWr_1BytePara        | para   | byte    | none    | ok     | F
* \ref TS_ExpWr_2BytePara        | para   | word    | none    | ok     | F
* \ref TS_ExpWr_4BytePara        | para   | long    | none    | ok     | F
* \ref TS_ExpWr_1ByteVar         | var    | byte    | none    | ok     | F
* \ref TS_ExpWr_2ByteVar         | var    | word    | none    | ok     | F
* \ref TS_ExpWr_4ByteVar         | var    | long    | none    | ok     | F
* \ref TS_ExpWr_2ByteVar_NoLen   | const  | long    | none    | ok     | F
* \ref TS_ExpWr_BadCmd           | --     | --      | --      | badcmd | R
* \ref TS_ExpWr_ObjNotExist      | badidx | --      | --      | ok     | R
* \ref TS_ExpWr_SubIdxNotExist   | badsub | --      | --      | ok     | R
* \ref TS_ExpWr_ReadOnly         | var    | word    | rd-only | ok     | R
* \ref TS_ExpWr_LenTooHigh       | var    | large   | --      | ok     | R
* \ref TS_ExpWr_LenTooLow        | var    | small   | --      | ok     | R
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
static CO_ERR MyTypeWriteErr(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t size)
{
    CO_ERR err = CO_ERR_TYPE_WR;
    CO_UNUSED(buf);
    CO_UNUSED(size);
    COObjTypeUserSDOAbort(obj, node, 0x11223344);

    return (err);
}
static const CO_OBJ_TYPE MyType = { MyTypeSize, 0, 0, MyTypeWriteErr };

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Write a parameter byte to object dictionary
*
* \details  This test checks, that the expedited download of "UNSIGNED8/RW" parameter
*           is working.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a parameter of type UNSIGNED8/RW.
*
* ####      Test Steps
*           1. Send SDO expedited download request
*
* ####      Test Checks
*           1. Check, that SDO server response is correct
*           2. Check, that value is written in object dictionary
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpWr_1BytePara)
{
    CO_NODE  node;
    uint16_t idx  = 0x2510;
    uint8_t  sub  = 1;
    uint8_t  val  = 0;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TUNSIGNED8, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x2F, idx, sub, 0x14131211);

    /* -- CHECK -- */
    CHK_SDO0_OK(idx, sub);

    TS_ASSERT(0x11 == val);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Write a parameter word to object dictionary
*
* \details  This test checks, that the expedited download of "UNSIGNED16/RW" parameter
*           is working.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a parameter of type UNSIGNED16/RW.
*
* ####      Test Steps
*           1. Send SDO expedited download request
*
* ####      Test Checks
*           1. Check, that SDO server response is correct
*           2. Check, that value is written in object dictionary
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpWr_2BytePara)
{
    CO_NODE  node;
    uint16_t idx  = 0x2510;
    uint8_t  sub  = 2;
    uint16_t val  = 0;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TUNSIGNED16, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x2B, idx, sub, 0x24232221);

    /* -- CHECK -- */
    CHK_SDO0_OK(idx, sub);

    TS_ASSERT(0x2221 == val);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Write a parameter long to object dictionary
*
* \details  This test checks, that the expedited download of "UNSIGNED32/RW" parameter
*           is working.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a parameter of type UNSIGNED32/RW.
*
* ####      Test Steps
*           1. Send SDO expedited download request
*
* ####      Test Checks
*           1. Check, that SDO server response is correct
*           2. Check, that value is written in object dictionary
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpWr_4BytePara)
{
    CO_NODE  node;
    uint16_t idx  = 0x2510;
    uint8_t  sub  = 3;
    uint32_t val  = 0;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x23, idx, sub, 0x44434241);

    /* -- CHECK -- */
    CHK_SDO0_OK(idx, sub);

    TS_ASSERT(0x44434241 == val);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Write a variable byte to object dictionary
*
* \details  This test checks, that the expedited download of "UNSIGNED8/RW" variable
*           is working.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a variable of type UNSIGNED8/RW.
*
* ####      Test Steps
*           1. Send SDO expedited download request
*
* ####      Test Checks
*           1. Check, that SDO server response is correct
*           2. Check, that value is written in object dictionary
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpWr_1ByteVar)
{
    CO_NODE  node;
    uint16_t idx  = 0x2500;
    uint8_t  sub  = 1;
    uint8_t  val  = 0;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x2F, idx, sub, 0x14131211);

    /* -- CHECK -- */
    CHK_SDO0_OK(idx, sub);

    TS_ASSERT(0x11 == val);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Write a variable word to object dictionary
*
* \details  This test checks, that the expedited download of "UNSIGNED16/RW" variable
*           is working.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a variable of type UNSIGNED16/RW.
*
* ####      Test Steps
*           1. Send SDO expedited download request
*
* ####      Test Checks
*           1. Check, that SDO server response is correct
*           2. Check, that value is written in object dictionary
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpWr_2ByteVar)
{
    CO_NODE  node;
    uint16_t idx  = 0x2500;
    uint8_t  sub  = 2;
    uint16_t val  = 0;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ____PRW), CO_TUNSIGNED16, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x2B, idx, sub, 0x24232221);

    /* -- CHECK -- */
    CHK_SDO0_OK(idx, sub);

    TS_ASSERT(0x2221 == val);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Write a variable long to object dictionary
*
* \details  This test checks, that the expedited download of "UNSIGNED32/RW" variable
*           is working.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a variable of type UNSIGNED32/RW.
*
* ####      Test Steps
*           1. Send SDO expedited download request
*
* ####      Test Checks
*           1. Check, that SDO server response is correct
*           2. Check, that value is written in object dictionary
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpWr_4ByteVar)
{
    CO_NODE  node;
    uint16_t idx  = 0x2500;
    uint8_t  sub  = 3;
    uint32_t val  = 0;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ____PRW), CO_TUNSIGNED32, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x23, idx, sub, 0x44434241);

    /* -- CHECK -- */
    CHK_SDO0_OK(idx, sub);

    TS_ASSERT(0x44434241 == val);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Write a variable word to object dictionary
*
* \details  This test checks, that the expedited download of "UNSIGNED16/RW" variable
*           (download size not indicated) is working.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a variable of type UNSIGNED16/RW.
*
* ####      Test Steps
*           1. Send SDO expedited download request without download size indication
*
* ####      Test Checks
*           1. Check, that SDO server response is correct
*           2. Check, that value is written in object dictionary
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpWr_2ByteVar_NoLen)
{
    CO_NODE  node;
    uint16_t idx  = 0x2500;
    uint8_t  sub  = 1;
    uint8_t  val  = 0;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x22, idx, sub, 0x14131211);

    /* -- CHECK -- */
    CHK_SDO0_OK(idx, sub);

    TS_ASSERT(0x11 == val);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Response to a bad SDO expedited download request command
*
* \details  This test will check, that the Abort code "Client/server command specifier not valid
*           or unknown" (abort code 0x05040001) is sent to a SDO request with a bad command byte.
*
* ####      Test Preparation
*           none
*
* ####      Test Steps
*           1. Send SDO expedited download request with bad command
*
* ####      Test Checks
*           1. Check, that SDO server aborts the request
*           2. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpWr_BadCmd)
{
    CO_NODE  node;
    uint16_t idx  = 0x2510;
    uint8_t  sub  = 3;
    uint32_t val  = 0;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0xE0, idx, sub, 0x44434241);

    /* -- CHECK -- */
    CHK_SDO0_ERR(idx, sub, 0x05040001);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Response to a SDO expedited download request addressing a not existing object entry
*
* \details  This test will check, that the Abort code "Object does not exist in the object
*           dictionary." (abort code 0x06020000) is sent to a SDO request addressing a not
*           existing object entry.
*
* ####      Test Preparation
*           none
*
* ####      Test Steps
*           1. Send SDO expedited download request addressing a non existing object entry
*
* ####      Test Checks
*           1. Check, that SDO server aborts the request
*           2. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpWr_ObjNotExist)
{
    CO_NODE  node;
    uint16_t idx  = 0x2100;
    uint8_t  sub  = 1;
    uint32_t val  = 0;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    idx++;
    TS_SDO_SEND (0x2B, idx, sub, 0x24232221);

    /* -- CHECK -- */
    CHK_SDO0_ERR(idx, sub, 0x06020000);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Response to a SDO expedited download request addressing a not existing subindex
*
* \details  This test will check, that the Abort code "Sub-index does not exist." (abort
*           code 0x06090011) is sent to a SDO request addressing a not existing subindex.
*
* ####      Test Preparation
*           none
*
* ####      Test Steps
*           1. Send SDO expedited download request addressing a non existing subindex
*
* ####      Test Checks
*           1. Check, that SDO server aborts the request
*           2. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpWr_SubIdxNotExist)
{
    CO_NODE  node;
    uint16_t idx  = 0x2100;
    uint8_t  sub  = 0;
    uint32_t val  = 0;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    sub++;
    TS_SDO_SEND (0x2B, idx, sub, 0x24232221);

    /* -- CHECK -- */
    CHK_SDO0_ERR(idx, sub, 0x06090011);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Response to a SDO expedited download request addressing a read-only object
*
* \details  This test will check, that the Abort code "Attempt to write a read only object."
*           (abort code 0x06010002) is sent to a SDO request addressing a read-only object.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a read-only object
*
* ####      Test Steps
*           1. Send SDO expedited download request addressing a read-only object
*
* ####      Test Checks
*           1. Check, that SDO server aborts the request
*           2. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpWr_ReadOnly)
{
    CO_NODE  node;
    uint16_t idx  = 0x2510;
    uint8_t  sub  = 6;
    uint16_t val  = 0;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____R_), CO_TUNSIGNED16, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x2B, idx, sub, 0x24232221);

    /* -- CHECK -- */
    CHK_SDO0_ERR(idx, sub, 0x06010002);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Response to a SDO expedited download request addressing a smaller object
*
* \details  This test will check, that the abort code "Data type doesn't match, length too high."
*           (abort code 0x06070012) is sent to a SDO request addressing a smaller object.
*
* ####      Test Preparation
*           none
*
* ####      Test Steps
*           1. Send SDO expedited download request addressing a smaller object
*
* ####      Test Checks
*           1. Check, that SDO server aborts the request
*           2. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpWr_LenTooHigh)
{
    CO_NODE  node;
    uint16_t idx  = 0x2510;
    uint8_t  sub  = 2;
    uint16_t val  = 0;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TUNSIGNED16, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x23, idx, sub, 0x24232221);

    /* -- CHECK -- */
    CHK_SDO0_ERR(idx, sub, 0x06070012);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Response to a SDO expedited download request addressing a larger object
*
* \details  This test will check, that the abort code "Data type doesn't match, length too low."
*           (abort code 0x06070013) is sent to a SDO request addressing a smaller object.
*
* ####      Test Preparation
*           none
*
* ####      Test Steps
*           1. Send SDO expedited download request addressing a larger object
*
* ####      Test Checks
*           1. Check, that SDO server aborts the request
*           2. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpWr_LenTooLow)
{
    CO_NODE  node;
    uint16_t idx  = 0x2510;
    uint8_t  sub  = 2;
    uint16_t val  = 0;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TUNSIGNED16, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x2F, idx, sub, 0x24232221);

    /* -- CHECK -- */
    CHK_SDO0_ERR(idx, sub, 0x06070013);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Response to a SDO expedited download request addressing a write-only object
*
* \details  This test will check, that the Abort code "Data cannot be transferred or stored to
*           the application." (abort code 0x08000020) is sent to a SDO request addressing an
*           object with a miss-configured data reference.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a miss-configured object (data reference is NULL)
*
* ####      Test Steps
*           1. Send SDO expedited download request addressing the miss-configured object
*
* ####      Test Checks
*           1. Check, that SDO server aborts the request
*           2. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpWr_DataNullPtr)
{
    CO_NODE  node;
    uint16_t idx  = 0x2510;
    uint8_t  sub  = 7;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(0));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x23, idx, sub, 0x14131211);

    /* -- CHECK -- */
    CHK_SDO0_ERR(idx, sub, 0x08000020);

    CHK_NO_ERR(&node);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Try to write with an error in type function to an object dictionary
*
* \details  This test checks, that the type function write() can abort with a user defined code.
*
* ####      Test Preparation
*           1. Prepare object dictionary including a variable of type MY_TYPE.
*
* ####      Test Steps
*           1. Send SDO expedited download request initiating a type error
*
* ####      Test Checks
*           1. Check, that SDO server response is correct
*           2. Check, that value is not written in object dictionary
*           3. Check, that CANopen stack executes this error free
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_ExpWr_UserWriteErr)
{
    CO_NODE  node;
    uint16_t idx  = 0x2500;
    uint8_t  sub  = 1;
    uint8_t  val  = 0;

    /* -- PREPARATION -- */
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(idx, sub, CO_OBJ_____RW), MY_TYPE, (CO_DATA)(&val));
    TS_CreateNode(&node,0);

    /* -- TEST -- */
    TS_SDO_SEND (0x2F, idx, sub, 0);

    /* -- CHECK -- */
    CHK_SDO0_ERR(idx, sub, 0x11223344);

    TS_ASSERT(0 == val);

    CHK_NO_ERR(&node);
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_EXP_DOWN()
{
    TS_Begin(__FILE__);

//    CanDiagnosticOn(0);

    TS_RUNNER(TS_ExpWr_1BytePara);
    TS_RUNNER(TS_ExpWr_2BytePara);
    TS_RUNNER(TS_ExpWr_4BytePara);
    TS_RUNNER(TS_ExpWr_1ByteVar);
    TS_RUNNER(TS_ExpWr_2ByteVar);
    TS_RUNNER(TS_ExpWr_4ByteVar);
    TS_RUNNER(TS_ExpWr_2ByteVar_NoLen);

    TS_RUNNER(TS_ExpWr_BadCmd);
    TS_RUNNER(TS_ExpWr_ObjNotExist);
    TS_RUNNER(TS_ExpWr_SubIdxNotExist);
    TS_RUNNER(TS_ExpWr_ReadOnly);
    TS_RUNNER(TS_ExpWr_LenTooHigh);
    TS_RUNNER(TS_ExpWr_LenTooLow);
    TS_RUNNER(TS_ExpWr_DataNullPtr);

    TS_RUNNER(TS_ExpWr_UserWriteErr);

//    CanDiagnosticOff(0);

    TS_End();
}

/*! @} */
