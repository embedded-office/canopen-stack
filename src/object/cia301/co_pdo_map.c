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

#define COT_ENTRY_SIZE    (uint32_t)4
#define COT_OBJECT_RPDO   (uint32_t)0x1600
#define COT_OBJECT_TPDO   (uint32_t)0x1A00
#define COT_OBJECT_NUM    (uint32_t)0x1FF

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/* type functions */
static uint32_t COTPdoMapSize (struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width);
static CO_ERR   COTPdoMapRead (struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTPdoMapWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTPdoMapInit (struct CO_OBJ_T *obj, struct CO_NODE_T *node);

/******************************************************************************
* PUBLIC GLOBALS
******************************************************************************/

const CO_OBJ_TYPE COTPdoMap = { COTPdoMapSize, COTPdoMapInit, COTPdoMapRead, COTPdoMapWrite, 0 };

/******************************************************************************
* PRIVATE TYPE FUNCTIONS
******************************************************************************/

static uint32_t COTPdoMapSize(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    return uint32->Size(obj, node, width);
}

static CO_ERR COTPdoMapRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    return uint32->Read(obj, node, buffer, size);
}

static CO_ERR COTPdoMapWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    CO_ERR    result = CO_ERR_NONE;
    CO_DICT  *cod;
    CO_OBJ   *objm;
    uint32_t  map;
    uint32_t  id;
    uint32_t  maps;
    uint16_t  pmapidx;
    uint16_t  pcomidx;
    uint8_t   mapn;

    CO_UNUSED(size);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(buffer, CO_ERR_BAD_ARG);

    cod     = &node->Dict;
    pmapidx = CO_GET_IDX(obj->Key);
    pcomidx = pmapidx - 0x200;
    map     = *(uint32_t*)buffer;

    /* check that PDO is inactive */
    (void)CODictRdLong(cod, CO_DEV(pcomidx, 1), &id);
    if ((id & CO_TPDO_COBID_OFF) == 0) {
        return (CO_ERR_OBJ_ACC);
    }

    /* check number of PDO mapping entries */
    (void)CODictRdByte(cod, CO_DEV(pmapidx, 0), &mapn);
    if (mapn != 0) {
        return (CO_ERR_OBJ_ACC);
    }

    /* check that linked object exists */
    objm = CODictFind(cod, map);
    if (objm == (CO_OBJ*)0) {
        return (CO_ERR_OBJ_MAP_TYPE);
    }

    /* check that linked object is PDO mappable */
    maps = CO_IS_PDOMAP(objm->Key);
    if (maps == 0) {
        return (CO_ERR_OBJ_MAP_TYPE);
    }

    /* check CAN access permissions for linked object */
    if ((pmapidx >= COT_OBJECT_RPDO) && (pmapidx <= COT_OBJECT_RPDO + COT_OBJECT_NUM)) {
        maps = CO_IS_WRITE(objm->Key);
        if (maps == 0) {
            return (CO_ERR_OBJ_MAP_TYPE);
        }
    } else if ((pmapidx >= COT_OBJECT_TPDO) && (pmapidx <= COT_OBJECT_TPDO + COT_OBJECT_NUM)) {
        maps = CO_IS_READ(objm->Key);
        if (maps == 0) {
            return (CO_ERR_OBJ_MAP_TYPE);
        }
    }

    /* ok, write new PDO mapping */
    result = uint32->Write(obj, node, &map, sizeof(map));
    return (result);
}

static CO_ERR COTPdoMapInit(struct CO_OBJ_T *obj, struct CO_NODE_T *node)
{
    CO_ERR result = CO_ERR_TYPE_INIT;

    CO_UNUSED(node);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);

    if ((CO_GET_IDX(obj->Key) >= COT_OBJECT_RPDO) &&
        (CO_GET_IDX(obj->Key) <= COT_OBJECT_RPDO + COT_OBJECT_NUM)) {
        if (CO_GET_SUB(obj->Key) > 0) {
            result = CO_ERR_NONE;
        }
    } else if ((CO_GET_IDX(obj->Key) >= COT_OBJECT_TPDO) &&
               (CO_GET_IDX(obj->Key) <= COT_OBJECT_TPDO + COT_OBJECT_NUM)) {
        if (CO_GET_SUB(obj->Key) > 0) {
            result = CO_ERR_NONE;
        }
    }
    return (result);
}
