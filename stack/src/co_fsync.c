/*
****************************************************************************************************
* (c) copyright by
*     Embedded Office GmbH & Co. KG       Tel : (07522) 97 00 08-0
*     August-Braun-Str. 1                 Fax : (07522) 97 00 08-99
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
* \file     co_fsync.c
*
* \brief    SYNC HANDLING
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_fsync.c#2 $
*
*           This source file implements the SYNC handling.
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
*                                        INTERNAL FUNCTIONS
****************************************************************************************************
*/

#if CO_SYNC_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief SYNC INITIALIZATION
*
* \ingroup INTERNAL
*
*          This function reads and caches the synchronisation object.
*
* \param[in,out]   sync         Pointer to sync object
*
* \param[in]       node         Pointer to parent node
*/
/*------------------------------------------------------------------------------------------------*/
void CO_SyncInit(CO_SYNC *sync, struct CO_NODE_T *node)
{
    CPU_INT16S err;                                   /* Local: error code                        */
    CPU_INT08U   i;                                   /* Local: loop counter                      */
                                                      /*------------------------------------------*/
    if ((sync == 0) || (node == 0)) {                 /* see, if one of the parameters are bad    */
        CO_NodeFatalError();                          /* yes: inform user                         */
        return;                                       /* abort sync initialization                */
    }
    sync->Node = node;                                /* link to parent node                      */
#if CO_TPDO_N > 0
    for (i = 0; i < CO_TPDO_N; i++) {                 /* loop through TPDO table                  */
        sync->TSync[i] = 0;                           /* clear transmission time                  */
        sync->TPdo[i]  = (CO_TPDO *)0;                /* invalidate pointer to TPDO               */
        sync->TNum[i]  = 0;                           /* clear number of SYNC till sending        */
    }
#endif
#if CO_RPDO_N > 0
    for (i = 0; i < CO_RPDO_N; i++) {                 /* clear through RPDO table                 */
        sync->RPdo[i]  = (CO_RPDO *)0;                /* invalidate pointer to RPDO               */
    }                                                 /*------------------------------------------*/
#endif
    err = CODirRdLong(&node->Dir,                     /* get sync identifier in object directory  */
                      CO_DEV(0x1005, 0),
                      &sync->CobId);
    if (err != CO_ERR_NONE) {                         /* see, if an error is detected             */
        node->Error = CO_ERR_CFG_1005_0;              /* set error                                */
        sync->CobId = 0;                              /* clear sync id                            */
    }                                                 /*------------------------------------------*/
}
#endif

#if CO_SYNC_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief SYNC MESSAGE HANDLER
*
* \ingroup INTERNAL
*
*          This function is called on receiving a SYNC message and handles the synchronous
*          PDOs as defined in the object directory.
*
* \param[in]       sync         Pointer to SYNC object
*/
/*------------------------------------------------------------------------------------------------*/
void CO_SyncHandler (CO_SYNC *sync)
{
    CPU_INT08U  i;                                    /* Local: loop counter                      */
                                                      /*------------------------------------------*/
#if CO_TPDO_N > 0
    for (i = 0; i < CO_TPDO_N; i++) {                 /* loop through transmit PDO table          */
        if (sync->TPdo[i] != 0) {                     /* see, if TPDO is synchronous              */
            if (sync->TNum[i] == 0) {                 /* see, if transmission type is 0           */
                CO_TPdoTx(sync->TPdo[i]);             /* send TPDO every SYNC                     */
            } else {
                if (sync->TSync[i] == sync->TNum[i]) {/* otherwise: see, if sync time fits        */
                    CO_TPdoTx(sync->TPdo[i]);         /* send TPDO at matching SYNC               */
                    sync->TSync[i] = 0;               /* reset sync time                          */
                }
            }
        }
    }
#endif
                                                      /*------------------------------------------*/
#if CO_RPDO_N > 0
    for (i = 0; i < CO_RPDO_N; i++) {                 /* loop through receive PDO table           */
        if (sync->RPdo[i] != 0) {                     /* see, if RPDO is synchronous              */
            CO_RPdoWrite(sync->RPdo[i],
                         &sync->RFrm[i]);
        }
    }
#endif
}
#endif

#if CO_SYNC_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief ADD SYNCHRONOUS PDO
*
* \ingroup INTERNAL
*
*          This function adds a PDO to the synchronous PDO tables.
*
* \param[in,out]   sync         Pointer to SYNC object
*
* \param[in]       num          Number of PDO
*
* \param[in]       msgType      CAN message type
*
* \param[in]       txtype       PDO transmission type
*/
/*------------------------------------------------------------------------------------------------*/
void CO_SyncAdd (CO_SYNC *sync, CPU_INT16U num, CPU_INT08U msgType, CPU_INT08U txtype)
{
#if CO_TPDO_N > 0                                     /*------------------------------------------*/
    if (msgType == CO_SYNC_FLG_TX) {                  /*     S Y N C H R O N O U S   T P D O      */
                                                      /*------------------------------------------*/
        if (sync->TPdo[num] == 0) {                   /* see, if PDO is not configured            */
            sync->TPdo[num] = &sync->Node->TPdo[num]; /* link TPDO to synchronous object          */
        }
        sync->TNum[num]  = txtype;                    /* set number of SYNC till send             */
        sync->TSync[num] = 0;                         /* reset SYNC counter                       */
    }
#endif

#if CO_RPDO_N > 0                                     /*------------------------------------------*/
    if (msgType == CO_SYNC_FLG_RX) {                  /*     S Y N C H R O N O U S   R P D O      */
                                                      /*------------------------------------------*/
        if (sync->RPdo[num] == 0) {                   /* see, if PDO is not configured            */
            sync->RPdo[num] = &sync->Node->RPdo[num]; /* link RPDO to synchronous object          */
        }
    }
#endif
}
#endif

#if CO_SYNC_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief REMOVE SYNCHRONOUS PDO
*
* \ingroup INTERNAL
*
*          This function removes a SYNC PDO from the SYNC PDO table.
*
* \param[in,out]   sync         Pointer to SYNC object
*
* \param[in]       num          Number of PDO
*
* \param[in]       msgType      CAN message type
*/
/*------------------------------------------------------------------------------------------------*/
void CO_SyncRemove (CO_SYNC *sync, CPU_INT16U num, CPU_INT08U msgType)
{
#if CO_TPDO_N > 0                                     /*------------------------------------------*/
    if (msgType == CO_SYNC_FLG_TX) {                  /*     S Y N C H R O N O U S   T P D O      */
                                                      /*------------------------------------------*/
        sync->TPdo[num]  = 0;                         /* indicate PDO is not synchronous          */
        sync->TNum[num]  = 0;                         /* reset number of SYNC till transmission   */
        sync->TSync[num] = 0;                         /* reset SYNC counter                       */
    }
#endif

#if CO_RPDO_N > 0                                     /*------------------------------------------*/
    if (msgType == CO_SYNC_FLG_RX) {                  /*     S Y N C H R O N O U S   R P D O      */
                                                      /*------------------------------------------*/
        sync->RPdo[num]  = 0;                         /* clear pointer to received frame          */
    }
#endif
}
#endif

#if CO_SYNC_EN > 0 && CO_RPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief RECEIVE SYNCHRONOUS PDO
*
* \ingroup INTERNAL
*
*          This function handles a received synchronous RPDO
*
* \param[in,out]   sync         Pointer to SYNC object
*
* \param[in]       frm          CAN Frame, received from CAN bus
*/
/*------------------------------------------------------------------------------------------------*/
void CO_SyncRx (CO_SYNC *sync, CO_IF_FRM *frm)
{
    CPU_INT16S i;                                     /* Local: counter variable                  */
    CPU_INT16S n;                                     /* Local: counter variable                  */
                                                      /*------------------------------------------*/
    for (i = 0; i < CO_RPDO_N; i++) {                 /* loop through the RPDO table              */
        if (sync->RPdo[i]->Identifier ==              /* see, if message matches RPDO             */
            frm->Identifier) {
            for (n=0; n < 8; n++) {                   /* yes: cpy payload of CAN message in cache */
                sync->RFrm[i].Data[n] = frm->Data[n]; /* copy data byte                           */
            }
            sync->RFrm[i].DLC = frm->DLC;             /* copy DLC of message                      */
            break;
        }
    }
}
#endif

#if CO_SYNC_EN > 0 && CO_TPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief UPDATE SYNC MANAGEMENT TABLES
*
* \ingroup INTERNAL
*
*          This function checks the given frame to be a SYNC message.
*
* \param[in,out]   sync         Pointer to sync object
*
* \param[in]       frm          CAN Frame, received from CAN bus
*
* \retval  =0      CAN message frame is a SYNC message
* \retval  <0      the given CAN message is no SYNC message
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CO_SyncUpdate (CO_SYNC *sync, CO_IF_FRM *frm)
{
    CPU_INT16S result = -1;                           /* Local: function result                   */
    CPU_INT08U i;                                     /* Local: loop counter                      */
                                                      /*------------------------------------------*/
    if (frm->Identifier == sync->CobId) {             /* see, if SYNC message is received         */
        for (i = 0; i < CO_TPDO_N; i++) {             /* loop through synchronous transmit PDOs   */
            if (sync->TPdo[i] != 0) {                 /* see, if TPDO is synchronous              */
                sync->TSync[i]++;                     /* increment SYNC counter for this TPDO     */
            }
        }
        result = 0;                                   /* indicate SNYC message detected/consumed  */
    }
    return (result);                                  /* return function result                   */
}
#endif

#if CO_SYNC_EN > 0 && CO_TPDO_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief RESTART SYNC TIMING
*
* \ingroup INTERNAL
*
*          This function is used to restart SYNC. It's called on NMT Start Operational and resets
*          the SYNC time counter.
*
* \param[in,out]   sync         Pointer to SYNC object
*/
/*------------------------------------------------------------------------------------------------*/
void CO_SyncRestart (CO_SYNC *sync)
{
    CPU_INT08U i;                                     /* Local: loop counter                      */
                                                      /*------------------------------------------*/
    for (i = 0; i < CO_TPDO_N; i++) {                 /* loop through TPDO table                  */
        if (sync->TPdo[i] != 0) {                     /* see, if TPDO is a synchronous PDO        */
            sync->TSync[i] = 0;                       /* reset received SYNC counter              */
        }
    }
}
#endif
