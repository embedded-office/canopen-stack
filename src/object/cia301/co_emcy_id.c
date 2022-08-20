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
#define COT_OBJECT        (uint16_t)0x1014
#define COT_COBID_MIN     (uint32_t)0x00000080

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/* type functions */
static uint32_t COTEmcyIdSize (struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width);
static CO_ERR   COTEmcyIdRead (struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTEmcyIdWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
static CO_ERR   COTEmcyIdInit (struct CO_OBJ_T *obj, struct CO_NODE_T *node);

/******************************************************************************
* PUBLIC GLOBALS
******************************************************************************/

const CO_OBJ_TYPE COTEmcyId = { COTEmcyIdSize, COTEmcyIdInit, COTEmcyIdRead, COTEmcyIdWrite, 0 };

/******************************************************************************
* PRIVATE TYPE FUNCTIONS
******************************************************************************/

static uint32_t COTEmcyIdSize(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    return uint32->Size(obj, node, width);
}

static CO_ERR COTEmcyIdRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    return uint32->Read(obj, node, buffer, size);
}

static CO_ERR COTEmcyIdWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *uint32 = CO_TUNSIGNED32;
    CO_ERR    result = CO_ERR_NONE;
    uint32_t  newId;
    uint32_t  oldId;

    ASSERT_EQU_ERR(size, COT_ENTRY_SIZE, CO_ERR_BAD_ARG);

    newId = *(uint32_t*)buffer;
    (void)uint32->Read(obj, node, &oldId, sizeof(oldId));

    /* when current entry is active, bits 0 to 29 shall not be changed */
    if ((oldId & CO_EMCY_COBID_OFF) == (uint32_t)0) {
        if ((newId & CO_EMCY_COBID_MASK) != (oldId & CO_EMCY_COBID_MASK)) {
            result = CO_ERR_OBJ_RANGE;
        } else {
            result = uint32->Write(obj, node, &newId, sizeof(newId));
        }
    } else {
        /* Conformance Test case : 2.11 - Test name : SDO 11
         * (note: can't find the requirement in DS301?)
         *
         * SDO 11: Use object 1014h:00h with min value 81h
         * SDO 11: Valid abort codes are 06090030h, 06090032h or 08000000h
         */
        if (newId >= COT_COBID_MIN) {
            result = uint32->Write(obj, node, &newId, sizeof(newId));
        } else {
            result = CO_ERR_OBJ_RANGE;
        }
    }
    return (result);
}

static CO_ERR COTEmcyIdInit (struct CO_OBJ_T *obj, struct CO_NODE_T *node)
{
    CO_ERR result = CO_ERR_TYPE_INIT;

    CO_UNUSED(node);
    ASSERT_PTR_ERR(obj, CO_ERR_BAD_ARG);

    /* check for emergency cob-id object */
    if (CO_DEV(COT_OBJECT, 0) == CO_GET_DEV(obj->Key)) {
        result = CO_ERR_NONE;
    }
    return(result);
}
