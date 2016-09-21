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
* \file     co_hbpro.h
*
* \brief    HEARTBEAT PRODUCER DEFINITIONS
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_hbprod.h#2 $
*
*           This include file defines the symbolic constants and data types for the CANopen
*           heartbeat producer handling.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_HBPROD_H_
#define CO_HBPROD_H_

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

/*
****************************************************************************************************
*                                     OBJECT TYPE DEFINITION
****************************************************************************************************
*/

extern CO_OBJ_TYPE COTNmtHbProd;                      /* Link to Heartbeat Producer Object Type   */

#define CO_THB_PROD    ((CO_OBJ_TYPE *)&COTNmtHbProd) /*!< Object Type Heartbeat Producer         */
#define CO_THEARTBEAT  CO_THB_PROD                    /*!< Definition for backward compatibility  */

/*
****************************************************************************************************
*                                   INTERNAL FUNCTION PROTOTYPES
****************************************************************************************************
*/

void       CO_NmtHbProdInit  (CO_NMT *nmt);
void       CO_NmtHbProdSend  (void *parg);

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_HBPRO_H_ */
