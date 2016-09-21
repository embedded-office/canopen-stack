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
* \file     co_hbcons.h
*
* \brief    HEARTBEAT CONSUMER DEFINITIONS
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_hbcons.h#2 $
*
*           This include file defines the symbolic constants and data types for the CANopen
*           heartbeat consumer handling.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_HBCONS_H_
#define CO_HBCONS_H_

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
#include "co_nmt.h"
#include "co_err.h"

/*
****************************************************************************************************
*                                     OBJECT TYPE DEFINITION
****************************************************************************************************
*/

extern CO_OBJ_TYPE COTNmtHbCons;                      /* Link to Heartbeat Consumer Object Type   */

#define CO_THB_CONS ((CO_OBJ_TYPE *)&COTNmtHbCons)    /*!< Object Type Heartbeat Consumer         */

/*
****************************************************************************************************
*                                        TYPE DEFINITIONS
****************************************************************************************************
*/

struct CO_NODE_T;
struct CO_HBCONS_T;

/*------------------------------------------------------------------------------------------------*/
/*! \brief HEARTBEAT CONSUMER STRUCTURE
*
* \ingroup OBJ
*
*          This structure holds all data, which are needed for the heartbeat consumer handling
*          within the object directory.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_HBCONS_T {
    struct CO_NODE_T   *Node;                         /*!< Link to parent node                    */
    struct CO_HBCONS_T *Next;                         /*!< Link to next consumer in active chain  */
    CO_MODE             State;                        /*!< Received Node-State                    */
    int16_t          Tmr;                          /*!< Timer Identifier                       */
    uint16_t          Time;                         /*!< Time   (Bit00-15 when read object)     */
    uint8_t          NodeId;                       /*!< NodeId (Bit16-23 when read object)     */
    uint8_t          Event;                        /*!< Event Counter                          */

} CO_HBCONS;

/*
****************************************************************************************************
*                                     API FUNCTION PROTOTYPES
****************************************************************************************************
*/

int16_t CONmtGetHbEvents     (CO_NMT *nmt, uint8_t nodeId);
CO_MODE    CONmtLastHbState     (CO_NMT *nmt, uint8_t nodeId);

/*
****************************************************************************************************
*                                   INTERNAL FUNCTION PROTOTYPES
****************************************************************************************************
*/

void       CO_NmtHbConsInit     (CO_NMT *nmt);
CO_ERR     CO_NmtHbConsActivate (CO_NMT *nmt, CO_HBCONS *hbc, uint16_t time, uint8_t nodeid);
int16_t CO_NmtHbConsCheck    (CO_NMT *nmt, CO_IF_FRM *frm);

/*
****************************************************************************************************
*                                  CALLBACK FUNCTION PROTOTYPES
****************************************************************************************************
*/

void       CO_NmtHbConsEvent    (CO_NMT *nmt, uint8_t nodeId);
void       CO_NmtHbConsChange   (CO_NMT *nmt, uint8_t nodeId, CO_MODE mode);

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_HBCONS_H_ */
