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
* \file     co_fsdo.h
*
* \brief    FULL SDO SERVER DEFINITIONS
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_fsdo.h#2 $
*
*           This include file defines the symbolic constants and data types for the SDO protocol
*           elements.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_FSDO_H_
#define CO_FSDO_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
****************************************************************************************************
*                                             INCLUDES
****************************************************************************************************
*/

#if CO_SDO_DYN_ID_EN > 0
#include "co_obj.h"
#endif

/*
****************************************************************************************************
*                                     SDO PROTOCOL ERROR CODES
****************************************************************************************************
*/

#define CO_SDO_ERR_TBIT               0x05030000      /*!< Toggle bit not alternated              */
#define CO_SDO_ERR_BLK_SIZE           0x05040002      /*!< Invalid block size                     */
#define CO_SDO_ERR_SEQ_NUM            0x05040003      /*!< Invalid Sequence number                */

/*
****************************************************************************************************
*                                             DEFINES
****************************************************************************************************
*/

#if CO_SDO_BLK_EN > 0 || CO_SDO_SEG_EN > 0
#define CO_SDO_BUF_BYTE     (CO_SDO_BUF_SEG*7)        /*!< transfer buffer size in byte           */
#endif

/*
****************************************************************************************************
*                                       OBJECT TYPE DEFINITIONS
****************************************************************************************************
*/

#if CO_SDO_DYN_ID_EN > 0
extern CO_OBJ_TYPE COTSdoId;                          /* Link to SDO-Id Object Type Structure     */

#define CO_TSDOID    ((CO_OBJ_TYPE *)&COTSdoId)       /*!< Object Type Dynamic SDO Identifier     */
#endif

/*
****************************************************************************************************
*                                         TYPE DEFINITIONS
****************************************************************************************************
*/

#if CO_SDO_SEG_EN > 0 || CO_SDO_BLK_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief SDO TRANSFER BUFFER
*
* \ingroup SDO
*
*          This structure holds the data, which are needed for the SDO transfer buffer.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_SDO_BUF_T {
    CPU_INT32U  Num;                                  /*!< Number of bytes in transfer buffer     */
    CPU_INT08U *Start;                                /*!< Pointer to start of transfer buffer    */
    CPU_INT08U *Cur;                                  /*!< Pointer to next free buffer location   */

} CO_SDO_BUF;
#endif

#if CO_SDO_SEG_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief SDO SEGMENTED TRANSFER
*
* \ingroup SDO
*
*          This structure holds the data, which are needed for the segmented SDO transfer.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_SDO_SEG_T {
    CPU_INT32U  Size;                                 /*!< Size of object entry                   */
    CPU_INT32U  Num;                                  /*!< Number of transfered bytes             */
    CPU_INT08U  TBit;                                 /*!< Segment toggle bit                     */

} CO_SDO_SEG;
#endif

#if CO_SDO_BLK_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief SDO BLOCK TRANSFER STATES
*
* \ingroup SDO
*
*          This enumeration holds the possible SDO server block transfer states.
*/
/*------------------------------------------------------------------------------------------------*/
typedef enum  CO_SDO_BLK_STATE_T {
    BLK_IDLE,                                         /*!< no block transfer ongoing              */
    BLK_DOWNLOAD,                                     /*!< block download active                  */
    BLK_UPLOAD,                                       /*!< block upload active                    */
    BLK_REPEAT,                                       /*!< block upload repeat request active     */
    BLK_DNWAIT                                        /*!< block download wait for next block/end */

} CO_SDO_BLK_STATE;
#endif

#if CO_SDO_BLK_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief SDO BLOCK TRANSFER
*
* \ingroup SDO
*
*          This structure holds the data, which are needed for the SDO block transfer.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_SDO_BLK_T {
    enum CO_SDO_BLK_STATE_T State;                    /*!< block transfer state                   */
    CPU_INT32U              Size;                     /*!< Size of object entry                   */
    CPU_INT32U              Len;                      /*!< remaining block length in byte         */
    CPU_INT08U              SegNum;                   /*!< number of segments in block            */
    CPU_INT08U              SegCnt;                   /*!< current segment number                 */
    CPU_INT08U              LastValid;                /*!< number of valid bytes in last segment  */

} CO_SDO_BLK;
#endif

/*
****************************************************************************************************
*                                       FUNCTION PROTOTYPES
****************************************************************************************************
*/

struct CO_SDO_T;                                      /* Declaration of SDO server object         */

#if CO_SDO_SEG_EN > 0
CPU_INT16S CO_SdoInitUploadSegmented     (struct CO_SDO_T *srv, CPU_INT32U width);
CPU_INT16S CO_SdoUploadSegmented         (struct CO_SDO_T *srv);
CPU_INT16S CO_SdoInitDownloadSegmented   (struct CO_SDO_T *srv);
CPU_INT16S CO_SdoDownloadSegmented       (struct CO_SDO_T *srv);
#endif

#if CO_SDO_BLK_EN > 0
CPU_INT16S CO_SdoInitDownloadBlock       (struct CO_SDO_T *srv);
CPU_INT16S CO_SdoDownloadBlock           (struct CO_SDO_T *srv);
CPU_INT16S CO_SdoEndDownloadBlock        (struct CO_SDO_T *srv);
CPU_INT16S CO_SdoInitUploadBlock         (struct CO_SDO_T *srv);
CPU_INT16S CO_SdoUploadBlock             (struct CO_SDO_T *srv);
CPU_INT16S CO_SdoAckUploadBlock          (struct CO_SDO_T *srv);
CPU_INT16S CO_SdoEndUploadBlock          (struct CO_SDO_T *srv);
#endif

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_FSDO_H_ */
