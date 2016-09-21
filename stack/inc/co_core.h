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
* \file     co_core.h
*
* \brief    CORE DEFINITIONS
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_core.h#3 $
*
*           This include file defines the symbolic constants and data types for the CANopen
*           protocoll stack.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/
/*! \defgroup API      Application API Functions
*                      The application can use the listed functions to interact with the 
*                      object directory, NMT state machine and other data of the stack.
*/
/*! \defgroup INTERNAL Internal Functions
*                      The listed functions are for information only. They <b>MUST</b> not be
*                      used within the application.
*/
/*! \defgroup CALLBACK Callback Functions
*                      The listed functions are callback functions. They can be used to perform
*                      application specific operations to the described events or operation state
*                      changes.
*/

#ifndef CO_CORE_H_
#define CO_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
****************************************************************************************************
*                                             INCLUDES
****************************************************************************************************
*/

#include "co_ver.h"
#include "co_tmr.h"
#include "co_nmt.h"
#include "co_hbprod.h"
#include "co_hbcons.h"
#include "co_emcy.h"
#include "co_dir.h"
#include "co_para.h"
#include "co_sdo.h"
#include "co_tpdo.h"
#include "co_rpdo.h"

#if CO_LSS_EN > 0
#include "co_lss.h"
#endif
#if CO_SYNC_EN > 0
#include "co_fsync.h"
#endif
#if CO_RPDO_DYN_COM_EN > 0 || CO_RPDO_DYN_MAP_EN > 0 || CO_TPDO_DYN_COM_EN > 0 || CO_TPDO_DYN_MAP_EN > 0
#include "co_fpdo.h"
#endif

/*
****************************************************************************************************
*                                         TYPE DEFINITIONS
****************************************************************************************************
*/

struct CO_DIR_T;                                      /* Declaration of CO_DIR structure          */
struct CO_EMCY_T;                                     /* Declaration of CO_EMCY structure         */
struct CO_EMCY_TBL_T;                                 /* Declaration of CO_EMCY_TBL structure     */
struct CO_NMT_T;                                      /* Declaration of CO_NMT structure          */
struct CO_SDO_T;                                      /* Declaration of CO_SDO structure          */
struct CO_TPDO_T;                                     /* Declaration of CO_TPDO structure         */
struct CO_TPDO_LINK_T;                                /* Declaration of CO_TPDO_LINK structure    */

/*------------------------------------------------------------------------------------------------*/
/*! \brief CANOPEN NODE
*
* \ingroup CORE
*
*          This data structure holds all informations, which represents a complete CANopen node.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_NODE_T {
    struct CO_DIR_T        Dir;                       /*!< The object directory                   */
    struct CO_IF_T         If;                        /*!< The bus driver interface               */
    struct CO_EMCY_T       Emcy;                      /*!< The node error state                   */
    struct CO_NMT_T        Nmt;                       /*!< The Network management                 */
    struct CO_TMR_T        Tmr;                       /*!< The Highspeed timer manager            */
    struct CO_SDO_T        Sdo[CO_SDO_N];             /*!< The SDO Server Array                   */
#if CO_SDO_SEG_EN > 0 || CO_SDO_BLK_EN > 0
    CPU_INT08U            *SdoBuf;                    /*!< Pointer to SDO Transfer Buffer Start   */
#endif
#if CO_RPDO_N > 0
    struct CO_RPDO_T       RPdo[CO_RPDO_N];           /*!< The Receive PDO Array                  */
#endif
#if CO_TPDO_N > 0
    struct CO_TPDO_T       TPdo[CO_TPDO_N];           /*!< The Transmit PDO Array                 */
    struct CO_TPDO_LINK_T  TMap[CO_TPDO_MAP_N];       /*!< The Transmit PDO mapping link Array    */
#endif
#if CO_SYNC_EN > 0
    struct CO_SYNC_T       Sync;                      /*!< The SYNC management tables             */
#endif
#if CO_LSS_EN > 0
    struct CO_LSS_T        Lss;                       /*!< The LSS slave handling                 */
#endif
    enum   CO_ERR_T        Error;                     /*!< The internal detected error code       */
    CPU_INT32U             Baudrate;                  /*!< The default CAN Baudrate               */
    CPU_INT08U             NodeId;                    /*!< The default CANopen Node-ID            */

} CO_NODE;

/*------------------------------------------------------------------------------------------------*/
/*! \brief NODE SPECIFICATION
*
* \ingroup CORE
*
*          This data structure holds all configurable components of a complete CANopen node.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_NODE_SPEC_T {
    CPU_INT08U              NodeId;                   /*!< specify default Node-Id                */
    CPU_INT32U              Baudrate;                 /*!< specify default Baudrate               */
    struct CO_OBJ_T        *Dir;                      /*!< specify object directory               */
    CPU_INT16U              DirLen;                   /*!< specify object directory (max) length  */
    struct CO_EMCY_TBL_T   *EmcyCode;                 /*!< specify application EMCY info fields   */
    struct CO_TMR_MEM_T    *TmrMem;                   /*!< specify timer memory blocks            */
    CPU_INT16U              TmrNum;                   /*!< specify number of timer memory blocks  */
    CO_IF_DRV               CanDrv;                   /*!< specify linked CAN bus driver          */
    CPU_INT08U             *SdoBuf;                   /*!< SDO Transfer Buffer Memory Start       */

} CO_NODE_SPEC;

/*
****************************************************************************************************
*                                       FUNCTION PROTOTYPES
****************************************************************************************************
*/

void       CONodeInit        (CO_NODE *node, CO_NODE_SPEC *spec);
void       CONodeStart       (CO_NODE *node);
void       CONodeStop        (CO_NODE *node);
CO_ERR     CONodeGetErr      (CO_NODE *node);
void       CONodeProcess     (CO_NODE *node);

#if CO_OBJ_PARA_EN > 0
CPU_INT16S CONodeParaLoad    (CO_NODE *node, CO_NMT_RESET type);
#endif

/*
****************************************************************************************************
*                                  INTERNAL FUNCTION PROTOTYPES
****************************************************************************************************
*/

#if CO_CB_FATAL_ERROR_EN == 0
void       CO_NodeFatalError (void);
#endif

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_CORE_H_ */
