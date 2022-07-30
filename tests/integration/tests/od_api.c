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

#define uint8_t_I_RO_KEY   0x2010
#define uint16_t_I_RO_KEY  0x2011
#define uint32_t_I_RO_KEY  0x2012
#define uint8_t_I_RW_KEY   0x2013
#define uint16_t_I_RW_KEY  0x2014
#define uint32_t_I_RW_KEY  0x2015
#define uint8_t_D_RW_KEY   0x2016
#define uint16_t_D_RW_KEY  0x2017
#define uint32_t_D_RW_KEY  0x2018

#define uint8_t_D_VALUE   0x34
#define uint16_t_D_VALUE  0x5678
#define uint32_t_D_VALUE  0x90ABCDEF

const uint8_t uint8_t_ro_indirect = 0x12;
const uint16_t uint16_t_ro_indirect = 0x3456;
const uint32_t uint32_t_ro_indirect = 0x7890abcd;
uint8_t uint8_t_rw_indirect = 0x43;
uint16_t uint16_t_rw_indirect = 0x8765;
uint32_t uint32_t_rw_indirect = 0xfedcba09;

#define MK_RO_KEY(type, typeCode) CO_KEY(type ## _I_RO_KEY, 0, CO_OBJ_____R_), typeCode, (CO_DATA)(&type ## _ro_indirect)
#define MK_RW_KEY(type, typeCode) CO_KEY(type ## _I_RW_KEY, 0, CO_OBJ_____RW), typeCode, (CO_DATA)(&type ## _rw_indirect)
#define MK_RW_D_KEY(type, typeCode) CO_KEY(type ## _D_RW_KEY, 0, CO_OBJ_D___RW), typeCode, (CO_DATA)(type ## _D_VALUE)

#define TS_READ_RO_GET(type, getter) CO_NODE node; \
    int16_t result; type toRead; setup(&node); \
    result = getter(&node.Dict, CO_DEV(type ## _I_RO_KEY, 0), &toRead); \
    TS_ASSERT(CO_ERR_NONE == result); TS_ASSERT(toRead == type ## _ro_indirect);

#define TS_READ_RW_GET(type, getter) CO_NODE node; \
    int16_t result; type toRead; setup(&node); \
    result = getter(&node.Dict, CO_DEV(type ## _I_RW_KEY, 0), &toRead); \
    TS_ASSERT(CO_ERR_NONE == result); TS_ASSERT(toRead == type ## _rw_indirect);

#define TS_D_READ(type, getter) CO_NODE node; \
    int16_t result; type toRead; setup(&node); \
    result = getter(&node.Dict, CO_DEV(type ## _D_RW_KEY, 0), &toRead); \
    TS_ASSERT(CO_ERR_NONE == result); TS_ASSERT(toRead == type ## _D_VALUE);

#define TS_WRITE_RW(type, setter, newValue) CO_NODE node; \
    int16_t result; type toWrite = newValue; setup(&node); \
    result = setter(&node.Dict, CO_DEV(type ## _I_RW_KEY, 0), toWrite); \
    TS_ASSERT(CO_ERR_NONE == result); TS_ASSERT(toWrite == type ## _rw_indirect);

#define TS_D_WRITE(type, setter, getter, newValue) CO_NODE node; \
    int16_t result; type toWrite = newValue; type checkValue; setup(&node); \
    result = setter(&node.Dict, CO_DEV(type ## _D_RW_KEY, 0), toWrite); \
    getter(&node.Dict, CO_DEV(type ## _D_RW_KEY, 0), &checkValue); \
    TS_ASSERT(CO_ERR_NONE == result); TS_ASSERT(toWrite == checkValue);

#define TS_WRITE_RO(type, setter, newValue) CO_NODE node; \
    int16_t result; type toWrite = newValue; setup(&node); \
    result = setter(&node.Dict, CO_DEV(type ## _I_RO_KEY, 0), toWrite); \
    TS_ASSERT(CO_ERR_NONE == result); TS_ASSERT(toWrite != type ## _ro_indirect);

void setup(CO_NODE *node) {
    TS_CreateMandatoryDir();
    TS_ODAdd(MK_RO_KEY(uint8_t, CO_TUNSIGNED8));
    TS_ODAdd(MK_RO_KEY(uint16_t, CO_TUNSIGNED16));
    TS_ODAdd(MK_RO_KEY(uint32_t, CO_TUNSIGNED32));
    TS_ODAdd(MK_RW_KEY(uint8_t, CO_TUNSIGNED8));
    TS_ODAdd(MK_RW_KEY(uint16_t, CO_TUNSIGNED16));
    TS_ODAdd(MK_RW_KEY(uint32_t, CO_TUNSIGNED32));
    TS_ODAdd(MK_RW_D_KEY(uint8_t, CO_TUNSIGNED8));
    TS_ODAdd(MK_RW_D_KEY(uint16_t, CO_TUNSIGNED16));
    TS_ODAdd(MK_RW_D_KEY(uint32_t, CO_TUNSIGNED32));
    TS_CreateNode(node, 0);
}


TS_DEF_MAIN(TS_uint8_t_ReadOnlyRead) {
    TS_READ_RO_GET(uint8_t, CODictRdByte)
}

TS_DEF_MAIN(TS_uint16_t_ReadOnlyRead) {
    TS_READ_RO_GET(uint16_t, CODictRdWord)
}

TS_DEF_MAIN(TS_uint32_t_ReadOnlyRead) {
    TS_READ_RO_GET(uint32_t, CODictRdLong)
}

TS_DEF_MAIN(TS_uint8_t_ReadWriteRead) {
    TS_READ_RW_GET(uint8_t, CODictRdByte)
}

TS_DEF_MAIN(TS_uint16_t_ReadWriteRead) {
    TS_READ_RW_GET(uint16_t, CODictRdWord)
}

TS_DEF_MAIN(TS_uint32_t_ReadWriteRead) {
    TS_READ_RW_GET(uint32_t, CODictRdLong)
}

TS_DEF_MAIN(TS_uint8_t_ReadWriteWrite) {
    TS_WRITE_RW(uint8_t, CODictWrByte, 0x1A)
}

TS_DEF_MAIN(TS_uint16_t_ReadWriteWrite) {
    TS_WRITE_RW(uint16_t, CODictWrWord, 0x1A2B)
}

TS_DEF_MAIN(TS_uint32_t_ReadWriteWrite) {
    TS_WRITE_RW(uint32_t, CODictWrLong, 0x1A2B3C4D)
}

// TS_DEF_MAIN(TS_uint8_t_ReadOnlyWrite) {
//     TS_WRITE_RO(uint8_t, CODictWrByte, 0x1A)
// }

// TS_DEF_MAIN(TS_uint16_t_ReadOnlyWrite) {
//     TS_WRITE_RO(uint16_t, CODictWrWord, 0x1A2B)
// }

// TS_DEF_MAIN(TS_uint32_t_ReadOnlyWrite) {
//     TS_WRITE_RO(uint32_t, CODictWrLong, 0x1A2B3C4D)
// }

TS_DEF_MAIN(TS_uint8_t_DirectRWRead) {
    TS_D_READ(uint8_t, CODictRdByte)
}

TS_DEF_MAIN(TS_uint16_t_DirectRWRead) {
    TS_D_READ(uint16_t, CODictRdWord)
}

TS_DEF_MAIN(TS_uint32_t_DirectRWRead) {
    TS_D_READ(uint32_t, CODictRdLong)
}

TS_DEF_MAIN(TS_uint8_t_DirectRWWrite) {
    TS_D_WRITE(uint8_t, CODictWrByte, CODictRdByte, 0x1A)
}

TS_DEF_MAIN(TS_uint16_t_DirectRWWrite) {
    TS_D_WRITE(uint16_t, CODictWrWord, CODictRdWord, 0x1A2B)
}

TS_DEF_MAIN(TS_uint32_t_DirectRWWrite) {
    TS_D_WRITE(uint32_t, CODictWrLong, CODictRdLong, 0x1A2B3C4D)
}



TS_DEF_MAIN(TS_OD_GetDirect)
{
    int16_t     result;
    CO_NODE     node;
    uint32_t    val;
    uint8_t    valByte;
    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x2000, 0, CO_OBJ_D___R_), CO_TUNSIGNED32, (CO_DATA)(123));
    TS_CreateNode(&node, 0);

    // result = CODictWrLong(&node.Dict, CO_DEV(0x1400,1), 0xC0000201);
    result = CODictRdByte(&node.Dict, CO_DEV(0x1018,0), &valByte);
    TS_ASSERT(CO_ERR_NONE == result);
    TS_ASSERT(valByte == 4);

    result = CODictRdLong(&node.Dict, CO_DEV(0x2000,0), &val);
    TS_ASSERT(CO_ERR_NONE == result);
    TS_ASSERT(val == 123);

    CHK_NO_ERR(&node); /* check error free stack execution         */
}

const uint8_t DemoString[] = "TestData\0";
CO_OBJ_STR DemoStringObj = {
    (uint32_t) 0,              /* variable for read position     */
    (uint8_t *)&DemoString[0]  /* start address of string memory */
 };

// TS_DEF_MAIN(TS_OD_GetStringReadOnly)
// {
//     int16_t     result;
//     CO_NODE     node;
//     uint8_t     buf[32];
//     uint32_t    size;
//     CO_OBJ      *obj;

//     TS_CreateMandatoryDir();
//     TS_ODAdd(CO_KEY(0x2001, 0, CO_OBJ_____R_), CO_TSTRING, (CO_DATA)(&DemoStringObj));
//     TS_CreateNode(&node, 0);
//     obj = CODictFind(&node.Dict, CO_DEV(0x2001, 0));
//     if (obj != NULL) {
//         size = COObjGetSize(obj, &node, (uint32_t)0);
//     }
//     result = CODictRdBuffer(&node.Dict, CO_DEV(0x2001,0), &buf[0], 32);
//     buf[size]=0;
//     TS_ASSERT(strcmp(buf, DemoString) == 0);
//     CHK_NO_ERR(&node); /* check error free stack execution         */
// }

TS_DEF_MAIN(TS_OD_GetIndirect)
{
    int16_t     result;
    uint16_t    val = 42, toRead; 
    CO_NODE     node;

    TS_CreateMandatoryDir();
    TS_ODAdd(CO_KEY(0x2002, 0, CO_OBJ_____RW), CO_TUNSIGNED16, (CO_DATA)(&val));
    TS_CreateNode(&node, 0);

    result = CODictRdWord(&node.Dict, CO_DEV(0x2002,0), &toRead);

    TS_ASSERT(result == CO_ERR_NONE);
    TS_ASSERT(val == toRead);

    result = CODictWrWord(&node.Dict, CO_DEV(0x2002,0), 123);
    TS_ASSERT(result == CO_ERR_NONE);
    TS_ASSERT(val == 123);

    CHK_NO_ERR(&node); /* check error free stack execution         */
}

SUITE_OD_API()
{
    TS_Begin(__FILE__);
    TS_RUNNER(TS_uint8_t_ReadOnlyRead);
    TS_RUNNER(TS_uint16_t_ReadOnlyRead);
    TS_RUNNER(TS_uint32_t_ReadOnlyRead);
    TS_RUNNER(TS_uint8_t_ReadWriteRead);
    TS_RUNNER(TS_uint16_t_ReadWriteRead);
    TS_RUNNER(TS_uint32_t_ReadWriteRead);
    TS_RUNNER(TS_uint8_t_DirectRWRead);
    TS_RUNNER(TS_uint16_t_DirectRWRead);
    TS_RUNNER(TS_uint32_t_DirectRWRead);

    TS_RUNNER(TS_uint8_t_ReadWriteWrite);
    TS_RUNNER(TS_uint16_t_ReadWriteWrite);
    TS_RUNNER(TS_uint32_t_ReadWriteWrite);
    // Needs exception handling when writing to const memory
    // TS_RUNNER(TS_uint8_t_ReadOnlyWrite);
    // TS_RUNNER(TS_uint16_t_ReadOnlyWrite);
    // TS_RUNNER(TS_uint32_t_ReadOnlyWrite);
    TS_RUNNER(TS_uint8_t_DirectRWWrite);
    TS_RUNNER(TS_uint16_t_DirectRWWrite);
    TS_RUNNER(TS_uint32_t_DirectRWWrite);

    TS_RUNNER(TS_OD_GetDirect);
    TS_RUNNER(TS_OD_GetIndirect);
    // TS_RUNNER(TS_OD_GetStringReadOnly);
    TS_End();
}
