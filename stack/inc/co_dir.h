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

#ifndef CO_DIR_H_
#define CO_DIR_H_

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

struct CO_NODE_T;
struct CO_OBJ_T;
    
/*! \brief OBJECT DIRECTORY
*
*    This data structure holds all informations, which represents the
*    object directory.
*/
typedef struct CO_DIR_T {
    struct CO_NODE_T *Node;      /*!< Ptr to parent CANopen node info        */
    struct CO_OBJ_T  *Root;      /*!< Ptr to root object of directory        */
    uint16_t          Num;       /*!< Current number of objects in directory */
    uint16_t          Max;       /*!< Maximal number of objects in directory */

} CO_DIR;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*! \brief  FIND OBJECT ENTRY IN DIRECTORY
*
*    This function searches the given key within the given object directory.
*
* \param cod
*    pointer to the CANopen object directory
*
* \param key
*    object entry key; should be generated with the macro CO_DEV()
*
* \retval  >0    The pointer to the identified object entry
* \retval  =0    Addressed object was not found
*/
struct CO_OBJ_T *CODirFind(CO_DIR *cod, uint32_t key);

/*! \brief  READ BYTE FROM OBJECT DIRECTORY
*
*    This function reads a 8bit value from the given object directory. The
*    object entry is addressed with the given key and the value will be
*    written to the given destination pointer.
*
* \param cod
*    pointer to the CANopen object directory
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
int16_t CODirRdByte(CO_DIR *cod, uint32_t key, uint8_t *val);

/*! \brief  READ WORD FROM OBJECT DIRECTORY
*
*    This function reads a 16bit value from the given object directory. The
*    object entry is addressed with the given key and the value will be
*    written to the given destination pointer.
*
* \param cod
*    pointer to the CANopen object directory
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
int16_t CODirRdWord(CO_DIR *cod, uint32_t key, uint16_t *val);

/*! \brief  READ LONG FROM OBJECT DIRECTORY
*
*    This function reads a 32bit value from the given object directory.
*    The object entry is addressed with the given key and the value will
*    be written to the given destination pointer.
*
* \param cod
*    pointer to the CANopen object directory
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
int16_t CODirRdLong(CO_DIR *cod, uint32_t key, uint32_t *val);

/*! \brief  WRITE BYTE TO OBJECT DIRECTORY
*
*    This function writes a 8bit value to the given object directory. The
*    object entry is addressed with the given key and the value will be read
*    from the given source pointer.
*
* \param cod
*    pointer to the CANopen object directory
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
int16_t CODirWrByte(CO_DIR *cod, uint32_t key, uint8_t val);

/*! \brief  WRITE WORD TO OBJECT DIRECTORY
*
*    This function writes a 16bit value to the given object directory. The
*    object entry is addressed with the given key and the value will be read
*    from the given source pointer.
*
* \param cod
*    pointer to the CANopen object directory
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
int16_t CODirWrWord(CO_DIR *cod, uint32_t key, uint16_t val);

/*! \brief  WRITE LONG TO OBJECT DIRECTORY
*
*    This function writes a 32bit value to the given object directory. The
*    object entry is addressed with the given key and the value will be read
*    from the given source pointer.
*
* \param cod
*    pointer to the CANopen object directory
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
int16_t CODirWrLong(CO_DIR *cod, uint32_t key, uint32_t val);

/*! \brief  READ BUFFER FROM OBJECT DIRECTORY
*
*    This function reads a buffer byte stream from the given object
*    directory. The object entry is addressed with the given key and the
*    bytes will be read from the given destination buffer of the given
*    length.
*
* \param cod
*    pointer to the CANopen object directory
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
int16_t CODirRdBuffer(CO_DIR *cod, uint32_t key, uint8_t *buf, uint32_t len);

/*! \brief  WRITE BUFFER TO OBJECT DIRECTORY
*
*    This function writes a buffer byte stream to the given object directory.
*    The object entry is addressed with the given key and the bytes will be
*    read from to the given source buffer of the given length.
*
* \param cod
*    pointer to the CANopen object directory
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
int16_t CODirWrBuffer(CO_DIR *cod, uint32_t key, uint8_t *buf, uint32_t len);

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/*! \brief  INIT OBJECT DIRECTORY
*
*    This function identifies the number of already configured object
*    directory entries within the given object entry array, starting at the
*    given address (root) with the given length (max).
*
*    The internal object directory information structure will be updated
*    with the identified results and linked to the given node information
*    structure.
*
* \param cod
*    pointer to object directory which must be initialized
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
* \retval  >=0    identified number of already configured object directory
*                 entries
*/
int16_t CO_DirInit(CO_DIR *cod,
                   struct CO_NODE_T *node,
                   struct CO_OBJ_T *root,
                   uint16_t max);

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_DIR_H_ */
