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
* \file     co_femcy.h
*
* \brief    FULL EMCY PRODUCER DEFINITIONS
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_femcy.h#2 $
*
*           This include file defines the symbolic constants and data types for the emergency
*           history handling.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_FEMCY_H_
#define CO_FEMCY_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
****************************************************************************************************
*                                            INCLUDES
****************************************************************************************************
*/

#include "co_obj.h"

/*
****************************************************************************************************
*                                     OBJECT TYPE DEFINITION
****************************************************************************************************
*/

#if CO_EMCY_HIST_EN > 0
extern CO_OBJ_TYPE COTEmcy;                           /* Link to EMCY Object Type Structure       */

#define CO_TEMCY        (CO_OBJ_TYPE *)&COTEmcy       /*!< Object Type EMCY History               */
#endif

/*
****************************************************************************************************
*                                          TYPE DEFINITION
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief EMCY HISTORY
*
* \ingroup EMCY
*
*          This structure holds all data, which are needed for the EMCY history management
*          within the object directory.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_EMCY_HIST_T {
    uint8_t Max;                                   /*!< Total length of EMCY history           */
    uint8_t Num;                                   /*!< Number of EMCY in history              */
    uint8_t Off;                                   /*!< Subindex-Offset to newest EMCY entry   */

} CO_EMCY_HIST;

/*
****************************************************************************************************
*                                         FUNCTION PROTOTYPES
****************************************************************************************************
*/

struct CO_EMCY_T;
struct CO_EMCY_USR_T;

#if CO_EMCY_HIST_EN > 0
void       COEmcyHistReset   (struct CO_EMCY_T *emcy);
#endif

/*
****************************************************************************************************
*                                    INTERNAL FUNCTION PROTOTYPES
****************************************************************************************************
*/

#if CO_EMCY_HIST_EN > 0
void       CO_EmcyHistInit   (struct CO_EMCY_T *emcy);
void       CO_EmcyHistAdd    (struct CO_EMCY_T *emcy, uint8_t err, struct CO_EMCY_USR_T *usr);
#endif

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_FEMCY_H_ */
