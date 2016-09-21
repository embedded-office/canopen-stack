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
* \file     co_fsync.h
*
* \brief    SYNC HANDLING DEFINITIONS
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_fsync.h#2 $
*
*           This include file defines the symbolic constants and data types for the SYNC
*           handling.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_SYNC_H_
#define CO_SYNC_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
****************************************************************************************************
*                                             INCLUDES
****************************************************************************************************
*/

#include "co_types.h"
#include "co_cfg.h"
#include "co_if.h"

/*
****************************************************************************************************
*                                             DEFINES
****************************************************************************************************
*/

#define CO_SYNC_FLG_TX    0x01                         /*!< message type indication  TPDO         */
#define CO_SYNC_FLG_RX    0x02                         /*!< message type indication: RPDO         */

/*
****************************************************************************************************
*                                            DATA TYPES
****************************************************************************************************
*/

#if CO_SYNC_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief SYNCHRONOUS PDO TABLE
*
* \ingroup SYNC
*
*          This structure contains all needed data to handle synchronous PDOs.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_SYNC_T {
    struct CO_NODE_T *Node;                           /*!< link to parent node                    */
    uint32_t        CobId;                          /*!< SYNC message identifier                */
    uint32_t        Time;                           /*!< SYNC time (num of received SYNC)       */
#if CO_RPDO_N > 0
    CO_IF_FRM         RFrm[CO_RPDO_N];                /*!< synchronous RPDO CAN message frame     */
    struct CO_RPDO_T *RPdo[CO_RPDO_N];                /*!< Pointer to synchronous RPDO            */
#endif
#if CO_TPDO_N > 0
    struct CO_TPDO_T *TPdo[CO_TPDO_N];                /*!< Pointer to synchronous TPDO            */
    uint8_t        TNum[CO_TPDO_N];                /*!< Number of SYNC until PDO shall be sent */
    uint8_t        TSync[CO_TPDO_N];               /*!< SYNC time when transmission must occur */
#endif

} CO_SYNC;
#endif

/*
****************************************************************************************************
*                                  INTERNAL FUNCTION PROTOTYPES
****************************************************************************************************
*/

#if CO_SYNC_EN > 0
void       CO_SyncInit    (CO_SYNC *sync, struct CO_NODE_T *node);
int16_t CO_SyncUpdate  (CO_SYNC *sync, CO_IF_FRM *frm);
void       CO_SyncHandler (CO_SYNC *sync);
void       CO_SyncAdd     (CO_SYNC *sync, uint16_t num, uint8_t msgType, uint8_t txtype);
void       CO_SyncRemove  (CO_SYNC *sync, uint16_t num, uint8_t msgType);
#endif

#if CO_SYNC_EN > 0 && CO_RPDO_N > 0
void       CO_SyncRx      (CO_SYNC *sync, CO_IF_FRM *frm);
#endif

#if CO_SYNC_EN > 0 && CO_TPDO_N > 0
void       CO_SyncRestart (CO_SYNC *sync);
#endif

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_SYNC_H_ */
