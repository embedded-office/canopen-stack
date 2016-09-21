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
* \file     co_rpdo.c
*
* \brief    RPDO MANAGEMENT
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_rpdo.c#5 $
*
*           This source file implements the RPDO handling.
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
*                                       INTERNAL FUNCTIONS
****************************************************************************************************
*/

#if CO_RPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief RPDO CLEAR
*
* \ingroup INTERNAL
*
*          This function sets the PDO related configuration data to a known state (inactive).
*
* \param[in,out]   pdo          Pointer to start of RPDO array
*
* \param[in]       node         Pointer to parent node object
*/
/*------------------------------------------------------------------------------------------------*/
void CO_RPdoClear(CO_RPDO *pdo, CO_NODE *node)
{
    int16_t num;                                   /* Local: loop through RPDO number          */
                                                      /*------------------------------------------*/
    if ((pdo == 0) || (node == 0)) {                  /* see, if argument pointers are invalid    */
        CO_NodeFatalError();                          /* inform user                              */
        return;                                       /* abort initialization                     */
    }
    for (num = 0; num < CO_RPDO_N; num++) {           /* loop through all possible supported RPDO */
        pdo[num].Node       = node;                   /* link CANopen node informations           */
        pdo[num].Identifier = 0;                      /* clear message identifier                 */
        pdo[num].ObjNum     = 0;                      /* indicate no configured message           */
    }
}
#endif

#if CO_RPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief RPDO INITIALIZATION
*
* \ingroup INTERNAL
*
*          This function checks the object directory and creates the PDO related
*          configuration data from the object directory settings.
*
* \param[in,out]   pdo          Pointer to start of RPDO array
*
* \param[in]       node         Pointer to parent node object
*/
/*------------------------------------------------------------------------------------------------*/
void CO_RPdoInit(CO_RPDO *pdo, CO_NODE *node)
{
    int16_t err;                                   /* Local: error code                        */
    uint8_t rnum;                                  /* LocaL: number of PDO Com Entries         */
    int16_t num;                                   /* Local: loop through RPDO number          */
                                                      /*------------------------------------------*/
    if ((pdo == 0) || (node == 0)) {                  /* see, if argument pointers are invalid    */
        CO_NodeFatalError();                          /* inform user                              */
        return;                                       /* abort initialization                     */
    }
    for (num = 0; num < CO_RPDO_N; num++) {           /* loop through all possible supported RPDO */
        pdo[num].Node       = node;                   /* link CANopen node informations           */
        pdo[num].Identifier = 0;                      /* clear message identifier                 */
        pdo[num].ObjNum     = 0;                      /* indicate no configured message           */
        err = CODirRdByte(&node->Dir,                 /* check RPDO communication settings        */
                          CO_DEV(0x1400,0),&rnum);
        if (err == CO_ERR_NONE) {                     /* RPDO Communication settings are existing */
            CO_RPdoReset(pdo, num);                   /* scan RPDO communication and mapping      */
        } else {
            node->Error = CO_ERR_NONE;                /* RPDOs are optional, remove error         */
        }
    }
}
#endif

#if CO_RPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief RESET RPDO COMMUNICATION PROFILE
*
* \ingroup INTERNAL
*
*          This function scans the object directory for the RPDO communication and mapping
*          profile. The function will fill the RPDO info structure with the corresponding
*          informations.
*
*          The following list shows the considered communication profile entries:
*          -# 0x1400+[num] : 0x01 = RPDO CAN identifier
*          -# 0x1400+[num] : 0x02 = Transmission type
*
* \note    The object entry 0x1400+[num] : 0x03..0x05 are reserved and should be not existing.
*          This is not checked with this function.
*
* \param[in,out]   pdo          Pointer to start of RPDO array
*
* \param[in]       num          Number of RPDO (0..511)
*
* \retval  =0      The communication profile is checked, and data are considered
* \retval  <0      At least one communication profile entry is invalid, further checks aborted
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_RPdoReset(CO_RPDO *pdo, int16_t num)
{
    CO_RPDO    *wp;                                   /* Local: Pointer to working RPDO           */
    CO_DIR     *cod;                                  /* Local: Pointer to object directory       */
    uint32_t  id      = CO_RPDO_COBID_OFF;          /* Local: COBID from profile                */
    int16_t  err;                                  /* Local: function call error code          */
    uint8_t  on;                                   /* Local: loop through object map           */
    uint8_t  type    = 0;                          /* Local: transmission type from profile    */
                                                      /*------------------------------------------*/
    wp             = &pdo[num];                       /* set pointer to working pdo               */
    cod            = &wp->Node->Dir;                  /* set pointer to object directory          */
    wp->Identifier = 0;                               /* clear message identifier                 */
    wp->ObjNum     = 0;                               /* indicate no configured message           */
    for (on = 0; on < 8; on++) {                      /* loop through object map array            */
        wp->Map[on] = 0;                              /* clear map entry                          */
    }
#if CO_SYNC_EN > 0
    if ((wp->Flag & CO_RPDO_FLG_S_) != 0) {           /* see, if RPDO is synchronized             */
        CO_SyncRemove(&pdo->Node->Sync,               /* remove RPDO from SYNC management table   */
                      num,
                      CO_SYNC_FLG_RX);
    }
#endif
    wp->Flag       = 0;                               /* clear flag attributes                    */
                                                      /*------------------------------------------*/
                                                      /*         C O M M U N I C A T I O N        */
                                                      /*------------------------------------------*/
    err = CODirRdByte(cod,                            /* get RPDO transmission type               */
                      CO_DEV(0x1400 + num, 2),
                      &type);
    if (err != CO_ERR_NONE) {                         /* see, if an read error is detected        */
        pdo->Node->Error = CO_ERR_RPDO_COM_OBJ;       /* set error code                           */
        return (-1);                                  /* return error indication                  */
    }
#if CO_SYNC_EN == 0
    if (type <= 240) {                                /* see, if synchronous transmission type    */
        pdo->Node->Error = CO_ERR_RPDO_COM_OBJ;       /* set error code                           */
        return (-1);                                  /* return error indication                  */
    }                                                 /*------------------------------------------*/
#endif
    err = CODirRdLong(cod,                            /* get RPDO identifier                      */
                      CO_DEV(0x1400 + num, 1),
                      &id);
    if (err != CO_ERR_NONE) {                         /* see, if an read error is detected        */
        pdo->Node->Error = CO_ERR_RPDO_COM_OBJ;       /* set error code                           */
        return (-1);                                  /* return error indication                  */
    }
    if ((id & CO_RPDO_COBID_REMOTE) == 0) {           /* see, if PDO is configured with RTR       */
        pdo->Node->Error = CO_ERR_RPDO_COM_OBJ;       /* set error code (RTR is not supported)    */
        return (-1);                                  /* return error indication                  */
    }
    if ((id & CO_RPDO_COBID_EXT) != 0) {              /* see, if PDO is configured with ext. ID   */
        pdo->Node->Error = CO_ERR_RPDO_COM_OBJ;       /* set error code (mixed IDs not supported) */
        return (-1);                                  /* return error indication                  */
    }
    if ((id & CO_RPDO_COBID_OFF) == 0) {
        pdo[num].Identifier = (id & 0x1FFFFFFF);      /* set identifier in message                */
        pdo[num].Flag       = CO_RPDO_FLG__E;         /* set message type (transmit message)      */
#if CO_SYNC_EN > 0
        if (type <= 240) {                            /* see, if transmission type is 0..240      */
            pdo[num].Flag  |= CO_RPDO_FLG_S_;         /* set message type to synchronous          */
        }
#endif
    } else {
        pdo[num].Identifier = CO_RPDO_COBID_OFF;      /* set identifier in message                */
        pdo[num].Flag       = 0;                      /* set message type (transmit message)      */
    }                                                 /*------------------------------------------*/
                                                      /*             M A P P I N G                */
                                                      /*------------------------------------------*/
    err = CO_RPdoGetMap(pdo, num);                    /* get RPDO mapping from object directory   */
    if (err != CO_ERR_NONE) {                         /* see, if an error is detected             */
        pdo->Node->Error = CO_ERR_RPDO_MAP_OBJ;       /* set error                                */
    }                                                 /*------------------------------------------*/
    if ((pdo[num].Flag & CO_RPDO_FLG__E) != 0) {      /* see, if message configuration is valid   */
#if CO_SYNC_EN > 0
        if (type <= 240) {                            /* transmission type: synchronous 0 .. 240  */
            CO_SyncAdd(&pdo[num].Node->Sync,
                       num,
                       CO_SYNC_FLG_RX,
                       type);
        }
#endif
    }                                                 /*------------------------------------------*/
    return (0);                                       /* return function result                   */
}
#endif

#if CO_RPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief GET PDO MAPPING DATA
*
* \ingroup INTERNAL
*
*          This function gets the PDO mapping datas out of the object directory and puts the
*          pre-calculated values in the CAN message configuration.
*
*          The following list shows the considered mapping profile entries:
*          -# 0x1600+[num] : 0x00 = Number of mapped signals (0..8)
*          -# 0x1600+[num] : 0x01..0x08 = Mapped signal
*
* \param[in,out]   pdo          Pointer to start of RPDO array
*
* \param[in]       num          Number of RPDO (0..511)
*
* \retval  =0      Mapping successful parsed out of object directory
* \retval  <0      At least one error is detected within the mapping configuration
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_RPdoGetMap (CO_RPDO *pdo, uint16_t num)
{
    CO_DIR      *cod;                                 /* Local: pointer to object directory       */
    CO_OBJ      *obj;                                 /* Local: pointer to object entry           */
    uint32_t   mapping;                             /* Local: PDO mapping information           */
    uint16_t   idx;                                 /* Local: object entry index                */
    uint16_t   link;                                /* Local: object entry index                */
    int16_t   err;                                 /* Local: error of CANopen functions        */
    uint8_t   on;                                  /* Local: Loop counter for mapped objects   */
    uint8_t   mapnum;                              /* Local: number of PDO mappings            */
    uint8_t   dlc;                                 /* Local: resulting data length code        */
    uint8_t   dummy = 0;                           /* Local: number of dummy bytes             */
                                                      /*------------------------------------------*/
    cod = &pdo[num].Node->Dir;                        /* get pointer to object directory          */
    idx = 0x1600 + num;                               /* set index to TPDO mapping profile        */
    err = CODirRdByte(cod, CO_DEV(idx, 0), &mapnum);  /* get number of mapped signals             */
    if (err != CO_ERR_NONE) {                         /* see, if an error is detected             */
        return (-1);                                  /* abort with error indication              */
    }
    dlc = 0;                                          /* clear number of bytes in message         */
                                                      /*------------------------------------------*/
    for (on=0; on < mapnum; on++) {                   /* Loop through all mapped signals          */
        err = CODirRdLong(cod,                        /* get mapping information of this PDO      */
                          CO_DEV(idx, 1+on),
                          &mapping);
        if (err != CO_ERR_NONE) {                     /* see, if an error is detected             */
            return (-1);                              /* abort with error indication              */
        }
                                                      /*lint -e{644} : mapping set in CODirRdLong */
        dlc += (uint8_t)(mapping & 0xFF) >> 3;     /* add number of bits, converted to bytes   */
        if (dlc > 8){                                 /* more than 8 byte not possible in a PDO   */
            return (-1);                              /* abort with error indication              */
        }
        link = mapping >> 16;                         /* get index out of the mapping             */
        if ((link == 2) || (link == 5)) {             /* see, if index is between 2 or 5; this    */
                                                      /* indicates a dummy BYTE mapping entry     */
            pdo[num].Map[on+dummy] = 0;               /* set object pointer indicating dummy byte */
            dummy++;                                  /* increment number of dummy bytes          */
        } else if ((link == 3) || (link == 6)) {      /* see, if index is between 3 or 6; this    */
                                                      /* indicates a dummy WORD mapping entry     */
            pdo[num].Map[on+dummy] = 0;               /* set object pointer indicating dummy byte */
            dummy++;                                  /* increment number of dummy bytes          */
            pdo[num].Map[on+dummy] = 0;               /* set object pointer indicating dummy byte */
            dummy++;                                  /* increment number of dummy bytes          */
        } else if ((link == 4) || (link == 7)) {      /* see, if index is between 3 or 6; this    */
                                                      /* indicates a dummy LONG mapping entry     */
            pdo[num].Map[on+dummy] = 0;               /* set object pointer indicating dummy byte */
            dummy++;                                  /* increment number of dummy bytes          */
            pdo[num].Map[on+dummy] = 0;               /* set object pointer indicating dummy byte */
            dummy++;                                  /* increment number of dummy bytes          */
            pdo[num].Map[on+dummy] = 0;               /* set object pointer indicating dummy byte */
            dummy++;                                  /* increment number of dummy bytes          */
            pdo[num].Map[on+dummy] = 0;               /* set object pointer indicating dummy byte */
            dummy++;                                  /* increment number of dummy bytes          */
        } else {
            obj = CODirFind(&pdo->Node->Dir, mapping);/* get mapping information this PDO         */
            if (obj == 0) {                           /* see, if object entry is not found        */
                return (-1);                          /* abort with error indication              */
            } else {
                pdo[num].Map[on+dummy] = obj;         /* add object pointer to RPDO map           */
            }
        }
    }
    pdo[num].ObjNum = mapnum+dummy;                   /* set number of RPDO object references     */
                                                      /*------------------------------------------*/
    return (0);
}
#endif

#if CO_RPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief RPDO RECEIVE
*
* \ingroup INTERNAL
*
*          This function is responsible for the distribution of a RPDO into the corresponding
*          signals within the object directory.
*
* \param[in]       pdo          Pointer to RPDO element
*
* \param[in]       num          Number of RPDO (0..511)
*
* \param[in]       frm          Pointer to received CAN frame
*/
/*------------------------------------------------------------------------------------------------*/
void CO_RPdoRx (CO_RPDO *pdo, uint16_t num, CO_IF_FRM *frm)
{
    int16_t  err = 0;                              /* Local: error code                        */
                                                      /*------------------------------------------*/
    if ((pdo[num].Flag & CO_RPDO_FLG_S_) == 0) {      /* if not a synchronized PDO                */
#if CO_CB_RPDO_RECEIVE_EN > 0
        err = CO_RPdoReceive(frm);                    /* Application callback function            */
#endif
        if (err == 0) {                               /* see, if msg is not consumed by callback  */
            CO_RPdoWrite(&pdo[num], frm);             /* write content to object directory        */
        }
#if CO_SYNC_EN > 0
    } else {                                          /* otherwise: if see if synchronized PDO    */
        CO_SyncRx(&pdo[num].Node->Sync, frm);         /* yes: handle synchronous receive PDO      */
#endif
    }
}
#endif

#if CO_RPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief RPDO CHECK
*
* \ingroup INTERNAL
*
*          This function is used to check the received CAN message frame to be a RPDO message.
*
* \param[in]       pdo          Pointer to start of RPDO array
*
* \param[in]       frm          Received CAN message frame
*
* \retval  >=0     The number of the destination RPDO
* \retval  <0      Not a valid RPDO CAN message
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_RPdoCheck(CO_RPDO *pdo, CO_IF_FRM *frm)
{
    int16_t result = -1;                           /* Local: function result                   */
    uint16_t n;                                     /* Local: PDO counter                       */
                                                      /*------------------------------------------*/
    n = 0;                                            /* reset PDO counter                        */
    while (n < CO_RPDO_N) {                           /* Loop through whole PDO table             */
        if ((pdo[n].Flag & CO_RPDO_FLG__E) != 0) {    /* Look for ID and receive type             */
            if (pdo[n].Identifier ==
                frm->Identifier) {
                result = n;
                break;
            }
        }                                             /*------------------------------------------*/
        n++;                                          /* increment counter                        */
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}
#endif

#if CO_RPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief RPDO WRITE
*
* \ingroup INTERNAL
*
*          This function is used to write the received CAN message data to the object directory.
*
* \param[in]       pdo          Pointer to start of RPDO array
*
* \param[in]       frm          Received CAN message frame
*/
/*------------------------------------------------------------------------------------------------*/
void CO_RPdoWrite(CO_RPDO *pdo, CO_IF_FRM *frm)
{
    CO_OBJ     *obj;                                  /* Local: pointer to linked object          */
    uint32_t  val32;                                /* Local: received long value               */
    uint16_t  val16;                                /* Local: received word value               */
    uint8_t  val08;                                /* Local: received byte value               */
    uint8_t  on;                                   /* Local: loop through linked objects       */
    uint8_t  sz;                                   /* Local: size of object entry              */
    uint8_t  dlc = 0;                              /* Local: data length code                  */
                                                      /*------------------------------------------*/
    for (on = 0; on < pdo->ObjNum; on++) {            /* loop through RPDO object map             */
        obj = pdo->Map[on];                           /* get next object pointer                  */
        if (obj != 0) {                               /* see, if not a dummy byte                 */
            sz = (uint8_t)COObjGetSize(obj, 0L);   /* get size of object                       */
            if (sz == CO_BYTE) {                      /* see, if object is a byte                 */
                val08 = CO_GET_BYTE(frm, dlc);        /* get byte from payload                    */
                dlc++;                                /* increment dlc position                   */
                COObjWrValue(obj,                     /* write object with message payload data   */
                             (void *)&val08,
                             sz,
                             pdo->Node->NodeId);
            } else if (sz == CO_WORD) {               /* otherwise, see if object is a word       */
                val16 = CO_GET_WORD(frm, dlc);        /* get word from payload                    */
                dlc += 2;                             /* increment dlc position                   */
                COObjWrValue(obj,                     /* write object with message payload data   */
                             (void *)&val16,
                             sz,
                             pdo->Node->NodeId);
            } else if (sz == CO_LONG) {               /* otherwise, see if object is a long       */
                val32 = CO_GET_LONG(frm, dlc);        /* get long from payload                    */
                dlc += 4;                             /* increment dlc position                   */
                COObjWrValue(obj,                     /* write object with message payload data   */
                             (void *)&val32,
                             sz,
                             pdo->Node->NodeId);
            }
        }
    }
}
#endif

/*
****************************************************************************************************
*                                       CALLBACK FUNCTIONS
****************************************************************************************************
*/

#if CO_CB_RPDO_RECEIVE_EN == 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  RPDO RECEIVE CALLBACK
*
* \ingroup  CALLBACK
*
*           This function is called just before the RPDO receiption will distribute the
*           RPDO message frame into the object directory. This callback function is able
*           to <i>consume</i> the RPDO message frame, e.g. the distribution into the
*           object directory will be skipped. Furthermore without <i>consuming</i> the
*           RPDO message frame, this function could modify the recieved data before
*           distribution takes place.
*
* \note     This function can be implemented within the application. This function is a
*           placeholder only. The application implementation of this function is activated
*           with the configuration define \ref CO_CB_RPDO_RECEIVE_EN.
*
* \param[in]       frm          Pointer to TPDO message frame
*
* \retval   =0     CAN message frame is not consumed
* \retval   >0     CAN message frame is consumed
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_RPdoReceive (CO_IF_FRM *frm)
{
    (void)frm;                                        /* unused, prevent compiler warning         */
    return (0);
}
#endif
