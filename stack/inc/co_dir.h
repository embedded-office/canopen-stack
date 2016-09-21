/*
****************************************************************************************************
* (c) copyright by
*     Embedded Office GmbH & Co. KG       Tel : (07522) 97 00 08-0
*     Friedrich-Ebert-Str. 20/1           Fax : (07522) 97 00 08-99
*     D-88239 Wangen                      Mail: info@embedded-office.de
*                                         Web : http://www.embedded-office.de
*
* All rights reserved. Confidential and Proprietary. Protected by international copyright laws.
* Knowledge of the source code may not be used to write a similar product.
* This file may only be used in accordance with a license and should not be
* redistributed in any way.
****************************************************************************************************
*/
/*!
****************************************************************************************************
* \file     co_dir.h
*
* \brief    OBJECT DIRECTORY DEFINITIONS
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_dir.h#2 $
*
*           This include file holds definitions for the CANopen object directory.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_DIR_H_
#define CO_DIR_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
****************************************************************************************************
*                                             INCLUDES
****************************************************************************************************
*/

#include "co_obj.h"
#include "co_err.h"
#if CO_OBJ_STRING_EN > 0 || CO_OBJ_DOMAIN_EN > 0
#include "co_fobj.h"
#endif

/*
****************************************************************************************************
*                                           DATA TYPES
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief OBJECT DIRECTORY
*
* \ingroup DIR
*
*          This data structure holds all informations, which represents the object directory.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_DIR_T {
    struct CO_NODE_T *Node;                           /*!< Ptr to parent CANopen node info        */
    struct CO_OBJ_T  *Root;                           /*!< Ptr to root object of directory        */
    uint16_t        Num;                            /*!< Current number of objects in directory */
    uint16_t        Max;                            /*!< Maximal number of objects in directory */

} CO_DIR;

/*
****************************************************************************************************
*                                       FUNCTION PROTOTYPES
****************************************************************************************************
*/

CO_OBJ     *CODirFind       (CO_DIR *cod, uint32_t key);
int16_t  CODirRdByte     (CO_DIR *cod, uint32_t key, uint8_t *value);
int16_t  CODirRdWord     (CO_DIR *cod, uint32_t key, uint16_t *value);
int16_t  CODirRdLong     (CO_DIR *cod, uint32_t key, uint32_t *value);
int16_t  CODirWrByte     (CO_DIR *cod, uint32_t key, uint8_t value);
int16_t  CODirWrWord     (CO_DIR *cod, uint32_t key, uint16_t value);
int16_t  CODirWrLong     (CO_DIR *cod, uint32_t key, uint32_t value);
int16_t  CODirRdBuffer   (CO_DIR *cod, uint32_t key, uint8_t *buffer, uint32_t len);
int16_t  CODirWrBuffer   (CO_DIR *cod, uint32_t key, uint8_t *buffer, uint32_t len);

/*
****************************************************************************************************
*                                  INTERNAL FUNCTION PROTOTYPES
****************************************************************************************************
*/

int16_t  CO_DirInit      (CO_DIR *cod, struct CO_NODE_T *node, CO_OBJ *root, uint16_t max);

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_DIR_H_ */
