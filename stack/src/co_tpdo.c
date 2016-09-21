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
* \file     co_tpdo.c
*
* \brief    TPDO MANAGEMENT
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_tpdo.c#6 $
*
*           This source file implements the PDO handling.
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

#if CO_TPDO_N > 0
static CPU_INT16S CO_TAsyncCtrl (CO_OBJ* obj, CPU_INT16U func, CPU_INT32U para);
static CPU_INT16S CO_TEventWrite(CO_OBJ* obj, void *buf, CPU_INT32U size);
#endif

/*
****************************************************************************************************
*                                       GLOBAL TYPE STRUCTURE
****************************************************************************************************
*/

#if CO_TPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief OBJECT TYPE ASYNCHRONOUS TPDO OBJECT
*
* \ingroup TPDO
*
*          This object type specializes the general handling of objects for object directory
*          entries, which triggers a TPDO event on changed values.
*/
/*------------------------------------------------------------------------------------------------*/
CO_OBJ_TYPE COTAsync = { 0, 0, 0, CO_TAsyncCtrl, 0, 0 };

/*------------------------------------------------------------------------------------------------*/
/*! \brief OBJECT TYPE TPDO EVENT TIMER
*
* \ingroup TPDO
*
*          This object type specializes the general handling of the event timer object within
*          the TPDO communication profile.
*/
/*------------------------------------------------------------------------------------------------*/
CO_OBJ_TYPE COTEvent = { 0, 0, 0, 0, 0, CO_TEventWrite };
#endif

/*
****************************************************************************************************
*                                            FUNCTIONS
****************************************************************************************************
*/

#if CO_TPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief TPDO EVENT TRIGGER VIA OBJECT
*
* \ingroup API
*
*          This function allows the application to trigger a TPDO event for the given object
*          entry. The event will be distributed to all TPDOs, which holds an active mapping
*          entry to this object entry.
*
* \param[in,out]   pdo          Pointer to start of TPDO array
*
* \param[in]       obj          Pointer to object entry
*/
/*------------------------------------------------------------------------------------------------*/
void COTPdoTrigObj (CO_TPDO *pdo, CO_OBJ *obj)
{
    CPU_INT32U n;                                     /* Local: loop counter through link map     */
    CPU_INT16U num;                                   /* Local: number of TPDO                    */
                                                      /*------------------------------------------*/
    if (CO_IS_PDOMAP(obj->Key) != 0) {                /* see, if the object is PDO mappable       */
        for (n=0; n < CO_TPDO_MAP_N; n++) {           /* loop through mapping link map            */
            if (pdo->Node->TMap[n].Obj == obj) {      /* see, if mapping object is addressed      */
                num = pdo->Node->TMap[n].Num;         /* get PDO number of linked object          */
                COTPdoTrigPdo(pdo, num);              /* trigger this PDO via PDO number          */
            }
        }
    } else {                                          /* otherwise: object is not PDO mappable    */
        pdo->Node->Error = CO_ERR_TPDO_OBJ_TRIGGER;   /* set error code                           */
    }
}
#endif

#if CO_TPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief TPDO EVENT TRIGGER VIA TPDO-NUM
*
* \ingroup API
*
*          This function allows the application to trigger a TPDO event for the given TPDO number.
*
* \param[in,out]   pdo          Pointer to start of TPDO array
*
* \param[in]       num          Number of TPDO (0..511)
*/
/*------------------------------------------------------------------------------------------------*/
void COTPdoTrigPdo (CO_TPDO *pdo, CPU_INT16U num)
{
    if (num < CO_TPDO_N) {                            /* see, if given TPDO number is valid       */
        CO_TPdoTx(&pdo[num]);                         /* transmit TPDO immediatelly               */
    } else {                                          /* otherwise: TPDO number is invalid        */
        pdo->Node->Error = CO_ERR_TPDO_NUM_TRIGGER;   /* set error code                           */
    }
}
#endif

/*
****************************************************************************************************
*                                       INTERNAL FUNCTIONS
****************************************************************************************************
*/

#if CO_TPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief TPDO CLEAR
*
* \ingroup INTERNAL
*
*          This function sets the PDO related configuration data to a known state (inactive).
*
* \param[in,out]   pdo          Pointer to start of TPDO array
*
* \param[in]       node         Pointer to parent node object
*/
/*------------------------------------------------------------------------------------------------*/
void CO_TPdoClear (CO_TPDO *pdo, CO_NODE *node)
{
    CPU_INT16U num;                                  /* Local: number of PDO                     */
    CPU_INT08U on;                                   /* Local: number of mapped objects in PDO   */
                                                     /*------------------------------------------*/
   if ((pdo == 0) || (node == 0)) {                  /* see, if argument pointers are invalid    */
       CO_NodeFatalError();                          /* inform user                              */
       return;                                       /* abort initialization                     */
   }
   CO_TPdoMapClear(node->TMap);                      /* initialize signal link table             */
   for (num = 0; num < CO_TPDO_N; num++) {           /* loop through all possible supported TPDO */
       pdo[num].Node       = node;                   /* link CANopen node informations           */
       pdo[num].EvTmr      = -1;                     /* indicate no active event timer           */
       pdo[num].InTmr      = -1;                     /* indicate no active inhibit timer         */
       pdo[num].Identifier = CO_TPDO_COBID_OFF;      /* indicate no configured message           */
       pdo[num].ObjNum     = 0;                      /* clear number of mapped TPDO objects      */
       for (on = 0; on < 8; on++) {                  /* loop through all object mappings         */
           pdo[num].Map[on] = 0;                     /* clear object mapping                     */
       }
   }
}
#endif

#if CO_TPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief TPDO INITIALIZATION
*
* \ingroup INTERNAL
*
*          This function checks the object directory and creates the PDO related
*          configuration data from the object directory settings.
*
* \param[in,out]   pdo          Pointer to start of TPDO array
*
* \param[in]       node         Pointer to parent node object
*/
/*------------------------------------------------------------------------------------------------*/
void CO_TPdoInit(CO_TPDO *pdo, CO_NODE *node)
{
    CPU_INT16S err;                                   /* Local: error code                        */
    CPU_INT16U num;                                   /* Local: number of PDO                     */
    CPU_INT08U on;                                    /* Local: number of mapped objects in PDO   */
    CPU_INT08U tnum;                                  /* LocaL: number of PDO Com Entries         */
                                                      /*------------------------------------------*/
    if ((pdo == 0) || (node == 0)) {                  /* see, if argument pointers are invalid    */
        CO_NodeFatalError();                          /* inform user                              */
        return;                                       /* abort initialization                     */
    }
    CO_TPdoMapClear(node->TMap);                      /* initialize signal link table             */
    for (num = 0; num < CO_TPDO_N; num++) {           /* loop through all possible supported TPDO */
        pdo[num].Node       = node;                   /* link CANopen node informations           */
        pdo[num].EvTmr      = -1;                     /* indicate no active event timer           */
        pdo[num].InTmr      = -1;                     /* indicate no active inhibit timer         */
        pdo[num].Identifier = CO_TPDO_COBID_OFF;      /* indicate no configured message           */
        pdo[num].ObjNum     = 0;                      /* clear number of mapped TPDO objects      */
        for (on = 0; on < 8; on++) {                  /* loop through all object mappings         */
            pdo[num].Map[on] = 0;                     /* clear object mapping                     */
        }
        err = CODirRdByte(&node->Dir,                 /* check TPDO communication settings        */
                          CO_DEV(0x1800,0),&tnum);
        if (err == CO_ERR_NONE) {                     /* TPDO Communication settings are existing */
            CO_TPdoReset(pdo, num);                   /* scan TPDO communication and mapping      */
        } else {
            node->Error = CO_ERR_NONE;                /* TPDOs are optional, remove error         */
        }
    }
}
#endif

#if CO_TPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief RESET TPDO COMMUNICATION PROFILE
*
* \ingroup INTERNAL
*
*          This function scans the object directory for the TPDO communication and mapping
*          profile. The function will fill the TPDO info structure with the corresponding
*          informations.
*
*          The following list shows the considered communication profile entries:
*          -# 0x1800+[num] : 0x01 = TPDO CAN identifier
*          -# 0x1800+[num] : 0x02 = Transmission type
*          -# 0x1800+[num] : 0x03 = Inhibit time
*          -# 0x1800+[num] : 0x05 = Event timer (for transmission type 254 and 255)
*
* \note    The object entry 0x1800+[num] : 0x04 is reserved and should be not existing. This is not
*          checked with this function.
*
* \param[in,out]   pdo          Pointer to start of TPDO array
*
* \param[in]       num          Number of TPDO (0..511)
*/
/*------------------------------------------------------------------------------------------------*/
void CO_TPdoReset(CO_TPDO *pdo, CPU_INT16U num)
{
    CO_TPDO    *wp;                                   /* Local: Pointer to working TPDO           */
    CO_DIR     *cod;                                  /* Local: Pointer to object directory       */
    CPU_INT32U  id      = CO_TPDO_COBID_OFF;          /* Local: COBID from profile                */
    CPU_INT16U  inhibit = 0;                          /* Local: inhibit time from profile [100us] */
    CPU_INT16U  timer   = 0;                          /* Local: event timer from profile [1ms]    */
    CPU_INT16S  err;                                  /* Local: function call error code          */
    CPU_INT08U  type    = 0;                          /* Local: transmission type from profile    */
                                                      /*------------------------------------------*/
    wp        = &pdo[num];                            /* set pointer to working pdo               */
    cod       = &wp->Node->Dir;                       /* set pointer to object directory          */
    if (wp->EvTmr >= 0) {                             /* see, if TPDO event timer is activated    */
        (void)COTmrDelete(&wp->Node->Tmr, wp->EvTmr); /* delete event timer                       */
        wp->EvTmr = -1;                               /* indicate no active event timer           */
    }
    if (wp->InTmr >= 0) {                             /* see, if TPDO inhibit timer is activated  */
        (void)COTmrDelete(&wp->Node->Tmr, wp->InTmr); /* delete inhibit timer                     */
        wp->InTmr = -1;                               /* indicate no active event timer           */
#if CO_SYNC_EN > 0
    }
    if ((wp->Flags & CO_TPDO_FLG_S__) != 0) {         /* see, if TPDO is synchronized             */
        CO_SyncRemove(&pdo->Node->Sync,               /* remove TPDO from SYNC management table   */
                      num,
                      CO_SYNC_FLG_TX);
#endif
    }
    wp->Flags = 0;                                    /* clear all TPDO flags                     */
                                                      /*------------------------------------------*/
                                                      /*         C O M M U N I C A T I O N        */
                                                      /*------------------------------------------*/
    err = CODirRdByte(cod,                            /* get TPDO transmission type               */
                      CO_DEV(0x1800 + num, 2),
                      &type);
    if (err != CO_ERR_NONE) {                         /* see, if an read error is detected        */
        pdo->Node->Error = CO_ERR_TPDO_COM_OBJ;       /* set error code                           */
        return;                                       /* return error indication                  */
    }                                                 /*------------------------------------------*/
    err = CODirRdWord(cod,                            /* get TPDO inhibit time                    */
                      CO_DEV(0x1800 + num, 3),
                      &inhibit);
    if (err != CO_ERR_NONE) {                         /* see, if an read error is detected        */
        err = (CPU_INT16S)CO_ERR_NONE;                /* reset error: inhibit time is optional    */
        pdo->Node->Error = CO_ERR_NONE;               /* reset error code                         */
    }
                                                      /*lint -e{644} : inhibit set in CODirRdWord */
    if (inhibit > 0) {                                /* see, if inhibit time is enabled          */
        inhibit = (inhibit + 9) / 10;                 /* convert inhibit[100us] to inhibit[1ms]   */
        if (inhibit <= CO_TPDO_TMR_MIN) {             /* see, if inhibit < granularity [ms]       */
            inhibit = CO_TPDO_TMR_MIN;                /* rise inhibit time to minimum possible    */
        } else {                                      /* otherwise: inhibit timer > min possible  */
            inhibit = CO_TPDO_ROUND(inhibit,          /* round up inhibit time to next possible   */
                      CO_TPDO_TMR_MIN);
        }
    }
    pdo[num].Inhibit = CO_TPDO_MS(inhibit);           /* convert and set TPDO inhibit[ticks]      */
                                                      /*------------------------------------------*/
                                                      /*lint -e{644} : type set in CoDirRdByte    */
    if ((type == 254) || (type == 255)) {             /* see, if transmission type = asynchronous */
        err = CODirRdWord(cod,                        /* get TPDO event timer cycle               */
                          CO_DEV(0x1800 + num, 5),
                          &timer);
        if (err != CO_ERR_NONE) {                     /* see, if an read error is detected        */
            err = (CPU_INT16S)CO_ERR_NONE;            /* reset error: event time is optional      */
            pdo->Node->Error = CO_ERR_NONE;           /* reset error code                         */
        }
        if (timer > 0) {                              /* see, if event timer is enabled           */
            if (timer <= CO_TPDO_TMR_MIN) {           /* see, if timer cylce <= min possible      */
                timer = CO_TPDO_TMR_MIN;              /* set timer cycle to minimum possible      */
            } else {                                  /* otherwise: timer cylce > min possible    */
                timer = CO_TPDO_ROUND(timer,          /* round up timer cycle to next possible    */
                            CO_TPDO_TMR_MIN);
            }
        }
#if CO_SYNC_EN == 0
    } else {                                          /* otherwise: synchronous transmission type */
        pdo->Node->Error = CO_ERR_TPDO_COM_OBJ;       /* set error code                           */
        return;                                       /* return error indication                  */
#endif
    }                                                 /*------------------------------------------*/
    err = CODirRdLong(cod,                            /* get TPDO identifier                      */
                      CO_DEV(0x1800 + num, 1),
                      &id);
    if (err != CO_ERR_NONE) {                         /* see, if an read error is detected        */
        pdo->Node->Error = CO_ERR_TPDO_COM_OBJ;       /* set error code                           */
        return;                                       /* return error indication                  */
    }
                                                      /*lint -e{644} : id set in CoDirRdLong      */
    if ((id & CO_TPDO_COBID_REMOTE) == 0) {           /* see, if PDO is configured with RTR       */
        pdo->Node->Error = CO_ERR_TPDO_COM_OBJ;       /* set error code (RTR is not supported)    */
        return;                                       /* return error indication                  */
    }
    if ((id & CO_TPDO_COBID_EXT) != 0) {              /* see, if PDO is configured with ext. ID   */
        pdo->Node->Error = CO_ERR_TPDO_COM_OBJ;       /* set error code (mixed IDs not supported) */
        return;                                       /* return error indication                  */
    }
    if ((id & CO_TPDO_COBID_OFF) == 0) {
        pdo[num].Identifier = (id & 0x1FFFFFFF);      /* set identifier in message                */
    } else {
        pdo[num].Identifier = CO_TPDO_COBID_OFF;      /* set identifier in message                */
    }                                                 /*------------------------------------------*/
                                                      /*             M A P P I N G                */
                                                      /*------------------------------------------*/
    err = CO_TPdoGetMap(pdo, num);                    /* get TPDO mapping from object directory   */
    if (err != CO_ERR_NONE) {                         /* see, if an error is detected             */
        pdo->Node->Error = CO_ERR_TPDO_MAP_OBJ;       /* set error                                */
        return;                                       /* return error indication                  */
    }                                                 /*------------------------------------------*/
    if (pdo[num].Identifier != CO_TPDO_COBID_OFF) {   /* see, if message configuration is valid   */
#if CO_SYNC_EN > 0
        if (type <= 240) {                            /* see, if transmission type is 0..240      */
            pdo[num].Flags |= CO_TPDO_FLG_S__;        /* set message type to synchronous          */
            CO_SyncAdd(&pdo->Node->Sync,              /* add TPDO to SYNC management table        */
                       num,
                       CO_SYNC_FLG_TX,
                       type);
        }
#endif
    }
    pdo[num].Event = CO_TPDO_MS(timer);               /* set TPDO event timer ticks               */
    if (pdo[num].Event > 0) {                         /* see, if event timer is enabled           */
        pdo[num].EvTmr = COTmrCreate(&pdo->Node->Tmr, /* create cyclic timer for event timer      */
                                     pdo[num].Event + num,
                                     0,
                                     CO_TPdoTmrEvent,
                                     &pdo[num]);
    }
}
#endif

#if CO_TPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief GET PDO MAPPING DATA
*
* \ingroup INTERNAL
*
*          This function gets the PDO mapping datas out of the object directory and puts the
*          pre-calculated values in the CAN message configuration.
*
*          The following list shows the considered mapping profile entries:
*          -# 0x1A00+[num] : 0x00 = Number of mapped signals (0..8)
*          -# 0x1A00+[num] : 0x01..0x08 = Mapped signal
*
* \param[in,out]   pdo          Pointer to start of TPDO array
*
* \param[in]       num          Number of TPDO (0..511)
*
* \retval  =0      Mapping successful parsed out of object directory
* \retval  <0      At least one error is detected within the mapping configuration
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CO_TPdoGetMap (CO_TPDO *pdo, CPU_INT16U num)
{
    CO_DIR      *cod;                                 /* Local: pointer to object directory       */
    CO_OBJ      *obj;                                 /* Local: pointer to object entry           */
    CPU_INT32U   mapping;                             /* Local: PDO mapping information           */
    CPU_INT16U   idx;                                 /* Local: object entry index                */
    CPU_INT16U   on;                                  /* Local: Loop counter for mapped objects   */
    CPU_INT16S   err;                                 /* Local: error of CANopen functions        */
    CPU_INT08U   mapnum;                              /* Local: number of PDO mappings            */
    CPU_INT08U   dlc;                                 /* Local: resulting data length code        */
                                                      /*------------------------------------------*/
    cod = &pdo[num].Node->Dir;                        /* get pointer to object directory          */
    idx = 0x1A00 + num;                               /* set index to TPDO mapping profile        */
    err = CODirRdByte(cod, CO_DEV(idx, 0), &mapnum);  /* get number of mapped signals             */
    if (err != CO_ERR_NONE) {                         /* see, if an error is detected             */
        return (-1);                                  /* abort with error indication              */
    }
    dlc = 0;                                          /* clear number of bytes in message         */
                                                      /*------------------------------------------*/
                                                      /*lint -e{644} : mapnum set in CODirRdByte  */
    for (on=0; on < mapnum; on++) {                   /* Loop through all mapped signals          */
        err = CODirRdLong(cod,                        /* get mapping information of this PDO      */
                          CO_DEV(idx, 1+on),
                          &mapping);
        if (err != CO_ERR_NONE) {                     /* see, if an error is detected             */
            return (-1);                              /* abort with error indication              */
        }
                                                      /*lint -e{644} : mapping set in CODirRdLong */
        dlc += (CPU_INT08U)(mapping & 0xFF) >> 3;     /* add number of bits, converted to bytes   */
        if (dlc > 8){                                 /* more than 8 byte not possible in a PDO   */
            return (-1);                              /* abort with error indication              */
        }
        obj = CODirFind(&pdo->Node->Dir, mapping);    /* get mapping information this PDO         */
        if (obj == 0) {                               /* see, if object entry is not found        */
            return (-1);                              /* abort with error indication              */
        } else {
            pdo[num].Map[on] = obj;                   /* add object pointer to TPDO map           */
            CO_TPdoMapAdd(pdo->Node->TMap, obj, num); /* add signal to TPDO number into link map  */
        }
    }
    pdo[num].ObjNum = mapnum;                         /* set number of TPDO object references     */
                                                      /*------------------------------------------*/
    return (0);
}
#endif

#if CO_TPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief TPDO TRIGGER EVENT TIMER CALLBACK
*
* \ingroup INTERNAL
*
*          This function is used to trigger a PDO with the event timer object (see subindex 5
*          of communication parameters).
*
* \param[in]       parg         Pointer to TPDO element
*/
/*------------------------------------------------------------------------------------------------*/
void CO_TPdoTmrEvent (void *parg)
{
    CO_TPDO *pdo;                                     /* Local: pointer to TPDO object            */
                                                      /*------------------------------------------*/
    pdo = (CO_TPDO *)parg;                            /* get pointer to TPDO                      */
    CO_TPdoTx(pdo);                                   /* transmit TPDO                            */
}
#endif

#if CO_TPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief TPDO END OF INHIBIT TIMER CALLBACK
*
* \ingroup INTERNAL
*
*          This function is used to release the transmission stop after the PDO inhibit time.
*
* \param[in]       parg         Pointer to TPDO element
*/
/*------------------------------------------------------------------------------------------------*/
void CO_TPdoEndInhibit (void *parg)
{
    CO_TPDO *pdo;                                     /* Local: pointer to TPDO object            */
                                                      /*------------------------------------------*/
    pdo = (CO_TPDO *)parg;                            /* get pointer to TPDO                      */
    pdo->Flags &= ~CO_TPDO_FLG__I_;                   /* clear inhibit time indication            */
    pdo->InTmr  = -1;                                 /* clear inhibit timer id                   */
    if ((pdo->Flags & CO_TPDO_FLG___E) != 0) {        /* see, if event occurs within inhibit time */
        pdo->Flags &= ~CO_TPDO_FLG___E;               /* clear event indication                   */
        CO_TPdoTx(pdo);                               /* yes: transmit TPDO                       */
    }
}
#endif

#if CO_TPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief TPDO TRANSMIT
*
* \ingroup INTERNAL
*
*          This function is responsible for the transmission of a TPDO.
*
*          If the inhibit time is enabled, further transmission will be disabled - and a one-shot
*          timer 'end of inhibit time' callback function is created.
*
* \param[in]       pdo          Pointer to TPDO element
*/
/*------------------------------------------------------------------------------------------------*/
void CO_TPdoTx (CO_TPDO *pdo)
{
    CO_IF_FRM   frm;                                  /* Local: memory for one CAN frame          */
    CPU_INT32U  sz;                                   /* Local: size of linked object             */
    CPU_INT32U  data;                                 /* Local: data buffer for TPDO massage      */
    CPU_INT08U  num;                                  /* Local: number of linked object           */
                                                      /*------------------------------------------*/
    if ( (pdo->Node->Nmt.Allowed &                    /* see, if PDO communication is not allowed */
          CO_PDO_ALLOWED           ) == 0) {
        return;                                       /* abort transmission of PDO                */
    }
    if ( (pdo->Flags & CO_TPDO_FLG__I_) != 0) {       /* see, if inhibit time is active           */
        pdo->Flags |= CO_TPDO_FLG___E;                /* remember transmission request            */
        return;                                       /* abort transmission of PDO                */
    }                                                 /*------------------------------------------*/
    if (pdo->EvTmr >= 0) {                            /* see, if cyclic event timer is active     */
        (void)COTmrDelete(&pdo->Node->Tmr,            /* yes: delete timer                        */
                          pdo->EvTmr);
                                                      /* possible errors registered in node       */
        pdo->EvTmr = -1;                              /* indicate deactivated event timer         */
    }
    if (pdo->Inhibit > 0) {                           /* see, if inhibit time is enabled          */
        pdo->InTmr = COTmrCreate(&pdo->Node->Tmr,     /* create one-shot timer for end of inhibit */
                                 pdo->Inhibit,
                                 0,
                                 CO_TPdoEndInhibit,
                                 (void*)pdo);
        if (pdo->InTmr< 0) {                          /* see, if no timer was available           */
            pdo->Node->Error = CO_ERR_TPDO_INHIBIT;   /* set error code                           */
        } else {
            pdo->Flags |= CO_TPDO_FLG__I_;            /* yes: stop further transmission of PDO    */
        }
    }
    if (pdo->Event > 0) {
        pdo->EvTmr = COTmrCreate(&pdo->Node->Tmr,     /* create one-shot timer for end of inhibit */
                               pdo->Event,
                               0,
                               CO_TPdoTmrEvent,
                               (void*)pdo);
        if (pdo->EvTmr < 0) {                         /* see, if no timer was available           */
            pdo->Node->Error = CO_ERR_TPDO_EVENT;     /* set error code                           */
        }
    }                                                 /*------------------------------------------*/
    frm.Identifier = pdo->Identifier;                 /* set message identifier                   */
    frm.DLC        = 0;                               /* clear DLC of TPDO message                */
    for (num = 0; num < pdo->ObjNum; num++) {         /* loop through all PDO objects             */
        sz = COObjGetSize(pdo->Map[num], 0L);         /* get size of PDO object                   */
        if (sz <= (CPU_INT32U)(8 - frm.DLC)) {        /* see, if size is possible for TPDOs       */
            (void)COObjRdValue(pdo->Map[num],         /* read object value                        */
                               &data,
                               CO_LONG,
                               pdo->Node->NodeId);

                                                      /*lint -e{644} : data set in CODirRdValue   */
            if (sz == CO_BYTE) {                      /* see, if object holds a byte value        */
                CO_SET_BYTE(&frm, data, frm.DLC);     /* copy byte value to can frame             */
            } else if (sz == CO_WORD) {               /* see, if object holds a word value        */
                CO_SET_WORD(&frm, data, frm.DLC);     /* copy word value to can frame             */
            } else if (sz == CO_LONG) {               /* see, if object holds a long value        */
                CO_SET_LONG(&frm, data, frm.DLC);     /* copy long value to can frame             */
            }                                         /*------------------------------------------*/
            frm.DLC += (CPU_INT08U)sz;                /* add size to DLC of message               */
        }
    }                                                 /*------------------------------------------*/
#if CO_CB_TPDO_TRANSMIT_EN > 0
    CO_TPdoTransmit(&frm);                            /* Application callback function            */
#endif

    (void)COIfSend(&pdo->Node->If, &frm);             /* Sent message via can bus                 */
}
#endif

#if CO_TPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief TPDO CLEAR LINK MAP
*
* \ingroup INTERNAL
*
*          This function is used to initialize the signal to TPDO link mapping table.
*
* \param[in,out]   map          Pointer to start of link mapping table
*/
/*------------------------------------------------------------------------------------------------*/
void CO_TPdoMapClear(CO_TPDO_LINK *map)
{
    CPU_INT16U id;                                    /* Local: loop counter through link map     */
                                                      /*------------------------------------------*/
    for (id = 0; id < CO_TPDO_MAP_N; id++) {          /* loop through all link map entries        */
        map[id].Obj  = 0;                             /* clear signal identifier                  */
        map[id].Num  = 0xFFFF;                        /* clear linked TPDO number                 */
    }
}
#endif

#if CO_TPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief TPDO LINK MAP ADD
*
* \ingroup INTERNAL
*
*          This function is used to add an entry into the signal to TPDO link mapping table.
*
* \param[in,out]   map          Pointer to start of link mapping table
*
* \param[in]       obj          Pointer to object entry
*
* \param[in]       num          Linked TPDO number
*/
/*------------------------------------------------------------------------------------------------*/
void CO_TPdoMapAdd(CO_TPDO_LINK *map, CO_OBJ *obj, CPU_INT16U num)
{
    CPU_INT16U id;                                    /* Local: loop counter through link map     */
                                                      /*------------------------------------------*/
    for (id = 0; id < CO_TPDO_MAP_N; id++) {          /* loop through all link map entries        */
        if (map[id].Obj == 0) {                       /* see, if link map entry is unused         */
            map[id].Obj = obj;                        /* yes: set object pointer                  */
            map[id].Num = num;                        /* yes: linked TPDO number                  */
            break;                                    /* finished; exit function                  */
        }
    }
}
#endif

#if CO_TPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief TPDO LINK MAP DEL VIA TPDO-NUM
*
* \ingroup INTERNAL
*
*          This function is used to delete all entries, which contains the given TPDO number.
*
* \param[in,out]   map          Pointer to start of link mapping table
*
* \param[in]       num          Linked TPDO number
*/
/*------------------------------------------------------------------------------------------------*/
void CO_TPdoMapDelNum(CO_TPDO_LINK *map, CPU_INT16U num)
{
    CPU_INT16U id;                                    /* Local: loop counter through link map     */
                                                      /*------------------------------------------*/
    for (id = 0; id < CO_TPDO_MAP_N; id++) {          /* loop through all link map entries        */
        if (map[id].Num == num) {                     /* see, if TPDO number matches given value  */
            map[id].Obj = 0;                          /* clear signal identifier                  */
            map[id].Num = 0xFFFF;                     /* clear linked TPDO number                 */
        }
    }
}
#endif

#if CO_TPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief TPDO LINK MAP DEL VIA SIGNAL-ID
*
* \ingroup INTERNAL
*
*          This function is used to delete all entries, which contains the given signal identifier.
*
* \param[in,out]   map          Pointer to start of link mapping table
*
* \param[in]       obj          Pointer to object entry
*/
/*------------------------------------------------------------------------------------------------*/
void CO_TPdoMapDelSig(CO_TPDO_LINK *map, CO_OBJ *obj)
{
    CPU_INT16U id;                                    /* Local: loop counter through link map     */
                                                      /*------------------------------------------*/
    for (id = 0; id < CO_TPDO_MAP_N; id++) {          /* loop through all link map entries        */
        if (map[id].Obj == obj) {                     /* see, if signal id matches given value    */
            map[id].Obj = 0;                          /* clear signal identifier                  */
            map[id].Num = 0xFFFF;                     /* clear linked TPDO number                 */
        }
    }
}
#endif

/*
****************************************************************************************************
*                                     LOCAL FUNCTION PROTOTYPES
****************************************************************************************************
*/

#if CO_TPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief TPDO OBJECT ACCESS CONTROL
*
* \ingroup INTERNAL
*
*          This function is used to trigger all mapped TPDOs on change of the corresponding
*          object entry value.
*
* \param[in]       obj          Pointer to object entry
*
* \param[in]       func         Control function code (must be \ref CO_TPDO_ASYNC)
*
* \param[in]       para         unused (should be 0)
*
* \retval  =0      Successful TPDO event triggered
* \retval  <0      Function aborted due to detected error
*/
/*------------------------------------------------------------------------------------------------*/
static CPU_INT16S CO_TAsyncCtrl (CO_OBJ* obj, CPU_INT16U func, CPU_INT32U para)
{
    CO_NODE    *node;                                 /* Local: pointer to parent node            */
    CPU_INT16S  result = -1;                          /* Local: function result                   */
                                                      /*------------------------------------------*/
    (void)para;                                       /* unused; prevent compiler warning         */

    if (func == CO_TPDO_ASYNC) {                      /* see, if correct function code is given   */
        node = obj->Type->Dir->Node;
        COTPdoTrigObj(node->TPdo, obj);               /* trigger TPDO via object reference        */
        result = 0;                                   /* indicate successful operation            */
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}
#endif

#if CO_TPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief TPDO EVENT TIMER OBJECT WRITE ACCESS
*
* \ingroup INTERNAL
*
*          This function is responsible for performing all neccessary activities during a write
*          access of the TPDO event timer communication object entry.
*
* \param[in,out]   obj         Ptr to addressed parameter object entry
*
* \param[in]       buf         Ptr to data buffer
*
* \param[in]       size        Size of given data in buffer
*
* \retval  =0      writing successful
* \retval  <0      error is detected and function aborted
*/
/*------------------------------------------------------------------------------------------------*/
static CPU_INT16S CO_TEventWrite(CO_OBJ* obj, void *buf, CPU_INT32U size)
{
    CO_DIR     *cod;                                  /* Local: pointer to object directory       */
    CO_NMT     *nmt;                                  /* Local: pointer to NMT management         */
    CO_TPDO    *pdo;                                  /* Local: pointer to TPDO data              */
    CPU_INT32U  cobid  =  0;                          /* Local: CAN Identifier                    */
    CPU_INT16U  cycTime;                              /* Local: new cycle timer                   */
    CPU_INT16U  num;                                  /* Local: number of TPDO                    */
    CPU_INT16S  err;                                  /* Local: function error codes              */
    CPU_INT16S  result = -1;                          /* Local: function result                   */
                                                      /*------------------------------------------*/
    err = CO_ObjWrDirect(obj,                         /* write value to object data element       */
                 (void *)buf,
                 size);
    if (err != CO_ERR_NONE) {                         /* see, if an error is detected             */
        return (-1);                                  /* abort function with error indication     */
    }
    cod     = obj->Type->Dir;                         /* get pointer to object directory          */
    nmt     = &cod->Node->Nmt;                        /* get pointer to NMT management            */
    num     = CO_GET_IDX(obj->Key);                   /* get addressed TPDO index                 */
    num    &= 0x1FF;                                  /* extract TPDO number out of index         */
    pdo     = &cod->Node->TPdo[num];                  /* get addressed TPDO data structure        */
    if (nmt->Mode != CO_OPERATIONAL) {                /* see, if not in OPERATIONAL state         */
        return (err);                                 /* finish with success; no special handling */
    }

    cycTime = (CPU_INT16U)(*(CPU_INT32U *)buf);       /* get new cycle time for event timer       */
    if (cycTime > 0) {                                /* see, if event timer is enabled           */
        if (cycTime <= CO_TPDO_TMR_MIN) {             /* see, if timer cylce <= min possible      */
            cycTime = CO_TPDO_TMR_MIN;                /* set timer cycle to minimum possible      */
        } else {                                      /* otherwise: timer cylce > min possible    */
            cycTime = CO_TPDO_ROUND(cycTime,          /* round up timer cycle to next possible    */
                                    CO_TPDO_TMR_MIN);
        }
    }
    pdo->Event = CO_TPDO_MS(cycTime);                 /* set TPDO event timer ticks               */
    if (pdo->EvTmr >= 0) {                            /* see, if a timer is active                */
        result = COTmrDelete(&pdo->Node->Tmr,         /* shutdown PDO timer                       */
                             pdo->EvTmr);
        if (result < 0) {                             /* shutdown failed                          */
            pdo->Node->Error = CO_ERR_TMR_DELETE;     /* set error                                */
        }                                             /*------------------------------------------*/
    }
    if (pdo->InTmr >= 0) {                            /* see, if a timer is active                */
        result = COTmrDelete(&pdo->Node->Tmr,         /* shutdown PDO timer                       */
                             pdo->InTmr);
        if (result < 0) {                             /* shutdown failed                          */
            pdo->Node->Error = CO_ERR_TMR_DELETE;     /* set error                                */
        }                                             /*------------------------------------------*/
    }

    (void)CODirRdLong(cod,                            /* get TPDO identifier                      */
                      CO_DEV(0x1800+num, 1),
                      &cobid);
                                                      /*lint -e{644} : cobid set in CODirRdLong   */
    if (((cobid & CO_TPDO_COBID_OFF) == 0) &&         /* see, if TPDO is enabled                  */
        (pdo->Event > 0)                   &&         /*   and event timer is enabled             */
        (nmt->Mode == CO_OPERATIONAL)) {              /*   and node NMT mode is OPERATIONAL       */
        pdo->EvTmr = COTmrCreate(&pdo->Node->Tmr,     /* Create TPDO timer                        */
                                 pdo->Event,
                                 0,
                                 CO_TPdoTmrEvent,
                                 (void*)pdo);
    }                                                 /*------------------------------------------*/
    return (err);                                     /* return error                             */
}
#endif
