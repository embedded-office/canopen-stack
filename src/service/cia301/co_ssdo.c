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
* PROTECTED API FUNCTIONS
******************************************************************************/

void COSdoInit(CO_SDO *srv, CO_NODE *node)
{
    uint8_t n;

    for (n=0; n < CO_SSDO_N; n++) {
        COSdoReset (srv, n, node);
        COSdoEnable(srv, n);
    }
}

WEAK_TEST
void COSdoReset(CO_SDO *srv, uint8_t num, CO_NODE *node)
{
    CO_SDO   *srvnum;
    uint32_t  offset;

    ASSERT_PTR(srv);
    ASSERT_LOWER(num, CO_SSDO_N);

    srvnum               = &srv[num];
    srvnum->Node         = node;
    srvnum->RxId         = CO_SDO_ID_OFF;
    srvnum->TxId         = CO_SDO_ID_OFF;
    srvnum->Frm          = 0;
    srvnum->Obj          = 0;
    offset               = num * CO_SDO_BUF_BYTE;
    srvnum->Buf.Start    = &node->SdoBuf[offset];
    srvnum->Buf.Cur      = &node->SdoBuf[offset];
    srvnum->Buf.Num      = 0;
    srvnum->Seg.TBit     = 0;
    srvnum->Seg.Num      = 0;
    srvnum->Seg.Size     = 0;
    srvnum->Blk.State    = BLK_IDLE;
}

WEAK_TEST
void COSdoEnable(CO_SDO *srv, uint8_t num)
{
    uint32_t rxId;
    uint32_t txId;
    CO_NODE *node;
    CO_SDO  *srvnum;
    CO_ERR   err;

    ASSERT_LOWER(num, CO_SSDO_N);

    srvnum       = &srv[num];
    srvnum->RxId = CO_SDO_ID_OFF;
    srvnum->TxId = CO_SDO_ID_OFF;

    node = srv->Node;
    err  = CODictRdLong(&node->Dict, CO_DEV(0x1200 + num, 1), &rxId);
    if (err != CO_ERR_NONE) {
        return;
    }
    err = CODictRdLong(&node->Dict, CO_DEV(0x1200 + num, 2), &txId);
    if (err != CO_ERR_NONE) {
        return;
    }

    if (((rxId & CO_SDO_ID_OFF) == 0) &&
        ((txId & CO_SDO_ID_OFF) == 0) ) {
        srvnum->RxId = rxId;
        srvnum->TxId = txId;
    }
}

CO_SDO *COSdoCheck(CO_SDO *srv, CO_IF_FRM *frm)
{
    CO_SDO  *result = 0;
    uint8_t  n;

    if (frm != 0) {
        n = 0;
        while ((n < CO_SSDO_N) && (result == 0)) {
            if (CO_GET_ID(frm) == srv[n].RxId) {
                CO_SET_ID(frm, srv[n].TxId);
                srv[n].Frm   = frm;
                srv[n].Abort = 0;
                if (srv[n].Obj == 0) {
                    srv[n].Idx = CO_GET_WORD(frm, 1);
                    srv[n].Sub = CO_GET_BYTE(frm, 3);
                }
                result = &srv[n];
            }
            n++;
        }
    }
    return (result);
}

CO_ERR COSdoResponse(CO_SDO *srv)
{
    CO_ERR  result = CO_ERR_SDO_ABORT;
    uint8_t cmd;

    cmd = CO_GET_BYTE(srv->Frm, 0);

    /* client abort */
    if (cmd == 0x80) {
        COSdoAbortReq(srv);
        return (result);
    }

    /* active block transfer */
    if (srv->Blk.State == BLK_DOWNLOAD) {
        result = COSdoDownloadBlock(srv);
        return (result);
    } else if (srv->Blk.State == BLK_DNWAIT) {
        if ((cmd & 0xE3) == 0xC1) {
            result = COSdoEndDownloadBlock(srv);
        } else {
            srv->Blk.State = BLK_DOWNLOAD;
            result = COSdoDownloadBlock(srv);
        }
        return (result);
    } else if (srv->Blk.State == BLK_UPLOAD) {
        if (cmd == 0xA1) {
            result = COSdoEndUploadBlock(srv);
        } else if ((cmd & 0xE3) == 0xA2) {
            result = COSdoAckUploadBlock(srv);
        } else {
            COSdoAbort(srv, CO_SDO_ERR_CMD);
            COSdoAbortReq(srv);
        }
        return (result);
    }

    /* expedited transfer */
    if ((cmd & 0xF2) == 0x22) {
        result = COSdoGetObject(srv, CO_SDO_WR);
        if (result == 0) {
            result = COSdoDownloadExpedited(srv);
        }
    } else if (cmd == 0x40) {
        result = COSdoGetObject(srv, CO_SDO_RD);
        if (result == 0) {
            result = COSdoUploadExpedited(srv);
        }

    /* segmented transfer */
    } else if ((cmd & 0xF2) == 0x20) {
        result = COSdoGetObject(srv, CO_SDO_WR);
        if (result == 0) {
            result = COSdoInitDownloadSegmented(srv);
        }
    } else if ((cmd & 0xE0) == 0x00) {
        result = COSdoDownloadSegmented(srv);
    } else if ((cmd & 0xEF) == 0x60) {
        result = COSdoUploadSegmented(srv);

    /* block transfer */
    } else if ((cmd & 0xF9) == 0xC0) {
        result = COSdoGetObject(srv, CO_SDO_WR);
        if (result == 0) {
            result = COSdoInitDownloadBlock(srv);
        }
    } else if ((cmd & 0xE3) == 0xA0) {
        result = COSdoInitUploadBlock(srv);
    } else if (cmd == 0xA3) {
        result = COSdoUploadBlock(srv);

    /* invalid or unknown command */
    } else {
        COSdoAbort(srv, CO_SDO_ERR_CMD);
        COSdoAbortReq(srv);
    }

    /* set DLC for the SDO response */
    CO_SET_DLC(srv->Frm, 8u);

    return (result);
}

CO_ERR COSdoGetObject(CO_SDO *srv, uint16_t mode)
{
    CO_ERR   result = CO_ERR_SDO_ABORT;
    CO_OBJ  *obj;
    uint32_t key;

    key = CO_DEV(srv->Idx, srv->Sub);
    obj = CODictFind(&srv->Node->Dict, key);
    if (obj != 0) {
        if (mode == CO_SDO_RD) {
            if (CO_IS_READ(obj->Key) != 0) {
                srv->Obj = obj;
                result   = CO_ERR_NONE;
            } else {
                COSdoAbort(srv, CO_SDO_ERR_RD);
            }
        } else {
            if (CO_IS_WRITE(obj->Key) != 0) {
                srv->Obj = obj;
                result   = CO_ERR_NONE;
            } else {
                COSdoAbort(srv, CO_SDO_ERR_WR);
            }
        }
    } else {
        if (srv->Sub == 0) {
            COSdoAbort(srv, CO_SDO_ERR_OBJ);
        } else {
            key = CO_DEV(srv->Idx, 0);
            obj = CODictFind(&srv->Node->Dict, key);
            if (obj != 0) {
                COSdoAbort(srv, CO_SDO_ERR_SUB);
            } else {
                COSdoAbort(srv, CO_SDO_ERR_OBJ);
            }
        }
    }
    return (result);
}

uint32_t COSdoGetSize(CO_SDO *srv, uint32_t width, bool strict)
{
    uint32_t result = 0;
    uint32_t size;

    size = COObjGetSize(srv->Obj, srv->Node, width);
    if (size == 0) {
        COSdoAbort(srv, CO_SDO_ERR_TOS);
        return (result);
    }

    if (width != 0) {
        if (size == width) {
            result = size;
        } else {
            if (width < size) {
                if (strict) {
                    COSdoAbort(srv, CO_SDO_ERR_LEN_SMALL);
                } else {
                    result = width;
                }
            } else {
                COSdoAbort(srv, CO_SDO_ERR_LEN_HIGH);
            }
        }
    } else {
        result = size;
    }
    return (result);
}

CO_ERR COSdoUploadExpedited(CO_SDO *srv)
{
    CO_ERR   result = CO_ERR_SDO_ABORT;
    CO_ERR   err;
    uint32_t size;
    uint32_t data   = 0;
    uint8_t  cmd;

    size = COSdoGetSize(srv, 0, true);
    if (size == 0) {
        return (result);
    } else if (size <= 4) {
        err = COObjRdValue(srv->Obj, srv->Node, (void *)&data, (uint8_t)size);
        if (err != CO_ERR_NONE) {
            if (srv->Abort > 0) {
                COSdoAbort(srv, srv->Abort);
            } else {
                COSdoAbort(srv, CO_SDO_ERR_TOS);
            }
            return (result);
        } else {
            cmd = (uint8_t)0x43u | (uint8_t)(((4u - size) & 0x3u) << 2u);
            CO_SET_BYTE(srv->Frm,  cmd, 0);
            CO_SET_LONG(srv->Frm, data, 4);
            srv->Obj = 0;
            result   = CO_ERR_NONE;
        }
    } else {
        result = COSdoInitUploadSegmented(srv, size);
    }

    if (result != CO_ERR_NONE) {
        COSdoAbort(srv, CO_SDO_ERR_CMD);
    }
    return (result);
}

CO_ERR COSdoDownloadExpedited(CO_SDO *srv)
{
    CO_ERR   result = CO_ERR_SDO_ABORT;
    CO_ERR   err;
    uint32_t size;
    uint32_t data;
    uint32_t width  =  0;
    uint8_t  cmd;

    cmd = CO_GET_BYTE(srv->Frm, 0);
    if ((cmd & 0x01) == 1) {
        width = 4 - ((cmd >> 2) & 0x03);
    }
    size = COSdoGetSize(srv, width, true);
    if ((size > 0) && (size <= 4)) {
        data   = CO_GET_LONG(srv->Frm, 4);
        err    = COObjWrValue(srv->Obj, srv->Node, (void*)&data, (uint8_t)size);
        if (err != CO_ERR_NONE) {
            if (srv->Abort > 0) {
                COSdoAbort(srv, srv->Abort);
            } else if (err == CO_ERR_OBJ_RANGE) {
                COSdoAbort(srv, CO_SDO_ERR_RANGE);
            } else if (err == CO_ERR_OBJ_MAP_TYPE) {
                COSdoAbort(srv, CO_SDO_ERR_OBJ_MAP);
            } else if (err == CO_ERR_OBJ_ACC) {
                COSdoAbort(srv, CO_SDO_ERR_TOS);
            } else if (err == CO_ERR_OBJ_MAP_LEN) {
                COSdoAbort(srv, CO_SDO_ERR_OBJ_MAP_N);
            } else if (err == CO_ERR_OBJ_INCOMPATIBLE) {
                COSdoAbort(srv, CO_SDO_ERR_PARA_INCOMP);
            } else {
                COSdoAbort(srv, CO_SDO_ERR_TOS);
            }
        } else {
            CO_SET_BYTE(srv->Frm, 0x60, 0);
            CO_SET_LONG(srv->Frm,    0, 4);
            srv->Obj = 0;
            result   = CO_ERR_NONE;
        }
    }
    return (result);
}

void COSdoAbort(CO_SDO *srv, uint32_t err)
{
    CO_SET_BYTE(srv->Frm,     0x80, 0);
    CO_SET_WORD(srv->Frm, srv->Idx, 1);
    CO_SET_BYTE(srv->Frm, srv->Sub, 3);
    CO_SET_LONG(srv->Frm,      err, 4);

    srv->Obj = 0;
}

CO_ERR COSdoInitUploadSegmented(CO_SDO *srv, uint32_t size)
{
    CO_ERR  result = CO_ERR_NONE;
    uint8_t cmd;

    /* setup CAN response */
    cmd = 0x41;
    CO_SET_BYTE(srv->Frm, cmd, 0);
    CO_SET_LONG(srv->Frm, size, 4);

    /* setup SDO buffer */
    srv->Buf.Cur  = srv->Buf.Start;
    srv->Buf.Num  = 0;
    result = COObjRdBufStart(srv->Obj, srv->Node, srv->Buf.Cur, 0);
    if (result != CO_ERR_NONE) {
        srv->Node->Error = CO_ERR_SDO_READ;
        COSdoAbort(srv, CO_SDO_ERR_HW_ACCESS);
        result = CO_ERR_SDO_ABORT;
    }

    /* setup SDO protocol state */
    srv->Seg.Size = size;
    srv->Seg.TBit = 0;
    srv->Seg.Num  = 0;

    return (result);
}

CO_ERR COSdoUploadSegmented(CO_SDO *srv)
{
    CO_ERR   result = CO_ERR_NONE;
    uint32_t width;
    uint8_t  cmd;
    uint8_t  c_bit  = 0;
    uint8_t  i;

    if (srv->Obj == 0) {
        COSdoAbort(srv, CO_SDO_ERR_CMD);
        return (CO_ERR_SDO_ABORT);
    }

    cmd = CO_GET_BYTE(srv->Frm, 0);
    if (((cmd >> 4) & 0x01) != srv->Seg.TBit) {
        COSdoAbort(srv, CO_SDO_ERR_TBIT);
        return (CO_ERR_SDO_ABORT);
    }

    width = srv->Seg.Size - srv->Seg.Num;
    if (width > 7) {
        width = 7;
    } else {
        c_bit = 1;
    }

    result = COObjRdBufCont(srv->Obj, srv->Node, srv->Buf.Start, width);
    if (result != CO_ERR_NONE) {
        COSdoAbort(srv, CO_SDO_ERR_TOS);
        return (CO_ERR_SDO_ABORT);
    }

    srv->Buf.Cur = srv->Buf.Start;
    for (i = 0; i < (uint8_t)width; i++) {
        CO_SET_BYTE(srv->Frm, *(srv->Buf.Cur), 1 + i);
        srv->Buf.Cur++;
    }
    for (i = (uint8_t)(width + 1); i <= 7; i++) {
        CO_SET_BYTE(srv->Frm, 0, 1 + i);
    }

    cmd = (uint8_t)0x00 |
          (uint8_t)(srv->Seg.TBit << 4) |
          (uint8_t)(((7 - width) << 1) & 0x0E) |
          (uint8_t)c_bit;
    CO_SET_BYTE(srv->Frm, cmd, 0);

    if (c_bit == 1) {
        srv->Seg.Size  = 0;
        srv->Seg.Num   = 0;
        srv->Seg.TBit  = 0;
        srv->Obj       = 0;
    } else {
        srv->Seg.Num  += width;
        srv->Seg.TBit ^= 0x01;
    }
    return (result);
}

CO_ERR COSdoInitDownloadSegmented(CO_SDO *srv)
{
    CO_ERR   result = CO_ERR_SDO_ABORT;
    uint32_t size;
    uint32_t width  = 0;
    uint8_t  cmd;

    cmd = CO_GET_BYTE(srv->Frm, 0);
    if ((cmd & 0x01) == 1) {
        width = CO_GET_LONG(srv->Frm, 4);
    }
    size = COSdoGetSize(srv, width, true);
    if (size > 0) {

        /* setup CAN response */
        CO_SET_BYTE(srv->Frm, 0x60, 0);
        CO_SET_LONG(srv->Frm, 0, 4);

        /* setup SDO buffer */
        srv->Buf.Cur  = srv->Buf.Start;
        srv->Buf.Num  = 0;

        if (size <= 4) {
            /* no action for basic type entry */
            result = CO_ERR_NONE;
        } else {
            result = COObjWrBufStart(srv->Obj, srv->Node, srv->Buf.Cur, 0);
        }
        if (result != CO_ERR_NONE) {
            srv->Node->Error = CO_ERR_SDO_WRITE;
            COSdoAbort(srv, CO_SDO_ERR_HW_ACCESS);
            result = CO_ERR_SDO_ABORT;
        }

        /* setup SDO protocol state */
        srv->Seg.Size = size;
        srv->Seg.TBit = 0;
        srv->Seg.Num  = 0;
    }
    return (result);
}

CO_ERR COSdoDownloadSegmented(CO_SDO *srv)
{
    CO_ERR   result = CO_ERR_NONE;
    uint32_t num;
    uint32_t len;
    uint8_t  n;
    uint8_t  cmd;
    uint8_t  bid;

    cmd = CO_GET_BYTE(srv->Frm, 0);
    if ((cmd >> 4) != srv->Seg.TBit) {
        COSdoAbort(srv, CO_SDO_ERR_TBIT);
        return (CO_ERR_SDO_ABORT);
    }

    n = ((cmd >> 1) & 0x07);
    if (n == 0) {
        num = srv->Seg.Size - srv->Seg.Num;
        if (num > 7) {
            num = 7;
        }
    } else {
        num = 7 - n;
    }

    bid = 1;
    while (num > 0) {
        *(srv->Buf.Cur) = CO_GET_BYTE(srv->Frm, bid);
        srv->Buf.Num++;
        srv->Buf.Cur++;
        bid++;
        num--;
    }
    srv->Seg.Num += srv->Buf.Num;

    len = (uint32_t)srv->Buf.Num;
    result = COObjWrBufCont(srv->Obj, srv->Node, srv->Buf.Start, len);
    if ((cmd & 0x01) == 0x01) {
        if (result != CO_ERR_NONE) {
            srv->Node->Error = CO_ERR_SDO_WRITE;
            COSdoAbort(srv, CO_SDO_ERR_HW_ACCESS);
            result = CO_ERR_SDO_ABORT;
        }
        srv->Seg.Size = 0;
        srv->Seg.Num  = 0;
        srv->Obj      = 0;
    } else {
        if (len <= 4) {
            result = CO_ERR_SDO_WRITE;
        }
        if (result != CO_ERR_NONE) {
            srv->Node->Error = CO_ERR_SDO_WRITE;
            if (result == CO_ERR_SDO_WRITE) {
                COSdoAbort(srv, CO_SDO_ERR_GENERAL);
            } else {
                COSdoAbort(srv, CO_SDO_ERR_HW_ACCESS);
            }
            result = CO_ERR_SDO_ABORT;
        }
        srv->Buf.Cur  = srv->Buf.Start;
        srv->Buf.Num  = 0;
    }

    cmd = (uint8_t)((1 << 5) | (srv->Seg.TBit << 4));
    if (result == CO_ERR_NONE) {
        CO_SET_BYTE(srv->Frm, cmd, 0);
        CO_SET_BYTE(srv->Frm, 0, 1);
        CO_SET_WORD(srv->Frm, 0, 2);
        CO_SET_LONG(srv->Frm, 0, 4);
    }

    srv->Seg.TBit ^= 0x01;
    return (result);
}

CO_ERR COSdoInitDownloadBlock(CO_SDO *srv)
{
    CO_ERR   result = CO_ERR_SDO_ABORT;
    uint32_t width  =  0;
    uint32_t size;
    uint8_t  cmd;

    cmd = CO_GET_BYTE(srv->Frm, 0);
    if ((cmd & 0x02) != 0) {
        width = CO_GET_LONG(srv->Frm, 4);
    }
    size = COSdoGetSize(srv, width, false);
    if (size > 0) {
        srv->Blk.State  = BLK_DOWNLOAD;
        srv->Blk.SegNum = CO_SDO_BUF_SEG;
        srv->Blk.SegCnt = 0;
        srv->Blk.SegOk  = 0;
        srv->Blk.Size   = size;
        srv->Blk.Len    = size;
        srv->Buf.Cur    = srv->Buf.Start;
        srv->Buf.Num    = 0;

        CO_SET_BYTE(srv->Frm, 0xA0, 0);
        CO_SET_LONG(srv->Frm, (uint32_t)CO_SDO_BUF_SEG, 4);
        
        if (size <= 4) {
            /* no action for basic type entry */
            result = CO_ERR_NONE;
        } else {
            result = COObjWrBufStart(srv->Obj, srv->Node, srv->Buf.Cur, 0);
        }
        if (result != CO_ERR_NONE) {
            srv->Node->Error = CO_ERR_SDO_WRITE;
            COSdoAbort(srv, CO_SDO_ERR_TOS);
            return (result);
        }
        result = CO_ERR_NONE;
    }
    return (result);
}

CO_ERR COSdoEndDownloadBlock(CO_SDO *srv)
{
    CO_ERR   result = CO_ERR_SDO_ABORT;
    uint32_t len;
    uint8_t  cmd;
    uint8_t  n;

    cmd = CO_GET_BYTE(srv->Frm, 0);
    if ((cmd & 0x01) != 0) {
        n      = (cmd & 0x1C) >> 2;
        len    = ((uint32_t)srv->Buf.Num - n);
        result = COObjWrBufCont(srv->Obj, srv->Node, srv->Buf.Start, len);
        if (result != CO_ERR_NONE) {
            srv->Node->Error = CO_ERR_SDO_WRITE;
            COSdoAbort(srv, CO_SDO_ERR_TOS);
        }
        CO_SET_BYTE(srv->Frm, 0xA1, 0);
        CO_SET_WORD(srv->Frm, 0, 1);
        CO_SET_BYTE(srv->Frm, 0, 3);
        CO_SET_LONG(srv->Frm, 0, 4);

        srv->Blk.State = BLK_IDLE;
        srv->Buf.Cur   = srv->Buf.Start;
        srv->Buf.Num   = 0;
        srv->Obj       = 0;
        result         = CO_ERR_NONE;
    }
    return (result);
}

CO_ERR COSdoDownloadBlock(CO_SDO *srv)
{
    CO_ERR   result = CO_ERR_SDO_SILENT;
    CO_ERR   err;
    uint32_t len;
    uint8_t  cmd;
    uint8_t  i;

    cmd = CO_GET_BYTE(srv->Frm, 0);
    if ((cmd & 0x7F) == (srv->Blk.SegCnt + 1)) {
        /* check, that we need at least 1 byte out of the payload */
        if (srv->Blk.Len > 0) {
            for (i = 0; i < 7; i++) {
                *(srv->Buf.Cur) = CO_GET_BYTE(srv->Frm, 1 + i);
                srv->Buf.Cur++;
                srv->Buf.Num++;
                if (srv->Blk.Len > 0) {
                    srv->Blk.Len--;
                }
            }
        } else {
            srv->Blk.State = BLK_IDLE;
            srv->Buf.Cur   = srv->Buf.Start;
            srv->Buf.Num   = 0;
            srv->Obj       = 0;
            COSdoAbort(srv, CO_SDO_ERR_LEN_HIGH);
            result = CO_ERR_SDO_ABORT;
            return (result);
        }
        srv->Blk.SegCnt++;
        if ((srv->Blk.SegCnt == CO_SDO_BUF_SEG) ||
             ((cmd & 0x80)   != 0             )) {

            CO_SET_BYTE(srv->Frm, 0xA2, 0);
            CO_SET_BYTE(srv->Frm, srv->Blk.SegCnt, 1);
            CO_SET_BYTE(srv->Frm, CO_SDO_BUF_SEG, 2);
            for (i = 3; i <= 7; i++) {
                CO_SET_BYTE(srv->Frm, 0, i);
            }
            srv->Blk.SegCnt  = 0;
            srv->Blk.State   = BLK_DNWAIT;
            result           = CO_ERR_NONE;
        }

        if (result == CO_ERR_NONE) {
            if ((cmd & 0x80) == 0) {
                len = (uint32_t)srv->Buf.Num;
                err = COObjWrBufCont(srv->Obj, srv->Node, srv->Buf.Start, len);
                if (err != CO_ERR_NONE) {
                    srv->Node->Error = CO_ERR_SDO_WRITE;
                }
                srv->Buf.Cur = srv->Buf.Start;
                srv->Buf.Num = 0;
            }
        }
    } else {
        if ((srv->Blk.SegCnt & 0x80) == 0) {
            srv->Blk.SegCnt |= 0x80;
        }

        if (((cmd & 0x7F) == CO_SDO_BUF_SEG) ||
            ((cmd & 0x80) != 0             )) {

            CO_SET_BYTE(srv->Frm, 0xA2, 0);
            CO_SET_BYTE(srv->Frm, srv->Blk.SegCnt & 0x7F, 1);
            CO_SET_BYTE(srv->Frm, CO_SDO_BUF_SEG, 2);
            CO_SET_BYTE(srv->Frm, 0, 3);
            CO_SET_LONG(srv->Frm, 0, 4);

            srv->Blk.SegCnt = 0;
            result          = CO_ERR_NONE;
        }
    }
    return (result);
}

CO_ERR COSdoInitUploadBlock(CO_SDO *srv)
{
    CO_ERR   result = CO_ERR_NONE;
    CO_ERR   err = CO_ERR_SDO_READ;
    uint32_t size;
    uint8_t  cmd;

    err = COSdoGetObject(srv, CO_SDO_RD);
    if (err != CO_ERR_NONE) {
        return (CO_ERR_SDO_ABORT);
    }

    srv->Blk.Size = COSdoGetSize(srv, 0, true);
    if (srv->Blk.Size == 0) {
        return (CO_ERR_SDO_ABORT);
    }
    srv->Blk.SegNum = CO_GET_BYTE(srv->Frm, 4);

    if ((srv->Blk.SegNum < 0x01) ||
        (srv->Blk.SegNum > 0x7F)) {
        COSdoAbort(srv, CO_SDO_ERR_BLK_SIZE);
        COSdoAbortReq(srv);
        return (CO_ERR_SDO_ABORT);
    } else {
        if (srv->Blk.SegNum > CO_SDO_BUF_SEG) {
            srv->Blk.SegNum = CO_SDO_BUF_SEG;
        }
    }

    size  = srv->Blk.Size;
    cmd   = 0xC2;

    srv->Blk.LastValid = 0xFF;
    srv->Blk.Len       = srv->Blk.Size;
    srv->Blk.SegOk     = 0;

    if (size <= 4) {
        /* no action for basic type entry */
        err = CO_ERR_NONE;
    } else {
        err = COObjRdBufStart(srv->Obj, srv->Node, srv->Buf.Cur, 0);
    }
    if (err != CO_ERR_NONE) {
        srv->Node->Error = CO_ERR_SDO_READ;
    }
    CO_SET_BYTE(srv->Frm, cmd, 0);
    CO_SET_LONG(srv->Frm, size, 4);
    return (result);
}

CO_ERR COSdoUploadBlock(CO_SDO *srv)
{
    CO_ERR   result = CO_ERR_SDO_SILENT;
    CO_ERR   err;
    uint32_t size;
    uint32_t num = 0;
    uint32_t txNum;
    uint32_t byteOk = 0;
    uint8_t *txBuf;
    uint8_t  finished =  0;
    uint8_t  seg;
    uint8_t  len;
    uint8_t  i;

    srv->Buf.Cur = srv->Buf.Start;
    srv->Buf.Num = 0u;
    num          = srv->Blk.SegNum * 7u;

    if (srv->Blk.State == BLK_REPEAT) {
        /* calculate number of bytes we need to repeat */
        byteOk        = srv->Blk.SegOk * 7u;
        num           = srv->Blk.SegCnt * 7u;
        num          -= byteOk;
        srv->Buf.Num  = num;
        srv->Blk.Len += num;
        if (srv->Blk.LastValid < 7) {
            srv->Blk.Len -= (7u - srv->Blk.LastValid);
        }
        if (srv->Blk.SegOk > 0) {
            /* remove successful transfered bytes at the front */
            srv->Buf.Cur  = srv->Buf.Start;
            txBuf         = srv->Buf.Start + byteOk;
            txNum         = num;
            while(txNum > 0) {
                *srv->Buf.Cur = *txBuf;
                srv->Buf.Cur++;
                txBuf++;
                txNum--;
            }
        } else {
            /* repeat whole buffer (no remaining bytes needed) */
            num = 0u;
        }
    }

    if (num > 0u) {
        if (srv->Blk.Size > num) {
            /* fill remaining buffer with data from object entry */
            err = COObjRdBufCont(srv->Obj, srv->Node, srv->Buf.Cur, num);
            if (err != CO_ERR_NONE) {
                srv->Node->Error = CO_ERR_SDO_READ;
            }
            srv->Blk.Size -= num;
        } else {
            /* read remaining data from object entry in buffer */
            if (srv->Blk.Size <= 4) {
                err = COObjRdBufCont(srv->Obj, srv->Node, srv->Buf.Cur, srv->Blk.Size);
            } else {
                err = COObjRdBufCont(srv->Obj, srv->Node, srv->Buf.Cur, num);
            } 
            if (err != CO_ERR_NONE) {
                srv->Node->Error = CO_ERR_SDO_READ;
            }
            srv->Blk.Size = 0;
        }
    }

    /* set DLC for block transfers */
    CO_SET_DLC(srv->Frm, 8u);

    srv->Blk.State  = BLK_UPLOAD;
    srv->Blk.SegCnt = 1;
    srv->Buf.Cur    = srv->Buf.Start;
    while ((srv->Blk.SegCnt <= srv->Blk.SegNum) && (finished == 0)) {
        seg  = srv->Blk.SegCnt;
        size = srv->Blk.Len;
        if (size > 7) {
            len           = 7;
            srv->Blk.Len -= 7;
            if (srv->Blk.SegCnt < srv->Blk.SegNum) {
                srv->Blk.SegCnt++;
            } else {
                finished = 1;
            }
        } else {
            len          = (uint8_t)size;
            srv->Blk.Len = 0;
            finished     = 1;
        }
        if (finished == 1) {
            if (srv->Blk.Len == 0) {
                seg |= 0x80;
            }
            srv->Blk.LastValid  = len;
        }
        CO_SET_BYTE(srv->Frm, seg, 0);
        for (i = 0; i < len; i++) {
            CO_SET_BYTE(srv->Frm, *(srv->Buf.Cur), 1+i);
            srv->Buf.Cur++;
            srv->Buf.Num--;
        }
        for (i = (uint8_t)len; i < 7; i++) {
            CO_SET_BYTE(srv->Frm, 0, 1 + i);
        }
        (void)COIfCanSend(&srv->Node->If, srv->Frm);
    }
    return (result);
}

CO_ERR COSdoAckUploadBlock(CO_SDO *srv)
{
    CO_ERR  result = CO_ERR_SDO_SILENT;
    uint8_t cmd;
    uint8_t seq;
    uint8_t val;
    uint8_t i;

    seq = CO_GET_BYTE(srv->Frm, 1);
    if (seq > srv->Blk.SegCnt) {
        COSdoAbort(srv, CO_SDO_ERR_SEQ_NUM);
        COSdoAbortReq(srv);
        return (CO_ERR_SDO_ABORT);
    } else if (seq < srv->Blk.SegCnt) {
        srv->Blk.State = BLK_REPEAT;
        srv->Blk.SegOk = seq;
        result         = COSdoUploadBlock(srv);
    } else if (srv->Blk.Len == 0) {
        if (srv->Blk.LastValid <= 7) {
            val = (uint8_t)srv->Blk.LastValid;
            cmd = (uint8_t)0xC0 |
                  (uint8_t)((7u - val) << 2) |
                  (uint8_t)0x01;
            CO_SET_BYTE(srv->Frm, cmd, 0);
            for (i = 1; i <= 7; i++) {
                CO_SET_BYTE(srv->Frm, 0, i);
            }
            result = CO_ERR_NONE;
        }
    } else {
        srv->Blk.SegNum = CO_GET_BYTE(srv->Frm, 2);
        if ((srv->Blk.SegNum < 0x01) ||
            (srv->Blk.SegNum > 0x7F)) {
            COSdoAbort(srv, CO_SDO_ERR_BLK_SIZE);
            COSdoAbortReq(srv);
            return (CO_ERR_SDO_ABORT);
        } else {
            if (srv->Blk.SegNum > CO_SDO_BUF_SEG) {
                srv->Blk.SegNum = CO_SDO_BUF_SEG;
            }
        }
        result = COSdoUploadBlock(srv);
    }
    return (result);
}

CO_ERR COSdoEndUploadBlock(CO_SDO *srv)
{
    CO_ERR result = CO_ERR_SDO_SILENT;

    srv->Blk.State = BLK_IDLE;
    srv->Obj       = 0;
    return (result);
}

void COSdoAbortReq(CO_SDO *srv)
{
    srv->Obj       =  0;
    srv->Idx       =  0;
    srv->Sub       =  0;
    srv->Buf.Cur   =  srv->Buf.Start;
    srv->Buf.Num   =  0;
    srv->Blk.State =  BLK_IDLE;
    srv->Seg.Num   =  0;
    srv->Seg.Size  =  0;
    srv->Seg.TBit  =  0;
}
