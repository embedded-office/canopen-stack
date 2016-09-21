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
* \file     co_fobj.h
*
* \brief    FULL OBJECT DIRECTORY ENTRY DEFINITIONS
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_fobj.h#2 $
*
*           This include file holds the definitions for the fully compliant object directory
*           entries.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_FOBJ_H_
#define CO_FOBJ_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
****************************************************************************************************
*                                     OBJECT TYPE DEFINITIONS
****************************************************************************************************
*/

#if CO_OBJ_STRING_EN > 0
extern CO_OBJ_TYPE COTString;                         /* Link to String Object Type Structure     */

#define CO_TSTRING      ((CO_OBJ_TYPE *)&COTString)   /*!< Object Type String                     */
#endif

#if CO_OBJ_DOMAIN_EN > 0
extern CO_OBJ_TYPE COTDomain;                         /* Link to String Object Type Structure     */

#define CO_TDOMAIN      ((CO_OBJ_TYPE *)&COTDomain)   /*!< Object Type Domain                     */
#endif

/*
****************************************************************************************************
*                                        TYPE DEFINITIONS
****************************************************************************************************
*/

#if CO_OBJ_DOMAIN_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief DOMAIN MANAGEMENT STRUCTURE
*
* \ingroup OBJ
*
*          This structure holds all data, which are needed for the domain object management
*          within the object directory.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_DOM_T {
    CPU_INT32U  Size;                                 /*!< Domain size information                */
    CPU_INT08U *Start;                                /*!< Domain start address                   */

} CO_DOM;
#endif

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_FOBJ_H_ */
