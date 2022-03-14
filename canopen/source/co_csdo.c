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

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

CO_CSDO * COCSdoFind(struct CO_NODE_T *node, uint8_t num) {
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
                           uint32_t index,
                           uint8_t sub,
                           uint8_t *buf,
                           uint32_t size,
                           CO_CSDO_CALLBACK_T callback,
                           uint32_t timeout) {
    CO_IF_FRM frm;

    if ((csdo == 0) || (callback == 0) ||
        (buf  == 0) || (size     == 0)) {
        /*
         * Invalid argument
         *
         * TODO: Maybe we can tolerate NULL callback,
         * but i do not see any reason why request
         * without callback should be triggered.
         */
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

    /*
     * Set client as busy to prevent its usage
     * until requested transfer is complete
     */
    csdo->State = CO_CSDO_STATE_BUSY;

    /* Update transfer info */
    csdo->Tfer.Type             = CO_CSDO_TRANSFER_UPLOAD;
    csdo->Tfer.Abort            = 0;
    csdo->Tfer.Idx              = index;
    csdo->Tfer.Sub              = sub;
    csdo->Tfer.Buf              = buf;
    csdo->Tfer.Size             = size;
    csdo->Tfer.Tmt              = timeout;
    csdo->Tfer.Call             = callback;

    /* Transmit transfer initiation directly */
    CO_SET_ID   (&frm, csdo->TxId       );
    CO_SET_DLC  (&frm, 8                );
    CO_SET_BYTE (&frm, 0x40          , 0);
    CO_SET_WORD (&frm, csdo->Tfer.Idx, 1);
    CO_SET_BYTE (&frm, csdo->Tfer.Sub, 3);

    (void)COIfCanSend(&csdo->Node->If, &frm);

    return CO_ERR_NONE;
}

CO_ERR COCSdoRequestDownload(CO_CSDO *csdo,
                             uint32_t index,
                             uint8_t sub,
                             uint8_t *buf,
                             uint32_t size,
                             CO_CSDO_CALLBACK_T callback,
                             uint32_t timeout) {
    CO_IF_FRM   frm;
    uint8_t     cmd;
    uint8_t     n;

    if ((csdo == 0) || (buf      == 0) ||
        (size == 0) || (callback == 0)) {
        /*
         * Invalid argument
         *
         * TODO: Maybe we can tolerate NULL callback,
         * but i do not see any reason why request
         * without callback should be triggered.
         */
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

    /*
     * Only SDO expedited transfer is
     * currently supported, decline
     * requests bigger than 4 bytes.
     */
    if (size > 4) {
        return CO_ERR_BAD_ARG;
    }

    /*
     * Set client as busy to prevent its usage
     * until requested transfer is complete
     */
    csdo->State = CO_CSDO_STATE_BUSY;
    /* Update transfer info */
    csdo->Tfer.Type             = CO_CSDO_TRANSFER_DOWNLOAD;
    csdo->Tfer.Abort            = 0;
    csdo->Tfer.Idx              = index;
    csdo->Tfer.Sub              = sub;
    csdo->Tfer.Buf              = buf;
    csdo->Tfer.Size             = size;
    csdo->Tfer.Tmt              = timeout;
    csdo->Tfer.Call             = callback;

    for (n = 4; n < 8; n++) {
        CO_SET_BYTE(&frm, *buff++, n);
    }

    cmd = ((0x23) | ((4 - size) << 2));
    CO_SET_BYTE(&frm, cmd, 0);

    /* Transmit transfer initiation directly */
    CO_SET_ID   (&frm, csdo->TxId       );
    CO_SET_DLC  (&frm, 8                );
    CO_SET_WORD (&frm, csdo->Tfer.Idx, 1);
    CO_SET_BYTE (&frm, csdo->Tfer.Sub, 3);
    (void)COIfCanSend(&csdo->Node->If, &frm);

    return CO_ERR_NONE;
}

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

void COCSdoInit(CO_CSDO *csdo, struct CO_NODE_T *node) {
    uint8_t n;

    for (n = 0; n < CO_CSDO_N; n++) {
        COCSdoReset(csdo, n, node);
        COCSdoEnable(csdo, n);
    }
}

void COCSdoReset(CO_CSDO *csdo, uint8_t num, struct CO_NODE_T *node) {
    CO_CSDO    *csdonum;
    int16_t     tid;

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
    csdonum                     = &csdo[num];
    csdonum->Node               = node;
    csdonum->Frm                = 0;
    csdonum->RxId               = CO_SDO_ID_OFF;
    csdonum->TxId               = CO_SDO_ID_OFF;
    csdonum->State              = CO_CSDO_STATE_INVALID;

    /* Reset transfer context */
    csdonum->Tfer.Csdo          = csdonum;
    csdonum->Tfer.Type          = CO_CSDO_TRANSFER_NONE;
    csdonum->Tfer.Abort         = 0;
    csdonum->Tfer.Idx           = 0;
    csdonum->Tfer.Sub           = 0;

    csdonum->Tfer.Tmt           = 0;
    csdonum->Tfer.Call          = 0;

    if (csdonum->Tfer.Tmr >= 0) {
        tid = COTmrDelete(&node->Tmr, csdonum->Tfer.Tmr);
        if (tid < 0) {
            node->Error = CO_ERR_TMR_DELETE;
            return;
        }
    }
    csdonum->Tfer.Tmr           = -1;
}

void COCSdoEnable(CO_CSDO *csdo, uint8_t num) {
    uint32_t    rxId, txId;
    CO_NODE    *node;
    CO_CSDO    *csdonum;
    CO_ERR      err;

    if (num >= CO_CSDO_N) {
        return;
    }
    csdonum                     = &csdo[num];
    csdonum->RxId               = CO_SDO_ID_OFF;
    csdonum->TxId               = CO_SDO_ID_OFF;
    csdonum->State              = CO_CSDO_STATE_INVALID;

    node = csdo->Node;
    err = CODictRdLong(&node->Dict, CO_DEV(0x1280 + num, 1), &txId);
    if (err != CO_ERR_NONE) {
        return;
    }
    err = CODictRdLong(&node->Dict, CO_DEV(0x1280 + num, 2), &rxId);
    if (err != CO_ERR_NONE) {
        return;
    }

    if (((rxId & CO_SDO_ID_OFF) == 0) &&
        ((txId & CO_SDO_ID_OFF) == 0)){
        csdonum->TxId   = txId;
        csdonum->RxId   = rxId;
        csdonum->State  = CO_CSDO_STATE_IDLE;
    }
}

CO_CSDO *COCSdoCheck(CO_CSDO *csdo, CO_IF_FRM *frm) {
    CO_CSDO    *result;
    uint8_t     n;

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
                csdo[n].Frm         = frm;
                csdo[n].Tfer.Abort  = 0;
                result = &csdo[n];
                break;
            }
            n++;
        }
    }

    return (result);
}

CO_ERR COCSdoResponse(CO_CSDO *csdo) {
    CO_ERR      result = CO_ERR_SDO_SILENT;
    uint16_t    index;
    uint8_t     cmd, sub;

    cmd = CO_GET_BYTE(csdo->Frm, 0);

    if (cmd == 0x80) {
        /* SDO abort protocol */
        index   = CO_GET_WORD(csdo->Frm, 1);
        sub     = CO_GET_BYTE(csdo->Frm, 3);
        if ((index == csdo->Tfer.Idx) &&
            (sub   == csdo->Tfer.Sub)) {
            csdo->Tfer.Abort = CO_GET_LONG(csdo->Frm, 4);
            COCSdoTransferFinalize(csdo);
            return (result);
        }
    }

    /*
     * Only expedited transfers are supported right now
     */
    if (cmd == 0x60) {
        result = COCSdoDownloadExpedited(csdo);
        return (result);
    }
    else if ((cmd & 0x43) != 0) {
        result = COCSdoUploadExpedited(csdo);
        return (result);
    } else {
        COCSdoAbort(csdo, CO_SDO_ERR_PARA_INCOMP);
    }
    return (result);
}

CO_ERR COCSdoUploadExpedited(CO_CSDO *csdo) {
    return CO_ERR_NONE;
}

CO_ERR COCSdoDownloadExpedited(CO_CSDO *csdo) {
    return CO_ERR_NONE;
}

void COCSdoTimeout(void *parg) {
    CO_CSDO    *csdo;

    csdo = (CO_CSDO *) parg;
    if (csdo->State == CO_CSDO_STATE_BUSY) {
        /* Abort SDO transfer because of timeout */
        COCSdoAbort(csdo, CO_SDO_ERR_TIMEOUT);
        /* Finalize aborted transfer */
        COCSdoTransferFinalize(csdo);
    }
}

void COCSdoAbort(CO_CSDO *csdo, uint32_t err) {
    CO_IF_FRM frm = {0};

    /*
     * In some cases, frame is not assigned
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

void COCSdoTransferFinalize(CO_CSDO *csdo) {
    CO_CSDO_CALLBACK_T      call;
    uint16_t                idx;
    uint8_t                 sub;
    uint32_t                code;
    int16_t                 tid;

    if (csdo->State == CO_CSDO_STATE_BUSY) {
        /* Fetch transfer information */
        idx         = csdo->Tfer.Idx;
        sub         = csdo->Tfer.Sub;
        code        = csdo->Tfer.Abort;
        call        = csdo->Tfer.Call;

        if (call != 0) {
            call(csdo, idx, sub, code);
        }

        /* Reset finished transfer information */
        csdo->Tfer.Type         = CO_CSDO_TRANSFER_NONE;
        csdo->Tfer.Abort        = 0;
        csdo->Tfer.Idx          = 0;
        csdo->Tfer.Sub          = 0;
        csdo->Tfer.Buf          = 0;
        csdo->Tfer.Size         = 0;
        csdo->Tfer.Tmt          = 0;
        csdo->Tfer.Call         = 0;

        if (csdo->Tfer.Tmr >= 0) {
            tid = COTmrDelete(&csdo->Node->Tmr, csdo->Tfer.Tmr);
            if (tid < 0) {
                csdo->Node->Error = CO_ERR_TMR_DELETE;
            } else {
                csdo->Tfer.Tmr   = -1;
            }
        }

        /* Release SDO client for next request */
        csdo->State = CO_CSDO_STATE_IDLE;
    }
}
