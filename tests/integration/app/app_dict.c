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

#include "app_dict.h"

/******************************************************************************
* PRIVATE MACROS
******************************************************************************/

/* set the referenced object entry (obj) to the given key, type and data */
#define OD_SET(obj,key,type,data)   do {     \
        (obj)->Key  = (uint32_t)(key);       \
        (obj)->Type = (CO_OBJ_TYPE *)(type); \
        (obj)->Data = data;                  \
    } while (0)

/* copy content of source object (src) into the destination object (dst) */
#define OD_CPY(dst,src)   OD_SET(dst, (src)->Key, (src)->Type, (src)->Data)

/* exchange the content of the objects (a) and (b) */
#define OD_SWAP(a,b)   do { \
        CO_OBJ x;           \
        OD_CPY(&x,a);       \
        OD_CPY(a,b);        \
        OD_CPY(b,&x);       \
    } while (0)

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

/* mask the index and subindex from a object entry key */
#define OD_IDX(key)   ((key) & 0xFFFFFF00L)

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*---------------------------------------------------------------------------*/
/*! \brief REQ-OD-0100
*
* \details Clear the given object entry array and keep the reference to
*          the given array in the dynamic object dictionary object.
*/
/*---------------------------------------------------------------------------*/
void ODInit(OD_DYN *self, CO_OBJ *root, uint32_t len)
{
    uint32_t  idx;
    CO_OBJ   *od;

    idx = 0;
    od  = root;
    while (idx < len) {
        OD_SET(od, 0, 0, (CO_DATA)(NULL));
        od++;
        idx++;
    }

    self->Root = root;
    self->Len  = len - 1;          /* ensure the existance of the end-marker */
    self->Used = 0;
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-OD-0110
*
* \details To keep the object entries a sorted list (ascending by key),
*          we need to find the right place for insertion. If given key
*          is found in the list, we modify the content of this entry.
*/
/*---------------------------------------------------------------------------*/
void ODAdd(OD_DYN *self, uint32_t key, CO_OBJ_TYPE *type, CO_DATA data)
{
    CO_OBJ  temp;
    CO_OBJ *od;

    if ((key == 0) ||                         /* end-marker is not possible */
        (self->Used == self->Len)) {          /* list is full */
        return;
    }

    od = self->Root;
    while ((od->Key != 0) &&
           (OD_IDX(od->Key) < OD_IDX(key))) {
        od++;                                 /* used entry with lower key */
    }

    if (OD_IDX(od->Key) == OD_IDX(key)) {     /* replace existing key */
        OD_SET(od, key, type, data);

    } else if (od->Key == 0) {                /* given key is last entry */
        OD_SET(od, key, type, data);
        self->Used++;

    } else {                                  /* insert key in the middle */
        OD_SET(&temp, key, type, data);
        do {
            OD_SWAP(od, &temp);
            od++;
        } while (od->Key != 0);
        OD_CPY(od, &temp);
        self->Used++;
    }
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-OD-0120
*
* \details Simple getter function to retrieve the protected start address
*          of the dynamic object dictionary.
*/
/*---------------------------------------------------------------------------*/
CO_OBJ *ODGetDict(OD_DYN *self)
{
    return (self->Root);
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-OD-0130
*
* \details Simple getter function to retrieve the protected object array size
*          of the dynamic object dictionary.
*/
/*---------------------------------------------------------------------------*/
uint32_t ODGetSize(OD_DYN *self)
{
    return (self->Len + 1);      /* include the end-marker in returned value */
}
