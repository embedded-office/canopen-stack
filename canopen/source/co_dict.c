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
* FUNCTIONS
******************************************************************************/

/*
* see function definition
*/
CO_OBJ *CODictFind(CO_DICT *cod, uint32_t key)
{
    CO_OBJ  *result = 0;
    CO_OBJ  *obj    = 0;
    int32_t  start  = 0;
    int32_t  end;
    int32_t  center;

    if (cod == 0) {
        cod->Node->Error = CO_ERR_BAD_ARG;
        return (result);
    }
    if (key == 0) {
        cod->Node->Error = CO_ERR_BAD_ARG;
        return (result);
    }

    key = CO_GET_DEV(key);
    end = cod->Num;
    while (start <= end) {
        center = start + ((end - start) / 2);
        obj    = &(cod->Root[center]);
        if (CO_GET_DEV(obj->Key) == key) {
            result = obj;
            break;
        }
        if (CO_GET_DEV(obj->Key) > key) {
            end    = center - 1;
        } else {
            start  = center + 1;
        }
    }

    if (result == 0) {
        cod->Node->Error = CO_ERR_OBJ_NOT_FOUND;
    }

    return(result);
}

/*
* see function definition
*/
CO_ERR CODictRdByte(CO_DICT *cod, uint32_t key, uint8_t *val)
{
    CO_ERR   result = CO_ERR_OBJ_NOT_FOUND;
    uint32_t sz;
    uint8_t  nodeid;
    CO_OBJ  *obj;

    if ((cod == 0) || (val == 0)) {
        return (CO_ERR_BAD_ARG);
    }

    obj = CODictFind(cod, key);
    if (obj != 0) {
        sz = COObjGetSize(obj, cod->Node, (uint32_t)CO_BYTE);
        if (sz != (uint32_t)CO_BYTE) {
            cod->Node->Error = CO_ERR_OBJ_SIZE;
            result           = CO_ERR_OBJ_SIZE;
        } else {
            nodeid = cod->Node->NodeId;
            result = COObjRdValue(obj, cod->Node, (void *)val, CO_BYTE, nodeid);
            if (result != CO_ERR_NONE) {
                cod->Node->Error = CO_ERR_OBJ_READ;
            }
        }
    }

    return(result);
}

/*
* see function definition
*/
CO_ERR CODictRdWord(CO_DICT *cod, uint32_t key, uint16_t *val)
{
    CO_ERR   result = CO_ERR_OBJ_NOT_FOUND;
    uint32_t sz;
    uint8_t  nodeid;
    CO_OBJ  *obj;

    if ((cod == 0) || (val == 0)) {
        return (CO_ERR_BAD_ARG);
    }

    obj = CODictFind(cod, key);
    if (obj != 0) {
        sz = COObjGetSize(obj, cod->Node, (uint32_t)CO_WORD);
        if (sz != (uint32_t)CO_WORD) {
            cod->Node->Error = CO_ERR_OBJ_SIZE;
            result           = CO_ERR_OBJ_SIZE;
        } else {
            nodeid = cod->Node->NodeId;
            result = COObjRdValue(obj, cod->Node, (void *)val, CO_WORD, nodeid);
            if (result != CO_ERR_NONE) {
                cod->Node->Error = CO_ERR_OBJ_READ;
            }
        }
    }

    return(result);
}

/*
* see function definition
*/
CO_ERR CODictRdLong(CO_DICT *cod, uint32_t key, uint32_t *val)
{
    uint32_t sz;
    CO_ERR   result = CO_ERR_OBJ_NOT_FOUND;
    uint8_t  nodeid;
    CO_OBJ  *obj;

    if ((cod == 0) || (val == 0)) {
        return (CO_ERR_BAD_ARG);
    }
    obj = CODictFind(cod, key);
    if (obj != 0) {
        sz = COObjGetSize(obj, cod->Node, (uint32_t)CO_LONG);
        if (sz != (uint32_t)CO_LONG) {
            cod->Node->Error = CO_ERR_OBJ_SIZE;
            result           = CO_ERR_OBJ_SIZE;
        } else {
            nodeid = cod->Node->NodeId;
            result = COObjRdValue(obj, cod->Node, (void *)val, CO_LONG, nodeid);
            if (result != CO_ERR_NONE) {
                cod->Node->Error = CO_ERR_OBJ_READ;
            }
        }
    }

    return(result);
}

/*
* see function definition
*/
CO_ERR CODictWrByte(CO_DICT *cod, uint32_t key, uint8_t val)
{
    CO_ERR   result = CO_ERR_OBJ_NOT_FOUND;
    uint32_t sz;
    uint8_t  nodeid;
    CO_OBJ  *obj;

    if (cod == 0) {
        return (CO_ERR_BAD_ARG);
    }
    obj = CODictFind(cod, key);
    if (obj != 0) {
        sz = COObjGetSize(obj, cod->Node, (uint32_t)CO_BYTE);
        if (sz != (uint32_t)CO_BYTE) {
            cod->Node->Error = CO_ERR_OBJ_SIZE;
            result           = CO_ERR_OBJ_SIZE;
        } else {
            nodeid = cod->Node->NodeId;
            result = COObjWrValue(obj, cod->Node, (void *)&val, CO_BYTE, nodeid);
            if (result != CO_ERR_NONE) {
                cod->Node->Error = CO_ERR_OBJ_WRITE;
            }
        }
    }

    return(result);
}

/*
* see function definition
*/
CO_ERR CODictWrWord(CO_DICT *cod, uint32_t key, uint16_t val)
{
    CO_ERR   result = CO_ERR_OBJ_NOT_FOUND;
    uint32_t sz;
    uint8_t  nodeid;
    CO_OBJ  *obj;

    if (cod == 0) {
        return (CO_ERR_BAD_ARG);
    }

    obj = CODictFind(cod, key);
    if (obj != 0) {
        sz = COObjGetSize(obj, cod->Node, (uint32_t)CO_WORD);
        if (sz != (uint32_t)CO_WORD) {
            cod->Node->Error = CO_ERR_OBJ_SIZE;
            result           = CO_ERR_OBJ_SIZE;
        } else {
            nodeid = cod->Node->NodeId;
            result = COObjWrValue(obj, cod->Node, (void *)&val, CO_WORD, nodeid);
            if (result != CO_ERR_NONE) {
                cod->Node->Error = CO_ERR_OBJ_WRITE;
            }
        }
    }

    return(result);
}

/*
* see function definition
*/
CO_ERR CODictWrLong(CO_DICT *cod, uint32_t key, uint32_t val)
{
    CO_ERR   result = CO_ERR_OBJ_NOT_FOUND;
    uint32_t sz;
    uint8_t  nodeid;
    CO_OBJ  *obj;

    if (cod == 0) {
        return (CO_ERR_BAD_ARG);
    }

    obj = CODictFind(cod, key);
    if (obj != 0) {
        sz = COObjGetSize(obj, cod->Node, (uint32_t)CO_LONG);
        if (sz != (uint32_t)CO_LONG) {
            cod->Node->Error = CO_ERR_OBJ_SIZE;
            result           = CO_ERR_OBJ_SIZE;
        } else {
            nodeid = cod->Node->NodeId;
            result = COObjWrValue(obj, cod->Node, (void *)&val, CO_LONG, nodeid);
            if (result != CO_ERR_NONE) {
                cod->Node->Error = CO_ERR_OBJ_WRITE;
            }
        }
    }

    return(result);
}

/*
* see function definition
*/
CO_ERR CODictRdBuffer(CO_DICT *cod, uint32_t key, uint8_t *buf, uint32_t len)
{
    CO_ERR   result = CO_ERR_OBJ_NOT_FOUND;
    CO_OBJ  *obj;

    if ((cod == 0) || (buf == 0)) {
        return (CO_ERR_BAD_ARG);
    }

    obj = CODictFind(cod, key);
    if (obj != 0) {
        result = COObjRdBufStart(obj, cod->Node, (void *)buf, (uint8_t)len);
        if (result != CO_ERR_NONE) {
            cod->Node->Error = CO_ERR_OBJ_READ;
        }
    }

    return(result);
}

CO_ERR CODictWrBuffer(CO_DICT *cod, uint32_t key, uint8_t *buf, uint32_t len)
{
    CO_ERR   result = CO_ERR_OBJ_NOT_FOUND;
    CO_OBJ  *obj;

    if ((cod == 0) || (buf == 0)) {
        return (CO_ERR_BAD_ARG);
    }

    obj = CODictFind(cod, key);
    if (obj != 0) {
        result = COObjWrBufStart(obj, cod->Node, (void *)buf, (uint8_t)len);
        if (result != CO_ERR_NONE) {
            cod->Node->Error = CO_ERR_OBJ_WRITE;
        }
    }

    return(result);
}

/*
* see function definition
*/
int16_t CODictInit(CO_DICT *cod, CO_NODE *node, CO_OBJ *root, uint16_t max)
{
    CO_OBJ   *obj;
    uint16_t  num = 0;

    if ((cod == 0) || (node == 0) || (root == 0)) {
        CONodeFatalError();
        return (-1);
    }
    if (max == 0) {
        node->Error = CO_ERR_BAD_ARG;
        return (-1);
    }
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
