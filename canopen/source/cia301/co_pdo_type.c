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

#include "co_core.h"

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

#define COT_ENTRY_SIZE    (uint32_t)1
#define COT_OBJECT_RPDO   (uint32_t)0x1400
#define COT_OBJECT_TPDO   (uint32_t)0x1800
#define COT_OBJECT_NUM    (uint32_t)0x1FF

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/* type functions */
static uint32_t COTPdoTypeSize (struct CO_OBJ_T *, struct CO_NODE_T *, uint32_t);
static CO_ERR   COTPdoTypeRead (struct CO_OBJ_T *, struct CO_NODE_T *, void*, uint32_t);
static CO_ERR   COTPdoTypeWrite(struct CO_OBJ_T *, struct CO_NODE_T *, void*, uint32_t);

/******************************************************************************
* PUBLIC GLOBALS
******************************************************************************/

const CO_OBJ_TYPE COTPdoType = { COTPdoTypeSize, 0, COTPdoTypeRead, COTPdoTypeWrite, 0 };

/******************************************************************************
* PRIVATE TYPE FUNCTIONS
******************************************************************************/

static uint32_t COTPdoTypeSize(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    const CO_OBJ_TYPE *uint8 = CO_TUNSIGNED8;
    return uint8->Size(obj, node, width);
}

static CO_ERR COTPdoTypeRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *val, uint32_t len)
{
    const CO_OBJ_TYPE *uint8 = CO_TUNSIGNED8;
    return uint8->Read(obj, node, val, len);
}

static CO_ERR COTPdoTypeWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *val, uint32_t size)
{
    const CO_OBJ_TYPE *uint8 = CO_TUNSIGNED8;
    CO_ERR    result = CO_ERR_NONE;
    CO_DICT  *cod;
    uint8_t   type;
    uint32_t  id;
    uint16_t  pcomidx;

    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(val, CO_ERR_BAD_ARG);
    ASSERT_EQU_ERR(size, 1u, CO_ERR_BAD_ARG);

    if (CO_GET_SUB(obj->Key) != 2u) {
        return (CO_ERR_PARA_IDX);
    }

    type    = *(uint8_t*)val;
    cod     = &node->Dict;
    pcomidx = CO_GET_IDX(obj->Key);
    if ((pcomidx >= COT_OBJECT_RPDO) && (pcomidx <= COT_OBJECT_RPDO + COT_OBJECT_NUM)) {
    } else if ((pcomidx >= COT_OBJECT_TPDO) && (pcomidx <= COT_OBJECT_TPDO + COT_OBJECT_NUM)) {
    } else {
        return (CO_ERR_PARA_IDX);
    }
    (void)CODictRdLong(cod, CO_DEV(pcomidx, 1u), &id);
    if ((id & CO_TPDO_COBID_OFF) == 0u) {
        result = CO_ERR_OBJ_RANGE;
    } else {
        result = uint8->Write(obj, node, &type, sizeof(type));
    }
    return (result);
}
