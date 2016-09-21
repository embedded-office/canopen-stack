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
* \file     co_tpdo.h
*
* \brief    TPDO MANAGEMENT DEFINITIONS
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_tpdo.h#3 $
*
*           This include file defines the symbolic constants and data types for the PDO handling.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_TPDO_H_
#define CO_TPDO_H_

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

#define CO_TPDO_COBID_OFF    ((uint32_t)1 << 31)    /*!< CAN identifier, marked as unused       */
#define CO_TPDO_COBID_REMOTE ((uint32_t)1 << 30)    /*!< CAN identifier, RTR is not allowed     */
#define CO_TPDO_COBID_EXT    ((uint32_t)1 << 29)    /*!< CAN identifier, extended format        */

#define CO_TPDO_FLG___E       0x01                    /*!< PDO event occured                      */
#define CO_TPDO_FLG__I_       0x02                    /*!< PDO TX inhibited                       */
#define CO_TPDO_FLG__IE       0x03                    /*!< PDO event occured + TX inhibited       */
#define CO_TPDO_FLG_S__       0x04                    /*!< PDO synced                             */
#define CO_TPDO_FLG_S_E       0x05                    /*!< PDO synced + event occured             */
#define CO_TPDO_FLG_SI_       0x06                    /*!< PDO synced + TX inhibited              */
#define CO_TPDO_FLG_SIE       0x07                    /*!< PDO synved + event occured + TX inh.   */

#define CO_TPDO_ASYNC         1                       /*!< Ctrl function code: asynchronous TPDO  */

#define CO_TPDO_TMR_MIN  (1000/CO_TMR_TICKS_PER_SEC)  /*!< event timer value granularity          */

/*lint -emacro( {734}, CO_TPDO_ROUND ) : Loss of precision (assignment) is ok */
#define CO_TPDO_ROUND(x,d)  (((x)/(d))*(d))

/*lint -emacro( {734}, CO_TPDO_MS ) : Loss of precision (assignment) is ok */
#define CO_TPDO_MS(x)       ((((uint32_t)(x))*CO_TMR_TICKS_PER_SEC)/1000)

/*
****************************************************************************************************
*                                         TYPE DEFINITIONS
****************************************************************************************************
*/

#if CO_TPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief TPDO SIGNAL LINK TABLE
*
* \ingroup TPDO
*
*          This structure holds all data, which are needed for managing the links from a signal
*          to all TPDOs, which has active mapping entries to this signal.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_TPDO_LINK_T {
    struct CO_OBJ_T *Obj;                             /*!< pointer to object                      */
    uint16_t       Num;                             /*!< currently mapped to TPDO-num (0..511)  */

} CO_TPDO_LINK;
#endif

#if CO_TPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief TPDO DATA
*
* \ingroup TPDO
*
*          This structure holds all data, which are needed for managing a single TPDO.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_TPDO_T {
    struct CO_NODE_T *Node;                           /*!< link to parent CANopen node            */
    uint32_t        Identifier;                     /*!< message identifier                     */
    struct CO_OBJ_T  *Map[8];                         /*!< pointer list with mapped objects       */
    int16_t        EvTmr;                          /*!< event timer id                         */
    uint16_t        Event;                          /*!< event time in timer ticks              */
    int16_t        InTmr;                          /*!< inhibit timer id                       */
    uint16_t        Inhibit;                        /*!< inhibit time in timer ticks            */
    uint8_t        Flags;                          /*!< info flags                             */
    uint8_t        ObjNum;                         /*!< Number of linked objects               */

} CO_TPDO;
#endif

/*
****************************************************************************************************
*                                     OBJECT TYPE DEFINITION
****************************************************************************************************
*/

#if CO_TPDO_N > 0
extern CO_OBJ_TYPE COTAsync;                          /* Link to Parameter Object Type Structure  */
#define CO_TASYNC        ((CO_OBJ_TYPE *)&COTAsync)   /*!< Object Type Asynchronous TPDO Object   */

extern CO_OBJ_TYPE COTEvent;                          /* Link to Parameter Object Type Structure  */
#define CO_TEVENT        ((CO_OBJ_TYPE *)&COTEvent)   /*!< Object Type TPDO Event Timer           */
#endif

/*
****************************************************************************************************
*                                       FUNCTION PROTOTYPES
****************************************************************************************************
*/

#if CO_TPDO_N > 0
void       COTPdoTrigObj      (CO_TPDO *tpdo, struct CO_OBJ_T *obj);
void       COTPdoTrigPdo      (CO_TPDO *tpdo, uint16_t num);
#endif

/*
****************************************************************************************************
*                                    INTERNAL FUNCTION PROTOTYPES
****************************************************************************************************
*/

#if CO_TPDO_N > 0
void       CO_TPdoClear       (CO_TPDO *pdo, struct CO_NODE_T *node);
void       CO_TPdoInit        (CO_TPDO *pdo, struct CO_NODE_T *node);
void       CO_TPdoReset       (CO_TPDO *pdo, uint16_t num);
int16_t CO_TPdoGetMap      (CO_TPDO *pdo, uint16_t num);
void       CO_TPdoTmrEvent    (void *parg);
void       CO_TPdoEndInhibit  (void *parg);
void       CO_TPdoTx          (CO_TPDO *pdo);

void       CO_TPdoMapClear    (CO_TPDO_LINK *map);
void       CO_TPdoMapAdd      (CO_TPDO_LINK *map, struct CO_OBJ_T *obj, uint16_t num);
void       CO_TPdoMapDelNum   (CO_TPDO_LINK *map, uint16_t num);
void       CO_TPdoMapDelSig   (CO_TPDO_LINK *map, struct CO_OBJ_T *obj);
#endif

/*
****************************************************************************************************
*                                    CALLBACK FUNCTION PROTOTYPES
****************************************************************************************************
*/

void       CO_TPdoTransmit    (CO_IF_FRM *frm);

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_TPDO_H_ */
