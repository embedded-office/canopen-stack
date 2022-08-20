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

#ifndef APP_DICT_H_
#define APP_DICT_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_core.h"

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

typedef struct OD_DYN_T {           /*!< this type represents a dynamic OD   */
    CO_OBJ   *Root;                 /*!< first element of object entry array */
    uint32_t  Len;                  /*!< length of object entry array        */
    uint32_t  Used;                 /*!< number of used object entries       */
} OD_DYN;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*---------------------------------------------------------------------------*/
/*! \brief INIT DYNAMIC OBJECT DICTIONARY
*
* \details Set the possible object entries of the dynamic object dictionary
*          to the given array of object entries (root) with length (len)
*
* \param   self
*          reference to the dynamic object dictionary
*
* \param   root
*          pointer to the first element of an object entry array
*
* \param   len
*          length of the given object entry array
*/
/*---------------------------------------------------------------------------*/
void ODInit(OD_DYN *self, CO_OBJ *root, uint32_t len);

/*---------------------------------------------------------------------------*/
/*! \brief ADD DYNAMIC OBJECT ENTRY
*
* \details Set or change the type and data of an object entry within the
*          dynamic object dictionary, addressed by the given object entry
*          key.
*
* \param   self
*          reference to the dynamic object dictionary
*
* \param   key
*          object entry key
*
* \param   type
*          type of the new or changed object entry
*
* \param   data
*          data of the new or changed object entry
*
* \note    You should generate the key with the macro CO_KEY()
*/
/*---------------------------------------------------------------------------*/
void ODAdd(OD_DYN *self, uint32_t key, CO_OBJ_TYPE *type, CO_DATA data);

/*---------------------------------------------------------------------------*/
/*! \brief GET DYNAMIC OBJECT DICTIONARY
*
* \details Use this function, when the start address of the object dictionary
*          is needed.
*
* \return  Pointer to the first element of the dynamic object dictionary
*/
/*---------------------------------------------------------------------------*/
CO_OBJ *ODGetDict(OD_DYN *self);

/*---------------------------------------------------------------------------*/
/*! \brief GET SIZE OF DYNAMIC OBJECT DICTIONARY
*
* \details Use this function, when the maximum possible size (including the
*          mandatory end-marker) of the object dictionary is needed.
*
* \return  Size of the object array for this dynamic object dictionary
*/
/*---------------------------------------------------------------------------*/
uint32_t ODGetSize(OD_DYN *self);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif
