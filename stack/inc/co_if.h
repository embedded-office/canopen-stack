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
* \file     co_if.h
*
* \brief    INTERFACE DEFINITIONS
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_if.h#3 $
*
*           This source file contains the interface definitions.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_IF_H_
#define CO_IF_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
****************************************************************************************************
*                                             INCLUDES
****************************************************************************************************
*/

#include "drv_can.h"                                  /* CAN device driver definitions            */
#include "can_bus.h"                                  /* CAN bus management                       */

/*
****************************************************************************************************
*                                              MACROS
****************************************************************************************************
*/

#define CO_GET_COBID(f)   ((CPU_INT32U)(f)->Identifier)

#define CO_SET_COBID(f,n) ((f)->Identifier=(CPU_INT32U)(n))

#define CO_GET_DLC(f)     ((CPU_INT08U)(f)->DLC)

#define CO_SET_DLC(f,n)   ((f)->DLC=(CPU_INT08U)(n))

#define CO_GET_BYTE(f,p)  (CPU_INT08U)(                  \
                            (CPU_INT08U)(f)->Data[(p)&0x7] \
                          )

#define CO_GET_WORD(f,p)  (CPU_INT16U)(                                     \
                            ( ( (CPU_INT16U)((f)->Data[((p)+1)&0x7]) ) << 8 ) | \
                            ( ( (CPU_INT16U)((f)->Data[((p)  )&0x7]) )      )   \
                          )

#define CO_GET_LONG(f,p)  (CPU_INT32U)(                                      \
                            ( ( (CPU_INT32U)((f)->Data[((p)+3)&0x7]) ) << 24 ) | \
                            ( ( (CPU_INT32U)((f)->Data[((p)+2)&0x7]) ) << 16 ) | \
                            ( ( (CPU_INT32U)((f)->Data[((p)+1)&0x7]) ) <<  8 ) | \
                            ( ( (CPU_INT32U)((f)->Data[((p)  )&0x7]) )       )   \
                          )

#define CO_SET_BYTE(f,n,p) {(f)->Data[(p)&0x7]=(CPU_INT08U)(n);}

/*lint -emacro( {572,778}, CO_SET_WORD ) : shifting values to 0 is ok */
#define CO_SET_WORD(f,n,p) {(f)->Data[(p)&0x7]=(CPU_INT08U)((CPU_INT16U)(n));\
                            (f)->Data[((p)+1)&0x7]=(CPU_INT08U)(((CPU_INT16U)(n))>>8);}

/*lint -emacro( {572,778}, CO_SET_LONG ) : shifting values to 0 is ok */
#define CO_SET_LONG(f,n,p) {(f)->Data[(p)&0x7]=(CPU_INT08U)((CPU_INT32U)(n));\
                            (f)->Data[((p)+1)&0x7]=(CPU_INT08U)(((CPU_INT32U)(n))>>8);\
                            (f)->Data[((p)+2)&0x7]=(CPU_INT08U)(((CPU_INT32U)(n))>>16);\
                            (f)->Data[((p)+3)&0x7]=(CPU_INT08U)(((CPU_INT32U)(n))>>24);}

/*
****************************************************************************************************
*                                        TYPE DEFINITIONS
****************************************************************************************************
*/

#define CO_IF_DRV     CANBUS_PARA*                    /*!< driver specific bus identifier type    */

struct CO_NODE_T;                                     /* Declaration of node structure            */

typedef struct CO_IF_FRM_T {                          /*!< Type, which represents a CAN frame     */
    CPU_INT32U        Identifier;                     /*!< CAN message identifier                 */
    CPU_INT08U        Data[8];                        /*!< CAN message Data (payload)             */
    CPU_INT08U        DLC;                            /*!< CAN message data length code (DLC)     */

} CO_IF_FRM;

typedef struct CO_IF_T {                              /*!< Driver interface structure             */
    struct CO_NODE_T *Node;                           /*!< Link to parent node                    */
    CO_IF_DRV         Drv;                            /*!< Specific bus identifier of CAN driver  */

} CO_IF;

/*
****************************************************************************************************
*                                       FUNCTION PROTOTYPES
****************************************************************************************************
*/

CPU_INT16S COIfRead     (CO_IF *cif, CO_IF_FRM *frm);
CPU_INT16S COIfSend     (CO_IF *cif, CO_IF_FRM *frm);
void       COIfReset    (CO_IF *cif);
void       COIfClose    (CO_IF *cif);

/*
****************************************************************************************************
*                                   INTERNAL FUNCTION PROTOTYPES
****************************************************************************************************
*/

void       CO_IfInit    (CO_IF *cif, struct CO_NODE_T *node);
void       CO_IfEnable  (CO_IF *cif, CPU_INT32U baudrate);

/*
****************************************************************************************************
*                                  CALLBACK FUNCTION PROTOTYPES
****************************************************************************************************
*/

void       CO_IfReceive (CO_IF_FRM *frm);

#ifdef __cplusplus
}
#endif

#endif /* CO_IF_H_ */
