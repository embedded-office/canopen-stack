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
* \file     co_nmt.h
*
* \brief    NMT SLAVE DEFINITIONS
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_nmt.h#3 $
*
*           This include file defines the symbolic constants and data types for the CANopen NMT
*           handling.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_NMT_H_
#define CO_NMT_H_

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

#define CO_NMT_ALLOWED   0x01                         /*!< indication of NMT transfers allowed    */
#define CO_BOOT_ALLOWED  0x02                         /*!< indication of BOOT transfers allowed   */
#define CO_EMCY_ALLOWED  0x04                         /*!< indication of EMCY transfers allowed   */
#define CO_TIME_ALLOWED  0x08                         /*!< indication of TIME transfers allowed   */
#define CO_SYNC_ALLOWED  0x10                         /*!< indication of SYNC transfers allowed   */
#define CO_SDO_ALLOWED   0x20                         /*!< indication of SDO transfers allowed    */
#define CO_PDO_ALLOWED   0x40                         /*!< indication of PDO transfers allowed    */

/*
****************************************************************************************************
*                                       TYPE DEFINITIONS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief RESET TYPE
*
* \ingroup NMT
*
*          This enumeration holds all possible reset types.
*/
/*------------------------------------------------------------------------------------------------*/
typedef enum CO_NMT_RESET_T {
    CO_RESET_NODE = 0,                                /*!< reset application (and communication)  */
    CO_RESET_COM,                                     /*!< reset communication                    */

    CO_RESET_NUM                                      /*!< number of reset types                  */
} CO_NMT_RESET;

/*------------------------------------------------------------------------------------------------*/
/*! \brief DEVICE MODE
*
* \ingroup NMT
*
*          This enumeration holds all possible CANopen device modes.
*/
/*------------------------------------------------------------------------------------------------*/
typedef enum CO_MODE_T {
    CO_INVALID = 0,                                   /*!< device in INVALID mode                 */
    CO_INIT,                                          /*!< device in INIT mode                    */
    CO_PREOP,                                         /*!< device in PRE-OPERATIONAL mode         */
    CO_OPERATIONAL,                                   /*!< device in OPERATIONAL mode             */
    CO_STOP,                                          /*!< device in STOP mode                    */
    CO_MODE_NUM                                       /*!< number of device modes                 */

} CO_MODE;

/*------------------------------------------------------------------------------------------------*/
/*! \brief NMT MANAGEMENT
*
* \ingroup NMT
*
*          This structure holds all data, which are needed for the NMT state machine management.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_NMT_T {
    struct CO_NODE_T   *Node;                         /*!< ptr to parent CANopen node info        */
    struct CO_HBCONS_T *HbCons;                       /*!< The used heartbeat consumer chain      */
    enum CO_MODE_T      Mode;                         /*!< NMT mode of this node                  */
    int16_t          Tmr;                          /*!< heartbeat producer timer identifier    */
    uint8_t          Allowed;                      /*!< encoding of allowed CAN objects        */

} CO_NMT;

/*
****************************************************************************************************
*                                       FUNCTION PROTOTYPES
****************************************************************************************************
*/

void       CONmtReset       (CO_NMT *nmt, CO_NMT_RESET type);
void       CONmtSetMode     (CO_NMT *nmt, CO_MODE mode);
CO_MODE    CONmtGetMode     (CO_NMT *nmt);
void       CONmtSetNodeId   (CO_NMT *nmt, uint8_t nodeId);
uint8_t CONmtGetNodeId   (CO_NMT *nmt);

CO_MODE    CONmtModeDecode  (uint8_t code);
uint8_t CONmtModeEncode  (CO_MODE mode);

/*
****************************************************************************************************
*                                   INTERNAL FUNCTION PROTOTYPES
****************************************************************************************************
*/

void       CO_NmtInit       (CO_NMT *nmt, struct CO_NODE_T *node);
void       CO_NmtBootup     (CO_NMT *nmt);
int16_t CO_NmtCheck      (CO_NMT *nmt, CO_IF_FRM *frm);

/*
****************************************************************************************************
*                                  CALLBACK FUNCTION PROTOTYPES
****************************************************************************************************
*/

void       CO_NmtModeChange (CO_NMT *nmt, CO_MODE mode);

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_NMT_H_ */
