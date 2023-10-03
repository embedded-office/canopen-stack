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

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_core.h"

#if USE_CSDO

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void   COCSdoReset                  (CO_CSDO *csdo, uint8_t num, struct CO_NODE_T *node);
static void   COCSdoEnable                 (CO_CSDO *csdo, uint8_t num);
static CO_ERR COCSdoUploadExpedited        (CO_CSDO *csdo);
static CO_ERR COCSdoDownloadExpedited      (CO_CSDO *csdo);
static CO_ERR COCSdoInitUploadSegmented    (CO_CSDO *csdo);
static CO_ERR COCSdoUploadSegmented        (CO_CSDO *csdo);
static CO_ERR COCSdoInitDownloadSegmented  (CO_CSDO *csdo);
static CO_ERR COCSdoDownloadSegmented      (CO_CSDO *csdo);
static CO_ERR COCSdoFinishDownloadSegmented(CO_CSDO *csdo);

// static CO_ERR COCSdoInitUploadBlock        (CO_CSDO *csdo);
// static CO_ERR COCSdoUploadSubBlock         (CO_CSDO *csdo);
static CO_ERR COCSdoInitDownloadBlock      (CO_CSDO *csdo);
static CO_ERR COCSdoDownloadSubBlock_Request    (CO_CSDO *csdo);
static CO_ERR COCSdoDownloadSubBlock       (CO_CSDO *csdo);
static CO_ERR COCSdoFinishDownloadBlock    (CO_CSDO *csdo);

static void   COCSdoAbort                  (CO_CSDO *csdo, uint32_t err);
static void   COCSdoTransferFinalize       (CO_CSDO *csdo);
static void   COCSdoTimeout                (void *parg);

static CO_ERR COCSdoRequestDownloadFull(CO_CSDO *csdo,
                             uint32_t key,
                             uint8_t *buffer,
                             uint32_t size,
                             CO_CSDO_CALLBACK_T callback,
                             uint32_t timeout,
                             blockTransfer_t* block);

// cbt TODO: Update UploadFull function to take blockTransfer_t instead of a block boolean
static CO_ERR COCSdoRequestUploadFull(CO_CSDO *csdo,
                            uint32_t key,
                            uint8_t *buf,
                            uint32_t size,
                            CO_CSDO_CALLBACK_T callback,
                            uint32_t timeout,
                            bool block);

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void COCSdoAbort(CO_CSDO *csdo, uint32_t err)
{
    CO_IF_FRM frm;

    /* store abort code */
    csdo->Tfer.Abort = err;

    /* send the SDO timeout response */
    if (err == CO_SDO_ERR_TIMEOUT) {
        CO_SET_ID  (&frm, csdo->TxId        );
        CO_SET_BYTE(&frm, 0x80,           0u);
        CO_SET_WORD(&frm, csdo->Tfer.Idx, 1u);
        CO_SET_BYTE(&frm, csdo->Tfer.Sub, 3u);
        CO_SET_LONG(&frm, err,            4u);
        CO_SET_DLC (&frm,                 8u);

        (void)COIfCanSend(&csdo->Node->If, &frm);
    }
}

static void COCSdoReset(CO_CSDO *csdo, uint8_t num, struct CO_NODE_T *node)
{
    CO_CSDO *csdonum;
    int16_t  tid;

    ASSERT_PTR(csdo);
    ASSERT_LOWER(num, (uint8_t)CO_CSDO_N);

    if (csdo->State == CO_CSDO_STATE_BUSY) {
        /* Abort ongoing trasfer */
        COCSdoAbort(csdo, CO_SDO_ERR_TOS_STATE);
    }

    /* Reset handle */
    csdonum        = &csdo[num];
    csdonum->Node  = node;
    csdonum->Frm   = NULL;
    csdonum->RxId  = CO_SDO_ID_OFF;
    csdonum->TxId  = CO_SDO_ID_OFF;
    csdonum->State = CO_CSDO_STATE_INVALID;

    /* Reset transfer context */
    csdonum->Tfer.Csdo    = csdonum;
    csdonum->Tfer.Type    = CO_CSDO_TRANSFER_NONE;
    csdonum->Tfer.Abort   = 0;
    csdonum->Tfer.Idx     = 0;
    csdonum->Tfer.Sub     = 0;

    csdonum->Tfer.Tmt     = 0;
    csdonum->Tfer.Call    = NULL;
    csdonum->Tfer.Buf_Idx = 0;
    csdonum->Tfer.TBit    = 0;

    // clear the block transfer struct.
    CO_CSDO_BLOCK_INIT(csdonum->Tfer.Block);

    if (csdonum->Tfer.Tmr >= 0) {
        tid = COTmrDelete(&(node->Tmr), csdonum->Tfer.Tmr);
        if (tid < 0) {
            node->Error = CO_ERR_TMR_DELETE;
            return;
        }
    }
}

static void COCSdoEnable(CO_CSDO *csdo, uint8_t num)
{
    uint32_t  rxId;
    uint32_t  txId;
    uint8_t   nodeId;
    CO_NODE  *node;
    CO_CSDO  *csdonum;
    CO_ERR    err;

    ASSERT_LOWER(num, (uint8_t)CO_CSDO_N);

    csdonum        = &csdo[num];
    csdonum->RxId  = CO_SDO_ID_OFF;
    csdonum->TxId  = CO_SDO_ID_OFF;
    csdonum->State = CO_CSDO_STATE_INVALID;

    node = csdo->Node;
    err = CODictRdLong(&node->Dict, CO_DEV((uint32_t)0x1280u + (uint32_t)num, 1u), &txId);
    if (err != CO_ERR_NONE) {
        return;
    }
    err = CODictRdLong(&node->Dict, CO_DEV((uint32_t)0x1280u + (uint32_t)num, 2u), &rxId);
    if (err != CO_ERR_NONE) {
        return;
    }
    err = CODictRdByte(&node->Dict, CO_DEV((uint32_t)0x1280u + (uint32_t)num, 3u), &nodeId);
    if (err != CO_ERR_NONE) {
        return;
    }

    if (((rxId & CO_SDO_ID_OFF) == (uint32_t)0) &&
        ((txId & CO_SDO_ID_OFF) == (uint32_t)0)){
        csdonum->TxId  = txId + nodeId;
        csdonum->RxId  = rxId + nodeId;
        csdonum->State = CO_CSDO_STATE_IDLE;
    }
}

static void COCSdoTransferFinalize(CO_CSDO *csdo)
{
    CO_CSDO_CALLBACK_T call;
    uint16_t           idx;
    uint8_t            sub;
    uint32_t           code;

    // Delete the timeout action timer so it does not persist
    // AMF: Okay even if csdo->Tfer.Tmr is NULL
    (void)COTmrDelete(&(csdo->Node->Tmr), csdo->Tfer.Tmr);

    if (csdo->State == CO_CSDO_STATE_BUSY) {
        /* Fetch transfer information */
        idx  = csdo->Tfer.Idx;
        sub  = csdo->Tfer.Sub;
        code = csdo->Tfer.Abort;
        call = csdo->Tfer.Call;

        if (call != NULL) {
            call(csdo, idx, sub, code);
        }

        /* Reset finished transfer information */
        csdo->Tfer.Type  = CO_CSDO_TRANSFER_NONE;
        csdo->Tfer.Abort = 0;
        csdo->Tfer.Idx   = 0;
        csdo->Tfer.Sub   = 0;
        csdo->Tfer.Buf   = NULL;
        csdo->Tfer.Size  = 0;
        csdo->Tfer.Tmt   = 0;
        csdo->Tfer.Call  = NULL;
        csdo->Tfer.Tmr   = -1;
        csdo->Tfer.Buf_Idx = 0;
        csdo->Tfer.TBit = 0;

        /* Reset block transfer information */
        CO_CSDO_BLOCK_INIT(csdo->Tfer.Block);

        /* Release SDO client for next request */
        csdo->Frm   = NULL;
        csdo->State = CO_CSDO_STATE_IDLE;
    }
}

static void COCSdoTimeout(void *parg)
{
    CO_CSDO *csdo;

    csdo = (CO_CSDO *)parg;
    if (csdo->State == CO_CSDO_STATE_BUSY) {
        /* Abort SDO transfer because of timeout */
        COCSdoAbort(csdo, CO_SDO_ERR_TIMEOUT);
        /* Finalize aborted transfer */
        COCSdoTransferFinalize(csdo);
    }
}

static CO_ERR COCSdoUploadExpedited(CO_CSDO *csdo)
{
    CO_ERR  result = CO_ERR_SDO_SILENT;
    uint8_t width;
    uint8_t cmd;
    uint8_t n;

    cmd = CO_GET_BYTE(csdo->Frm, 0u);
    width = 4u - ((cmd >> 2u) & 0x03u);
    if (width > 0u) {
        if (width > (uint8_t)csdo->Tfer.Size) {
            COCSdoAbort(csdo, CO_SDO_ERR_MEM);
        } else {
            csdo->Tfer.Size = width;
            for (n = 0u; n < width; n++) {
                csdo->Tfer.Buf[n] = CO_GET_BYTE(csdo->Frm, n + 4u);
            }
        }
    }
    COCSdoTransferFinalize(csdo);
    return (result);
}

static CO_ERR COCSdoDownloadExpedited(CO_CSDO *csdo)
{
    COCSdoTransferFinalize(csdo);
    return CO_ERR_SDO_SILENT;
}

static CO_ERR COCSdoInitUploadSegmented(CO_CSDO *csdo)
{
    CO_ERR    result = CO_ERR_SDO_SILENT;
    uint32_t  obj_size;
    uint32_t  ticks;
    uint16_t  Idx;
    uint8_t   Sub;
    CO_IF_FRM frm;

    obj_size = CO_GET_LONG(csdo->Frm, 4u);
    Idx = CO_GET_WORD(csdo->Frm, 1u);
    Sub = CO_GET_BYTE(csdo->Frm, 3u);

    /* verify size, Idx, Sub */
    if ((obj_size <= csdo->Tfer.Size) &&
        (Idx == csdo->Tfer.Idx) &&
        (Sub == csdo->Tfer.Sub)) {

        csdo->Tfer.Size = obj_size;
        result = CO_ERR_NONE;

        /* setup CAN request */
        CO_SET_ID  (&frm, csdo->TxId);
        CO_SET_DLC (&frm, 8);
        CO_SET_BYTE(&frm, 0x60, 0u);
        CO_SET_WORD(&frm, 0, 1u);
        CO_SET_BYTE(&frm, 0, 3u);
        CO_SET_LONG(&frm, 0, 4u);

        /* refresh timer */
        (void)COTmrDelete(&(csdo->Node->Tmr), csdo->Tfer.Tmr);
        ticks = COTmrGetTicks(&(csdo->Node->Tmr), csdo->Tfer.Tmt, CO_TMR_UNIT_1MS);
        csdo->Tfer.Tmr = COTmrCreate(&(csdo->Node->Tmr), ticks, 0, &COCSdoTimeout, csdo);

        (void)COIfCanSend(&csdo->Node->If, &frm);
    } else {
        COCSdoAbort(csdo, CO_SDO_ERR_LEN);
        COCSdoTransferFinalize(csdo);
    }

    return result;
}

static CO_ERR COCSdoUploadSegmented(CO_CSDO *csdo)
{
    CO_ERR    result = CO_ERR_SDO_SILENT;
    uint32_t  ticks;
    uint8_t   cmd;
    uint8_t   n;
    CO_IF_FRM frm;

    cmd = CO_GET_BYTE(csdo->Frm, 0u);
    if (((cmd >> 4u) & 0x01u) == csdo->Tfer.TBit) {

        for (n = 1u; (n < 8u) && (csdo->Tfer.Buf_Idx < csdo->Tfer.Size); n++) {
            csdo->Tfer.Buf[csdo->Tfer.Buf_Idx] = CO_GET_BYTE(csdo->Frm, n);
            csdo->Tfer.Buf_Idx++;
        }

        if ((cmd & 0x01u) == 0x00u) {
            csdo->Tfer.TBit ^= 0x01u;

            CO_SET_ID  (&frm, csdo->TxId       );
            CO_SET_DLC (&frm, 8u               );

            if (csdo->Tfer.TBit == 0x01u){
                CO_SET_BYTE(&frm, 0x70, 0u);
            } else {
                CO_SET_BYTE(&frm, 0x60, 0u);
            }

            CO_SET_WORD(&frm, 0, 1u);
            CO_SET_BYTE(&frm, 0, 3u);
            CO_SET_LONG(&frm, 0, 4u);

            /* refresh timer */
            (void)COTmrDelete(&(csdo->Node->Tmr), csdo->Tfer.Tmr);
            ticks = COTmrGetTicks(&(csdo->Node->Tmr), csdo->Tfer.Tmt, CO_TMR_UNIT_1MS);
            csdo->Tfer.Tmr = COTmrCreate(&(csdo->Node->Tmr), ticks, 0, &COCSdoTimeout, csdo);

            (void)COIfCanSend(&csdo->Node->If, &frm);
        } else {
            COCSdoTransferFinalize(csdo);
        }
    } else {
        COCSdoAbort(csdo, CO_SDO_ERR_TBIT);
        COCSdoTransferFinalize(csdo);
    }

    return result;
}

static CO_ERR COCSdoInitDownloadSegmented(CO_CSDO *csdo)
{
    CO_ERR    result = CO_ERR_SDO_SILENT;
    uint32_t  ticks;
    uint16_t  Idx;
    uint8_t   Sub;
    uint8_t   n;
    uint32_t  width;
    uint8_t   c_bit = 1;
    uint8_t   cmd;
    CO_IF_FRM frm;

    Idx = CO_GET_WORD(csdo->Frm, 1u);
    Sub = CO_GET_BYTE(csdo->Frm, 3u);
    if ((Idx == csdo->Tfer.Idx) &&
        (Sub == csdo->Tfer.Sub)) {

        CO_SET_ID  (&frm, csdo->TxId);
        CO_SET_DLC (&frm, 8u);
        CO_SET_LONG(&frm, 0, 0u);
        CO_SET_LONG(&frm, 0, 4u);

        width = csdo->Tfer.Size - csdo->Tfer.Buf_Idx;
        if (width > 7u) {
            width = 7u;
            c_bit = 0u;
        }

        for (n = 1; n <= width; n++) {
            CO_SET_BYTE(&frm, csdo->Tfer.Buf[csdo->Tfer.Buf_Idx], n);
            csdo->Tfer.Buf_Idx++;
        }

        cmd = (uint8_t)(csdo->Tfer.TBit << 4u) |
              (uint8_t)(((7u - width) << 1u)) |
              (uint8_t)(c_bit);
        CO_SET_BYTE(&frm, cmd, 0u);

        /* refresh timer */
        (void)COTmrDelete(&(csdo->Node->Tmr), csdo->Tfer.Tmr);
        ticks = COTmrGetTicks(&(csdo->Node->Tmr), csdo->Tfer.Tmt, CO_TMR_UNIT_1MS);
        csdo->Tfer.Tmr = COTmrCreate(&(csdo->Node->Tmr), ticks, 0, &COCSdoTimeout, csdo);

        (void)COIfCanSend(&csdo->Node->If, &frm);
    } else {
        COCSdoAbort(csdo, CO_SDO_ERR_TBIT);
        COCSdoTransferFinalize(csdo);
    }

    return result;
}

static CO_ERR COCSdoDownloadSegmented(CO_CSDO *csdo)
{
    CO_ERR    result = CO_ERR_SDO_SILENT;
    uint32_t  ticks;
    uint8_t   cmd;
    uint8_t   n;
    uint32_t  width;
    uint8_t   c_bit = 1;
    CO_IF_FRM frm;

    cmd = CO_GET_BYTE(csdo->Frm, 0u);
    if (((cmd >> 4u) & 0x01u) == csdo->Tfer.TBit) {
        csdo->Tfer.TBit ^= 0x01u;

        CO_SET_ID  (&frm, csdo->TxId       );
        CO_SET_DLC (&frm, 8                );

         /* clean frm data */
        CO_SET_LONG(&frm, 0, 0u);
        CO_SET_LONG(&frm, 0, 4u);

        width = csdo->Tfer.Size - csdo->Tfer.Buf_Idx;

        if (width > 7u) {
            width = 7u;
            c_bit = 0u;
        }

        for (n = 1; n <= width; n++) {
            CO_SET_BYTE(&frm, csdo->Tfer.Buf[csdo->Tfer.Buf_Idx], n);
            csdo->Tfer.Buf_Idx++;
        }

        cmd = (uint8_t)(csdo->Tfer.TBit << 4u) |
              (uint8_t)(((7u - width) << 1u)) |
              (uint8_t)(c_bit);

        CO_SET_BYTE(&frm, cmd, 0u);

         /* refresh timer */
        (void)COTmrDelete(&(csdo->Node->Tmr), csdo->Tfer.Tmr);
        ticks = COTmrGetTicks(&(csdo->Node->Tmr), csdo->Tfer.Tmt, CO_TMR_UNIT_1MS);
        csdo->Tfer.Tmr = COTmrCreate(&(csdo->Node->Tmr), ticks, 0, &COCSdoTimeout, csdo);

        (void)COIfCanSend(&csdo->Node->If, &frm);
    } else {
        COCSdoAbort(csdo, CO_SDO_ERR_TBIT);
        COCSdoTransferFinalize(csdo);
    }

    return result;
}

static CO_ERR COCSdoFinishDownloadSegmented(CO_CSDO *csdo)
{
    COCSdoTransferFinalize(csdo);
    return CO_ERR_SDO_SILENT;
}
/******************************************************************************
* Block Upload Transfer Functions 
* ****************************************************************************/
// cbt TODO: Implement this function
// static CO_ERR COCSdoInitUploadBlock        (CO_CSDO *csdo);
// cbt TODO: Implement this function
// static CO_ERR COCSdoUploadSubBlock         (CO_CSDO *csdo);

/******************************************************************************
* Block Download Transfer Functions 
* ****************************************************************************/
// Set up client side after getting confirmation from the server
static CO_ERR COCSdoInitDownloadBlock      (CO_CSDO *csdo)
{
    CO_ERR    result = CO_ERR_SDO_SILENT;
    uint16_t  Idx;
    uint8_t   Sub;
    uint8_t cmd;
    CO_IF_FRM frm;

    cmd = CO_GET_BYTE(csdo->Frm, 0u);

    Idx = CO_GET_WORD(csdo->Frm, BLOCK_DOWNLOAD_FRM_INIT_RESPONSE_IDX_BYTE_OFFSET);
    Sub = CO_GET_BYTE(csdo->Frm, BLOCK_DOWNLOAD_FRM_INIT_RESPONSE_SUB_BYTE_OFFSET);

    // makes no difference
     /* clean frm data */
    CO_SET_LONG(&frm, 0, 0u);
    CO_SET_LONG(&frm, 0, 4u);

    // verfiy block transfer from the correct Idx and Sub
    uint8_t cmd_scs = READ_BITS(cmd, BLOCK_DOWNLOAD_CMD_SCS_CCS_BIT_OFFSET, BLOCK_DOWNLOAD_CMD_SCS_CCS_BIT_MASK);
    if ((cmd_scs == BLOCK_DOWNLOAD_CMD_SCS) && 
        (Idx == csdo->Tfer.Idx) &&
        (Sub == csdo->Tfer.Sub)) {

        //TODO: part of https://neurio.atlassian.net/browse/FWU-426
        // uint8_t cmd_cc = READ_BITS(cmd, BLOCK_DOWNLOAD_INIT_REQUEST_CMD_CRC_BIT_OFFSET, 0x01);
        // if (cmd_cc == BLOCK_DOWNLOAD_CMD_SC_CC_CRC_SUPPORTED) {
        // }
        csdo->Tfer.Block.NumSegs= CO_GET_BYTE(csdo->Frm, BLOCK_DOWNLOAD_FRM_INIT_RESPONSE_BLKSIZE_BYTE_OFFSET);


        CO_SET_ID  (&frm, csdo->TxId);
        CO_SET_DLC (&frm, 8u);

         /* clean frm data */
        CO_SET_LONG(&frm, 0, 0u);
        CO_SET_LONG(&frm, 0, 4u);

        // send the first sub-block
        return COCSdoDownloadSubBlock_Request( csdo );
     } else {
        COCSdoAbort(csdo, CO_SDO_ERR_TBIT); 
        COCSdoTransferFinalize(csdo);
    }
    return result; 
}

static CO_ERR COCSdoDownloadSubBlock_Request( CO_CSDO *csdo ) {
    CO_ERR    result = CO_ERR_SDO_SILENT;
    uint32_t  ticks;
    uint32_t width;
    uint32_t n;
    CO_IF_FRM frm;
    uint8_t cmd = 0;
    CO_CSDO_BLOCK_T* block = &csdo->Tfer.Block;

    block->SeqNum = 1;
    block->Continue = BLOCK_DOWNLOAD_CMD_C_CONTINUE_SEGMENTS;

    while ((block->SeqNum <= block->NumSegs) && 
           (block->Continue == BLOCK_DOWNLOAD_CMD_C_CONTINUE_SEGMENTS) ) {

        CO_SET_ID  (&frm, csdo->TxId       );
        CO_SET_DLC (&frm, 8                );

         /* clean frm data */
        CO_SET_LONG(&frm, 0, 0u);
        CO_SET_LONG(&frm, 0, 4u);

        cmd = 0;
        width = block->Size - block->Index;
        if (width > BLOCK_DOWNLOAD_FRM_SUBBLOCK_REQUEST_SEGDATA_BYTE_SIZE){
            width = BLOCK_DOWNLOAD_FRM_SUBBLOCK_REQUEST_SEGDATA_BYTE_SIZE;
            block->Continue = BLOCK_DOWNLOAD_CMD_C_CONTINUE_SEGMENTS;
        } else {
            // last segment is being sent
            block->Continue = BLOCK_DOWNLOAD_CMD_C_NO_MORE_SEGMENTS;
        }
        CLEAR_SET_BITS(cmd,                             
                block->Continue,                        
                BLOCK_DOWNLOAD_CMD_C_BIT_OFFSET,        
                BLOCK_DOWNLOAD_CMD_C_BIT_MASK);

        block->BytesInLastSeg = width;

        for (n = 1; n <= width; n++){
            // send the byte and increment the index
            CO_SET_BYTE(&frm, block->Buf[block->Index++], n);
        }
        // fill any unused bytes with zeros (should be false until last segment)
        while (n <= BLOCK_DOWNLOAD_FRM_SUBBLOCK_REQUEST_SEGDATA_BYTE_SIZE){
            CO_SET_BYTE(&frm, 0u, n);
            n++;
        }
        // set the seq num in the cmd byte
        cmd |= SET_BITS(block->SeqNum,                          
                BLOCK_DOWNLOAD_SUBBLOCK_CMD_SEQNUM_BIT_OFFSET,  
                BLOCK_DOWNLOAD_SUBBLOCK_CMD_SEQNUM_BIT_MASK);
        block->SeqNum++;
        
        CO_SET_BYTE(&frm, cmd, 0u);

         /* refresh timer */
        (void)COTmrDelete(&(csdo->Node->Tmr), csdo->Tfer.Tmr);
        ticks = COTmrGetTicks(&(csdo->Node->Tmr), csdo->Tfer.Tmt, CO_TMR_UNIT_1MS);
        csdo->Tfer.Tmr = COTmrCreate(&(csdo->Node->Tmr), ticks, 0, &COCSdoTimeout, csdo);

        (void)COIfCanSend(&csdo->Node->If, &frm);
    }
    return result;
}

static CO_ERR COCSdoDownloadSubBlock       (CO_CSDO *csdo)
{
    CO_ERR    result = CO_ERR_SDO_SILENT;
    uint32_t  ticks;
    uint8_t cmd;
    CO_IF_FRM frm;
    CO_CSDO_BLOCK_T* block = &csdo->Tfer.Block;

    cmd = CO_GET_BYTE(csdo->Frm, 0u);
    uint8_t akseq = CO_GET_BYTE(csdo->Frm, BLOCK_DOWNLOAD_FRM_SUBBLOCK_RESPONSE_ACKSEQ_BYTE_OFFSET);
    uint8_t blksize = CO_GET_BYTE(csdo->Frm, BLOCK_DOWNLOAD_FRM_SUBBLOCK_RESPONSE_BLKSIZE_BYTE_OFFSET); 
    
    // verfy that we are looking at a sub-block response from server
    uint8_t cmd_scs = READ_BITS(cmd,                
            BLOCK_DOWNLOAD_CMD_SCS_CCS_BIT_OFFSET,  
            BLOCK_DOWNLOAD_CMD_SCS_CCS_BIT_MASK);
    
    CO_SET_ID  (&frm, csdo->TxId       );
    CO_SET_DLC (&frm, 8                );

     /* clean frm data */
    CO_SET_LONG(&frm, 0, 0u);
    CO_SET_LONG(&frm, 0, 4u);

    // TODO: this check is done before the call to this function so redundant but leaving for now. 
    if( cmd_scs == BLOCK_DOWNLOAD_CMD_SCS ) {
        // send next block if final block has not been sent or if server did 
        // not receive all data in final block
        
        // we have more segments to send, update response from server and send another block
        block->NumSegs = blksize;
        // update the block start index based on last received seq from server
        block->Block_Start_Index += (akseq * BLOCK_DOWNLOAD_FRM_SUBBLOCK_REQUEST_SEGDATA_BYTE_SIZE);
        // reset the block index
        block->Index = block->Block_Start_Index;

        if( block->Index < block->Size ){
            COCSdoDownloadSubBlock_Request( csdo );
        } else {
            // last block was sent and server confirmed receipt of all 
            // segments. Send the block download end frame
            // Generate and send end transfer frame

            cmd = SET_BITS( BLOCK_DOWNLOAD_CMD_CCS,                 
                            BLOCK_DOWNLOAD_CMD_SCS_CCS_BIT_OFFSET,  
                            BLOCK_DOWNLOAD_CMD_SCS_CCS_BIT_MASK);
            cmd |= SET_BITS(BLOCK_DOWNLOAD_FRM_SUBBLOCK_REQUEST_SEGDATA_BYTE_SIZE - block->BytesInLastSeg,   
                            BLOCK_DOWNLOAD_CMD_END_N_BIT_OFFSET,    
                            BLOCK_DOWNLOAD_CMD_END_N_BIT_MASK);
            cmd |= SET_BITS(BLOCK_DOWNLOAD_CMD_SS_CS_END,           
                            BLOCK_DOWNLOAD_CMD_CS_BIT_OFFSET,       
                            BLOCK_DOWNLOAD_CMD_CS_BIT_MASK);

            // Send crc from the csdo transfer block
            CO_SET_WORD(&frm, csdo->Tfer.Block.crc, 1u);

            CO_SET_BYTE(&frm, cmd, 0u);

             /* refresh timer */
            (void)COTmrDelete(&(csdo->Node->Tmr), csdo->Tfer.Tmr);
            ticks = COTmrGetTicks(&(csdo->Node->Tmr), csdo->Tfer.Tmt, CO_TMR_UNIT_1MS);
            csdo->Tfer.Tmr = COTmrCreate(&(csdo->Node->Tmr), ticks, 0, &COCSdoTimeout, csdo);

            (void)COIfCanSend(&csdo->Node->If, &frm);
        }
    } else {
        COCSdoAbort(csdo, CO_SDO_ERR_TBIT); 
        COCSdoTransferFinalize(csdo);
    }
    return result;
}

static CO_ERR COCSdoFinishDownloadBlock    (CO_CSDO *csdo) {
    CO_ERR    result = CO_ERR_SDO_SILENT;
    uint8_t cmd;

    cmd= CO_GET_BYTE(csdo->Frm, 0u);
    uint8_t cmd_scs = READ_BITS(cmd,BLOCK_DOWNLOAD_CMD_SCS_CCS_BIT_OFFSET,BLOCK_DOWNLOAD_CMD_SCS_CCS_BIT_MASK);
    uint8_t cmd_ss = READ_BITS(cmd,BLOCK_DOWNLOAD_CMD_SS_BIT_OFFSET,BLOCK_DOWNLOAD_CMD_SS_BIT_MASK);
    if( (cmd_scs != BLOCK_DOWNLOAD_CMD_SCS) || (cmd_ss != BLOCK_DOWNLOAD_CMD_SS_CS_END) ) {
        COCSdoAbort(csdo, CO_SDO_ERR_TBIT); 
    }
    COCSdoTransferFinalize(csdo);
    return result;
}

/******************************************************************************
* PROTECTED API FUNCTIONS
******************************************************************************/

void COCSdoInit(CO_CSDO *csdo, struct CO_NODE_T *node)
{
    uint8_t n;

    for (n = 0; n < (uint8_t)CO_CSDO_N; n++) {
        node->CSdo[n].Tfer.Tmr = -1;
        COCSdoReset(csdo, n, node);
        COCSdoEnable(csdo, n);
    }
}

CO_CSDO *COCSdoCheck(CO_CSDO *csdo, CO_IF_FRM *frm)
{
    CO_CSDO *result;
    uint8_t  n;

    result = NULL;

    if (frm != NULL) {
        n = 0;
        while ((n       < (uint8_t)CO_CSDO_N) &&
               (result == NULL              )) {
            /*
             * Match configured COB-ID
             * and current client state.
             * Idle client state means
             * that it did not initiate
             * any transfer (or timed out),
             * which means we are not
             * interested in response
             * anymore.
             */
            if ((CO_GET_ID(frm) == csdo[n].RxId) &&
                (csdo[n].State  == CO_CSDO_STATE_BUSY)) {
                /*
                 * Update frame with COB-ID
                 * and return client handle
                 * for further processing.
                 */
                CO_SET_ID(frm, csdo[n].TxId);
                csdo[n].Frm        = frm;
                csdo[n].Tfer.Abort = 0;
                result = &csdo[n];
                break;
            }
            n++;
        }
    }

    return (result);
}

CO_ERR COCSdoResponse(CO_CSDO *csdo)
{
    CO_ERR   result = CO_ERR_SDO_SILENT;
    uint16_t index;
    uint8_t  cmd;
    uint8_t  sub;

    cmd = CO_GET_BYTE(csdo->Frm, 0u);

    if (cmd == 0x80u) {
        /* SDO abort protocol */
        index = CO_GET_WORD(csdo->Frm, 1u);
        sub   = CO_GET_BYTE(csdo->Frm, 3u);
        if ((index == csdo->Tfer.Idx) &&
            (sub   == csdo->Tfer.Sub)) {
            csdo->Tfer.Abort = CO_GET_LONG(csdo->Frm, 4u);
            COCSdoTransferFinalize(csdo);
            return (result);
        }
    }
    
    if (csdo->Tfer.Type == CO_CSDO_TRANSFER_DOWNLOAD_BLOCK) {
        uint8_t ss = READ_BITS(cmd,                 
                BLOCK_DOWNLOAD_CMD_SS_BIT_OFFSET,   
                BLOCK_DOWNLOAD_CMD_SS_BIT_MASK);

        uint8_t scs = READ_BITS(cmd,                    
                BLOCK_DOWNLOAD_CMD_SCS_CCS_BIT_OFFSET,  
                BLOCK_DOWNLOAD_CMD_SCS_CCS_BIT_MASK);

        if( scs == BLOCK_DOWNLOAD_CMD_SCS ){
            if ( ss == BLOCK_DOWNLOAD_CMD_SS_CS_INITIATE) {
                // response back from init 
                return COCSdoInitDownloadBlock(csdo);
            }
            else if ( ss == BLOCK_DOWNLOAD_CMD_SS_DOWNLOAD_RESPONSE) {
                return COCSdoDownloadSubBlock(csdo);
            }
            else if ( ss == BLOCK_DOWNLOAD_CMD_SS_CS_END ){
                // finalize download
                return COCSdoFinishDownloadBlock(csdo);
            } else {
                COCSdoAbort(csdo, CO_SDO_ERR_CMD);
                COCSdoTransferFinalize(csdo);
            }
        }
    }
    else if (csdo->Tfer.Type == CO_CSDO_TRANSFER_UPLOAD_BLOCK) {
        //TODO: Implement Block Upload: https://neurio.atlassian.net/browse/FWU-328
    }

    else if (csdo->Tfer.Type == CO_CSDO_TRANSFER_UPLOAD_SEGMENT) {
        if (cmd == 0x41u) {
            (void)COCSdoInitUploadSegmented(csdo);
        } else if ((cmd & 0xE0u) == 0x00u) {
            (void)COCSdoUploadSegmented(csdo);
        } else if ((cmd == 0x43u) || (cmd == 0x47u)|| (cmd == 0x4Bu)|| (cmd == 0x4Fu)) {
            // 4F = 1 byte, 4B = 2 bytes, 47 = 3 bytes, 43 = 4 bytes
            (void)COCSdoUploadExpedited(csdo);
        }else {
            COCSdoAbort(csdo, CO_SDO_ERR_CMD);
            COCSdoTransferFinalize(csdo);
        }
    } else if (csdo->Tfer.Type == CO_CSDO_TRANSFER_DOWNLOAD_SEGMENT) {
        if (cmd == 0x60u) {
            (void)COCSdoInitDownloadSegmented(csdo);
        } else if (((cmd & 0xE0u) ==  0x20u) ) {
            if (csdo->Tfer.Size > csdo->Tfer.Buf_Idx) {
                (void)COCSdoDownloadSegmented(csdo);
            } else {
                /* wait last response */
                (void)COCSdoFinishDownloadSegmented(csdo);
            }
        } else {
            COCSdoAbort(csdo, CO_SDO_ERR_CMD);
            COCSdoTransferFinalize(csdo);
        }
    } else if (cmd == 0x60u) {
        result = COCSdoDownloadExpedited(csdo);
        return (result);
    } else if ((cmd & 0x43u) != 0u) {
        result = COCSdoUploadExpedited(csdo);
        return (result);
    } else {
        COCSdoAbort(csdo, CO_SDO_ERR_PARA_INCOMP);
    }

    return (result);
}

static CO_ERR COCSdoRequestDownloadFull(CO_CSDO *csdo,
                             uint32_t key,
                             uint8_t *buffer,
                             uint32_t size,
                             CO_CSDO_CALLBACK_T callback,
                             uint32_t timeout,
                             blockTransfer_t* block)
{
    CO_IF_FRM frm;
    uint8_t   cmd;
    uint8_t   n;
    uint32_t  num;
    uint32_t  ticks;

    ASSERT_PTR_ERR(csdo, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(buffer, CO_ERR_BAD_ARG);
    ASSERT_NOT_ERR(size, (uint32_t)0, CO_ERR_BAD_ARG);

    if (callback == (CO_CSDO_CALLBACK_T)NULL) {
        /* no callback is given */
        return CO_ERR_BAD_ARG;
    }
    if (csdo->State == CO_CSDO_STATE_INVALID) {
        /* Requested SDO client is disabled */
        return CO_ERR_SDO_OFF;
    }
    if (csdo->State == CO_CSDO_STATE_BUSY) {
        /* Requested SDO client is busy */
        return CO_ERR_SDO_BUSY;
    }

    /* Set client as busy to prevent its usage
     * until requested transfer is complete
     */
    csdo->State = CO_CSDO_STATE_BUSY;

    /* Update transfer info */
    csdo->Tfer.Abort   = 0;
    csdo->Tfer.Idx     = CO_GET_IDX(key);
    csdo->Tfer.Sub     = CO_GET_SUB(key);
    csdo->Tfer.Buf     = buffer;
    csdo->Tfer.Size    = size;
    csdo->Tfer.Tmt     = timeout;
    csdo->Tfer.Call    = callback;
    csdo->Tfer.Buf_Idx = 0;
    csdo->Tfer.TBit    = 0;

    if ((block != NULL )&&(block->blockMode == true))
    {
        CO_CSDO_BLOCK_INIT(csdo->Tfer.Block);

        csdo->Tfer.Block.Buf = buffer;
        csdo->Tfer.Block.Size = size;
        csdo->Tfer.Type = CO_CSDO_TRANSFER_DOWNLOAD_BLOCK;

        cmd = (CLIENT_BLOCK_DOWNLOAD_INIT_CMD << CMD_OFFSET_BITS ) | 
              (BLOCK_DOWNLOAD_CMD_S_SIZE_INDICATED << BLOCK_DOWNLOAD_INIT_REQUEST_CMD_S_BIT_OFFSET);

        if (block->crc == true) {
            cmd |= (BLOCK_DOWNLOAD_CMD_SC_CC_CRC_SUPPORTED << CLIENT_BLOCK_DOWNLOAD_REQUEST_CRC_BIT);
            csdo->Tfer.Block.crc = block->crc_value;
        }

        CO_SET_BYTE(&frm, cmd, 0u);

        // Set size (in num bytes) value
        CO_SET_LONG(&frm, size, 4u);

        // multiplexer value set later (Tfer.Idx and Tfer.Sub) 
    }
    else if (size <= (uint32_t)4u) {
        csdo->Tfer.Type = CO_CSDO_TRANSFER_DOWNLOAD;

        cmd = ((0x23u) | ((4u - (uint8_t)size) << 2u));
        CO_SET_BYTE(&frm, cmd, 0u);

        num = size;
        for (n = 4u; n < 8u; n++) {
            if (num > (uint8_t)0u) {
                CO_SET_BYTE(&frm, *buffer, n);
                num--;
                buffer++;
            } else {
                CO_SET_BYTE(&frm, 0u, n);
            }
        }
    } else {
        csdo->Tfer.Type = CO_CSDO_TRANSFER_DOWNLOAD_SEGMENT;

        cmd = 0x21u;
        CO_SET_BYTE(&frm,  cmd, 0u);
        CO_SET_LONG(&frm, size, 4u);
    }

    /* Transmit transfer initiation directly */
    CO_SET_ID  (&frm, csdo->TxId        );
    CO_SET_DLC (&frm, 8u                );
    CO_SET_WORD(&frm, csdo->Tfer.Idx, 1u);
    CO_SET_BYTE(&frm, csdo->Tfer.Sub, 3u);

    ticks = COTmrGetTicks(&(csdo->Node->Tmr), timeout, CO_TMR_UNIT_1MS);
    csdo->Tfer.Tmr = COTmrCreate(&(csdo->Node->Tmr), ticks, 0, &COCSdoTimeout, csdo);

    (void)COIfCanSend(&csdo->Node->If, &frm);

    return CO_ERR_NONE;
}
/******************************************************************************
* PUBLIC API FUNCTIONS
******************************************************************************/

CO_CSDO *COCSdoFind(struct CO_NODE_T *node, uint8_t num)
{
    CO_CSDO *result;

    ASSERT_PTR_ERR(node, 0);
    ASSERT_LOWER_ERR(num, (uint8_t)CO_CSDO_N, 0);

    result = &node->CSdo[num];
    if (result->State <= CO_CSDO_STATE_INVALID) {
        /* Client is found but not enabled */
        result = NULL;
    }

    return (result);
}

//TODO: Implement the upload side of block transfer
CO_ERR COCSdoRequestUpload(CO_CSDO *csdo,
                           uint32_t key,
                           uint8_t *buf,
                           uint32_t size,
                           CO_CSDO_CALLBACK_T callback,
                           uint32_t timeout)
{
    return COCSdoRequestUploadFull(csdo, key, buf, size, callback, timeout, false);
}

CO_ERR COCSdoRequestUploadBlock(CO_CSDO *csdo,
                            uint32_t key,
                            uint8_t *buf,
                            uint32_t size,
                            CO_CSDO_CALLBACK_T callback,
                            uint32_t timeout)
{
    return COCSdoRequestUploadFull(csdo, key, buf, size, callback, timeout, true);
}

CO_ERR COCSdoRequestUploadFull(CO_CSDO *csdo,
                           uint32_t key,
                           uint8_t *buf,
                           uint32_t size,
                           CO_CSDO_CALLBACK_T callback,
                           uint32_t timeout,
                           bool block)
{
    CO_IF_FRM frm;
    uint32_t  ticks;

    ASSERT_PTR_ERR(csdo, CO_ERR_BAD_ARG);
    ASSERT_PTR_ERR(buf, CO_ERR_BAD_ARG);
    ASSERT_NOT_ERR(size, (uint32_t)0, CO_ERR_BAD_ARG);

    if (callback == (CO_CSDO_CALLBACK_T)NULL) {
        /* no callback is given */
        return CO_ERR_BAD_ARG;
    }
    if (csdo->State == CO_CSDO_STATE_INVALID) {
        /* Requested SDO client is disabled */
        return CO_ERR_SDO_OFF;
    }
    if (csdo->State == CO_CSDO_STATE_BUSY) {
        /* Requested SDO client is busy */
        return CO_ERR_SDO_BUSY;
    }

    /* Set client as busy to prevent its usage
     * until requested transfer is complete
     */
    csdo->State = CO_CSDO_STATE_BUSY;

    /* Update transfer info */
    // cbt TODO: Add logic for block transfer
    if (block == true ){
        csdo->Tfer.Type = CO_CSDO_TRANSFER_UPLOAD_BLOCK;
    }
    else if (size <= (uint32_t)4) {
        /* expedited transfer */
        csdo->Tfer.Type = CO_CSDO_TRANSFER_UPLOAD;
    } else {
        /* segmented transfer */
        csdo->Tfer.Type = CO_CSDO_TRANSFER_UPLOAD_SEGMENT;
    }
    csdo->Tfer.Abort   = 0;
    csdo->Tfer.Idx     = CO_GET_IDX(key);
    csdo->Tfer.Sub     = CO_GET_SUB(key);
    csdo->Tfer.Buf     = buf;
    csdo->Tfer.Size    = size;
    csdo->Tfer.Tmt     = timeout;
    csdo->Tfer.Call    = callback;
    csdo->Tfer.Buf_Idx = 0;
    csdo->Tfer.TBit    = 0;

    /* Transmit transfer initiation directly */
    CO_SET_ID  (&frm, csdo->TxId        );
    CO_SET_DLC (&frm, 8u                );
    CO_SET_BYTE(&frm, 0x40          , 0u);
    CO_SET_WORD(&frm, csdo->Tfer.Idx, 1u);
    CO_SET_BYTE(&frm, csdo->Tfer.Sub, 3u);
    CO_SET_LONG(&frm, 0,              4u);

    ticks = COTmrGetTicks(&(csdo->Node->Tmr), timeout, CO_TMR_UNIT_1MS);
    csdo->Tfer.Tmr = COTmrCreate(&(csdo->Node->Tmr), ticks, 0, &COCSdoTimeout, csdo);

    (void)COIfCanSend(&csdo->Node->If, &frm);

    return CO_ERR_NONE;
}

CO_ERR COCSdoRequestDownload(CO_CSDO *csdo,
                             uint32_t key,
                             uint8_t *buf,
                             uint32_t size,
                             CO_CSDO_CALLBACK_T callback,
                             uint32_t timeout,
                             blockTransfer_t *block)
{
    return COCSdoRequestDownloadFull(csdo, key, buf, size, callback, timeout, block);
}


#endif
