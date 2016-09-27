/******************************************************************************
* (c) by Embedded Office GmbH & Co. KG, http://www.embedded-office.com
*------------------------------------------------------------------------------
* This file is part of CANopenStack, an open source CANopen Stack.
* Project home page is <https://github.com/MichaelHillmann/CANopenStack.git>.
* For more information on CANopen see < http ://www.can-cia.org/>.
*
* CANopenStack is free and open source software: you can redistribute
* it and / or modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation, either version 2 of the
* License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
******************************************************************************/

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_dir.h"

#include "co_obj.h"
#include "co_core.h"

/******************************************************************************
* FUNCTIONS
******************************************************************************/

/*
* see function definition
*/
CO_OBJ *CODirFind(CO_DIR *cod, uint32_t key)
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
int16_t CODirRdByte(CO_DIR *cod, uint32_t key, uint8_t *val)
{
    uint32_t sz;
    int16_t  result = CO_ERR_OBJ_NOT_FOUND;
    uint8_t  nodeid;
    CO_OBJ  *obj;

    if ((cod == 0) || (val == 0)) {
        return (CO_ERR_BAD_ARG);
    }

    obj = CODirFind(cod, key);
    if (obj != 0) {
        sz = COObjGetSize(obj, (uint32_t)CO_BYTE);
        if (sz != (uint32_t)CO_BYTE) {
            cod->Node->Error = CO_ERR_OBJ_SIZE;
            result           = CO_ERR_OBJ_SIZE;
        } else {
            nodeid = cod->Node->NodeId;
            result = COObjRdValue(obj, (void *)val, CO_BYTE, nodeid);
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
int16_t CODirRdWord(CO_DIR *cod, uint32_t key, uint16_t *val)
{
    uint32_t sz;
    int16_t  result = CO_ERR_OBJ_NOT_FOUND;
    uint8_t  nodeid;
    CO_OBJ  *obj;

    if ((cod == 0) || (val == 0)) {
        return (CO_ERR_BAD_ARG);
    }

    obj = CODirFind(cod, key);
    if (obj != 0) {
        sz = COObjGetSize(obj, (uint32_t)CO_WORD);
        if (sz != (uint32_t)CO_WORD) {
            cod->Node->Error = CO_ERR_OBJ_SIZE;
            result           = CO_ERR_OBJ_SIZE;
        } else {
            nodeid = cod->Node->NodeId;
            result = COObjRdValue(obj, (void *)val, CO_WORD, nodeid);
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
int16_t CODirRdLong(CO_DIR *cod, uint32_t key, uint32_t *val)
{
    uint32_t sz;
    int16_t  result = CO_ERR_OBJ_NOT_FOUND;
    uint8_t  nodeid;
    CO_OBJ  *obj;

    if ((cod == 0) || (val == 0)) {
        return (CO_ERR_BAD_ARG);
    }
    obj = CODirFind(cod, key);
    if (obj != 0) {
        sz = COObjGetSize(obj, (uint32_t)CO_LONG);
        if (sz != (uint32_t)CO_LONG) {
            cod->Node->Error = CO_ERR_OBJ_SIZE;
            result           = CO_ERR_OBJ_SIZE;
        } else {
            nodeid = cod->Node->NodeId;
            result = COObjRdValue(obj, (void *)val, CO_LONG, nodeid);
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
int16_t CODirWrByte(CO_DIR *cod, uint32_t key, uint8_t val)
{
    uint32_t sz;
    int16_t  result = CO_ERR_OBJ_NOT_FOUND;
    uint8_t  nodeid;
    CO_OBJ  *obj;

    if (cod == 0) {
        return (CO_ERR_BAD_ARG);
    }
    obj = CODirFind(cod, key);
    if (obj != 0) {
        sz = COObjGetSize(obj, (uint32_t)CO_BYTE);
        if (sz != (uint32_t)CO_BYTE) {
            cod->Node->Error = CO_ERR_OBJ_SIZE;
            result           = CO_ERR_OBJ_SIZE;
        } else {
            nodeid = cod->Node->NodeId;
            result = COObjWrValue(obj, (void *)&val, CO_BYTE, nodeid);
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
int16_t CODirWrWord(CO_DIR *cod, uint32_t key, uint16_t val)
{
    uint32_t sz;
    int16_t  result = CO_ERR_OBJ_NOT_FOUND;
    uint8_t  nodeid;
    CO_OBJ  *obj;

    if (cod == 0) {
        return (CO_ERR_BAD_ARG);
    }

    obj = CODirFind(cod, key);
    if (obj != 0) {
        sz = COObjGetSize(obj, (uint32_t)CO_WORD);
        if (sz != (uint32_t)CO_WORD) {
            cod->Node->Error = CO_ERR_OBJ_SIZE;
            result           = CO_ERR_OBJ_SIZE;
        } else {
            nodeid = cod->Node->NodeId;
            result = COObjWrValue(obj, (void *)&val, CO_WORD, nodeid);
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
int16_t CODirWrLong(CO_DIR *cod, uint32_t key, uint32_t val)
{
    uint32_t sz;
    int16_t  result = CO_ERR_OBJ_NOT_FOUND;
    uint8_t  nodeid;
    CO_OBJ  *obj;

    if (cod == 0) {
        return (CO_ERR_BAD_ARG);
    }

    obj = CODirFind(cod, key);
    if (obj != 0) {
        sz = COObjGetSize(obj, (uint32_t)CO_LONG);
        if (sz != (uint32_t)CO_LONG) {
            cod->Node->Error = CO_ERR_OBJ_SIZE;
            result           = CO_ERR_OBJ_SIZE;
        } else {
            nodeid = cod->Node->NodeId;
            result = COObjWrValue(obj, (void *)&val, CO_LONG, nodeid);
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
int16_t CODirRdBuffer(CO_DIR *cod, uint32_t key, uint8_t *buf, uint32_t len)
{
    int16_t  result = CO_ERR_OBJ_NOT_FOUND;
    CO_OBJ  *obj;

    if ((cod == 0) || (buf == 0)) {
        return (CO_ERR_BAD_ARG);
    }

    obj = CODirFind(cod, key);
    if (obj != 0) {
        result = COObjRdBufStart(obj, (void *)buf, (uint8_t)len);
        if (result != CO_ERR_NONE) {
            cod->Node->Error = CO_ERR_OBJ_READ;
        }
    }

    return(result);
}

int16_t CODirWrBuffer(CO_DIR *cod, uint32_t key, uint8_t *buf, uint32_t len)
{
    int16_t  result = CO_ERR_OBJ_NOT_FOUND;
    CO_OBJ  *obj;

    if ((cod == 0) || (buf == 0)) {
        return (CO_ERR_BAD_ARG);
    }

    obj = CODirFind(cod, key);
    if (obj != 0) {
        result = COObjWrBufStart(obj, (void *)buf, (uint8_t)len);
        if (result != CO_ERR_NONE) {
            cod->Node->Error = CO_ERR_OBJ_WRITE;
        }
    }

    return(result);
}

/*
* see function definition
*/
int16_t CO_DirInit(CO_DIR *cod, CO_NODE *node, CO_OBJ *root, uint16_t max)
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
        if (obj->Type != 0) {
            obj->Type->Dir = cod;
        }
        num++;
        obj++;
    }

    cod->Root  = root;
    cod->Num   = num;
    cod->Max   = max;
    cod->Node  = node;

    return ((int16_t)num);
}
