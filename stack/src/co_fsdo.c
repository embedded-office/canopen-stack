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
* \file     co_fsdo.c
*
* \brief    FULL SDO SERVER
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_fsdo.c#5 $
*
*           This source file implements the missing parts to get a fully compliant SDO server.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

/*
****************************************************************************************************
*                                             INCLUDES
****************************************************************************************************
*/

#include "co_core.h"

/*
****************************************************************************************************
*                                     LOCAL FUNCTION PROTOTYPES
****************************************************************************************************
*/

#if CO_SDO_DYN_ID_EN > 0
static int16_t CO_TSdoIdWrite (CO_OBJ* obj, void *buf, uint32_t size);
#endif

/*
****************************************************************************************************
*                                       GLOBAL TYPE STRUCTURE
****************************************************************************************************
*/

#if CO_SDO_DYN_ID_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief OBJECT TYPE SDO IDENTIFIER
*
* \ingroup SDO
*
*          This object type specializes the general handling of objects for the object directory
*          entries holding a SDO identifier. These entries are designed to provide the feature
*          of changing a SDO identifier.
*/
/*------------------------------------------------------------------------------------------------*/
CO_OBJ_TYPE COTSdoId = { 0, 0, 0, 0, 0, CO_TSdoIdWrite };
#endif

/*
****************************************************************************************************
*                                            FUNCTIONS
****************************************************************************************************
*/

#if CO_SDO_SEG_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  INIT SEGMENTED UPLOAD
*
* \ingroup  INTERNAL
*
*           This function generates the response for 'Initiate SDO Upload Protocol'.
*
*           Entry condition for this function is the SDO request command byte with the
*           following condition:
* \code
*           +---+---+---+---+---+---+---+---+
*           |     2     | x | x | x | X | x | => condition = "command & 0xE0 must be 0x40"
*           +---+---+---+---+---+---+---+---+
*             7   6   5   4   3   2   1   0
* \endcode
*
* \param[in,out]   srv               Pointer to SDO server object
*
* \param[in]       size              Object size in byte
*
* \retval   =0  on success
* \retval   <0  on SDO abort
*
* \ds301    9.2.2.2.5
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_SdoInitUploadSegmented (CO_SDO *srv, uint32_t size)
{
    int16_t result;                                /* Local: function result                   */
    uint8_t cmd;                                   /* Local: SDO command byte                  */
                                                      /*------------------------------------------*/
    cmd = 0x41;                                       /* response info: scs=2, n=0, e=0 and s=1   */
    CO_SET_BYTE(srv->Frm,  cmd, 0);                   /* response: command byte                   */
    CO_SET_LONG(srv->Frm, size, 4);                   /* object size in byte #4..#7               */

    srv->Seg.Size = size;                             /* store size of object entry               */
    srv->Seg.TBit = 0;                                /* first segment should have toggle bit = 0 */
    srv->Seg.Num  = 0;                                /* reset number of transfered bytes         */
    srv->Buf.Cur  = srv->Buf.Start;                   /* set next free location                   */
    srv->Buf.Num  = 0;                                /* clear number of bytes in buffer          */

    result = COObjRdBufStart(srv->Obj,                /* initialize object read                   */
                             srv->Buf.Cur, 0);
    if (result != CO_ERR_NONE) {                      /* see, if an error was detected            */
        srv->Node->Error = CO_ERR_SDO_READ;           /* set error code                           */
        result           = CO_ERR_NONE;               /* clear local error                        */
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}
#endif

#if CO_SDO_SEG_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  INIT SEGMENTED DOWNLOAD
*
* \ingroup  INTERNAL
*
*           This function generates the response for 'Initiate SDO Download protocol'.
*
*           Entry condition for this function is the SDO request command byte with the
*           following condition:
* \code
*           +---+---+---+---+---+---+---+---+
*           |     1     | 0 |   n   | 0 | s | => condition = "command & 0xF2 must be 0x20"
*           +---+---+---+---+---+---+---+---+
*             7   6   5   4   3   2   1   0
* \endcode
*
* \param[in,out]   srv               Pointer to SDO server object
*
* \retval   =0  on success
* \retval   <0  on SDO abort
*
* \ds301    9.2.2.2.2
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_SdoInitDownloadSegmented (CO_SDO *srv)
{
    uint32_t size;                                  /* Local: size of object entry in bytes     */
    uint32_t width  =  0;                           /* Local: data width in byte                */
    int16_t result = -1;                           /* Local: function result                   */
    uint8_t cmd;                                   /* Local: SDO command byte                  */
                                                      /*------------------------------------------*/
    cmd = CO_GET_BYTE(srv->Frm, 0);                   /* get command specifier                    */
    if ((cmd & 0x01) == 1) {                          /* see, if data size is indicated (s=1/0)   */
        width = CO_GET_LONG(srv->Frm, 4);             /* yes: get data size information           */
    }
    size = CO_SdoGetSize(srv, width);                 /* check data width acc. object             */
    if (size > 0) {                                   /* see, if size is valid                    */

        CO_SET_BYTE(srv->Frm, 0x60, 0);               /* scs=3: initiate download response        */
        CO_SET_LONG(srv->Frm,    0, 4);               /* clear all 4 data bytes with 0            */

        srv->Seg.Size = size;                         /* store size of object entry               */
        srv->Seg.TBit = 0;                            /* first segment should have toggle bit = 0 */
        srv->Seg.Num  = 0;                            /* reset number of transfered bytes         */

        srv->Buf.Cur  = srv->Buf.Start;               /* set next free buffer location            */
        srv->Buf.Num  = 0;                            /* clear number of bytes in buffer          */

        result = COObjWrBufStart(srv->Obj,            /* initialize object write                  */
                                 srv->Buf.Cur, 0);
        if (result != CO_ERR_NONE) {                  /* see, if an error was detected            */
            srv->Node->Error = CO_ERR_SDO_WRITE;      /* set error code                           */
            result           = CO_ERR_NONE;           /* clear local error                        */
        }
    }                                                 /*------------------------------------------*/
    return(result);                                   /* return function result                   */
}
#endif

#if CO_SDO_SEG_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  SEGMENTED DOWNLOAD
*
* \ingroup  INTERNAL
*
*           This function generates the response for 'Download SDO Segment Protocol'.
*
*           Entry condition for this function is the SDO request command byte with the
*           following condition:
* \code
*           +---+---+---+---+---+---+---+---+
*           |     0     | t |     n     | c | => condition = "command & 0xE0 must be 0x00"
*           +---+---+---+---+---+---+---+---+
*             7   6   5   4   3   2   1   0
* \endcode
*
* \param[in,out]   srv               Pointer to SDO server object
*
* \retval   =0  on success
* \retval   <0  on SDO abort
*
* \ds301    9.2.2.2.3
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_SdoDownloadSegmented (CO_SDO *srv)
{
    uint32_t num;                                   /* Local: number of bytes for transfer      */
    int16_t result = 0;                            /* Local: function result                   */
    uint8_t n;                                     /* Local: bitfield 'n' from command byte    */
    uint8_t cmd;                                   /* Local: SDO command byte                  */
    uint8_t bid;                                   /* Local: byte id (loop counter)            */
                                                      /*------------------------------------------*/
    cmd = CO_GET_BYTE(srv->Frm, 0);                   /* extract SDO command byte                 */
    if ((cmd >> 4) != srv->Seg.TBit) {                /* check for bad toggle bit                 */
        CO_SdoAbort(srv, CO_SDO_ERR_TBIT);            /* Abort message: toggle bit error          */
        return (-1);
    }
                                                      /*------------------------------------------*/
    n = ((cmd >> 1) & 0x07);                          /* get bitfield 'n' from command byte       */
    if (n == 0) {                                     /* if no segment size is indicated          */
        num = srv->Seg.Size - srv->Seg.Num;           /* calculate remaining bytes in object      */
        if (num > 7) {                                /* see, if transfer width is > 7            */
            num = 7;                                  /* yes: limit to 7                          */
        }
    } else {                                          /* otherwise: size is given in command      */
        num = 7 - n;                                  /* set number of bytes to given value       */
    }
                                                      /*------------------------------------------*/
    bid = 1;                                          /* set byte id to first data byte           */
    while (num > 0) {                                 /* loop through all valid bytes             */
        *(srv->Buf.Cur) = CO_GET_BYTE(srv->Frm, bid); /* copy data into transfer buffer           */
        srv->Buf.Num++;                               /* increment number of bytes in buffer      */
        srv->Buf.Cur++;                               /* move current to next free location       */
        bid++;                                        /* switch to next data byte                 */
        num--;                                        /* decrement remaining valid data bytes     */
    }
    srv->Seg.Num += srv->Buf.Num;                     /* clear number of transfered bytes         */
                                                      /*------------------------------------------*/
    if ((cmd & 0x01) == 0x01) {                       /* if no more segments                      */
        result = COObjWrBufCont(srv->Obj,             /* last write to object entry               */
                                srv->Buf.Start,
                                srv->Buf.Num);
        if (result != CO_ERR_NONE) {                  /* see, if an error was detected            */
            CO_SdoAbort(srv, CO_SDO_ERR_TOS);         /* Abort 'Data cannot be transfered'        */
            result = -1;
        }
        srv->Seg.Size = 0;                            /* clear object entry size                  */
        srv->Seg.Num  = 0;                            /* clear number of transfered bytes         */
        srv->Obj      = 0;                            /* indicate finished transfer               */

    } else {                                          /* otherwise: if segments remain            */
        result = COObjWrBufCont(srv->Obj,             /* buffered write to object entry           */
                                srv->Buf.Start,
                                srv->Buf.Num);
        if (result != CO_ERR_NONE) {                  /* see, if an error was detected            */
            CO_SdoAbort(srv, CO_SDO_ERR_TOS);         /* Abort 'Data cannot be transfered'        */
            result = -1;
        }
        srv->Buf.Cur  = srv->Buf.Start;               /* set next free buffer location            */
        srv->Buf.Num  = 0;                            /* clear number of bytes in buffer          */
    }
                                                      /*------------------------------------------*/
    cmd = (uint8_t)((1 << 5) |
                       (srv->Seg.TBit << 4));
    if (result == 0) {                                /* see, if no error is detected             */
        CO_SET_BYTE(srv->Frm, cmd, 0);                /* set response, scs=1, t=toggle bit        */
        CO_SET_BYTE(srv->Frm,   0, 1);
        CO_SET_WORD(srv->Frm,   0, 2);
        CO_SET_LONG(srv->Frm,   0, 4);
    }

    srv->Seg.TBit ^= 0x01;                            /* toggle the toggle bit                    */
                                                      /*------------------------------------------*/
    return result;                                    /* return function result                   */
}
#endif

#if CO_SDO_SEG_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  SEGMENTED UPLOAD
*
* \ingroup  INTERNAL
*
*           This function generates the response for 'Upload SDO Segment Protocol'.
*
*           Entry condition for this function is the SDO request command byte with the
*           following condition:
* \code
*           +---+---+---+---+---+---+---+---+
*           |     3     | t |       0       | => condition = "command & 0xEF must be 0x60"
*           +---+---+---+---+---+---+---+---+
*             7   6   5   4   3   2   1   0
* \endcode
*
* \param[in,out]   srv               Pointer to SDO server object
*
* \retval   =0  on success
* \retval   <0  on SDO abort
*
* \ds301    9.2.2.2.6
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_SdoUploadSegmented (CO_SDO *srv)
{
    uint32_t  width;                                /* Local: object width in byte              */
    int16_t  result = 0;                           /* Local: function result                   */
    uint8_t  cmd;                                  /* Local: SDO command byte                  */
    uint8_t  c_bit  =  0;                          /* Local: indicator if last segment         */
    uint8_t  i;                                    /* Local: counter variable                  */
                                                      /*------------------------------------------*/
    if (srv->Obj == 0) {                              /* if segmented transfer not started        */
        CO_SdoAbort(srv, CO_SDO_ERR_CMD);             /* Abort message: command error             */
        return (-1);
    }
                                                      /*------------------------------------------*/
    cmd = CO_GET_BYTE(srv->Frm, 0);                   /* get command byte                         */
    if (((cmd >> 4) & 0x01) != srv->Seg.TBit) {       /* check for toggle bit                     */
        CO_SdoAbort(srv, CO_SDO_ERR_TBIT);            /* Abort message: toggle bit error          */
        return (-1);
    }
                                                      /*------------------------------------------*/
    width = srv->Seg.Size - srv->Seg.Num;             /* calculate remaining bytes                */
    if (width > 7) {                                  /* if remaining data is longer than 7 bytes */
        width = 7;                                    /* use full segment size                    */
    } else {                                          /* otherwise:                               */
        c_bit = 1;                                    /* set c-bit, last segment to be transfered */
    }
                                                      /*------------------------------------------*/
    if (c_bit == 1) {                                 /* see, if this is the last segment         */
        result = COObjRdBufCont(srv->Obj,             /* yes: last read data into transfer buffer */
                                srv->Buf.Start,
                                width);
    } else {                                          /* otherwise: buffered reading              */
        result = COObjRdBufCont(srv->Obj,             /* read data into transfer buffer           */
                                srv->Buf.Start,
                                width);
    }
    if (result != CO_ERR_NONE) {
        CO_SdoAbort(srv, CO_SDO_ERR_TOS);             /* Abort 'Data cannot be transfered'        */
        return (-1);
    }

    srv->Buf.Cur = srv->Buf.Start;
    for (i = 0; i < (uint8_t)width; i++) {
        CO_SET_BYTE(srv->Frm, *(srv->Buf.Cur), 1+i);
        srv->Buf.Cur++;
    }
    for (i = (uint8_t)(width + 1); i <= 7; i++) {  /* clear rest of can frame                  */
        CO_SET_BYTE(srv->Frm, 0, 1+i);
    }                                                 /*------------------------------------------*/

    cmd = (uint8_t)0x00 |                          /* response info: scs = 0,                  */
          (uint8_t)(srv->Seg.TBit << 4) |          /* t = togglebit,                           */
          (uint8_t)(((7 - width) << 1) & 0x0E) |   /* n = 7 - width                            */
          (uint8_t)c_bit;                          /* and c = c-bit                            */
    CO_SET_BYTE(srv->Frm, cmd, 0);

    if (c_bit == 1) {                                 /* if last segment                          */
        srv->Seg.Size  = 0;                           /* clear segmented object size              */
        srv->Seg.Num   = 0;                           /* clear number of transmitted bytes        */
        srv->Seg.TBit  = 0;                           /* clear toggle bit                         */
        srv->Obj       = 0;                           /* indicate finished transfer               */
    } else {                                          /* otherwise: segments remaining            */
        srv->Seg.Num  += width;                       /* update number of transmitted bytes       */
        srv->Seg.TBit ^= 0x01;                        /* toggle the toggle bit                    */
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}
#endif

#if CO_SDO_BLK_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  INIT BLOCK DOWNLOAD
*
* \ingroup  INTERNAL
*
*           This function generates the response for 'Initiate SDO Block Download Protocol'.
*
*           Entry condition for this function is the SDO request command byte with the
*           following condition:
* \code
*           +---+---+---+---+---+---+---+---+
*           |     6     |   0   | cc| s | 0 | => condition = "command & 0xF9 must be 0xC0"
*           +---+---+---+---+---+---+---+---+
*             7   6   5   4   3   2   1   0
* \endcode
*
* \param[in,out]   srv               Pointer to SDO server object
*
* \retval   =0  on success
* \retval   <0  on SDO abort
*
* \ds301    9.2.2.2.9
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_SdoInitDownloadBlock (CO_SDO *srv)
{
    uint32_t size;                                  /* Local: Size object entry                 */
    uint32_t width  =  0;                           /* Local: data width in byte                */
    int16_t result = -1;                           /* Local: function result                   */
    uint8_t cmd;                                   /* Local: SDO command byte                  */
                                                      /*------------------------------------------*/
    cmd = CO_GET_BYTE(srv->Frm, 0);                   /* get command specifier                    */
    if ((cmd & 0x02) != 0) {                          /* if s = 1, size indicated                 */
        width = CO_GET_LONG(srv->Frm, 4);             /* get size information from CAN frame      */
    }
    size = CO_SdoGetSize(srv, width);                 /* check data width acc. object             */
    if (size == 0) {                                  /* see, if an error is detected             */
        CO_SdoAbort(srv, CO_SDO_ERR_TOS);             /* Abort message: Data cannot be transfered */
        return (result);                              /* abort with error indication              */
    }
    if (width <= size) {                              /* see, if size is valid                    */
        result = COObjWrBufStart(srv->Obj,            /* initialize object write                  */
                                 srv->Buf.Cur, 0);
        if (result != CO_ERR_NONE) {
            srv->Node->Error = CO_ERR_SDO_WRITE;      /* set error code                           */
            result           = CO_ERR_NONE;           /* clear local error                        */
        }


        srv->Blk.SegCnt = 0;                          /* clear segment counter                    */
        srv->Blk.State  = BLK_DOWNLOAD;               /* set block transfer active                */
        srv->Blk.SegNum = CO_SDO_BUF_SEG;             /* set number of segments per blocks        */

        CO_SET_BYTE(srv->Frm, 0xA0, 0);               /* scs=5,sc=0,ss=0: initiate download       */
                                                      /* response, no checksum supported          */
        CO_SET_LONG(srv->Frm,                         /* set max. number of segments per block    */
                    (uint32_t)CO_SDO_BUF_SEG,
                    4);
    } else {                                          /* otherwise: size is invalid               */
        CO_SdoAbort(srv, CO_SDO_ERR_LEN_HIGH);        /* Abort 'datatype length too high'         */
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}                                                     /*------------------------------------------*/
#endif

#if CO_SDO_BLK_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  END BLOCK DOWNLOAD
*
* \ingroup  INTERNAL
*
*           This function generates the response for 'End SDO Block Download Protocol'.
*
*           Entry condition for this function is the SDO request command byte with the
*           following condition:
* \code
*           +---+---+---+---+---+---+---+---+
*           |     6     |     n     | 0 | 1 | => condition = "command & 0xE3 must be 0xC1"
*           +---+---+---+---+---+---+---+---+
*             7   6   5   4   3   2   1   0
* \endcode
*
* \param[in,out]   srv               Pointer to SDO server object
*
* \retval   =0  on success
* \retval   <0  on SDO abort
*
* \ds301    9.2.2.2.11
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_SdoEndDownloadBlock (CO_SDO *srv)
{
    int16_t result = -1;                           /* Local: function result                   */
    uint8_t cmd;                                   /* Local: SDO command byte                  */
    uint8_t n;                                     /* Local: bit field 'n' in command byte     */
                                                      /*------------------------------------------*/
    cmd = CO_GET_BYTE(srv->Frm, 0);                   /* get command byte                         */
    if ((cmd & 0x01) != 0) {                          /* if cs=1, end block download              */
        n      = (cmd & 0x1C) >> 2;                   /* get n out of command byte                */
        result = COObjWrBufCont(srv->Obj,             /* last write to object entry               */
                                srv->Buf.Start,
                                ((uint32_t)srv->Buf.Num - n));
        if (result != CO_ERR_NONE) {                  /* see, if an error is detected             */
            srv->Node->Error = CO_ERR_SDO_WRITE;      /* yes: indicate error in node              */
            result = -1;                              /* set result for sdo response              */
        }
        CO_SET_BYTE(srv->Frm, 0xA1, 0);               /* scs=5,x=0,ss=1: end download             */
        CO_SET_WORD(srv->Frm,    0, 1);               /* clear data                               */
        CO_SET_BYTE(srv->Frm,    0, 3);
        CO_SET_LONG(srv->Frm,    0, 4);

        srv->Blk.State = BLK_IDLE;                    /* set block transfer to idle               */
        srv->Buf.Cur   = srv->Buf.Start;              /* reset transfer buffer working location   */
        srv->Buf.Num   = 0;                           /* clear number of bytes in transfer buffer */
        srv->Obj       = 0;                           /* clear object entry reference             */
        result         = 0;                           /* indicate success                         */
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}                                                     /*------------------------------------------*/
#endif

#if CO_SDO_BLK_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  BLOCK DOWNLOAD
*
* \ingroup  INTERNAL
*
*           This function generates the response for 'Download SDO Block Segment Protocol'.
*
*           Entry condition for this function is activation of the block download state. The
*           SDO request command byte contains the state after this CAN frame:
* \code
*           +---+---+---+---+---+---+---+---+
*           | c |      0 < seqno < 128      | => c=0: more segments to be downloaded
*           +---+---+---+---+---+---+---+---+    C=1: enter END SDO block download phase
*             7   6   5   4   3   2   1   0
* \endcode
*
* \param[in,out]   srv               Pointer to SDO server object
*
* \retval   =0  on success
* \retval   <0  on SDO abort
*
* \ds301    9.2.2.2.10
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_SdoDownloadBlock (CO_SDO *srv)
{
    int16_t result = -2;                           /* Local: function result                   */
    int16_t err;                                   /* Local: error code                        */
    uint8_t cmd;                                   /* Local: sequence number                   */
    uint8_t i;                                     /* Local: counter                           */
                                                      /*------------------------------------------*/
    cmd = CO_GET_BYTE(srv->Frm, 0);                   /* get sequence counter                     */
    if ((cmd & 0x7F) == (srv->Blk.SegCnt + 1)) {      /* if segment counter fits segment number   */
        for (i = 0; i < 7; i++) {                     /* copy data to transfer buffer             */
            *(srv->Buf.Cur) = CO_GET_BYTE(srv->Frm, 1+i);
            srv->Buf.Cur++;
            srv->Buf.Num++;
        }
        srv->Blk.SegCnt++;                            /* increment segment counter                */
        if ( (srv->Blk.SegCnt == CO_SDO_BUF_SEG) ||   /* if max. segment of block transfered or   */
             ((cmd & 0x80)  != 0             ) ) {    /*  if last segment of last block indicated */

            CO_SET_BYTE(srv->Frm,            0xA2, 0);/* scs=5; ss=2                              */
            CO_SET_BYTE(srv->Frm, srv->Blk.SegCnt, 1);/* set ackseq for response                  */
            CO_SET_BYTE(srv->Frm,  CO_SDO_BUF_SEG, 2);/* set blksize for response                 */
            for (i = 3; i <= 7; i++) {                /* clear rest of frame                      */
                CO_SET_BYTE(srv->Frm, 0, i);
            }                                         /*------------------------------------------*/
            srv->Blk.SegCnt  = 0;                     /* reset segment counter                    */
            srv->Blk.State   = BLK_DNWAIT;            /* set to waiting state                     */
            result           = 0;                     /* indicate block confirmation              */
        }                                             /*------------------------------------------*/

        if (result == 0) {                            /* if last segment of block                 */
            if ((cmd & 0x80) == 0) {                  /* yes, if not last segment of last block   */
                err = COObjWrBufCont(srv->Obj,        /* write transfer buffer to domain          */
                                     srv->Buf.Start,
                                     (uint32_t)srv->Buf.Num);
                if (err != CO_ERR_NONE) {             /* see, if an error is detected             */
                    srv->Node->Error =                /* yes: indicate error in node              */
                        CO_ERR_SDO_WRITE;
                }
                srv->Buf.Cur = srv->Buf.Start;
                srv->Buf.Num = 0;
            }
        }
    } else {                                          /* otherwise: sequence count mismatch       */
        if ((srv->Blk.SegCnt & 0x80) == 0) {          /* if first error                           */
            srv->Blk.SegCnt |= 0x80;                  /* indicate appeared error                  */
        }                                             /*------------------------------------------*/

        if ( ((cmd & 0x7F) == CO_SDO_BUF_SEG) ||      /* if max. segment of block transfered or   */
             ((cmd & 0x80) != 0             ) ) {     /*  if last segment of last block indicated */

            CO_SET_BYTE(srv->Frm, 0xA2, 0);           /* scs=5; ss=2                              */
            CO_SET_BYTE(srv->Frm, srv->Blk.SegCnt & 0x7F, 1); /* ackseq with last successful seg. */
            CO_SET_BYTE(srv->Frm, CO_SDO_BUF_SEG, 2); /* set blksize for response                 */
            CO_SET_BYTE(srv->Frm, 0, 3);              /* clear rest of message                    */
            CO_SET_LONG(srv->Frm, 0, 4);
                                                      /*------------------------------------------*/
            srv->Blk.SegCnt = 0;                      /* reset segment counter                    */
            srv->Buf.Cur    = srv->Buf.Start;         /* reset transfer buffer                    */
            srv->Buf.Num    = 0;
            result          = 0;                      /* indicate block confirmation              */
        }
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}                                                     /*------------------------------------------*/
#endif

#if CO_SDO_BLK_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  INIT BLOCK UPLOAD
*
* \ingroup  INTERNAL
*
*           This function generates the response for 'Initiate SDO Block Upload Protocol',
*           sub-command 'initate upload request'
*
*           Entry condition for this function is the SDO request command byte with the
*           following condition:
* \code
*           +---+---+---+---+---+---+---+---+
*           |     5     | 0 | cc| 0 |   0   | => condition = "command & 0xE7 must be 0xA0"
*           +---+---+---+---+---+---+---+---+
*             7   6   5   4   3   2   1   0
* \endcode
*
* \param[in,out]   srv               Pointer to SDO server object
*
* \retval   =0  on success
* \retval   <0  on SDO abort
*
* \ds301    9.2.2.2.13
*
* \todo     Implement the PST to dynamically switch to segmented or expedited transfer if
*           number of segments is below the given threshold.
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_SdoInitUploadBlock (CO_SDO *srv)
{
    uint32_t size;                                  /* Local: size of object entry              */
    int16_t result = 0;                            /* Local: function result                   */
    int16_t err;                                   /* Local: error variable                    */
    uint8_t cmd;                                   /* Local: SDO command byte                  */
                                                      /*------------------------------------------*/
    err = CO_SdoGetObject(srv, CO_SDO_RD);            /* get addressed object entry               */
    if (err < 0) {                                    /* see, if object entry is not existing     */
        return (-1);                                  /* yes: stop processing                     */
    }

    srv->Blk.Size   = CO_SdoGetSize(srv, 0);          /* get size of object entry                 */
    if (srv->Blk.Size == 0) {                         /* see, if an error is detected             */
        return (-1);                                  /* yes: stop processing                     */
    }
    srv->Blk.SegNum = CO_GET_BYTE(srv->Frm, 4);       /* get max. number of segments per block    */

    if ( (srv->Blk.SegNum < 0x01) ||                  /* if invalid number of segments 1..0x7F    */
         (srv->Blk.SegNum > 0x7F) ) {
        CO_SdoAbort   (srv, CO_SDO_ERR_BLK_SIZE);     /* Abort 'invalid block size'               */
        CO_SdoAbortReq(srv);                          /* perform abort of SDO transfer            */
        return (-1);                                  /* send response                            */
    } else {                                          /* otherwise: blksize is in valid range     */
        if (srv->Blk.SegNum > CO_SDO_BUF_SEG) {       /* see, if blksize is > transfer buffer     */
            srv->Blk.SegNum = CO_SDO_BUF_SEG;         /* yes: limit to transfer buffer            */
        }
    }

    size  = srv->Blk.Size;                            /* setup response                           */
    cmd   = 0xC2;                                     /* scs=6,sc=0,ss=0: initiate download       */
                                                      /* response, no checksum supported          */
    srv->Blk.LastValid = 0xFF;                        /* set last valid bytes invalid             */
    srv->Blk.Len       = srv->Blk.Size;               /* set remaining bytes to transfer          */
                                                      /*------------------------------------------*/
    err = COObjRdBufStart(srv->Obj,                   /* initialize object read                   */
                            srv->Buf.Cur, 0);
    if (err != CO_ERR_NONE) {
        srv->Node->Error = CO_ERR_SDO_READ;           /* set error code                           */
    }
    CO_SET_BYTE(srv->Frm,  cmd, 0);                   /* set SDO response command byte            */
    CO_SET_LONG(srv->Frm, size, 4);                   /* indicate object entry size in bytes      */

    return (result);                                  /* return function result                   */
}
#endif

#if CO_SDO_BLK_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  BLOCK UPLOAD
*
* \ingroup  INTERNAL
*
*           This function generates the response for 'Initiate SDO Block Upload Protocol',
*           sub-command 'start upload'.
*
*           Entry condition for this function is the SDO request command byte with the
*           following condition, or the already started block upload state:
* \code
*           +---+---+---+---+---+---+---+---+
*           |     5     |     0     |   3   | => condition = "command must be 0xA3"
*           +---+---+---+---+---+---+---+---+
*             7   6   5   4   3   2   1   0
* \endcode
*
* \param[in,out]   srv               Pointer to SDO server object
*
* \retval   =0  on success
* \retval   <0  on SDO abort
*
* \ds301    9.2.2.2.13
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_SdoUploadBlock (CO_SDO *srv)
{
    uint32_t size;                                  /* Local: remaining object entry size       */
    int16_t result   = -2;                         /* Local: function result                   */
    int16_t err;                                   /* Local: error variable                    */
    uint8_t finished =  0;                         /* Local: end of object indication          */
    uint8_t seg;                                   /* Local: segment counter                   */
    uint8_t len;                                   /* Local: used bytes in frame               */
    uint8_t   i;                                   /* Local: loop counter                      */
                                                      /*------------------------------------------*/
    srv->Buf.Cur = srv->Buf.Start;                    /* clr reading location in transfer buffer  */
    srv->Buf.Num = 7 * srv->Blk.SegNum;               /* calculate number of bytes in buffer      */
    if (srv->Blk.State == BLK_REPEAT) {               /* see, if loaded block must be repeated    */
        srv->Blk.Len += srv->Buf.Num;                 /* yes: add block again to remaining bytes  */
    } else {                                          /* otherwise: load new block into buffer    */
        if (srv->Blk.Size > srv->Buf.Num) {           /* see, if domain > transfer buffer         */
            err = COObjRdBufCont(srv->Obj,            /* fill transfer buffer for reading         */
                                 srv->Buf.Start,
                                 (uint32_t)srv->Buf.Num);
            if (err != CO_ERR_NONE) {                 /* see, if an error is detected             */
                srv->Node->Error = CO_ERR_SDO_READ;   /* set error code                           */
            }
            srv->Blk.Size -= srv->Buf.Num;            /* update remaining object size             */
        } else {                                      /* otherwise: domain <= transfer buffer     */
            srv->Buf.Num   = srv->Blk.Size;           /* set reading to object entry size         */
            err = COObjRdBufCont(srv->Obj,            /* fill transfer buffer for reading         */
                                 srv->Buf.Start,
                                 (uint32_t)srv->Buf.Num);
            if (err != CO_ERR_NONE) {                 /* see, if an error is detected             */
                srv->Node->Error = CO_ERR_SDO_READ;   /* set error code                           */
            }
            srv->Blk.Size = 0;                        /* set remaining object size to 0           */
        }
    }                                                 /*------------------------------------------*/
    srv->Blk.State  = BLK_UPLOAD;                     /* set block transfer active                */
    srv->Blk.SegCnt = 1;                              /* restart segments in transmission buffer  */
    while ( (srv->Blk.SegCnt <= srv->Blk.SegNum) &&   /* loop for number of segments              */
            (finished        ==               0) ) {  /*   or end of object entry reached         */
        seg  = srv->Blk.SegCnt;                       /* set segment counter                      */
        size = srv->Blk.Len;                          /* calculate remaining bytes in object      */
        if (size > 7) {                               /* if more than 7 bytes remain              */
            len           = 7;                        /* set actual data length                   */
            srv->Blk.Len -= 7;
            if (srv->Blk.SegCnt < srv->Blk.SegNum) {  /* see, if buffer end is not reached        */
                srv->Blk.SegCnt++;                    /* yes: inc. number of transmitted segments */
            } else {                                  /* otherwise: end of block reached          */
                finished = 1;                         /* indicate last segment in block           */
            }
        } else {                                      /* otherwise: last segment of last block    */
            len          = (uint8_t)size;          /* set length                               */
            srv->Blk.Len = 0;
            finished     = 1;                         /* end on next loop                         */
        }
        if (finished == 1) {                          /* see, if this is the last segment         */
            if (srv->Blk.Len == 0) {                  /* yes: see, if this is the last block      */
                seg |= 0x80;                          /* yes: c=1, last segment in last block     */
            }
            srv->Blk.LastValid  = len;                /* set valid bytes of last segment in blk   */
        }                                             /*------------------------------------------*/
        CO_SET_BYTE(srv->Frm, seg, 0);                /* set sequence byte                        */
        for (i = 0; i < len; i++) {                   /* copy data from transfer buffer to frame  */
            CO_SET_BYTE(srv->Frm,                     /* copy bytes from transfer buffer to frame */
                        *(srv->Buf.Cur),
                        1+i);
            srv->Buf.Cur++;                           /* update working read pointer in buffer    */
            srv->Buf.Num--;                           /* decrement remaining bytes in buffer      */
        }
        for (i = (uint8_t)len; i < 7; i++) {       /* clear rest of can frame                  */
            CO_SET_BYTE(srv->Frm, 0, 1+i);
        }
        (void)COIfSend(&srv->Node->If, srv->Frm);     /* send SDO response at once                */
                                                      /* possible error registered in node        */
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}
#endif

#if CO_SDO_BLK_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  CONFIRM BLOCK UPLOAD
*
* \ingroup  INTERNAL
*
*           This function generates the response for 'Upload SDO Block Segment Protocol'.
*
*           Entry condition for this function is the SDO request command byte with the
*           following condition:
* \code
*           +---+---+---+---+---+---+---+---+
*           |     5     | x | x | x |   2   | => condition = "command & 0xE3 must be 0xA2"
*           +---+---+---+---+---+---+---+---+
*             7   6   5   4   3   2   1   0
* \endcode
*
* \param[in,out]   srv               Pointer to SDO server object
*
* \retval   =0  on success
* \retval   <0  on SDO abort
*
* \ds301    9.2.2.2.14
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_SdoAckUploadBlock (CO_SDO *srv)
{
    int16_t result = -2;                           /* Local: function result                   */
    uint8_t cmd;                                   /* Local: SDO command byte                  */
    uint8_t seq;                                   /* Local: sequence counter                  */
    uint8_t val;                                   /* Local: last valid segment                */
    uint8_t   i;                                   /* Local: sequence counter                  */

    seq = CO_GET_BYTE(srv->Frm, 1);
    if (seq > 0x7F) {                                 /* if sequence number out of range          */
        CO_SdoAbort   (srv,CO_SDO_ERR_SEQ_NUM);       /* Abort message: invalid sequence number   */
        CO_SdoAbortReq(srv);                          /* perform abort of SDO transfer            */
        return (-1);                                  /* send response                            */

    } else if (seq != srv->Blk.SegCnt) {              /* if frames not receive correctly          */
        srv->Blk.State  = BLK_REPEAT;                 /* repeat block transfer                    */
        result          = CO_SdoUploadBlock(srv);     /* re-send this block                       */
    } else if (srv->Blk.Len == 0) {                   /* if last segment of last block            */
        if (srv->Blk.LastValid <= 7) {                /* if last valid bytes are valid            */
            val = (uint8_t)srv->Blk.LastValid;     /* get last valid received segments         */
            cmd = (uint8_t)0xC0 |                  /* scs=6, ss=1, n=number of unused bytes    */
                  (uint8_t)((7u - val) << 2) |
                  (uint8_t)0x01;
            CO_SET_BYTE(srv->Frm, cmd, 0);
            for (i = 1; i <= 7; i++) {                /* clear remaining bytes                    */
                CO_SET_BYTE(srv->Frm, 0, i);
            }                                         /*------------------------------------------*/
            result = 0;                               /* send response                            */
        }

    } else {                                          /* otherwise: continue with next block      */
        srv->Blk.SegNum = CO_GET_BYTE(srv->Frm, 2);   /* set number of segments for next block    */
        if ( (srv->Blk.SegNum < 0x01) ||              /* if invalid number of segments 1..0x7F    */
             (srv->Blk.SegNum > 0x7F) ) {
            CO_SdoAbort   (srv, CO_SDO_ERR_BLK_SIZE); /* Abort 'invalid block size'               */
            CO_SdoAbortReq(srv);                      /* perform abort of SDO transfer            */
            return (-1);                              /* send response                            */
        } else {                                      /* otherwise: blksize is in valid range     */
            if (srv->Blk.SegNum > CO_SDO_BUF_SEG) {   /* see, if blksize is > transfer buffer     */
                srv->Blk.SegNum = CO_SDO_BUF_SEG;     /* yes: limit to transfer buffer            */
            }
        }
        result = CO_SdoUploadBlock(srv);              /* send next block                          */
    }
                                                      /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}
#endif

#if CO_SDO_BLK_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  END BLOCK UPLOAD
*
* \ingroup  INTERNAL
*
*           This function generates the response for 'End SDO Block Upload Protocol'.
*
*           Entry condition for this function is the SDO request command byte with the
*           following condition:
* \code
*           +---+---+---+---+---+---+---+---+
*           |     5     |     0     |   1   | => condition = "command must be 0xA1"
*           +---+---+---+---+---+---+---+---+
*             7   6   5   4   3   2   1   0
* \endcode
*
* \param[in,out]   srv               Pointer to SDO server object
*
* \retval   =0  on success
* \retval   <0  on SDO abort
*
* \ds301    9.2.2.2.15
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_SdoEndUploadBlock (CO_SDO *srv)
{
    int16_t result = -2;                           /* Local: function result                   */
                                                      /*------------------------------------------*/
    srv->Blk.State = BLK_IDLE;                        /* set block transfer to idle               */
    srv->Obj       = 0;                               /* clear object entry reference             */
                                                      /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}
#endif

#if CO_SDO_BLK_EN > 0 || CO_SDO_SEG_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  SDO ABORT REQUEST
*
* \ingroup  INTERNAL
*
*           This function is called on receiving an SDO abort request. The requested SDO transfer
*           will be cancled.
*
* \param[in,out]   srv               Pointer to SDO server object
*/
/*------------------------------------------------------------------------------------------------*/
void CO_SdoAbortReq (CO_SDO *srv)
{
#if CO_SDO_BLK_EN > 0 || CO_SDO_SEG_EN > 0
    srv->Obj       =  0;
    srv->Idx       =  0;
    srv->Sub       =  0;
    srv->Buf.Cur   =  srv->Buf.Start;                 /* reset transfer buffer working location   */
    srv->Buf.Num   =  0;                              /* clear number of bytes in transfer buffer */
#endif
#if CO_SDO_BLK_EN > 0
    srv->Blk.State =  BLK_IDLE;                       /* set block transfer to idle               */
#endif
#if CO_SDO_SEG_EN > 0
    srv->Seg.Num   =  0;                              /* clear number of transfered bytes         */
    srv->Seg.Size  =  0;                              /* clear size of segmented object entry     */
    srv->Seg.TBit  =  0;                              /* clear toggle bit                         */
#endif
}
#endif

/*
****************************************************************************************************
*                                         LOCAL FUNCTIONS
****************************************************************************************************
*/

#if CO_SDO_DYN_ID_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  WRITE SDO IDENTIFIER
*
* \ingroup  INTERNAL
*
*           This write function is responsible for the object write procedure for a write access
*           to a SDO CAN identifier.
*
* \param[in,out]   obj         Ptr to addressed object entry
*
* \param[in]       buf         Ptr to data buffer
*
* \param[in]       size        Size of given data in buffer
*
* \retval         =CO_ERR_NONE  Successfully operation
* \retval        !=CO_ERR_NONE  An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_TSdoIdWrite(CO_OBJ* obj, void *buf, uint32_t size)
{
    uint32_t  newval;                               /* Local: write value                       */
    uint32_t  curval;                               /* Local: current CAN identifier value      */
    CO_NODE    *node;                                 /* Local: pointer to parent node            */
    int16_t  err = CO_ERR_NONE;                    /* Local: error code                        */
    uint8_t  num;                                  /* Local: number of SDO server              */
                                                      /*------------------------------------------*/
    if ((obj == 0) ||                                 /* see, if one of the arguments is invalid  */
        (buf == 0) || (size != CO_LONG)) {
        return (CO_ERR_BAD_ARG);                      /* yes: abort with error indication         */
    }
    newval = *(uint32_t *)buf;                      /* get write value                          */
    (void)CO_ObjRdDirect(obj, &curval, CO_LONG);      /* get current value (existance is checked) */
    num    = CO_GET_IDX(obj->Key) & 0x7F;             /* get number of SDO server                 */
    node   = obj->Type->Dir->Node;                    /* get pointer to parent node               */
                                                      /*------------------------------------------*/
    if ((curval & CO_SDO_COBID_OFF) == 0) {           /* see, if write is limited to disable ID   */
        if ((newval & CO_SDO_COBID_OFF) != 0) {       /* yes: see, if write access disables ID    */
            err = CO_ObjWrDirect(obj, &newval,        /* yes: set write value in object entry     */
                                 CO_LONG);
            if (err == CO_ERR_NONE) {                 /* see, if write access was successful      */
                CO_SdoReset(node->Sdo, num, node);    /* for safety: reset addressed SDO server   */
            }
        } else {                                      /* otherwise: write is not allowed          */
            return (CO_ERR_OBJ_RANGE);                /* abort write operation                    */
        }
    } else {                                          /* otherwise: current COBID is disabled     */
        err = CO_ObjWrDirect(obj, &newval, CO_LONG);  /* write new value to COBID                 */
    }                                                 /*------------------------------------------*/
    if (err == CO_ERR_NONE) {                         /* see, if no error is detected             */
        CO_SdoEnable(node->Sdo, num);                 /* update SDO server status                 */
    }
    return (err);                                     /* return function result                   */
}
#endif
