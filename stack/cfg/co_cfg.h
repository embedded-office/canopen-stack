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
* \file     co_cfg.h
*
* \brief    CANOPEN CONFIGURATION
*
* \author   hillmann
* \version  1.0
* \date     08.02.2016
*
*           CANopen Quickstart Demo
*
* \note     This file is generated and MUST NOT be modified by hand.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_CFG_H_
#define CO_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
****************************************************************************************************
*                                          INCLUDES
****************************************************************************************************
*/

//Todo: #include "co_obj_cfg.h"                               /* generated object directory configuration */

/*
****************************************************************************************************
*                          C A N O P E N   C O N F I G U R A T I O N
****************************************************************************************************
*/
                                                      /*===== CALLBACK FUNCTIONS =================*/
#define CO_CB_FATAL_ERROR_EN    0
#define CO_CB_PARA_LOAD_EN      0
#define CO_CB_PARA_SAVE_EN      0
#define CO_CB_PARA_DEFAULT_EN   0
#define CO_CB_IF_RECEIVE_EN     0
#define CO_CB_TPDO_TRANSMIT_EN  0
#define CO_CB_RPDO_RECEIVE_EN   0
#define CO_CB_NMT_CHANGE_EN     0
#define CO_CB_HBC_EVENT_EN      0
#define CO_CB_HBC_CHANGE_EN     0
#define CO_CB_LSS_STORE_EN      0

                                                      /*===== OBJECT DIRECTORY ===================*/
#define CO_OBJ_PARA_EN          1
#define CO_OBJ_STRING_EN        0
#define CO_OBJ_DOMAIN_EN        0

                                                      /*===== SDO SERVER =========================*/
#define CO_SDO_N                1
#define CO_SDO_DYN_ID_EN        0
#define CO_SDO_SEG_EN           0
#define CO_SDO_BLK_EN           0
#define CO_SDO_BUF_SEG          127

                                                      /*===== EMERGENCY ==========================*/
#define CO_EMCY_N               0
#define CO_EMCY_REG_CLASS_EN    0
#define CO_EMCY_EMCY_MAN_EN     0
#define CO_EMCY_HIST_EN         0
#define CO_EMCY_HIST_MAN_EN     0

                                                      /*===== TIMER MANAGEMENT ===================*/
#define CO_TMR_TICKS_PER_SEC    100
#define CO_TMR_N                5

                                                      /*===== PDO TRANSFER =======================*/
#define CO_RPDO_N               0
#define CO_RPDO_DYN_COM_EN      0
#define CO_RPDO_DYN_MAP_EN      0

#define CO_TPDO_N               1
#define CO_TPDO_MAP_N           3
#define CO_TPDO_DYN_COM_EN      0
#define CO_TPDO_DYN_MAP_EN      0

                                                      /*===== SYNC ===============================*/
#define CO_SYNC_EN              0

                                                      /*===== LSS ================================*/
#define CO_LSS_EN               0
#define CO_STD_BAUD_1M          0
#define CO_STD_BAUD_800K        0
#define CO_STD_BAUD_500K        0
#define CO_STD_BAUD_250K        250000
#define CO_STD_BAUD_125K        0
#define CO_STD_BAUD_50K         0
#define CO_STD_BAUD_20K         0
#define CO_STD_BAUD_10K         0

/*
****************************************************************************************************
*                                COMPILE TIME CONFIGURATION CHECKS
****************************************************************************************************
*/

#if CO_TPDO_N == 0 && CO_RPDO_N == 0 && CO_SYNC_EN > 0
#error "co_cfg.h: CO_TPDO_N or CO_RPDO_N must be > 0 when setting CO_SYNC_EN to 1"
#endif

#if CO_TPDO_N == 0 && CO_TPDO_MAP_N > 0
#error "co_cfg.h: CO_TPDO_N must be > 0 when setting CO_TPDO_MAP_N > 0"
#endif
#if CO_TPDO_N > 0 && CO_TPDO_MAP_N == 0
#undef  CO_TPDO_MAP_N
#define CO_TPDO_MAP_N   (CO_TPDO_N*8)
#endif

#if  CO_TPDO_N == 0 && CO_TPDO_DYN_COM_EN > 0
#error "co_cfg.h: CO_TPDO_N must be > 0 when setting CO_TPDO_DYN_COM_EN > 0"
#endif
#if  CO_TPDO_N == 0 && CO_TPDO_DYN_MAP_EN > 0
#error "co_cfg.h: CO_TPDO_N must be > 0 when setting CO_TPDO_DYN_MAP_EN > 0"
#endif
#if CO_TPDO_DYN_COM_EN == 0 && CO_TPDO_DYN_MAP_EN > 0
#error "co_cfg.h: CO_TPDO_DYN_COM_EN must be 1 when setting CO_TPDO_DYN_MAP_EN to 1"
#endif

#if  CO_RPDO_N == 0 && CO_RPDO_DYN_COM_EN > 0
#error "co_cfg.h: CO_RPDO_N must be > 0 when setting CO_RPDO_DYN_COM_EN to 1"
#endif
#if  CO_RPDO_N == 0 && CO_RPDO_DYN_MAP_EN > 0
#error "co_cfg.h: CO_RPDO_N must be > 0 when setting CO_RPDO_DYN_MAP_EN to 1"
#endif
#if CO_RPDO_DYN_COM_EN == 0 && CO_RPDO_DYN_MAP_EN > 0
#error "co_cfg.h: CO_RPDO_DYN_COM_EN must be 1 when setting CO_RPDO_DYN_MAP_EN to 1"
#endif

#if CO_SDO_BLK_EN == 0 && CO_SDO_SEG_EN > 0 && CO_SDO_BUF_SEG != 1
#error "co_cfg.h: CO_SDO_BUF_SEG must be 1 when CO_SDO_SEG_EN is set to 1 and CO_SDO_BLK_EN is set to 0."
#endif

#if CO_SDO_N == 1 && CO_SDO_DYN_ID_EN > 0
#error "co_cfg.h: CO_SDO_N must be > 1 when setting CO_SDO_DYN_ID_EN to 1"
#endif

#if CO_EMCY_N == 0 && CO_EMCY_HIST_EN > 0
#error "co_cfg.h: CO_EMGY_N must be > 0 when setting CO_EMGY_HIST_EN to 1"
#endif

#if CO_EMCY_HIST_EN == 0 && CO_EMCY_HIST_MAN_EN > 0
#error "co_cfg.h: CO_EMCY_HIST_EN must be 1 when setting CO_EMCY_HIST_MAN_EN to 1"
#endif

#if CO_TMR_TICKS_PER_SEC > 1000
#error "co_cfg.h: maximum value for CO_TMR_TICKS_PER_SEC is 1000"
#endif

#if CO_LSS_EN > 0 && CO_OBJ_PARA_EN == 0
#error "co_cfg.h: CO_OBJ_PARA_EN must be 1 when setting CO_LSS_EN to 1"
#endif

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_CFG_H_ */

