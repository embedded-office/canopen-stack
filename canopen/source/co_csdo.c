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
* PUBLIC FUNCTIONS
******************************************************************************/

CO_CSDO *COCSdoFind(struct CO_NODE_T *node, uint8_t num) {
    CO_CSDO *result;

    if ((node == 0) || (num >= CO_CSDO_N)) {
        /* Invalid argument check */
        return 0;
    }

    result = &node->CSdo[num];
    if (result->State <= CO_CSDO_STATE_INVALID) {
        /* Client is found but not enabled */
        result = 0;
    }

    return (result);
}

CO_ERR COCSdoRequestUpload(CO_CSDO *csdo,
                           uint32_t key,
                           uint8_t *buf,
                           uint32_t size,
                           CO_CSDO_CALLBACK_T callback,
                           uint32_t timeout) {
    CO_IF_FRM frm;
    uint32_t  ticks;

    if ((csdo == 0) || (callback == 0) ||
        (buf  == 0) || (size     == 0)) {
        /* Invalid argument */
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
    if (size <= 4) {
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
    CO_SET_ID  (&frm, csdo->TxId       );
    CO_SET_DLC (&frm, 8                );
    CO_SET_BYTE(&frm, 0x40          , 0);
    CO_SET_WORD(&frm, csdo->Tfer.Idx, 1);
    CO_SET_BYTE(&frm, csdo->Tfer.Sub, 3);
    CO_SET_LONG(&frm, 0,              4);

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
                             uint32_t timeout) {
    CO_IF_FRM frm;
    uint8_t   cmd;
    uint8_t   n;
    uint32_t  ticks;

    if ((csdo == 0) || (buf      == 0) ||
        (size == 0) || (callback == 0)) {
        /* Invalid argument */
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
    csdo->Tfer.Buf     = buf;
    csdo->Tfer.Size    = size;
    csdo->Tfer.Tmt     = timeout;
    csdo->Tfer.Call    = callback;
    csdo->Tfer.Buf_Idx = 0;
    csdo->Tfer.TBit    = 0;

    if (size <= 4) {
        csdo->Tfer.Type = CO_CSDO_TRANSFER_DOWNLOAD;

        cmd = ((0x23) | ((4 - size) << 2));
        CO_SET_BYTE(&frm, cmd, 0);

        for (n = 4; n < 8; n++) {
            if (size > 0) {
                CO_SET_BYTE(&frm, *buf++, n);
                size--;
            } else {
                CO_SET_BYTE(&frm, 0u, n);
            }
        }
    } else {
        csdo->Tfer.Type = CO_CSDO_TRANSFER_DOWNLOAD_SEGMENT;

        cmd = 0x21;
        CO_SET_BYTE(&frm, cmd, 0);
        CO_SET_LONG(&frm, size, 4);
    }

    /* Transmit transfer initiation directly */
    CO_SET_ID  (&frm, csdo->TxId       );
    CO_SET_DLC (&frm, 8                );
    CO_SET_WORD(&frm, csdo->Tfer.Idx, 1);
    CO_SET_BYTE(&frm, csdo->Tfer.Sub, 3);

    ticks = COTmrGetTicks(&(csdo->Node->Tmr), timeout, CO_TMR_UNIT_1MS);
    csdo->Tfer.Tmr = COTmrCreate(&(csdo->Node->Tmr), ticks, 0, &COCSdoTimeout, csdo);

    (void)COIfCanSend(&csdo->Node->If, &frm);

    return CO_ERR_NONE;
}

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

void COCSdoInit(CO_CSDO *csdo, struct CO_NODE_T *node) {
    uint8_t n;

    for (n = 0; n < CO_CSDO_N; n++) {
        node->CSdo[n].Tfer.Tmr = -1;
        COCSdoReset(csdo, n, node);
        COCSdoEnable(csdo, n);
    }
}

void COCSdoReset(CO_CSDO *csdo, uint8_t num, struct CO_NODE_T *node) {
    CO_CSDO *csdonum;
    int16_t  tid;

    if (csdo == 0) {
        return;
    }
    if (num > CO_CSDO_N) {
        return;
    }

    if (csdo->State == CO_CSDO_STATE_BUSY) {
        /* Abort ongoing trasfer */
        COCSdoAbort(csdo, CO_SDO_ERR_TOS_STATE);
    }

    /* Reset handle */
    csdonum        = &csdo[num];
    csdonum->Node  = node;
    csdonum->Frm   = 0;
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
    csdonum->Tfer.Call    = 0;
    csdonum->Tfer.Buf_Idx = 0;
    csdonum->Tfer.TBit    = 0;

    if (csdonum->Tfer.Tmr >= 0) {
        tid = COTmrDelete(&(node->Tmr), csdonum->Tfer.Tmr);
        if (tid < 0) {
            node->Error = CO_ERR_TMR_DELETE;
            return;
        }
    }
}

void COCSdoEnable(CO_CSDO *csdo, uint8_t num) {
    uint32_t  rxId;
    uint32_t  txId;
    uint8_t   nodeId;
    CO_NODE  *node;
    CO_CSDO  *csdonum;
    CO_ERR    err;

    if (num >= CO_CSDO_N) {
        return;
    }
    csdonum        = &csdo[num];
    csdonum->RxId  = CO_SDO_ID_OFF;
    csdonum->TxId  = CO_SDO_ID_OFF;
    csdonum->State = CO_CSDO_STATE_INVALID;

    node = csdo->Node;
    err = CODictRdLong(&node->Dict, CO_DEV(0x1280 + num, 1), &txId);
    if (err != CO_ERR_NONE) {
        return;
    }
    err = CODictRdLong(&node->Dict, CO_DEV(0x1280 + num, 2), &rxId);
    if (err != CO_ERR_NONE) {
        return;
    }
    err = CODictRdByte(&node->Dict, CO_DEV(0x1280 + num, 3), &nodeId);
    if (err != CO_ERR_NONE) {
        return;
    }

    if (((rxId & CO_SDO_ID_OFF) == 0) &&
        ((txId & CO_SDO_ID_OFF) == 0)){
        csdonum->TxId  = txId + nodeId;
        csdonum->RxId  = rxId + nodeId;
        csdonum->State = CO_CSDO_STATE_IDLE;
    }
}

CO_CSDO *COCSdoCheck(CO_CSDO *csdo, CO_IF_FRM *frm) {
    CO_CSDO *result;
    uint8_t  n;

    result = 0;

    if (frm != 0) {
        n = 0;
        while ((n < CO_CSDO_N) && (result == 0)) {
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

CO_ERR COCSdoResponse(CO_CSDO *csdo) {
    CO_ERR   result = CO_ERR_SDO_SILENT;
    uint16_t index;
    uint8_t  cmd, sub;

    cmd = CO_GET_BYTE(csdo->Frm, 0);

    if (cmd == 0x80) {
        /* SDO abort protocol */
        index = CO_GET_WORD(csdo->Frm, 1);
        sub   = CO_GET_BYTE(csdo->Frm, 3);
        if ((index == csdo->Tfer.Idx) &&
            (sub   == csdo->Tfer.Sub)) {
            csdo->Tfer.Abort = CO_GET_LONG(csdo->Frm, 4);
            COCSdoTransferFinalize(csdo);
            return (result);
        }
    }

    if (csdo->Tfer.Type == CO_CSDO_TRANSFER_UPLOAD_SEGMENT) {
        if (cmd == 0x41) {
            COCSdoInitUploadSegmented(csdo);
        } else if ((cmd & 0xE0 ) == 0x00) {
            COCSdoUploadSegmented(csdo);
        } else {
            COCSdoAbort(csdo, CO_SDO_ERR_CMD);
            COCSdoTransferFinalize(csdo);
        }
    } else if (csdo->Tfer.Type == CO_CSDO_TRANSFER_DOWNLOAD_SEGMENT) {
        if (cmd == 0x60) {
            COCSdoInitDownloadSegmented(csdo);
        } else if (((cmd & 0xE0) ==  0x20) ) {
            if (csdo->Tfer.Size > csdo->Tfer.Buf_Idx) {
                COCSdoDownloadSegmented(csdo);
            } else {
                /* wait last response */
                COCSdoFinishDownloadSegmented(csdo);
            }
        } else {
            COCSdoAbort(csdo, CO_SDO_ERR_CMD);
            COCSdoTransferFinalize(csdo);
        }
    } else if (cmd == 0x60) {
        result = COCSdoDownloadExpedited(csdo);
        return (result);
    } else if ((cmd & 0x43) != 0) {
        result = COCSdoUploadExpedited(csdo);
        return (result);
    } else {
        COCSdoAbort(csdo, CO_SDO_ERR_PARA_INCOMP);
    }
    
    return (result);
}

void COCSdoTimeout(void *parg) {
    CO_CSDO *csdo;

    csdo = (CO_CSDO *)parg;
    if (csdo->State == CO_CSDO_STATE_BUSY) {
        /* Abort SDO transfer because of timeout */
        COCSdoAbort(csdo, CO_SDO_ERR_TIMEOUT);
        /* Finalize aborted transfer */
        COCSdoTransferFinalize(csdo);
    }
}

void COCSdoTransferFinalize(CO_CSDO *csdo) {
    CO_CSDO_CALLBACK_T call;
    uint16_t           idx;
    uint8_t            sub;
    uint32_t           code;

    if (csdo->State == CO_CSDO_STATE_BUSY) {
        /* Fetch transfer information */
        idx  = csdo->Tfer.Idx;
        sub  = csdo->Tfer.Sub;
        code = csdo->Tfer.Abort;
        call = csdo->Tfer.Call;

        if (call != 0) {
            call(csdo, idx, sub, code);
        }

        /* Reset finished transfer information */
        csdo->Tfer.Type  = CO_CSDO_TRANSFER_NONE;
        csdo->Tfer.Abort = 0;
        csdo->Tfer.Idx   = 0;
        csdo->Tfer.Sub   = 0;
        csdo->Tfer.Buf   = 0;
        csdo->Tfer.Size  = 0;
        csdo->Tfer.Tmt   = 0;
        csdo->Tfer.Call  = 0;
        csdo->Tfer.Tmr   = -1;
        csdo->Tfer.Buf_Idx = 0;
        csdo->Tfer.TBit = 0;

        /* Release SDO client for next request */
        csdo->Frm   = 0;
        csdo->State = CO_CSDO_STATE_IDLE;
    }
}

void COCSdoAbort(CO_CSDO *csdo, uint32_t err) {
    CO_IF_FRM frm;

    /* In some cases, frame is not assigned
     * to SDO client handle. A local instance
     * of frame will be attached and transmitted
     * directly during function call.
     */
    if (csdo->Frm == 0) {
        csdo->Frm = &frm;
        CO_SET_ID(csdo->Frm, csdo->TxId);
    }

    csdo->Tfer.Abort = err;

    CO_SET_BYTE(csdo->Frm,           0x80, 0);
    CO_SET_WORD(csdo->Frm, csdo->Tfer.Idx, 1);
    CO_SET_BYTE(csdo->Frm, csdo->Tfer.Sub, 3);
    CO_SET_LONG(csdo->Frm,            err, 4);
    CO_SET_DLC (csdo->Frm,                 8);

    if (err == CO_SDO_ERR_TIMEOUT) {
        (void)COIfCanSend(&csdo->Node->If, csdo->Frm);
    }
}

/* EXPEDITED UPLOAD */

CO_ERR COCSdoUploadExpedited(CO_CSDO *csdo) {
    CO_ERR   result = CO_ERR_SDO_SILENT;
    uint32_t width;
    uint8_t  cmd;
    uint8_t  n;

    cmd = CO_GET_BYTE(csdo->Frm, 0);
    width = 4 - ((cmd >> 2) & 0x03);
    if ((width > 0) && (width <= 4)) {
        if (width > csdo->Tfer.Size) {
            COCSdoAbort(csdo, CO_SDO_ERR_MEM);
        } else {
            for (n = 0; n < width; n++) {
                csdo->Tfer.Buf[n] = CO_GET_BYTE(csdo->Frm, n + 4);
            }
        }
    }
    COCSdoTransferFinalize(csdo);
    return (result);
}

/* EXPEDITED DOWNLOAD */

CO_ERR COCSdoDownloadExpedited(CO_CSDO *csdo) {
    COCSdoTransferFinalize(csdo);
    return CO_ERR_SDO_SILENT;
}

/* SEGMENTED UPLOAD */

CO_ERR COCSdoInitUploadSegmented(CO_CSDO *csdo) {
    CO_ERR    result = CO_ERR_SDO_SILENT;
    uint32_t  obj_size;
    uint32_t  ticks;
    uint16_t  Idx;
    uint8_t   Sub;
    CO_IF_FRM frm;

    obj_size = CO_GET_LONG(csdo->Frm, 4);
    Idx = CO_GET_WORD(csdo->Frm, 1);
    Sub = CO_GET_BYTE(csdo->Frm, 3);

    /* verify size, Idx, Sub */
    if ((obj_size == csdo->Tfer.Size) &&
        (Idx == csdo->Tfer.Idx) &&
        (Sub == csdo->Tfer.Sub)) {

        result = CO_ERR_NONE;

        /* setup CAN request */
        CO_SET_ID  (&frm, csdo->TxId);
        CO_SET_DLC (&frm, 8);
        CO_SET_BYTE(&frm, 0x60, 0);
        CO_SET_WORD(&frm, 0, 1);
        CO_SET_BYTE(&frm, 0, 3);
        CO_SET_LONG(&frm, 0, 4);

        /* refresh timer */
        COTmrDelete(&(csdo->Node->Tmr), csdo->Tfer.Tmr);
        ticks = COTmrGetTicks(&(csdo->Node->Tmr), csdo->Tfer.Tmt, CO_TMR_UNIT_1MS);
        csdo->Tfer.Tmr = COTmrCreate(&(csdo->Node->Tmr), ticks, 0, &COCSdoTimeout, csdo);

        (void)COIfCanSend(&csdo->Node->If, &frm);
    } else {
        COCSdoAbort(csdo, CO_SDO_ERR_LEN);
        COCSdoTransferFinalize(csdo);
    }

    return result;
}

CO_ERR COCSdoUploadSegmented(CO_CSDO *csdo) {
    CO_ERR    result = CO_ERR_SDO_SILENT;
    uint32_t  ticks;
    uint8_t   cmd;
    uint8_t   n;
    CO_IF_FRM frm;

    cmd = CO_GET_BYTE(csdo->Frm, 0);
    if (((cmd >> 4) & 0x01) == csdo->Tfer.TBit) {

        for (n = 1; (n < 8) && (csdo->Tfer.Buf_Idx < csdo->Tfer.Size); n++) {
            csdo->Tfer.Buf[csdo->Tfer.Buf_Idx] = CO_GET_BYTE(csdo->Frm, n);
            csdo->Tfer.Buf_Idx++;
        }

        if ((cmd & 0x01) == 0x00) {
            csdo->Tfer.TBit ^= 0x01;

            CO_SET_ID  (&frm, csdo->TxId       );
            CO_SET_DLC (&frm, 8                );

            if (csdo->Tfer.TBit == 0x01){
                CO_SET_BYTE(&frm, 0x70, 0);
            } else {
                CO_SET_BYTE(&frm, 0x60, 0);
            }

            CO_SET_WORD(&frm, 0, 1);
            CO_SET_BYTE(&frm, 0, 3);
            CO_SET_LONG(&frm, 0, 4);

            /* refresh timer */
            COTmrDelete(&(csdo->Node->Tmr), csdo->Tfer.Tmr);
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

/* SEGMENTED DOWNLOAD */

CO_ERR COCSdoInitDownloadSegmented(CO_CSDO *csdo) {
    CO_ERR    result = CO_ERR_SDO_SILENT;
    uint32_t  ticks;
    uint16_t  Idx;
    uint8_t   Sub;
    uint8_t   n;
    uint8_t   width;
    uint8_t   c_bit = 1;
    uint8_t   cmd;
    CO_IF_FRM frm;

    Idx = CO_GET_WORD(csdo->Frm, 1);
    Sub = CO_GET_BYTE(csdo->Frm, 3);
    if ((Idx == csdo->Tfer.Idx) &&
        (Sub == csdo->Tfer.Sub)) {

        CO_SET_ID  (&frm, csdo->TxId);
        CO_SET_DLC (&frm, 8         );
        CO_SET_LONG(&frm, 0, 0);
        CO_SET_LONG(&frm, 0, 4);

        width = csdo->Tfer.Size - csdo->Tfer.Buf_Idx;
        if (width > 7) {
            width = 7;
            c_bit = 0;
        }

        for (n = 1; n <= width; n++) {
            CO_SET_BYTE(&frm, csdo->Tfer.Buf[csdo->Tfer.Buf_Idx], n);
            csdo->Tfer.Buf_Idx++;
        }

        cmd = (uint8_t)(csdo->Tfer.TBit << 4) |
              (uint8_t)(((7 - width) << 1)) | 
              (uint8_t)(c_bit);
        CO_SET_BYTE(&frm, cmd, 0);

        /* refresh timer */
        COTmrDelete(&(csdo->Node->Tmr), csdo->Tfer.Tmr);
        ticks = COTmrGetTicks(&(csdo->Node->Tmr), csdo->Tfer.Tmt, CO_TMR_UNIT_1MS);
        csdo->Tfer.Tmr = COTmrCreate(&(csdo->Node->Tmr), ticks, 0, &COCSdoTimeout, csdo);

        (void)COIfCanSend(&csdo->Node->If, &frm);
    } else {
        COCSdoAbort(csdo, CO_SDO_ERR_TBIT); 
        COCSdoTransferFinalize(csdo);
    }

    return result;
}

CO_ERR COCSdoDownloadSegmented(CO_CSDO *csdo) {
    CO_ERR    result = CO_ERR_SDO_SILENT;
    uint32_t  ticks;
    uint8_t   cmd;
    uint8_t   n;
    uint8_t   width;
    uint8_t   c_bit = 1;
    CO_IF_FRM frm;

    cmd = CO_GET_BYTE(csdo->Frm, 0);
    if (((cmd >> 4) & 0x01) == csdo->Tfer.TBit) {
        csdo->Tfer.TBit ^= 0x01;

        CO_SET_ID  (&frm, csdo->TxId       );
        CO_SET_DLC (&frm, 8                );

         /* clean frm data */
        CO_SET_LONG(&frm, 0, 0);
        CO_SET_LONG(&frm, 0, 4);

        width = csdo->Tfer.Size - csdo->Tfer.Buf_Idx;

        if (width > 7) {
            width = 7;
            c_bit = 0;
        }
        
        for (n = 1; n <= width; n++) {
            CO_SET_BYTE(&frm, csdo->Tfer.Buf[csdo->Tfer.Buf_Idx], n);
            csdo->Tfer.Buf_Idx++;
        }

        cmd = (uint8_t)(csdo->Tfer.TBit << 4) |
              (uint8_t)(((7 - width) << 1)) | 
              (uint8_t)(c_bit);

        CO_SET_BYTE(&frm, cmd, 0);

         /* refresh timer */
        COTmrDelete(&(csdo->Node->Tmr), csdo->Tfer.Tmr);
        ticks = COTmrGetTicks(&(csdo->Node->Tmr), csdo->Tfer.Tmt, CO_TMR_UNIT_1MS);
        csdo->Tfer.Tmr = COTmrCreate(&(csdo->Node->Tmr), ticks, 0, &COCSdoTimeout, csdo);

        (void)COIfCanSend(&csdo->Node->If, &frm);
    } else {
        COCSdoAbort(csdo, CO_SDO_ERR_TBIT);
        COCSdoTransferFinalize(csdo);
    }

    return result;
}

CO_ERR COCSdoFinishDownloadSegmented(CO_CSDO *csdo) {
    COCSdoTransferFinalize(csdo);
    return CO_ERR_SDO_SILENT;
}

#endif
