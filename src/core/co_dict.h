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

#ifndef CO_DICT_H_
#define CO_DICT_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"
#include "co_err.h"

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

struct CO_NODE_T;              /* Declaration of canopen node structure      */
struct CO_OBJ_T;               /* Declaration of object entry structure      */

/*! \brief OBJECT dictionary
*
*    This data structure holds all informations, which represents the
*    object dictionary.
*/
typedef struct CO_DICT_T {
    struct CO_NODE_T *Node;     /*!< Ptr to parent CANopen node info         */
    struct CO_OBJ_T  *Root;     /*!< Ptr to root object of dictionary        */
    uint16_t          Num;      /*!< Current number of objects in dictionary */
    uint16_t          Max;      /*!< Maximal number of objects in dictionary */

} CO_DICT;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*! \brief  FIND OBJECT ENTRY IN DICTIONARY
*
*    This function searches the given key within the given object dictionary.
*
* \param cod
*    pointer to the object dictionary
*
* \param key
*    object entry key; should be generated with the macro CO_DEV()
*
* \retval  >0    The pointer to the identified object entry
* \retval  =0    Addressed object was not found
*/
struct CO_OBJ_T *CODictFind(CO_DICT *cod, uint32_t key);

/*! \brief  READ BYTE FROM OBJECT DICTIONARY
*
*    This function reads a 8bit value from the given object dictionary. The
*    object entry is addressed with the given key and the value will be
*    written to the given destination pointer.
*
* \param cod
*    pointer to the object dictionary
*
* \param key
*    object entry key; should be generated with the macro CO_DEV()
*
* \param val
*    pointer to the value destination
*
* \retval   =CO_ERR_NONE    Successfully operation
* \retval  !=CO_ERR_NONE    An error is detected
*/
CO_ERR CODictRdByte(CO_DICT *cod, uint32_t key, uint8_t *val);

/*! \brief  READ WORD FROM OBJECT DICTIONARY
*
*    This function reads a 16bit value from the given object dictionary. The
*    object entry is addressed with the given key and the value will be
*    written to the given destination pointer.
*
* \param cod
*    pointer to the object dictionary
*
* \param key
*    object entry key; should be generated with the macro CO_DEV()
*
* \param val
*    pointer to the value destination
*
* \retval   =CO_ERR_NONE    Successfully operation
* \retval  !=CO_ERR_NONE    An error is detected
*/
CO_ERR CODictRdWord(CO_DICT *cod, uint32_t key, uint16_t *val);

/*! \brief  READ LONG FROM OBJECT DICTIONARY
*
*    This function reads a 32bit value from the given object dictionary.
*    The object entry is addressed with the given key and the value will
*    be written to the given destination pointer.
*
* \param cod
*    pointer to the object dictionary
*
* \param key
*    object entry key; should be generated with the macro CO_DEV()
*
* \param val
*    pointer to the value destination
*
* \retval   =CO_ERR_NONE    Successfully operation
* \retval  !=CO_ERR_NONE    An error is detected
*/
CO_ERR CODictRdLong(CO_DICT *cod, uint32_t key, uint32_t *val);

/*! \brief  WRITE BYTE TO OBJECT DICTIONARY
*
*    This function writes a 8bit value to the given object dictionary. The
*    object entry is addressed with the given key and the value will be read
*    from the given source pointer.
*
* \param cod
*    pointer to the object dictionary
*
* \param key
*    object entry key; should be generated with the macro CO_DEV()
*
* \param val
*    the source value
*
* \retval   =CO_ERR_NONE    Successfully operation
* \retval  !=CO_ERR_NONE    An error is detected
*/
CO_ERR CODictWrByte(CO_DICT *cod, uint32_t key, uint8_t val);

/*! \brief  WRITE WORD TO OBJECT DICTIONARY
*
*    This function writes a 16bit value to the given object dictionary. The
*    object entry is addressed with the given key and the value will be read
*    from the given source pointer.
*
* \param cod
*    pointer to the object dictionary
*
* \param key
*    object entry key; should be generated with the macro CO_DEV()
*
* \param val
*    the source value
*
* \retval   =CO_ERR_NONE    Successfully operation
* \retval  !=CO_ERR_NONE    An error is detected
*/
CO_ERR CODictWrWord(CO_DICT *cod, uint32_t key, uint16_t val);

/*! \brief  WRITE LONG TO OBJECT DICTIONARY
*
*    This function writes a 32bit value to the given object dictionary. The
*    object entry is addressed with the given key and the value will be read
*    from the given source pointer.
*
* \param cod
*    pointer to the object dictionary
*
* \param key
*    object entry key; should be generated with the macro CO_DEV()
*
* \param val
*    the source value
*
* \retval   =CO_ERR_NONE    Successfully operation
* \retval  !=CO_ERR_NONE    An error is detected
*/
CO_ERR CODictWrLong(CO_DICT *cod, uint32_t key, uint32_t val);

/*! \brief  READ BUFFER FROM OBJECT DICTIONARY
*
*    This function reads a buffer byte stream from the given object
*    dictionary. The object entry is addressed with the given key and the
*    bytes will be read from the given destination buffer of the given
*    length.
*
* \param cod
*    pointer to the object dictionary
*
* \param key
*    object entry key; should be generated with the macro CO_DEV()
*
* \param buf
*    pointer to the destination buffer
*
* \param len
*    length of destination buffer
*
* \retval   =CO_ERR_NONE    Successfully operation
* \retval  !=CO_ERR_NONE    An error is detected
*/
CO_ERR CODictRdBuffer(CO_DICT *cod, uint32_t key, uint8_t *buf, uint32_t len);

/*! \brief  WRITE BUFFER TO OBJECT DICTIONARY
*
*    This function writes a buffer byte stream to the given object dictionary.
*    The object entry is addressed with the given key and the bytes will be
*    read from to the given source buffer of the given length.
*
* \param cod
*    pointer to the object dictionary
*
* \param key
*    object entry key; should be generated with the macro CO_DEV()
*
* \param buf
*    pointer to the source bytes
*
* \param len
*    length of byte buffer
*
* \retval   =CO_ERR_NONE    Successfully operation
* \retval  !=CO_ERR_NONE    An error is detected
*/
CO_ERR CODictWrBuffer(CO_DICT *cod, uint32_t key, uint8_t *buf, uint32_t len);

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/*! \brief  INIT OBJECT DICTIONARY
*
*    This function identifies the number of already configured object
*    dictionary entries within the given object entry array, starting at the
*    given address (root) with the given length (max).
*
*    The internal object dictionary information structure will be updated
*    with the identified results and linked to the given node information
*    structure.
*
* \param cod
*    pointer to object dictionary which must be initialized
*
* \param node
*    pointer to the CANopen device node information structure
*
* \param root
*    pointer to the start of the object entry array
*
* \param max
*    the length of the object entry array
*
* \retval   <0    An argument error is detected.
* \retval  >=0    identified number of already configured object dictionary
*                 entries
*/
int16_t CODictInit(CO_DICT *cod,
                  struct CO_NODE_T *node,
                  struct CO_OBJ_T *root,
                  uint16_t max);

/*! \brief  INIT OBJECT DICTIONARY OBJECTS
*
*    This function calls all existing type init functions for all object
*    entries within the configured object dictionary.
*
* \param cod
*    pointer to object dictionary which must be initialized
*
* \param node
*    pointer to the CANopen device node information structure
*
* \retval   =CO_ERR_NONE    Successfully operation
* \retval  !=CO_ERR_NONE    An error is detected
*/
CO_ERR CODictObjInit(CO_DICT *cod, struct CO_NODE_T *node);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_DICT_H_ */
