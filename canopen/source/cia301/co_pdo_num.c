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
#define COT_OBJECT_RPDO   (uint32_t)0x1600
#define COT_OBJECT_TPDO   (uint32_t)0x1A00
#define COT_OBJECT_NUM    (uint32_t)0x1FF

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/* type functions */
static uint32_t COTPdoMapNSize (struct CO_OBJ_T *, struct CO_NODE_T *, uint32_t);
static CO_ERR   COTPdoMapNRead (struct CO_OBJ_T *, struct CO_NODE_T *, void*, uint32_t);
static CO_ERR   COTPdoMapNWrite(struct CO_OBJ_T *, struct CO_NODE_T *, void*, uint32_t);

/******************************************************************************
* PUBLIC GLOBALS
******************************************************************************/

const CO_OBJ_TYPE COTPdoMapN = { COTPdoMapNSize, 0, COTPdoMapNRead, COTPdoMapNWrite, 0 };

/******************************************************************************
* PRIVATE TYPE FUNCTIONS
******************************************************************************/

static uint32_t COTPdoMapNSize(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    const CO_OBJ_TYPE *uint8 = CO_TUNSIGNED8;
    return uint8->Size(obj, node, width);
}

static CO_ERR COTPdoMapNRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *val, uint32_t len)
{
    const CO_OBJ_TYPE *uint8 = CO_TUNSIGNED8;
    return uint8->Read(obj, node, val, len);
}

static CO_ERR COTPdoMapNWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *val, uint32_t size)
{
    const CO_OBJ_TYPE *uint8 = CO_TUNSIGNED8;
    CO_ERR    result = CO_ERR_NONE;
    CO_DICT  *cod;
    uint32_t  id;
    uint32_t  mapentry;
    uint16_t  pmapidx;
    uint16_t  pcomidx;
    uint8_t   mapbytes;
    uint8_t   mapnum;
    uint8_t   i;

    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(val, CO_ERR_BAD_ARG);
    ASSERT_EQU_ERR(size, 1u, CO_ERR_BAD_ARG);

    if (CO_GET_SUB(obj->Key) != 0) {
        return (CO_ERR_TPDO_MAP_OBJ);
    }
    mapnum = (uint8_t)(*(uint8_t *)val);
    if (mapnum > 8) {
        return (CO_ERR_OBJ_MAP_LEN);
    }

    cod     = &node->Dict;
    pmapidx = CO_GET_IDX(obj->Key);
    if ((pmapidx >= COT_OBJECT_RPDO) && (pmapidx <= COT_OBJECT_RPDO + COT_OBJECT_NUM)) {
    } else if ((pmapidx >= COT_OBJECT_TPDO) && (pmapidx <= COT_OBJECT_TPDO + COT_OBJECT_NUM)) {
    } else {
        return (CO_ERR_TPDO_MAP_OBJ);
    }

    pcomidx = pmapidx - 0x200;
    (void)CODictRdLong(cod, CO_DEV(pcomidx, 1), &id);
    if ((id & CO_TPDO_COBID_OFF) == 0) {
        result = CO_ERR_OBJ_ACC;
    } else {
        mapbytes = 0;
        for (i = 1; i <= mapnum; i++) {
            result = CODictRdLong(cod, CO_DEV(pmapidx, i), &mapentry);
            if (result != CO_ERR_NONE) {
                return (CO_ERR_OBJ_MAP_TYPE);
            }
            mapbytes += ((uint8_t)mapentry) >> 3u;
        }

        if (mapbytes > 8) {
            return (CO_ERR_OBJ_MAP_LEN);
        }

        result = uint8->Write(obj, node, &mapnum, sizeof(mapnum));
    }
    return (result);
}
