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

#include "co_obj.h"                                   /* object entry functions                   */
#include "co_err.h"                                   /* CANopen error code definitions           */
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
    CPU_INT16U        Num;                            /*!< Current number of objects in directory */
    CPU_INT16U        Max;                            /*!< Maximal number of objects in directory */

} CO_DIR;

/*
****************************************************************************************************
*                                       FUNCTION PROTOTYPES
****************************************************************************************************
*/

CO_OBJ     *CODirFind       (CO_DIR *cod, CPU_INT32U key);
CPU_INT16S  CODirRdByte     (CO_DIR *cod, CPU_INT32U key, CPU_INT08U *value);
CPU_INT16S  CODirRdWord     (CO_DIR *cod, CPU_INT32U key, CPU_INT16U *value);
CPU_INT16S  CODirRdLong     (CO_DIR *cod, CPU_INT32U key, CPU_INT32U *value);
CPU_INT16S  CODirWrByte     (CO_DIR *cod, CPU_INT32U key, CPU_INT08U value);
CPU_INT16S  CODirWrWord     (CO_DIR *cod, CPU_INT32U key, CPU_INT16U value);
CPU_INT16S  CODirWrLong     (CO_DIR *cod, CPU_INT32U key, CPU_INT32U value);
CPU_INT16S  CODirRdBuffer   (CO_DIR *cod, CPU_INT32U key, CPU_INT08U *buffer, CPU_INT32U len);
CPU_INT16S  CODirWrBuffer   (CO_DIR *cod, CPU_INT32U key, CPU_INT08U *buffer, CPU_INT32U len);

/*
****************************************************************************************************
*                                  INTERNAL FUNCTION PROTOTYPES
****************************************************************************************************
*/

CPU_INT16S  CO_DirInit      (CO_DIR *cod, struct CO_NODE_T *node, CO_OBJ *root, CPU_INT16U max);

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_DIR_H_ */
