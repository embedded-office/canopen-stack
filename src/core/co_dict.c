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

#include "co_dict.h"

#include "co_obj.h"
#include "co_core.h"

/******************************************************************************
* PUBLIC API FUNCTIONS
******************************************************************************/

CO_OBJ *CODictFind(CO_DICT *cod, uint32_t key)
{
    CO_OBJ  *result = NULL;
    CO_OBJ  *obj;
    uint32_t pattern;
    int32_t  start = 0;
    int32_t  end;
    int32_t  center;

    ASSERT_PTR_ERR(cod, NULL);
    ASSERT_NOT_ERR(key, 0, NULL);
    ASSERT_PTR_ERR(cod->Root, NULL);

    pattern = CO_GET_DEV(key);
    end = cod->Num;
    while (start <= end) {
        center = start + ((end - start) / 2);
        obj    = &(cod->Root[center]);
        if (CO_GET_DEV(obj->Key) == pattern) {
            result = obj;
            break;
        }
        if (CO_GET_DEV(obj->Key) > pattern) {
            end    = center - 1;
        } else {
            start  = center + 1;
        }
    }
    return(result);
}

CO_ERR CODictRdByte(CO_DICT *cod, uint32_t key, uint8_t *val)
{
    CO_ERR   result = CO_ERR_OBJ_NOT_FOUND;
    uint32_t sz;
    CO_OBJ  *obj;

    ASSERT_PTR_ERR(cod, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(val, CO_ERR_BAD_ARG);

    obj = CODictFind(cod, key);
    if (obj != NULL) {
        sz = COObjGetSize(obj, cod->Node, (uint32_t)1u);
        if (sz != (uint32_t)1u) {
            result = CO_ERR_OBJ_SIZE;
        } else {
            result = COObjRdValue(obj, cod->Node, (void *)val, 1u);
        }
    }
    return(result);
}

CO_ERR CODictRdWord(CO_DICT *cod, uint32_t key, uint16_t *val)
{
    CO_ERR   result = CO_ERR_OBJ_NOT_FOUND;
    uint32_t sz;
    CO_OBJ  *obj;

    ASSERT_PTR_ERR(cod, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(val, CO_ERR_BAD_ARG);

    obj = CODictFind(cod, key);
    if (obj != NULL) {
        sz = COObjGetSize(obj, cod->Node, (uint32_t)2u);
        if (sz != (uint32_t)2u) {
            result = CO_ERR_OBJ_SIZE;
        } else {
            result = COObjRdValue(obj, cod->Node, (void *)val, 2u);
        }
    }
    return(result);
}

CO_ERR CODictRdLong(CO_DICT *cod, uint32_t key, uint32_t *val)
{
    uint32_t sz;
    CO_ERR   result = CO_ERR_OBJ_NOT_FOUND;
    CO_OBJ  *obj;

    ASSERT_PTR_ERR(cod, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(val, CO_ERR_BAD_ARG);

    obj = CODictFind(cod, key);
    if (obj != NULL) {
        sz = COObjGetSize(obj, cod->Node, (uint32_t)4u);
        if (sz != (uint32_t)4u) {
            result = CO_ERR_OBJ_SIZE;
        } else {
            result = COObjRdValue(obj, cod->Node, (void *)val, 4u);
        }
    }
    return(result);
}

CO_ERR CODictWrByte(CO_DICT *cod, uint32_t key, uint8_t val)
{
    CO_ERR   result = CO_ERR_OBJ_NOT_FOUND;
    uint32_t sz;
    CO_OBJ  *obj;

    ASSERT_PTR_ERR(cod, CO_ERR_BAD_ARG);

    obj = CODictFind(cod, key);
    if (obj != NULL) {
        sz = COObjGetSize(obj, cod->Node, (uint32_t)1u);
        if (sz != (uint32_t)1u) {
            result = CO_ERR_OBJ_SIZE;
        } else {
            result = COObjWrValue(obj, cod->Node, (void *)&val, 1u);
        }
    }
    return(result);
}

CO_ERR CODictWrWord(CO_DICT *cod, uint32_t key, uint16_t val)
{
    CO_ERR   result = CO_ERR_OBJ_NOT_FOUND;
    uint32_t sz;
    CO_OBJ  *obj;

    ASSERT_PTR_ERR(cod, CO_ERR_BAD_ARG);

    obj = CODictFind(cod, key);
    if (obj != NULL) {
        sz = COObjGetSize(obj, cod->Node, (uint32_t)2u);
        if (sz != (uint32_t)2u) {
            result = CO_ERR_OBJ_SIZE;
        } else {
            result = COObjWrValue(obj, cod->Node, (void *)&val, 2u);
        }
    }
    return(result);
}

CO_ERR CODictWrLong(CO_DICT *cod, uint32_t key, uint32_t val)
{
    CO_ERR   result = CO_ERR_OBJ_NOT_FOUND;
    uint32_t sz;
    CO_OBJ  *obj;

    ASSERT_PTR_ERR(cod, CO_ERR_BAD_ARG);

    obj = CODictFind(cod, key);
    if (obj != NULL) {
        sz = COObjGetSize(obj, cod->Node, (uint32_t)4u);
        if (sz != (uint32_t)4u) {
            result = CO_ERR_OBJ_SIZE;
        } else {
            result = COObjWrValue(obj, cod->Node, (void *)&val, 4u);
        }
    }
    return(result);
}

CO_ERR CODictRdBuffer(CO_DICT *cod, uint32_t key, uint8_t *buf, uint32_t len)
{
    CO_ERR   result = CO_ERR_OBJ_NOT_FOUND;
    CO_OBJ  *obj;

    ASSERT_PTR_ERR(cod, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(buf, CO_ERR_BAD_ARG);

    obj = CODictFind(cod, key);
    if (obj != NULL) {
        result = COObjRdBufStart(obj, cod->Node, (void *)buf, (uint8_t)len);
    }
    return(result);
}

CO_ERR CODictWrBuffer(CO_DICT *cod, uint32_t key, uint8_t *buf, uint32_t len)
{
    CO_ERR   result = CO_ERR_OBJ_NOT_FOUND;
    CO_OBJ  *obj;

    ASSERT_PTR_ERR(cod, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(buf, CO_ERR_BAD_ARG);

    obj = CODictFind(cod, key);
    if (obj != NULL) {
        result = COObjWrBufStart(obj, cod->Node, (void *)buf, (uint8_t)len);
    }
    return(result);
}

int16_t CODictInit(CO_DICT *cod, CO_NODE *node, CO_OBJ *root, uint16_t max)
{
    CO_OBJ   *obj;
    uint16_t  num = 0;

    ASSERT_PTR_ERR(cod,  -1);
    ASSERT_PTR_ERR(node, -1);
    ASSERT_PTR_ERR(root, -1);
    ASSERT_NOT_ERR(max, (uint16_t)0, -1);

    obj = root;
    while ((obj->Key != 0) && (num < max)) {
        num++;
        obj++;
    }
    cod->Root  = root;
    cod->Num   = num;
    cod->Max   = max;
    cod->Node  = node;
    return ((int16_t)num);
}

CO_ERR CODictObjInit(CO_DICT *cod, CO_NODE *node)
{
    CO_ERR    result = CO_ERR_NONE;
    CO_ERR    err;
    CO_OBJ   *obj;

    ASSERT_PTR_ERR(cod,  -1);
    ASSERT_PTR_ERR(node, -1);

    obj = cod->Root;
    while (obj->Key != 0) {
        obj++;
        if (obj->Key != 0) {
            err = COObjInit(obj, node);
            if (err != CO_ERR_NONE) {
                result = err;
            }
        }
    }
    return (result);
}
