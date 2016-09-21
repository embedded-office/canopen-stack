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
* \file     co_sdo.h
*
* \brief    SDO SERVER DEFINITIONS
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_sdo.h#3 $
*
*           This include file defines the symbolic constants and data types for the SDO protocol
*           elements.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_SDO_H_
#define CO_SDO_H_

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
#include "co_if.h"
#if CO_SDO_BLK_EN > 0 || CO_SDO_SEG_EN > 0 || CO_SDO_DYN_ID_EN > 0
#include "co_fsdo.h"
#endif

/*
****************************************************************************************************
*                                     SDO PROTOCOL ERROR CODES
****************************************************************************************************
*/

#define CO_SDO_ERR_CMD                0x05040001      /*!< SDO command specifier invalid/unknown  */
#define CO_SDO_ERR_RD                 0x06010001      /*!< Attempt to read a write only object    */
#define CO_SDO_ERR_WR                 0x06010002      /*!< Attempt to write a read only object    */
#define CO_SDO_ERR_OBJ                0x06020000      /*!< Object doesn't exist in directory      */
#define CO_SDO_ERR_LEN_HIGH           0x06070012      /*!< Length of parameter too high           */
#define CO_SDO_ERR_LEN_SMALL          0x06070013      /*!< Length of parameter too small          */
#define CO_SDO_ERR_SUB                0x06090011      /*!< Subindex doesn't exist in directory    */
#define CO_SDO_ERR_RANGE              0x06090030      /*!< Value range of parameter exceeded      */
#define CO_SDO_ERR_TOS                0x08000020      /*!< Data can't be transfered or stored     */
#define CO_SDO_ERR_TOS_STATE          0x08000022      /*!< Data can't be transfered or stored,    */
                                                      /*   because of the present device state    */
#define CO_SDO_ERR_OBJ_MAP            0x06040041      /*!< Object cannot be mapped to the PDO     */
#define CO_SDO_ERR_OBJ_MAP_N          0x06040042      /*!< Number and length exceed PDO           */
#define CO_SDO_ERR_PARA_INCOMP        0x06040043      /*!< parameter incompatibility reason       */

/*
****************************************************************************************************
*                                             DEFINES
****************************************************************************************************
*/

#define CO_SDO_RD                     1               /*!< Object read access                     */
#define CO_SDO_WR                     2               /*!< Object write access                    */

#define CO_SDO_COBID_OFF   ((CPU_INT32U)1<<31)        /*!< Disabled SDO server / COBID            */

/*
****************************************************************************************************
*                                         TYPE DEFINITIONS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief SDO SERVER
*
* \ingroup SDO
*
*          This structure holds all data, which are needed for managing a single SDO server.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_SDO_T {
    struct CO_NODE_T    *Node;                        /*!< Link to node info structure            */
    struct CO_OBJ_T     *Obj;                         /*!< Addressed Object Entry Reference       */
    CO_IF_FRM           *Frm;                         /*!< SDO request/response CAN frame         */
    CPU_INT32U           RxId;                        /*!< SDO request CAN identifier             */
    CPU_INT32U           TxId;                        /*!< SDO response CAN identifier            */
    CPU_INT16U           Idx;                         /*!< Extracted Multiplexer Index            */
    CPU_INT08U           Sub;                         /*!< Extracted Multiplexer Subindex         */
#if CO_SDO_SEG_EN > 0 || CO_SDO_BLK_EN > 0
    struct CO_SDO_BUF_T  Buf;                         /*!< Transfer buffer management structure   */
#endif
#if CO_SDO_SEG_EN > 0
    struct CO_SDO_SEG_T  Seg;                         /*!< Segmented transfer control structure   */
#endif
#if CO_SDO_BLK_EN > 0
    struct CO_SDO_BLK_T  Blk;                         /*!< Block transfer control structure       */
#endif

} CO_SDO;

/*
****************************************************************************************************
*                                       FUNCTION PROTOTYPES
****************************************************************************************************
*/

void       CO_SdoInit              (CO_SDO *srv, struct CO_NODE_T *node);
void       CO_SdoReset             (CO_SDO *srv, CPU_INT08U num, struct CO_NODE_T *node);
void       CO_SdoEnable            (CO_SDO *srv, CPU_INT08U num);
CO_SDO    *CO_SdoCheck             (CO_SDO *srv, CO_IF_FRM *frm);
CPU_INT16S CO_SdoResponse          (CO_SDO *srv);
void       CO_SdoAbortReq          (CO_SDO *srv);
void       CO_SdoAbort             (CO_SDO *srv, CPU_INT32U err);
CPU_INT16S CO_SdoGetObject         (CO_SDO *srv, CPU_INT16U mode);
CPU_INT32U CO_SdoGetSize           (CO_SDO *srv, CPU_INT32U width);
CPU_INT16S CO_SdoDownloadExpedited (CO_SDO *srv);
CPU_INT16S CO_SdoUploadExpedited   (CO_SDO *srv);

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_SDO_H_ */
