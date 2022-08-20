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
* PROTECTED API FUNCTIONS
******************************************************************************/

CO_ERR COObjInit (struct CO_OBJ_T *obj, struct CO_NODE_T *node)
{
    CO_ERR result = CO_ERR_NONE;      /* optional function: no default error */
    const CO_OBJ_TYPE *type;

    ASSERT_PTR_ERR(obj,       CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(obj->Type, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(node,      CO_ERR_BAD_ARG);

    type = obj->Type;
    if (type->Init != NULL) {
        result = type->Init(obj, node);
    }
    return (result);
}

CO_ERR COObjReset (struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t para)
{
    CO_ERR result = CO_ERR_NONE;      /* optional function: no default error */
    const CO_OBJ_TYPE *type;

    ASSERT_PTR_ERR(obj,       CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(obj->Type, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(node,      CO_ERR_BAD_ARG);

    type = obj->Type;
    if (type->Reset != NULL) {
        result = type->Reset(obj, node, para);
    }
    return (result);
}

CO_ERR COObjRdBufStart(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint8_t *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *type;
    CO_ERR result = CO_ERR_OBJ_ACC;

    ASSERT_PTR_ERR(obj,       CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(obj->Type, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(node,      CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(buffer,    CO_ERR_BAD_ARG);

    type = obj->Type;
    if (type->Read != NULL) {
        (void)COObjReset(obj, node, 0);
        result = type->Read(obj, node, (void *)buffer, size);
    }
    return (result);
}

CO_ERR COObjRdBufCont(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint8_t *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *type;
    CO_ERR result = CO_ERR_OBJ_ACC;

    ASSERT_PTR_ERR(obj,       CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(obj->Type, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(node,      CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(buffer,    CO_ERR_BAD_ARG);

    type = obj->Type;
    if (type->Read != NULL) {
        result = type->Read(obj, node, (void *)buffer, size);
    }
    return (result);
}

CO_ERR COObjWrBufStart(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint8_t *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *type;
    CO_ERR result = CO_ERR_OBJ_ACC;

    ASSERT_PTR_ERR(obj,       CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(obj->Type, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(node,      CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(buffer,    CO_ERR_BAD_ARG);

    type = obj->Type;
    if (type->Write != NULL) {
        (void)COObjReset(obj, node, 0);
        result = type->Write(obj, node, (void *)buffer, size);
    }
    return (result);
}

CO_ERR COObjWrBufCont(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint8_t *buffer, uint32_t size)
{
    const CO_OBJ_TYPE *type;
    CO_ERR result = CO_ERR_OBJ_ACC;

    ASSERT_PTR_ERR(obj,       CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(obj->Type, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(node,      CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(buffer,    CO_ERR_BAD_ARG);

    type = obj->Type;
    if (type->Write != NULL) {
        result = type->Write(obj, node, (void *)buffer, size);
    }
    return (result);
}

void COObjTypeUserSDOAbort(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t abort)
{
    uint8_t n;

    ASSERT_PTR(node);

    for (n = 0; n < CO_SSDO_N; n++) {
        if (node->Sdo[n].Obj == obj) {
            node->Sdo[n].Abort = abort;
            break;
        }
    }
}

/******************************************************************************
* PUBLIC API FUNCTIONS
******************************************************************************/

uint32_t COObjGetSize(struct CO_OBJ_T *obj, CO_NODE *node, uint32_t width)
{
    const CO_OBJ_TYPE *type;
    uint32_t result = 0;

    ASSERT_PTR_ERR(obj,       0);
    ASSERT_PTR_ERR(obj->Type, 0);
    ASSERT_PTR_ERR(node,      0);

    type = obj->Type;
    if (type->Size != NULL) {
        result = type->Size(obj, node, width);
    }
    return (result);
}

CO_ERR COObjRdValue(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *value, uint8_t width)
{
    const CO_OBJ_TYPE *type;
    CO_ERR result = CO_ERR_OBJ_ACC;

    ASSERT_PTR_ERR(obj,       CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(obj->Type, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(node,      CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(value,     CO_ERR_BAD_ARG);

    type = obj->Type;
    if (type->Read != NULL) {
        result = type->Read(obj, node, value, width);
    }
    return(result);
}

CO_ERR COObjWrValue(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *value, uint8_t width)
{
    const CO_OBJ_TYPE *type;
    CO_ERR result = CO_ERR_OBJ_ACC;

    ASSERT_PTR_ERR(obj,       CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(obj->Type, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(node,      CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(value,     CO_ERR_BAD_ARG);

    type = obj->Type;
    if (type->Write != NULL) {
        result = type->Write(obj, node, value, width);
    }
    return (result);
}
