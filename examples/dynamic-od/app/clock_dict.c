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

#include "clock_dict.h"

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void ObjSet(CO_OBJ *obj, uint32_t key, const CO_OBJ_TYPE *type, CO_DATA data)
{
    obj->Key  = key;
    obj->Type = type;
    obj->Data = data;
}

static void ObjCpy(CO_OBJ *a, CO_OBJ *b)
{
    a->Key  = b->Key;
    a->Type = b->Type;
    a->Data = b->Data;
}

static void ObjSwap(CO_OBJ *a, CO_OBJ *b)
{
    CO_OBJ x;

    ObjCpy(&x,  a);
    ObjCpy( a,  b);
    ObjCpy( b, &x);
}

static int16_t ObjCmp(CO_OBJ *a, CO_OBJ *b)
{
    int16_t result = 1;

    if (CO_GET_DEV(a->Key) == CO_GET_DEV(b->Key)) {
        result = 0;

    } else if (CO_GET_DEV(a->Key) < CO_GET_DEV(b->Key)) {
        result = -1;
    }

    return (result);
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

void ODInit (OD_DYN *self, CO_OBJ *root, uint32_t length)
{
    uint32_t  idx;
    CO_OBJ    end = CO_OBJ_DICT_ENDMARK;
    CO_OBJ   *od;

    idx = 0;
    od  = root;
    while (idx < length) {
        ObjCpy(od, &end);
        od++;
        idx++;
    }

    self->Root   = root;
    self->Length = length - 1;
    self->Used   = 0;
}

void ODAddUpdate(OD_DYN *self, uint32_t key, const CO_OBJ_TYPE *type, CO_DATA data)
{
    CO_OBJ  temp;
    CO_OBJ *od;

    if ((key == 0) ||
        (self->Used == self->Length)) {
        return;
    }

    od = self->Root;
    ObjSet(&temp, key, type, data);

    /* find position in dictionary */
    while ((od->Key != 0) && (ObjCmp(od, &temp) < 0)) {
        od++;
    }

    if (ObjCmp(od, &temp) == 0) {        /* Change existing entry */
        ObjSet(od, key, type, data);

    } else if (od->Key == 0) {           /* Append at end of dictionary */
        ObjSet(od, key, type, data);
        self->Used++;

    } else {                             /* Insert in middle of dictionary */
        do {
            ObjSwap(od, &temp);
            od++;
        } while (od->Key != 0);
        ObjCpy(od, &temp);
        self->Used++;
    }
}
