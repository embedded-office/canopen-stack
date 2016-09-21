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
* \file     co_sdo.c
*
* \brief    SDO SERVER
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_sdo.c#5 $
*
*           This source file implements the SDO server.
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
*                                            FUNCTIONS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief  INIT SDO SERVER
*
* \ingroup  INTERNAL
*
*           This function reads the content of the CANopen object directory with index 1200+[n]
*           subindex 1 (for RX) and subindex 2 (for TX), where n is a counter from 0 to the
*           maximal number of supported SDO servers (CO_SDO_N).
*
* \param[in,out]   srv        Ptr to root element of SDO server array
*
* \param[in]       node       Ptr to parent CANopen node
*/
/*------------------------------------------------------------------------------------------------*/
void CO_SdoInit(CO_SDO *srv, CO_NODE *node)
{
    CPU_INT08U n;                                     /* Local: Loop counter for SDO Lists        */
                                                      /*------------------------------------------*/
    for (n=0; n < CO_SDO_N; n++) {                    /* Loop through all SDO servers             */
        CO_SdoReset (srv, n, node);                   /* reset SDO server internal data fields    */
        CO_SdoEnable(srv, n);                         /* enable SDO server acc. object directory  */
    }
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  RESET SDO SERVER
*
* \ingroup  INTERNAL
*
*           This function resets all internal data of the given SDO server 'num'.
*
* \param[in,out]   srv        Ptr to root element of SDO server array
*
* \param[in]       num        Number of SDO server
*
* \param[in]       node       Ptr to parent CANopen node
*/
/*------------------------------------------------------------------------------------------------*/
void CO_SdoReset(CO_SDO *srv, CPU_INT08U num, CO_NODE *node)
{
    CO_SDO     *srvnum;                               /* Local: pointer to SDO server #num        */
#if CO_SDO_SEG_EN > 0 || CO_SDO_BLK_EN > 0
    CPU_INT32U  offset;                               /* Local: Offset within SDO buffer memory   */
#endif
                                                      /*------------------------------------------*/
    if (srv == 0) {                                   /* see, if ptr is invalid                   */
        return;                                       /* yes: ignore function call                */
    }
    if (num >= CO_SDO_N) {                            /* see, if number is out of range           */
        return;                                       /* yes: ignore function call                */
    }
    srvnum               = &srv[num];                 /* get ptr to addressed SDO server          */
    srvnum->Node         = node;                      /* link node to this server                 */
    srvnum->RxId         = CO_SDO_COBID_OFF;          /* clear request identifier                 */
    srvnum->TxId         = CO_SDO_COBID_OFF;          /* clear response identifier                */
    srvnum->Frm          = 0;                         /* clear CAN frame reference                */
    srvnum->Obj          = 0;                         /* clear obect entry reference              */
#if CO_SDO_SEG_EN > 0 || CO_SDO_BLK_EN > 0
    offset               = num * CO_SDO_BUF_BYTE;     /* calculate offset of SDO buffer in memory */
    srvnum->Buf.Start    = &node->SdoBuf[offset];     /* set start address of transfer buffer     */
    srvnum->Buf.Cur      = &node->SdoBuf[offset];     /* set next free buffer location            */
    srvnum->Buf.Num      = 0;                         /* clear number of bytes in buffer          */
#endif
#if CO_SDO_SEG_EN > 0
    srvnum->Seg.TBit     = 0;                         /* clear segmented transfer toggle bit      */
    srvnum->Seg.Num      = 0;                         /* clear segmented transfer length          */
    srvnum->Seg.Size     = 0;                         /* set segmented transfer counter inactive  */
#endif
#if CO_SDO_BLK_EN > 0
    srvnum->Blk.State    = BLK_IDLE;                  /* set block transfer not active            */
#endif
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  ENABLE SDO SERVER
*
* \ingroup  INTERNAL
*
*           This function reads the content of the CANopen object directory for a given SDO
*           server number 'num' from the with index 1200+[num] subindex 1 (for RX) and subindex 2
*           (for TX). The identifiers for request and response will be set acc. the CANopen
*           rules. If everything is ok, the SDO server is enabled after this function call.
*
* \param[in,out]   srv        Ptr to root element of SDO server array
*
* \param[in]       num        Number of SDO server
*/
/*------------------------------------------------------------------------------------------------*/
void CO_SdoEnable(CO_SDO *srv, CPU_INT08U num)
{
    CPU_INT32U  rxId;                                 /* Local: CAN identifier for SDO requests   */
    CPU_INT32U  txId;                                 /* Local: CAN identifier for SDO response   */
    CO_NODE    *node;                                 /* Local: pointer to parent node            */
    CO_SDO     *srvnum;                               /* Local: pointer to SDO server #num        */
    CPU_INT16S  err;                                  /* Local: error variable                    */
                                                      /*------------------------------------------*/
    if (num >= CO_SDO_N) {                            /* see, if number is out of range           */
        return;                                       /* yes: ignore function call                */
    }
    srvnum       = &srv[num];                         /* get ptr to addressed SDO server          */
    srvnum->RxId = CO_SDO_COBID_OFF;                  /* disable SDO server for function abort    */
    srvnum->TxId = CO_SDO_COBID_OFF;

    node   = srv->Node;                               /* get parent node                          */
    err    = CODirRdLong(&node->Dir,                  /* read object entry with RX identifier     */
                         CO_DEV(0x1200 + num, 1),
                         &rxId);
    if (err != CO_ERR_NONE) {                         /* see, if an error is detected             */
        return;                                       /* abort function                           */
    }
    err = CODirRdLong(&node->Dir,                     /* read object entry with TX identifier     */
                      CO_DEV(0x1200 + num, 2),
                             &txId);
    if (err != CO_ERR_NONE) {                         /* see, if an error is detected             */
        return;                                       /* abort function                           */
    }
                                                      /*lint -e{644} : rxId and txId set above    */
    if (((rxId & CO_SDO_COBID_OFF) == 0) &&           /* see, if both identifiers are valid       */
        ((txId & CO_SDO_COBID_OFF) == 0) ) {
        srvnum->RxId = rxId;                          /* yes: set request and response ID's       */
        srvnum->TxId = txId;
    }
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  CHECK FOR SDO FRAME
*
* \ingroup  INTERNAL
*
*           This function checks the given frame to be a SDO request. If the frame is identified
*           to be a SDO request, the identifier in the frame will be modified to be the
*           corresponding SDO response.
*
* \param[in,out]   srv        Ptr to root element of SDO server array
*
* \param[in,out]   frm        Frame, received from CAN bus
*
* \retval   >0     pointer to addressed SDO server
* \retval   =0     not a SDO request or an error is detected.
*/
/*------------------------------------------------------------------------------------------------*/
CO_SDO *CO_SdoCheck(CO_SDO *srv, CO_IF_FRM *frm)
{                                                     /*------------------------------------------*/
    CO_SDO     *result = 0;                           /* Local: function result                   */
    CPU_INT08U  n;                                    /* Local: SDO counter                       */
                                                      /*------------------------------------------*/
    if (frm != 0) {                                   /* see, if CAN frame is a valid pointer     */
        n = 0;                                        /* reset SDO counter                        */
        while ((n < CO_SDO_N) && (result == 0)) {     /* Loop through SDO server list             */
            if (CO_GET_COBID(frm) == srv[n].RxId) {   /* see, if received ID is equal request ID  */
                CO_SET_COBID(frm, srv[n].TxId);       /* set corresponding response ID            */
                srv[n].Frm = frm;                     /* store request in SDO server object       */
                if (srv[n].Obj == 0) {
                    srv[n].Idx = CO_GET_WORD(frm, 1);
                    srv[n].Sub = CO_GET_BYTE(frm, 3);
                }
                result = &srv[n];                     /* return pointer to SDO server             */
            }
            n++;                                      /* increment SDO identifier                 */
        }                                             /*------------------------------------------*/
    }
    return (result);                                  /* return function result                   */
}                                                     /*------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief  GENERATE SDO RESPONSE
*
* \ingroup  INTERNAL
*
*           This function interprets the data byte #0 of the SDO request and organizes the
*           generation of the corresponding response.
*
* \param[in,out]   srv        Ptr to root element of SDO server array
*
* \retval   =0     successful SDO response
* \retval   <0     an error is detected
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CO_SdoResponse(CO_SDO *srv)
{
    CPU_INT16S  result = -1;                          /* Local: function result                   */
    CPU_INT08U  cmd;                                  /* Local: command byte                      */
                                                      /*------------------------------------------*/
    cmd = CO_GET_BYTE(srv->Frm, 0);                   /* request: get command byte                */
                                                      /*------------------------------------------*/
                                                      /*                A B O R T                 */
                                                      /*------------------------------------------*/
    if (cmd == 0x80) {                                /* cs=4: abort request                      */
#if CO_SDO_SEG_EN > 0 || CO_SDO_BLK_EN > 0
        CO_SdoAbortReq(srv);                          /* perform abort of SDO transfer            */
#endif
        return (-2);                                  /* no transmission of response frame        */
    }

#if CO_SDO_BLK_EN > 0
    if (srv->Blk.State == BLK_DOWNLOAD) {             /* see, if block download is active         */
                                                      /*------------------------------------------*/
        result = CO_SdoDownloadBlock(srv);            /* perform block download                   */
        return (result);                              /* return function result                   */

    } else if (srv->Blk.State == BLK_DNWAIT) {        /* otherwise see, if download is waiting    */

                                                      /*------------------------------------------*/
        if ((cmd & 0xE3) == 0xC1) {                   /*       E N D   B L K   D O W N L O A D    */
                                                      /*------------------------------------------*/
            result = CO_SdoEndDownloadBlock(srv);     /* perform end block download               */
        } else {                                      /* otherwise: within block segments         */
            srv->Blk.State   = BLK_DOWNLOAD;          /* set to download active                   */
            result = CO_SdoDownloadBlock(srv);        /* perform block download                   */
        }
        return (result);                              /* return function result                   */

    } else if (srv->Blk.State == BLK_UPLOAD) {        /* otherwise see, if block upload is active */
                                                      /*------------------------------------------*/
        if (cmd == 0xA1) {                            /*         E N D   B L K   U P L O A D      */
                                                      /*------------------------------------------*/
            result = CO_SdoEndUploadBlock(srv);       /* perform end block upload                 */
                                                      /*------------------------------------------*/
        } else if ((cmd & 0xE3) == 0xA2) {            /* otherwise: within block segments         */
                                                      /*------------------------------------------*/
            result = CO_SdoAckUploadBlock(srv);       /*         A C K   B L K   U P L O A D      */
                                                      /*------------------------------------------*/
        } else {                                      /* otherwise: unknown response in upload    */
            CO_SdoAbort(srv, CO_SDO_ERR_CMD);         /* Abort message: command invalid/unknown   */
        }
        return (result);                              /* return function result                   */
    }
#endif

                                                      /*------------------------------------------*/
    if ((cmd & 0xF2) == 0x22) {                       /*   E X P E D I T E D   D O W N L O A D    */
                                                      /*------------------------------------------*/
        result = CO_SdoGetObject(srv, CO_SDO_WR);     /* yes: link addressed object entry         */
        if (result == 0) {                            /* see, if an error is detected             */
            result = CO_SdoDownloadExpedited(srv);    /* perform expedited download transfer      */
        }
                                                      /*------------------------------------------*/
    } else if (cmd == 0x40) {                         /*                U P L O A D               */
                                                      /*------------------------------------------*/
        result = CO_SdoGetObject(srv, CO_SDO_RD);     /* yes: link addressed object entry         */
        if (result == 0) {                            /* see, if an error is detected             */
            result = CO_SdoUploadExpedited(srv);      /* perform the upload transfer              */
        }

#if CO_SDO_SEG_EN > 0
                                                      /*------------------------------------------*/
    } else if ((cmd & 0xF2) == 0x20) {                /*    I N I T   S E G   D O W N L O A D     */
                                                      /*------------------------------------------*/
        result = CO_SdoGetObject(srv, CO_SDO_WR);     /* yes: link addressed object entry         */
        if (result == 0) {                            /* see, if an error is detected             */
            result = CO_SdoInitDownloadSegmented(srv);/* perform init of download transfer        */
        }
                                                      /*------------------------------------------*/
    } else if ((cmd & 0xE0) == 0x00) {                /*          S E G   D O W N L O A D         */
                                                      /*------------------------------------------*/
        result = CO_SdoDownloadSegmented(srv);        /* perform segmented download               */
                                                      /*------------------------------------------*/
    } else if ((cmd & 0xEF) == 0x60) {                /*            S E G   U P L O A D           */
                                                      /*------------------------------------------*/
        result = CO_SdoUploadSegmented(srv);          /* perform segmented upload                 */
#endif

#if CO_SDO_BLK_EN > 0
                                                      /*------------------------------------------*/
    } else if ((cmd & 0xF9) == 0xC0) {                /*     I N I T   B L K   D O W N L O A D    */
                                                      /*------------------------------------------*/
        result = CO_SdoGetObject(srv, CO_SDO_WR);     /* yes: link addressed object entry         */
        if (result == 0) {                            /* see, if an error is detected             */
            result = CO_SdoInitDownloadBlock(srv);    /* perform init block download              */
        }
                                                      /*------------------------------------------*/
    } else if ((cmd & 0xE3) == 0xA0) {                /*       I N I T   B L K   U P L O A D      */
                                                      /*------------------------------------------*/
        result = CO_SdoInitUploadBlock(srv);          /* perform init block upload                */
                                                      /*------------------------------------------*/
    } else if (cmd == 0xA3) {                         /*             B L K   U P L O A D          */
                                                      /*------------------------------------------*/
        result = CO_SdoUploadBlock(srv);              /* perform block upload                     */
#endif

    } else {                                          /* otherwise: not supported/invalid command */
        CO_SdoAbort(srv, CO_SDO_ERR_CMD);             /* Abort message: command invalid/unknown   */
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  GET ADDRESSED OBJECT
*
* \ingroup  INTERNAL
*
*           This function looks for the addressed object in the object directory and generates
*           several abort messages on bad addresses. If everything is fine, this function
*           stores the corresponding object entry reference in the SDO server object and returns 0.
*
* \param[in,out]   srv        Ptr to root element of SDO server array
*
* \param[in]       mode       Access mode (\ref CO_SDO_RD or \ref CO_SDO_WR)
*
* \retval   =0     successful operation
* \retval   <0     abort
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CO_SdoGetObject(CO_SDO *srv, CPU_INT16U mode)
{
    CO_OBJ     *obj;                                  /* Local: reference to object entry         */
    CPU_INT32U  key;                                  /* Local: object entry key                  */
    CPU_INT16S  result = -1;                          /* Local: function result                   */
                                                      /*------------------------------------------*/
    key = CO_DEV(srv->Idx, srv->Sub);                 /* construct key of object entry            */
    obj = CODirFind(&srv->Node->Dir, key);            /* search addressed object entry            */
    if (obj != 0) {                                   /* see, if object entry is found            */
        if (mode == CO_SDO_RD) {                      /* see, if request wants to read            */
            if (CO_IS_READ(obj->Key) != 0) {          /* see, if object is readable               */
                srv->Obj = obj;                       /* store found object entry                 */
                result   = 0;                         /* indicate successful operation            */
            } else {
                CO_SdoAbort(srv, CO_SDO_ERR_RD);      /* yes: Abort 'object is write only'        */
            }
        } else {                                      /* otherwise: the request wants to write    */
            if (CO_IS_WRITE(obj->Key) != 0) {         /* see, if object is writable               */
                srv->Obj = obj;                       /* store found object entry                 */
                result   = 0;                         /* indicate successful operation            */
            } else {
                CO_SdoAbort(srv, CO_SDO_ERR_WR);      /* yes: Abort 'object is read only'         */
            }
        }
    } else {                                          /* otherwise: object is not found           */
        if (srv->Sub == 0) {                          /* see, if accessed subindex was 0          */
            CO_SdoAbort(srv, CO_SDO_ERR_OBJ);         /* Abort 'object not existant'              */
        } else {                                      /* otherwise: access subindex > 0           */
            key = CO_DEV(srv->Idx, 0);                /* construct key of object entry main index */
            obj = CODirFind(&srv->Node->Dir, key);    /* search main index object entry           */
            if (obj != 0) {                           /* see, if main index is existing in OD     */
                CO_SdoAbort(srv, CO_SDO_ERR_SUB);     /* yes: Abort 'subindex not existant'       */
            } else {                                  /* otherwise: object is not existing in OD  */
                CO_SdoAbort(srv, CO_SDO_ERR_OBJ);     /* Abort 'object not existant'              */
            }
        }
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  GET SIZE OF OBJECT
*
* \ingroup  INTERNAL
*
*           This function checks the given size acc. the addressed object size in the object
*           directory and generates several abort messages on bad size informations. If
*           everything is fine, this function returns the size of the object in byte.
*
* \param[in,out]   srv               Pointer to SDO server object
*
* \param[in]       width             Parameter size in byte (or 0 if unknown)
*
* \retval   =0     Abort for known width
* \retval   >0     Size of Object
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT32U CO_SdoGetSize(CO_SDO *srv, CPU_INT32U width)
{
    CPU_INT32U result = 0;                            /* Local: function result                   */
    CPU_INT32U size;                                  /* Local: object size in byte               */
                                                      /*------------------------------------------*/
    size = COObjGetSize(srv->Obj, width);             /* Get size of object in byte               */
    if (size == 0) {                                  /* object access failed (bad config)        */
        CO_SdoAbort(srv, CO_SDO_ERR_TOS);             /* Abort message: Data cannot be transfered */
        return (result);                              /* abort function with error indication     */
    }
                                                      /*------------------------------------------*/
    if (width != 0) {                                 /* see, if a data width is given            */
        if (size == width) {                          /* yes: see, if object size = data width    */
            result = size;                            /* yes: data ok                             */
        } else {                                      /* otherwise: data width mismatch error     */
            if (size < width) {                       /* see if object is < data bytes            */
                CO_SdoAbort(srv, CO_SDO_ERR_LEN_HIGH);/* yes: Abort 'datatype length too high'    */
            } else {                                  /* otherwise: object is > data bytes        */
                CO_SdoAbort(srv, CO_SDO_ERR_LEN_SMALL);/* Abort 'datatype length too small'       */
            }
        }
    } else {                                          /* otherwise: data width unknown            */
        result = size;                                /* assume data ok                           */
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  EXPEDITED UPLOAD PROTOCOL
*
* \ingroup  INTERNAL
*
*           This function generates the response for a SDO expedited upload request. The function
*           checks the object size for matching into a simple expedited transfer. If possible this
*           transfer is used. The segmented transfer (if support available) is initiated in any
*           other cases.
*
* \param[in,out]   srv               Pointer to SDO server object
*
* \retval   =0     successful operation
* \retval   <0     abort
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CO_SdoUploadExpedited(CO_SDO *srv)
{
    CPU_INT32U  size;                                 /* Local: object size in byte               */
    CPU_INT32U  data   = 0;                           /* Local: object entry value                */
    CPU_INT16S  result = -1;                          /* Local: function result                   */
    CPU_INT16S  num;                                  /* Local: number of read bytes              */
    CPU_INT08U  cmd;                                  /* Local: SDO command byte                  */
                                                      /*------------------------------------------*/
    size = CO_SdoGetSize(srv, 0);                     /* check data width acc. object             */
    if (size == 0) {                                  /* see, if an error is detected             */
        return (result);                              /* abort function                           */
    } else if ((size > 0) && (size <= 4)) {           /* see, if size is in valid range           */
                                                      /*------------------------------------------*/
        num = COObjRdValue(srv->Obj,                  /* read value from object entry             */
                           (void *)&data,
                           CO_LONG,
                           srv->Node->NodeId);
        if (num != CO_ERR_NONE) {                     /* see, if an error is detected             */
            CO_SdoAbort(srv, CO_SDO_ERR_TOS);         /* Abort message: Data cannot be transfered */
            return (result);                          /* return and confirm message               */

        } else {                                      /* otherwise: reading successfull           */

            cmd = 0x43 | (((4-size) & 0x3) << 2);     /* response: scs=2, n=4-width, e=1, s=1     */
            CO_SET_BYTE(srv->Frm,  cmd, 0);           /* response: command byte                   */
                                                      /*lint -e{644} : data set in COObjRdValue   */
            CO_SET_LONG(srv->Frm, data, 4);           /* data in byte #4..#7                      */
            srv->Obj = 0;                             /* indicates: transfer finished             */
            result   = 0;                             /* indicates: transfer accepted             */
        }
    }

#if CO_SDO_SEG_EN > 0
    if (size > 4) {                                   /* see, if size is in valid range           */
        result = CO_SdoInitUploadSegmented(srv, size);/* initialize segmented transfer            */
    }
#endif

    if (result < 0) {                                 /* otherwise: no transfer possible          */
        CO_SdoAbort(srv, CO_SDO_ERR_CMD);             /* Abort 'command invalid/unknown'          */
    }
    return (result);                                  /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  EXPEDITED DOWNLOAD PROTOCOL
*
* \ingroup  INTERNAL
*
*           This function generates the response for a SDO expedited download request.
*
* \param[in,out]   srv               Pointer to SDO server object
*
* \retval   =0     successful operation
* \retval   <0     abort
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CO_SdoDownloadExpedited(CO_SDO *srv)
{
    CPU_INT32U  size;                                 /* Local: object entry size in byte         */
    CPU_INT32U  data;                                 /* Local: object entry value                */
    CPU_INT32U  width  =  0;                          /* Local: number of data bytes              */
    CPU_INT16S  result = -1;                          /* Local: function result                   */
    CPU_INT16S  num;                                  /* Local: number of written bytes           */
    CPU_INT08U  cmd;                                  /* Local: SDO command byte                  */
                                                      /*------------------------------------------*/
    cmd = CO_GET_BYTE(srv->Frm, 0);                   /* extract request command byte             */
    if ((cmd & 0x01) == 1) {                          /* see, if size is indicated (s=1/0)        */
        width = 4 - ((cmd >> 2) & 0x03);              /* calculate number of data bytes           */
    }
    size = CO_SdoGetSize(srv, width);                 /* check data width acc. object             */
    if ((size > 0) && (size <= 4)) {                  /* see, if size is in valid range           */
                                                      /*------------------------------------------*/
        data = CO_GET_LONG(srv->Frm, 4);              /* extract data from CAN frame              */

        num = COObjWrValue(srv->Obj,                  /* write value to object entry              */
                           (void*)&data,
                           CO_LONG,
                           srv->Node->NodeId);
        if (num != CO_ERR_NONE) {                     /* see, if write detects an error           */

            if (num ==  CO_ERR_OBJ_RANGE) {
                CO_SdoAbort(srv,                      /* Abort 'cannot be stored'                 */
                            CO_SDO_ERR_RANGE);

            } else if (num == CO_ERR_OBJ_MAP_TYPE) {
                CO_SdoAbort(srv,                      /* Abort 'cannot be mapped'                 */
                            CO_SDO_ERR_OBJ_MAP);


            } else if (num == CO_ERR_OBJ_ACC) {
                CO_SdoAbort(srv,                      /* Abort 'Data cannot be transfered'        */
                            CO_SDO_ERR_TOS);

            } else if (num == CO_ERR_OBJ_MAP_LEN) {
                CO_SdoAbort(srv,                      /* Abort 'length exceeds pdo'               */
                            CO_SDO_ERR_OBJ_MAP_N);

            } else if (num == CO_ERR_OBJ_INCOMPATIBLE) {
                CO_SdoAbort(srv,                      /* Abort 'parameter incompatibility reason' */
                            CO_SDO_ERR_PARA_INCOMP);
            } else {                                  /* otherwise: not in communication profile  */
                CO_SdoAbort(srv, CO_SDO_ERR_TOS);     /* Abort 'Data cannot be transfered'        */
            }

        } else {                                      /* otherwise: write successfull             */
            CO_SET_BYTE(srv->Frm, 0x60, 0);           /* scs=3: initiate download response        */
            CO_SET_LONG(srv->Frm,    0, 4);           /* clear all 4 data bytes with 0            */
            srv->Obj = 0;                             /* indicates: transfer finished             */
            result   = 0;                             /* indicates: transfer accepted             */
        }
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  ABORT PROTOCOL
*
* \ingroup  INTERNAL
*
*           This function generates the response for a SDO abort.
*
* \param[in,out]   srv               Pointer to SDO server object
*
* \param[in]       err               The error code to be inserted in the data bytes #4..#7
*/
/*------------------------------------------------------------------------------------------------*/
void CO_SdoAbort(CO_SDO *srv, CPU_INT32U err)
{
    CO_SET_BYTE(srv->Frm,     0x80, 0);               /* set response: cs=4 (Abort SDO Transfer)  */
    CO_SET_WORD(srv->Frm, srv->Idx, 1);               /* set index                                */
    CO_SET_BYTE(srv->Frm, srv->Sub, 3);               /* set subindex                             */
    CO_SET_LONG(srv->Frm,      err, 4);               /* set error code into data bytes #4..#7    */

    srv->Obj = 0;                                     /* indicates: transfer finished             */
}
