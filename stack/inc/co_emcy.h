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
* \file     co_emcy.h
*
* \brief    EMCY PRODUCER DEFINITIONS
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_emcy.h#2 $
*
*           This include file defines the symbolic constants and data types for the emergency
*           handling.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_EMCY_H_
#define CO_EMCY_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
****************************************************************************************************
*                                             INCLUDES
****************************************************************************************************
*/

#include "cpu.h"                                      /* CPU configuration                        */
#include "co_cfg.h"                                   /* CANopen configuration                    */
#if CO_EMCY_HIST_EN > 0
#include "co_femcy.h"
#endif

/*
****************************************************************************************************
*                                             DEFINES
****************************************************************************************************
*/

#define CO_EMCY_STORAGE        (1+((CO_EMCY_N-1)/8))  /*!< bytes, needed for CO_EMCY_N bits       */

/*
****************************************************************************************************
*                                          TYPE DEFINITIONS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief EMCY CODE
*
* \ingroup EMCY
*
*          The following defines holds the base values for the pre-defined standard error groups.
*          The values can be used to simplify the definition of standard conform EMCY error
*          codes.
*
* \see CO_EMCY_TBL
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_EMCY_CODE_NO_ERR                0x0000     /*!< error reset or no error                */
#define CO_EMCY_CODE_GEN_ERR               0x1000     /*!< generic error                          */
#define CO_EMCY_CODE_CUR_ERR               0x2000     /*!< current                                */
#define CO_EMCY_CODE_CUR_INPUT_ERR         0x2100     /*!< current, device input side             */
#define CO_EMCY_CODE_CUR_INTERN_ERR        0x2200     /*!< current inside the device              */
#define CO_EMCY_CODE_CUR_OUTPUT_ERR        0x2300     /*!< current, device output side            */
#define CO_EMCY_CODE_VOL_ERR               0x3000     /*!< voltage                                */
#define CO_EMCY_CODE_VOL_INPUT_ERR         0x3100     /*!< mains voltage                          */
#define CO_EMCY_CODE_VOL_INTERN_ERR        0x3200     /*!< voltage inside the device              */
#define CO_EMCY_CODE_VOL_OUTPUT_ERR        0x3300     /*!< output voltage                         */
#define CO_EMCY_CODE_TEMP_ERR              0x4000     /*!< temperature                            */
#define CO_EMCY_CODE_TEMP_AMBIENT_ERR      0x4100     /*!< ambient temperature                    */
#define CO_EMCY_CODE_TEMP_DEVICE_ERR       0x4200     /*!< device temperature                     */
#define CO_EMCY_CODE_HW_ERR                0x5000     /*!< device hardware                        */
#define CO_EMCY_CODE_SW_ERR                0x6000     /*!< device software                        */
#define CO_EMCY_CODE_SW_INTERNAL_ERR       0x6100     /*!< internal software                      */
#define CO_EMCY_CODE_SW_USER_ERR           0x6200     /*!< user software                          */
#define CO_EMCY_CODE_SW_DATASET_ERR        0x6300     /*!< data set                               */
#define CO_EMCY_CODE_ADD_MODULES_ERR       0x7000     /*!< additional modules                     */
#define CO_EMCY_CODE_MON_ERR               0x8000     /*!< monitoring                             */
#define CO_EMCY_CODE_MON_COM_ERR           0x8100     /*!< communication                          */
#define CO_EMCY_CODE_MON_COM_OVERRUN_ERR   0x8110     /*!< CAN overrun (objects lost)             */
#define CO_EMCY_CODE_MON_COM_PASSIVE_ERR   0x8120     /*!< CAN in error passive mode              */
#define CO_EMCY_CODE_MON_COM_HEARTBEAT_ERR 0x8130     /*!< life guard error or heartbeat error    */
#define CO_EMCY_CODE_MON_COM_RECOVER_ERR   0x8140     /*!< recovered from bus off                 */
#define CO_EMCY_CODE_MON_COM_COLLISION_ERR 0x8150     /*!< transmit COB-ID collision              */
#define CO_EMCY_CODE_MON_PROT_ERR          0x8200     /*!< protocol error                         */
#define CO_EMCY_CODE_MON_PROT_PDO_IGN_ERR  0x8210     /*!< PDO not processed due to length error  */
#define CO_EMCY_CODE_MON_PROT_PDO_LEN_ERR  0x8220     /*!< PDO length exceeded                    */
#define CO_EMCY_CODE_EXT_ERR               0x9000     /*!< external error                         */
#define CO_EMCY_CODE_ADD_FUNC_ERR          0xF000     /*!< additional functions                   */
#define CO_EMCY_CODE_DEV_ERR               0xFF00     /*!< device specific                        */

/*------------------------------------------------------------------------------------------------*/
/*! \brief EMCY REGISTER BITS
*
* \ingroup EMCY
*
*          The following defines holds the bit-number within the error register object (1001h) in
*          the object directory. These values can be used to specify a standard conform EMCY
*          error code relation table.
*
* \see CO_EMCY_TBL
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_EMCY_REG_GENERAL        0                  /*!< general error (includes all classes)   */
#if CO_EMCY_REG_CLASS_EN > 0
#define CO_EMCY_REG_CURRENT        1                  /*!< error class: current                   */
#define CO_EMCY_REG_VOLTAGE        2                  /*!< error class: voltage                   */
#define CO_EMCY_REG_TEMP           3                  /*!< error class: temperature               */
#define CO_EMCY_REG_COM            4                  /*!< error class: communication             */
#define CO_EMCY_REG_PROFILE        5                  /*!< error class: profile specific error    */
#define CO_EMCY_REG_MANUFACTURER   7                  /*!< error class: manufacturer specific     */
#define CO_EMCY_REG_NUM            8                  /*!< number of supported error classes      */
#else
#define CO_EMCY_REG_NUM            1                  /*!< number of supported error classes      */
#endif

/*------------------------------------------------------------------------------------------------*/
/*! \brief EMCY MANUFACTURER SPECIFIC FIELDS
*
* \ingroup EMCY
*
*          This structure holds the optional manufacturer specific fields for the EMCY message
*          and for the EMCY history.
*
* \note    To reduce memory consumption, the configuration values of \ref CO_EMCY_HIST_MAN_EN
*          and \ref CO_EMCY_EMCY_MAN_EN may remove some bytes. If no manufacturer specific
*          field is enabled, the structure holds an unused dummy byte.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_EMCY_USR_T {
#if CO_EMCY_HIST_MAN_EN > 0
    CPU_INT16U            Hist;                       /*!< manufacturer specific field in History */
#endif
#if CO_EMCY_EMCY_MAN_EN > 0
    CPU_INT08U            Emcy[5];                    /*!< manufacturer specific field in EMCY    */
#endif
#if CO_EMCY_HIST_MAN_EN == 0 && CO_EMCY_EMCY_MAN_EN == 0
    CPU_INT08U            Dummy;                      /*!< manufacturer specific field is unused  */
#endif

} CO_EMCY_USR;

/*------------------------------------------------------------------------------------------------*/
/*! \brief EMCY CODE DEFINITION TABLE
*
* \ingroup EMCY
*
*          This structure holds the EMCY code and the relation to the error register bit.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_EMCY_TBL_T {
    CPU_INT08U Reg;                                   /*!< bit number (0..7) in error register    */
    CPU_INT16U Code;                                  /*!< error code (category see CO_EMCY_CODE) */

} CO_EMCY_TBL;

/*------------------------------------------------------------------------------------------------*/
/*! \brief EMCY MANAGEMENT
*
* \ingroup EMCY
*
*          This structure holds the EMCY defintion table and informations to manage the change
*          detection on all individual EMCY codes.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_EMCY_T {
    struct CO_NODE_T      *Node;                      /*!< link to parent node information        */
    struct CO_EMCY_TBL_T  *Root;                      /*!< root pointer to user EMCY table        */
#if CO_EMCY_HIST_EN > 0
    struct CO_EMCY_HIST_T  Hist;                      /*!< EMCY history informations              */
#endif
    CPU_INT08U             Cnt[CO_EMCY_REG_NUM];      /*!< Count occurance of register bits       */
    CPU_INT08U             Err[CO_EMCY_STORAGE];      /*!< individual error code status storage   */

} CO_EMCY;

/*
****************************************************************************************************
*                                         FUNCTION PROTOTYPES
****************************************************************************************************
*/

#if CO_EMCY_N > 0
void       COEmcySet         (CO_EMCY *emcy, CPU_INT08U err, CO_EMCY_USR *usr);
void       COEmcyClr         (CO_EMCY *emcy, CPU_INT08U err);
CPU_INT16S COEmcyGet         (CO_EMCY *emcy, CPU_INT08U err);
CPU_INT16S COEmcyCnt         (CO_EMCY *emcy);
void       COEmcyReset       (CO_EMCY *emcy, CPU_INT08U silent);
#endif

/*
****************************************************************************************************
*                                    INTERNAL FUNCTION PROTOTYPES
****************************************************************************************************
*/

#if CO_EMCY_N > 0
void       CO_EmcyInit       (CO_EMCY *emcy, struct CO_NODE_T *node, CO_EMCY_TBL *root);
CPU_INT16S CO_EmcyCheck      (CO_EMCY *emcy);
CPU_INT16S CO_EmcyGetErr     (CO_EMCY *emcy, CPU_INT08U err);
CPU_INT16S CO_EmcySetErr     (CO_EMCY *emcy, CPU_INT08U err, CPU_INT08U state);
void       CO_EmcySend       (CO_EMCY *emcy, CPU_INT08U err, CO_EMCY_USR *usr, CPU_INT08U state);
void       CO_EmcyUpdate     (CO_EMCY *emcy, CPU_INT08U err, CO_EMCY_USR *usr, CPU_INT08U state);
#endif

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_EMCY_H_ */
