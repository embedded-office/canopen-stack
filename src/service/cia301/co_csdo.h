/******************************************************************************
   Copyright 2020 Embedded Office GmbH & Co. KG

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
******************************************************************************/

#ifndef CO_CSDO_H_
#define CO_CSDO_H_

#if defined __cplusplus
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"
#include "co_sdo.h"
#include "co_if.h"

/*****************************************************************************
 * Constants
 * **************************************************************************/
#define READ_BITS(x,offset,mask)    (((x) >> (offset)) & (mask))
#define CLEAR_SET_BITS(x,val,offset,mask)   \
    do{                                     \
        x &= ~((mask) << (offset));         \
        x |= ((val) << (offset));           \
    } while(0);
#define SET_BITS(val,offset,mask)   (((val)  & (mask)) << (offset))
#define CLEAR_BITS(x,offset, mask)  (x &= ~((mask) << (offset)))
        

/***** Block Download CMD bit offsets ****************************************/
// server/client command specifier
#define BLOCK_DOWNLOAD_CMD_SCS  5
#define BLOCK_DOWNLOAD_CMD_CCS  6
#define BLOCK_DOWNLOAD_CMD_SCS_CCS_BIT_OFFSET       5
#define BLOCK_DOWNLOAD_CMD_SCS_CCS_BIT_MASK         0b111

// server client CRC support
#define BLOCK_DOWNLOAD_CMD_SC_CC_CRC_NOT_SUPPORTED  0
#define BLOCK_DOWNLOAD_CMD_SC_CC_CRC_SUPPORTED      1

// download init size indicator
#define BLOCK_DOWNLOAD_CMD_S_SIZE_NOT_INDICATED     0
#define BLOCK_DOWNLOAD_CMD_S_SIZE_INDICATED         1

// server/client subcommand
#define BLOCK_DOWNLOAD_CMD_SS_CS_INITIATE       0
#define BLOCK_DOWNLOAD_CMD_SS_CS_END            1
#define BLOCK_DOWNLOAD_CMD_SS_DOWNLOAD_RESPONSE 2
#define BLOCK_DOWNLOAD_CMD_CS_BIT_OFFSET        0
#define BLOCK_DOWNLOAD_CMD_CS_BIT_MASK          0b1
#define BLOCK_DOWNLOAD_CMD_SS_BIT_OFFSET        0
#define BLOCK_DOWNLOAD_CMD_SS_BIT_MASK          0b11

// client continue bit
#define BLOCK_DOWNLOAD_CMD_C_CONTINUE_SEGMENTS  0
#define BLOCK_DOWNLOAD_CMD_C_NO_MORE_SEGMENTS   1
#define BLOCK_DOWNLOAD_CMD_C_BIT_OFFSET         7
#define BLOCK_DOWNLOAD_CMD_C_BIT_MASK           0b1

// init cmd bit offsets
#define BLOCK_DOWNLOAD_INIT_REQUEST_CMD_CRC_BIT_OFFSET  2 
#define BLOCK_DOWNLOAD_INIT_REQUEST_CMD_S_BIT_OFFSET    1

// sub-block seqnum
#define BLOCK_DOWNLOAD_SUBBLOCK_CMD_SEQNUM_BIT_OFFSET   0
#define BLOCK_DOWNLOAD_SUBBLOCK_CMD_SEQNUM_BIT_MASK     0b1111111

// end frame offsets
#define BLOCK_DOWNLOAD_CMD_END_N_BIT_OFFSET             2
#define BLOCK_DOWNLOAD_CMD_END_N_BIT_MASK               0b111

/***** Block download byte offsets *******************************************/
#define CANOPEN_SDO_MULTIPLEXER_IDX_OFFSET                          0
#define CANOPEN_SDO_MULITPLEXER_SUB_OFFSET                          2
#define BLOCK_DOWNLOAD_FRM_INIT_REQUEST_MULITPLEXER_BYTE_OFFSET     1
#define BLOCK_DOWNLOAD_FRM_INIT_REQUEST_MULTIPLEXER_BYTE_SIZE       3
#define BlOCK_DOWNLOAD_FRM_INIT_REQUEST_SIZE_BYTE_OFFSET            4
#define BLOCK_DOWNLOAD_FRM_INIT_REQUEST_IDX_BYTE_OFFSET             (BLOCK_DOWNLOAD_FRM_INIT_REQUEST_MULITPLEXER_BYTE_OFFSET + CANOPEN_SDO_MULTIPLEXER_IDX_OFFSET)
#define BLOCK_DOWNLOAD_FRM_INIT_REQUEST_SUB_BYTE_OFFSET             (BLOCK_DOWNLOAD_FRM_INIT_REQUEST_MULITPLEXER_BYTE_OFFSET + CANOPEN_SDO_MULITPLEXER_SUB_OFFSET)

#define BLOCK_DOWNLOAD_FRM_INIT_RESPONSE_MULTIPLEXER_BYTE_OFFSET    1
#define BLOCK_DOWNLOAD_FRM_INIT_RESPONSE_MULITPLEXER_BYTE_SIZE      3
#define BLOCK_DOWNLOAD_FRM_INIT_RESPONSE_BLKSIZE_BYTE_OFFSET        4
#define BLOCK_DOWNLOAD_FRM_INIT_RESPONSE_BLKSIZE_BYTE_SIZE          1
#define BLOCK_DOWNLOAD_FRM_INIT_RESPONSE_IDX_BYTE_OFFSET             (BLOCK_DOWNLOAD_FRM_INIT_RESPONSE_MULTIPLEXER_BYTE_OFFSET + CANOPEN_SDO_MULTIPLEXER_IDX_OFFSET)
#define BLOCK_DOWNLOAD_FRM_INIT_RESPONSE_SUB_BYTE_OFFSET             (BLOCK_DOWNLOAD_FRM_INIT_RESPONSE_MULTIPLEXER_BYTE_OFFSET + CANOPEN_SDO_MULITPLEXER_SUB_OFFSET)

#define BLOCK_DOWNLOAD_FRM_SUBBLOCK_REQUEST_SEGDATA_BYTE_OFFSET     1
#define BLOCK_DOWNLOAD_FRM_SUBBLOCK_REQUEST_SEGDATA_BYTE_SIZE       7

#define BLOCK_DOWNLOAD_FRM_SUBBLOCK_RESPONSE_ACKSEQ_BYTE_OFFSET     1
#define BLOCK_DOWNLOAD_FRM_SUBBLOCK_RESPONSE_ACKSEQ_BYTE_SIZE       1
#define BLOCK_DOWNLOAD_FRM_SUBBLOCK_RESPONSE_BLKSIZE_BYTE_OFFSET    2
#define BLOCK_DOWNLOAD_FRM_SUBBLOCK_RESPONSE_BLKSIZE_BYTE_SIZE      1

#define BLOCK_DOWNLOAD_FRM_END_CRC_REQUEST_BYTE_OFFSET              1
#define BLOCK_DOWNLOAD_FRM_END_CRC_REQEUST_BYTE_SIZE                2
/********************************/

#define CMD_OFFSET_BITS 5
#define CMD_OFFSET_MASK    0x7 
#define GET_CMD(cmd) (((cmd)>>CMD_OFFSET_BITS)&CMD_OFFSET_MASK)

/* command to send to server to initiate a block transfer */
#define CLIENT_BLOCK_DOWNLOAD_INIT_CMD              (0x6u)
/* command received from sub-block response */
#define CLIENT_BLOCK_DOWNLOAD_SUBBLOCK_RESPOSE      (0x05)
    
/* response back from server after initiating a block download request */
#define CLIENT_BLOCK_DONWLOAD_RESP_CMD  (0x5u)  
/* size indicator bit for block download request. Should always be set */
#define CLIENT_BLOCK_SIZE_INDICATOR_BIT           0
/* CC bit CRC client generation bit */
#define CLIENT_BLOCK_DOWNLOAD_REQUEST_CRC_BIT       2


/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

/*! \brief SDO CLIENT STATE
 *
 *   This enumerations holds the possible SDO client transfer states.
 *   It is mainly used for deciding if next user request is possible.
 *
 */
typedef enum {
    CO_CSDO_STATE_INVALID = 0,       /*!< SDO client is not enabled          */
    CO_CSDO_STATE_IDLE    = 1,       /*!< SDO client is idle                 */
    CO_CSDO_STATE_BUSY    = 2,       /*!< SDO client handling transfer       */

} CO_CSDO_STATE;

/*! \brief SDO CLIENT TRANSFER TYPE
 *
 *   This enumeration holds the possible SDO client transfer types.
 *   Currently, only expedited transfer is supported, it might be
 *   required to extend this enumeration in future in case segmented
 *   and block transfer support will be added.
 */
typedef enum {
    CO_CSDO_TRANSFER_NONE     = 0,   /*!< No transfer is currently ongoing   */
    CO_CSDO_TRANSFER_UPLOAD   = 1,   /*!< SDO upload is being executed       */
    CO_CSDO_TRANSFER_DOWNLOAD = 2,   /*!< SDO download is being executed     */
    CO_CSDO_TRANSFER_UPLOAD_SEGMENT = 3,  /*!< SDO segment upload is being executed     */
    CO_CSDO_TRANSFER_DOWNLOAD_SEGMENT = 4, /*!< SDO segment download is being executed     */
    CO_CSDO_TRANSFER_UPLOAD_BLOCK = 5, 
    CO_CSDO_TRANSFER_DOWNLOAD_BLOCK = 6, 

} CO_CSDO_TRANSFER_TYPE;

struct CO_CSDO_T;

/*! \brief
 *
 *   Notification on SDO client transfer complete operation.
 *
 * \param csdo
 *   Reference to SDO client that initiated transfer
 *
 * \param index
 *   Dictionary index
 *
 * \param sub
 *   Dictionary subindex
 *
 * \param code
 *   Abort code of transfer (=0 if transfer was successful)
 *
 */
typedef void (*CO_CSDO_CALLBACK_T)(struct CO_CSDO_T *csdo,
                                   uint16_t          index,
                                   uint8_t           sub,
                                   uint32_t          code);


/*! \brief SDO SEGMENTED TRANSFER
*
*    This structure holds the data, which are needed for the segmented
*    SDO transfer.
*/
typedef struct CO_CSDO_SEG_T {
    uint32_t  Size;              /*!< Size of object entry                 */
    uint32_t  Num;               /*!< Number of transfered bytes            */
    uint8_t   TBit;              /*!< Segment toggle bit                    */

} CO_CSDO_SEG;


/*! \brief SDO BLOCK TRANSFER
 *
 *  This structure holds the data needed for Block SDO transfer
 */
typedef struct {
    uint32_t    Size;               /*!< Number of bytes to transfer                    */
    uint8_t     *Buf;               /*!< Pointer to data to transfer                    */
    uint32_t    Block_Start_Index;  /*!< Starting buffer index of the last block sent   */
    uint32_t    Index;              /*!< Index of last byte sent                        */
    uint8_t     NumSegs;            /*!< Number of segements to send in this block      */ 
    uint8_t     SeqNum;             /*!< Sequence number of current sub block           */
    uint8_t     Continue;           /*!< Set if last sub-block is being sent            */
    uint8_t     BytesInLastSeg;     /*!< Number of bytes sent in the last segment       */
    uint16_t    crc;                /*!< CTC of data to be sent                         */
} CO_CSDO_BLOCK_T;

#define CO_CSDO_BLOCK_INIT(block) do {      \
    block.Size              = 0;      \
    block.Buf               = NULL;   \
    block.Block_Start_Index = 0;      \
    block.Index             = 0;      \
    block.NumSegs           = 0;      \
    block.SeqNum            = 0;      \
    block.Continue          = BLOCK_DOWNLOAD_CMD_C_CONTINUE_SEGMENTS;\
    block.BytesInLastSeg    = 0;      \
} while(0);

/*! \brief SDO CLIENT TRANSFER
 *
 *   This structure contains information required for handling ongoing SDO
 *   transfer requested by this node.
 *
 */
typedef struct CO_CSDO_TRANSFER_T {
    struct CO_CSDO_T      *Csdo;        /*!< Reference to responsible CSDO   */
    CO_CSDO_TRANSFER_TYPE  Type;        /*!< Type of current transfer        */
    uint32_t               Abort;       /*!< Abort code of transfer          */
    uint16_t               Idx;         /*!< Accessed dictionary index       */
    uint8_t                Sub;         /*!< Accessed dictionary subindex    */
    uint32_t               Tmt;         /*!< Timeout value in milliseconds   */
    uint8_t               *Buf;         /*!< Reference to transfered data    */
    uint32_t               Size;        /*!< Transfered data size            */
    int16_t                Tmr;         /*!< Identifier of timeout timer     */
    CO_CSDO_CALLBACK_T     Call;        /*!< Notification callback           */
    uint32_t               Buf_Idx;     /*!< Buffer Index                    */
    uint8_t                TBit;        /*!< Segment toggle bit              */
    CO_CSDO_BLOCK_T        Block;
} CO_CSDO_TRANSFER;

/*! \brief SDO CLIENT
 *
 *   This structure contains information required for SDO client
 *   functionality implementation.
 */
typedef struct CO_CSDO_T {
    struct CO_NODE_T *Node;             /*!< Link to node info structure     */
    CO_IF_FRM        *Frm;              /*!< SDO req/res CAN frame           */
    uint32_t          RxId;             /*!< Identifier for CSDO response    */
    uint32_t          TxId;             /*!< Identifier for CSDO requests    */
    uint8_t           NodeId;           /*!< Node-Id of addressed SDO server */
    CO_CSDO_STATE     State;            /*!< Current CSDO state              */
    CO_CSDO_TRANSFER  Tfer;             /*!< Current CSDO transfer info      */
} CO_CSDO;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*! \brief
 *
 *   This function checks if SDO client structure number 'num' exists and is
 *   enabled. If valid handle is found, its reference is returned.
 *
 * \param node
 *   Reference to parent CANopen node
 *
 * \param num
 *   Index of SDO client
 *
 * \retval  >0  valid SDO client has been found
 * \retval  =0  SDO client 'num' is not valid or does not exist
 *
 */
CO_CSDO *COCSdoFind(struct CO_NODE_T *node, uint8_t num);

/*! \brief
 *
 *   This function initiates SDO upload sequence. User should provide its
 *   own notification callback which is executed upon transfer completion.
 *
 * \param csdo
 *   Reference to SDO client
 *
 * \param key
 *    object entry key; should be generated with the macro CO_DEV()
 *
 * \param buf
 *   Reference to data to be downloaded to server
 *
 * \param size
 *   Size of downloaded data
 *
 * \param callback
 *   Notification callback on tranfer end (complete or abort)
 *
 * \param
 *   SDO server response timeout in milliseconds
 *
 * \retval  ==CO_ERR_NONE   transfer initiated successfuly
 * \retval  !=CO_ERR_NONE   SDO client is busy or invalid
 *
 */
CO_ERR COCSdoRequestUpload(CO_CSDO *csdo,
                           uint32_t key,
                           uint8_t *buf,
                           uint32_t size,
                           CO_CSDO_CALLBACK_T callback,
                           uint32_t timeout);


CO_ERR COCSdoRequestUploadBlock(CO_CSDO *csdo,
                           uint32_t key,
                           uint8_t *buf,
                           uint32_t size,
                           CO_CSDO_CALLBACK_T callback,
                           uint32_t timeout);

/*! \brief
 *
 *   This function initiates SDO download sequence. User should provide its
 *   own notification callback which is executed upon transfer completion.
 *
 * \param csdo
 *   Reference to SDO client
 *
 * \param key
 *    object entry key; should be generated with the macro CO_DEV()
 *
 * \param buf
 *   Reference to data to be downloaded to server
 *
 * \param size
 *   Size of downloaded data
 *
 * \param callback
 *   Notification callback on transfer end (complete or abort)
 *
 * \param timeout
 *   SDO server response timeout in milliseconds
 *
 * \param block
 *   Block transfer object. Can be null for segmented
 *
 * \retval  ==CO_ERR_NONE   transfer initiated successfuly
 * \retval  !=CO_ERR_NONE   SDO client is busy or invalid
 *
 */
CO_ERR COCSdoRequestDownload(CO_CSDO *csdo,
                             uint32_t key,
                             uint8_t *buf,
                             uint32_t size,
                             CO_CSDO_CALLBACK_T callback,
                             uint32_t timeout,
                             blockTransfer_t *block);

/******************************************************************************
* PROTECTED API FUNCTIONS
******************************************************************************/

/*! \brief
 *
 *   This function reads the content of the object dictionary with
 *   index 1280h+[n] subindex 1 (for TX) and subindex 2 (for RX),
 *   where n is a counter from 0 to maximal number of supported SDO
 *   clients specified by user or in `co_config.h` (CO_CSDO_N).
 *
 * \param csdo
 *   Reference to SDO client
 *
 * \param index
 *   Reference to parent CANopen node
 *
 */
void COCSdoInit(CO_CSDO *csdo, struct CO_NODE_T *node);

/*! \brief  CHECK FOR RESPONSE TO SDO CLIENT
*
*    This function checks the given frame to be a response to SDO
*    client request. If the frame is identified to be a valid
*    response, it is processed and in case response is required
*    (segmented or block transfer), frame is modified to be the
*    corresponding SDO response.
*
* \param csdo
*    Ptr to root element of SDO client array
*
* \param frm
*    Frame, received from CAN bus
*
* \retval  >0    pointer to addressed SDO client
* \retval  =0    not a SDO request or error is detected
*/
CO_CSDO *COCSdoCheck(CO_CSDO *csdo, CO_IF_FRM *frm);

/*! \brief  GENERATE SDO CLIENT RESPONSE
*
*    This function interprets the data byte #0 of the response
*    from addressed SDO server request and organizes the
*    generation of the corresponding response.
*
* \param csdo
*    Ptr to addressed SDO client
*
* \retval  ==CO_ERR_NONE        transmit SDO client frame
* \retval  ==CO_ERR_SDO_ABORT   transmit SDO abort frame
* \retval  ==CO_ERR_SDO_SILENT  transfer complete, do not transmit
*/
CO_ERR COCSdoResponse(CO_CSDO *csdo);

#if defined __cplusplus
}
#endif

#endif /* CO_CSDO_H_ */
