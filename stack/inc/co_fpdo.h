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
* \file     co_fpdo.h
*
* \brief    FULL PDO HANDLING DEFINITIONS
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_fpdo.h#2 $
*
*           This include file defines the symbolic constants and data types for the PDO protocol
*           elements.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_FPDO_H_
#define CO_FPDO_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
****************************************************************************************************
*                                       OBJECT TYPE DEFINITIONS
****************************************************************************************************
*/

#if CO_RPDO_DYN_MAP_EN > 0 || CO_TPDO_DYN_MAP_EN > 0
extern CO_OBJ_TYPE COTPdoMapN;                        /* Link to PDO Map Numbers Type Structure   */
extern CO_OBJ_TYPE COTPdoMap;                         /* Link to PDO Mapping Type Structure       */

#define CO_TPDONUM   ((CO_OBJ_TYPE *)&COTPdoMapN)     /*!< Object Type Dynamic PDO Map Numbers    */
#define CO_TPDOMAP   ((CO_OBJ_TYPE *)&COTPdoMap)      /*!< Object Type Dynamic PDO Mapping        */
#endif

#if CO_RPDO_DYN_COM_EN > 0 || CO_TPDO_DYN_COM_EN > 0
extern CO_OBJ_TYPE COTPdoId;                          /* Link to PDO Identifier Type Structure    */
extern CO_OBJ_TYPE COTPdoType;                        /* Link to PDO Transmission Type Structure  */

#define CO_TPDOID    ((CO_OBJ_TYPE *)&COTPdoId)       /*!< Object Type Dynamic PDO Identifier     */
#define CO_TPDOTYPE  ((CO_OBJ_TYPE *)&COTPdoType)     /*!< Object Type Dynamic PDO Transm. Type   */
#endif

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_FPDO_H_ */
