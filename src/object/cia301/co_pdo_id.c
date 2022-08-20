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
#define COT_OBJECT_RPDO   (uint32_t)0x1400
#define COT_OBJECT_TPDO   (uint32_t)0x1800
#define COT_OBJECT_NUM    (uint32_t)0x1FF
#define COT_OBJECT_SUB    (uint8_t)1

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/* type functions */
static uint32_t COTPdoIdSize (struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width);
static CO_ERR   COTPdoIdRead (struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTPdoIdWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTPdoIdInit (struct CO_OBJ_T *obj, struct CO_NODE_T *node);

/******************************************************************************
* PUBLIC GLOBALS
******************************************************************************/

const CO_OBJ_TYPE COTPdoId = { COTPdoIdSize, COTPdoIdInit, COTPdoIdRead, COTPdoIdWrite, 0 };

/******************************************************************************
* PRIVATE TYPE FUNCTIONS
******************************************************************************/

static uint32_t COTPdoIdSize(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    return uint32->Size(obj, node, width);
}

static CO_ERR COTPdoIdRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    return uint32->Read(obj, node, buffer, size);
}

static CO_ERR COTPdoIdWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    CO_ERR    result = CO_ERR_NONE;
    CO_NMT   *nmt;
    CO_TPDO  *tpdo = 0;
    CO_RPDO  *rpdo = 0;
    uint32_t  nid;
    uint32_t  oid;
    uint16_t  pcomidx;
    uint16_t  num;

    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(buffer, CO_ERR_BAD_ARG);
    ASSERT_EQU_ERR(size, 4u, CO_ERR_BAD_ARG);

    nid = *(uint32_t*)buffer;
    /* PDO with extended CAN identifiers is not supported */
    if ((nid & CO_TPDO_COBID_EXT) != 0) {
        return (CO_ERR_OBJ_RANGE);
    }

    nmt     = &node->Nmt;
    pcomidx = CO_GET_IDX(obj->Key);
    if (pcomidx <= COT_OBJECT_RPDO + COT_OBJECT_NUM) {
        rpdo = node->RPdo;
        num  = pcomidx & COT_OBJECT_NUM;
    } else {
        /* PDO with RTR allowed is not supported */
        if ((nid & CO_TPDO_COBID_REMOTE) == 0) {
            return (CO_ERR_OBJ_RANGE);
        }
        tpdo = node->TPdo;
        num  = pcomidx & COT_OBJECT_NUM;
    }

    (void)uint32->Read(obj, node, &oid, sizeof(oid));
    if ((oid & CO_TPDO_COBID_OFF) == 0) {
        if ((nid & CO_TPDO_COBID_OFF) != 0) {
            result = uint32->Write(obj, node, &nid, sizeof(nid));
            if (nmt->Mode == CO_OPERATIONAL) {
                if (tpdo != 0) {
                    COTPdoReset(tpdo, num);
                }
                if (rpdo != 0) {
                    CORPdoReset(rpdo, num);
                }
            }
        } else {
            result = CO_ERR_OBJ_RANGE;
        }
    } else {
        if ((nid & CO_TPDO_COBID_OFF) != 0) {
            result = uint32->Write(obj, node, &nid, sizeof(nid));
        } else {
            result = uint32->Write(obj, node, &nid, sizeof(nid));
            if (nmt->Mode == CO_OPERATIONAL) {
                if (tpdo != 0) {
                    COTPdoReset(tpdo, num);
                }
                if (rpdo != 0) {
                    CORPdoReset(rpdo, num);
                }
            }
        }
    }
    return (result);
}

static CO_ERR COTPdoIdInit(struct CO_OBJ_T *obj, struct CO_NODE_T *node)
{
    CO_ERR result = CO_ERR_TYPE_INIT;

    CO_UNUSED(node);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);

    if ((CO_GET_IDX(obj->Key) >= COT_OBJECT_RPDO) &&
        (CO_GET_IDX(obj->Key) <= COT_OBJECT_RPDO + COT_OBJECT_NUM)) {
        if (CO_GET_SUB(obj->Key) == COT_OBJECT_SUB) {
            result = CO_ERR_NONE;
        }
    } else if ((CO_GET_IDX(obj->Key) >= COT_OBJECT_TPDO) &&
               (CO_GET_IDX(obj->Key) <= COT_OBJECT_TPDO + COT_OBJECT_NUM)) {
        if (CO_GET_SUB(obj->Key) == COT_OBJECT_SUB) {
            result = CO_ERR_NONE;
        }
    }
    return (result);
}
