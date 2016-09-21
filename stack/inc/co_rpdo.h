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
* \file     co_rpdo.h
*
* \brief    RPDO MANAGEMENT DEFINITIONS
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_rpdo.h#3 $
*
*           This include file defines the symbolic constants and data types for the PDO handling.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_RPDO_H_
#define CO_RPDO_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
****************************************************************************************************
*                                             INCLUDES
****************************************************************************************************
*/

#include "co_obj.h"
#include "co_if.h"

/*
****************************************************************************************************
*                                             DEFINES
****************************************************************************************************
*/

#define CO_RPDO_COBID_OFF    ((uint32_t)1 << 31)    /*!< CAN identifier, marked as unused       */
#define CO_RPDO_COBID_REMOTE ((uint32_t)1 << 30)    /*!< CAN identifier, RTR is not allowed     */
#define CO_RPDO_COBID_EXT    ((uint32_t)1 << 29)    /*!< CAN identifier, extended format        */

#define CO_RPDO_ASYNC        1                        /*!< Ctrl function code: asynchronous RPDO  */

#define CO_RPDO_FLG__E       1                        /*!< Flag #0 indicates: enabled RPDO        */
#define CO_RPDO_FLG_S_       2                        /*!< Flag #1 indicates: synchronized RPDO   */

/*
****************************************************************************************************
*                                         TYPE DEFINITIONS
****************************************************************************************************
*/

#if CO_RPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief RPDO DATA
*
* \ingroup RPDO
*
*          This structure holds all data, which are needed for managing a single RPDO.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_RPDO_T {
    struct CO_NODE_T *Node;                           /*!< link to parent CANopen node            */
    uint32_t        Identifier;                     /*!< message identifier                     */
    struct CO_OBJ_T  *Map[8];                         /*!< pointer list with mapped objects       */
    uint8_t        ObjNum;                         /*!< Number of linked objects               */
    uint8_t        Flag;                           /*!< Flags attributed of PDO                */

} CO_RPDO;
#endif

/*
****************************************************************************************************
*                                    INTERNAL FUNCTION PROTOTYPES
****************************************************************************************************
*/

#if CO_RPDO_N > 0
void       CO_RPdoClear       (CO_RPDO *pdo, struct CO_NODE_T *node);
void       CO_RPdoInit        (CO_RPDO *pdo, struct CO_NODE_T *node);
int16_t CO_RPdoReset       (CO_RPDO *pdo, int16_t num);
int16_t CO_RPdoGetMap      (CO_RPDO *pdo, uint16_t num);
int16_t CO_RPdoCheck       (CO_RPDO *pdo, CO_IF_FRM *frm);
void       CO_RPdoRx          (CO_RPDO *pdo, uint16_t num, CO_IF_FRM *frm);
void       CO_RPdoWrite       (CO_RPDO *pdo, CO_IF_FRM *frm);
#endif

/*
****************************************************************************************************
*                                    CALLBACK FUNCTION PROTOTYPES
****************************************************************************************************
*/

int16_t CO_RPdoReceive     (CO_IF_FRM *frm);

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_RPDO_H_ */
