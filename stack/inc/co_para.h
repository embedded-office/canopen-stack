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
* \file     co_para.h
*
* \brief    PARAMETER GROUP DEFINITIONS
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_para.h#2 $
*
*           This include file holds definitions for the parameter object groups.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_PARA_H_
#define CO_PARA_H_

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

/*
****************************************************************************************************
*                                            DEFINES
****************************************************************************************************
*/

#define CO_PARA____        0x0000                     /*!< disable                                */
#define CO_PARA___E        0x0001                     /*!< enable  (on command)                   */
#define CO_PARA__A_        0x0002                     /*!< enable  (autonomously)                 */
#define CO_PARA__AE        0x0003                     /*!< enable  (autonomously and on command)  */

/*
****************************************************************************************************
*                                           DATA TYPES
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief PARAMETER GROUP INFO
*
* \ingroup PARA
*
*          This structure holds the informations of a parameter group. The parameter group
*          is used within the special function parameter object in an object directory.
*
* \note    This structure may be placed into ROM to reduce RAM usage.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_PARA_T {
    uint32_t             Size;                      /*!< Size of parameter memory block         */
    uint8_t            *Start;                     /*!< Start of parameter memory block        */
    uint8_t            *Default;                   /*!< Start of default memory block          */
    enum CO_NMT_RESET_T    Type;                      /*!< Parameter reset type                   */
    void                  *Ident;                     /*!< Ptr to User Ident-Code for this group  */
    uint32_t             Value;                     /*!< value when reading parameter object    */

} CO_PARA;

/*
****************************************************************************************************
*                                     OBJECT TYPE DEFINITION
****************************************************************************************************
*/

#if CO_OBJ_PARA_EN > 0
extern CO_OBJ_TYPE COTPara;                           /* Link to Parameter Object Type Structure  */

#define CO_TPARA        ((CO_OBJ_TYPE *)&COTPara)     /*!< Object Type Parameter                  */
#endif

/*
****************************************************************************************************
*                                       FUNCTION PROTOTYPES
****************************************************************************************************
*/

#if CO_OBJ_PARA_EN > 0
void       COParaStore       (CO_PARA *pg, struct CO_NODE_T *node);
void       COParaRestore     (CO_PARA *pg, struct CO_NODE_T *node);
#endif

/*
****************************************************************************************************
*                                  CALLBACK FUNCTION PROTOTYPES
****************************************************************************************************
*/

#if CO_OBJ_PARA_EN > 0
int16_t CO_ParaLoad       (CO_PARA *pg);
int16_t CO_ParaSave       (CO_PARA *pg);
int16_t CO_ParaDefault    (CO_PARA *pg);
#endif

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_PARA_H_ */
