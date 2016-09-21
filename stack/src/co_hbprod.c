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
* \file     co_hbpro.c
*
* \brief    HEARTBEAT PRODUCER
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_hbprod.c#2 $
*
*           This source file implements the CANopen heartbeat producer handling.
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

static int16_t CO_TNmtHbProdWrite  (CO_OBJ *obj, void *buf, uint32_t size);

/*
****************************************************************************************************
*                                            GOBAL DATA
****************************************************************************************************
*/

/*
****************************************************************************************************
*                                       GLOBAL TYPE STRUCTURE
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief OBJECT TYPE HEARTBEAT PRODUCER
*
* \ingroup NMT
*
*          This object type specializes the general handling of objects for the object directory
*          entry 0x1017. This entries is designed to provide the heartbeat producer cycle time.
*/
/*------------------------------------------------------------------------------------------------*/
CO_OBJ_TYPE COTNmtHbProd = { 0, 0, 0, 0, 0, CO_TNmtHbProdWrite };

/*
****************************************************************************************************
*                                       INTERNAL FUNCTIONS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief  HEARTBEAT PRODUCER INITIALIZATION
*
* \ingroup  INTERNAL
*
*           This function initializes the CANopen heartbeat producer.
*
* \param[in,out]   nmt             reference to NMT structure
*
* \param[in]       node            pointer to parent node structure
*/
/*------------------------------------------------------------------------------------------------*/
void CO_NmtHbProdInit (CO_NMT *nmt)
{
    CO_NODE    *node;
    int16_t  err;                                  /* Local: error variable                    */
    uint16_t  cycTime;                              /* Local: heartbeat producer cycle time     */
                                                      /*------------------------------------------*/
    if (nmt == 0) {                                   /* see, if any ptr parameters are invalid   */
        CO_NodeFatalError();                          /* inform user                              */
        return;                                       /* abort further initializations            */
    }
    node = nmt->Node;
    err = CODirRdWord(&node->Dir,                     /* read heartbeat producer time             */
                      CO_DEV(0x1017,0),
                      &cycTime);
    if (err != CO_ERR_NONE) {                         /* see, if an error is detected             */
        node->Error = CO_ERR_CFG_1017_0;              /* set error code                           */
    }
    if (nmt->Tmr >= 0) {                              /* see, if an heartbeat timer is running    */
        err = COTmrDelete(&node->Tmr, nmt->Tmr);      /* delete heartbeat timer                   */
        if (err < 0) {                                /* see, if an error is detected             */
            node->Error = CO_ERR_TMR_DELETE;          /* set error code                           */
        }
    }
                                                      /*lint -e{644} : cycTime set in CODirRdWord */
    if (cycTime > 0) {                                /* if cycle time is valid                   */
        nmt->Tmr = COTmrCreate(&node->Tmr,            /* create heartbeat timer                   */
                               CO_TMR_TICKS(cycTime),
                               CO_TMR_TICKS(cycTime),
                               CO_NmtHbProdSend,
                               nmt);
        if (nmt->Tmr < 0) {                           /* see, if an error is detected             */
            node->Error = CO_ERR_TMR_CREATE;          /* set error indication                     */
        }
    } else {                                          /* otherwise: heartbeat is disabled         */
        nmt->Tmr = -1;                                /* set heartbeat timer to invalid value     */
    }
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  HEARTBEAT PROTOCOL
*
* \ingroup  INTERNAL
*
*           This function is a (Timerevent-)callback function. The heartbeat message will be
*           generated and sent to the configured CAN bus.
*
* \param[in]       parg             reference to NMT structure
*
* \ds301    9.2.6.2.2
*/
/*------------------------------------------------------------------------------------------------*/
void CO_NmtHbProdSend(void *parg)
{
    CO_IF_FRM    frm;                                 /* Local: heartbeat CAN frame               */
    CO_NMT      *nmt;                                 /* LocaL: reference to NMT strucutre        */
    uint8_t   state;                               /* Local: NMT mode in heartbeat encoding    */
                                                      /*------------------------------------------*/
    nmt = (CO_NMT *)parg;                             /* get reference to the NMT structure       */
    if ((nmt->Allowed & CO_NMT_ALLOWED) == 0) {       /* see, if NMT messages are not allowed     */
        return;                                       /* exit function without heartbeat message  */
    }

    state = CONmtModeEncode(nmt->Mode);               /* encode current NMT mode                  */
                                                      /*------------------------------------------*/
    CO_SET_COBID(&frm, 1792 + nmt->Node->NodeId);     /* set heartbeat CAN identifier             */
    CO_SET_DLC  (&frm, 1);                            /* set fixed DLC (1 byte)                   */
    CO_SET_BYTE (&frm, state, 0);                     /* set data byte #0 to NMT mode encoding    */
                                                      /*------------------------------------------*/
    (void)COIfSend(&nmt->Node->If, &frm);             /* send heartbeat message                   */
                                                      /* possible error registered in node        */
}

/*
****************************************************************************************************
*                                           LOCAL FUNCTIONS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief  WRITE HEARTBEAT PRODUCER TIME
*
* \ingroup  INTERNAL
*
*           This function is a typed object write function, special for object entry 1017.
*           This entry defines the refresh rate of the heartbeat message.
*
* \param[in,out]   obj               ptr to accessed object entry
*
* \param[in]       buf               ptr to write value
*
* \param[in]       size              size of write value
*
* \retval   >0     heartbeat cycle time write successful
* \retval   <=0    an error is detected and function aborted
*/
/*------------------------------------------------------------------------------------------------*/
static int16_t CO_TNmtHbProdWrite (CO_OBJ* obj, void *buf, uint32_t size)
{
    CO_NODE    *node;                                 /* Local: pointer to parent node            */
    uint16_t  cycTime;                              /* Local: cycle time                        */
    int16_t  result = CO_ERR_OBJ_ACC;              /* Local: function result                   */
                                                      /*------------------------------------------*/
    if (size != CO_LONG) {                            /* see, if size is incorrect                */
        return (CO_ERR_BAD_ARG);                      /* abort function with error indication     */
    }
    if (CO_GET_IDX(obj->Key) != 0x1017) {             /* see, if wrong object entry is accessed   */
        return (CO_ERR_CFG_1017_0);                   /* abort function with error indication     */
    }
    cycTime = (uint16_t)(*(uint32_t *)buf);       /* get cycle time from written value        */
    node    = obj->Type->Dir->Node;                   /* get parent node                          */
                                                      /*------------------------------------------*/
    if (node->Nmt.Tmr >= 0) {                         /* if timer active                          */
        result = COTmrDelete(&node->Tmr,              /* shutdown heartbeat timer                 */
                              node->Nmt.Tmr);
        if (result < 0) {                             /* shutdown failed                          */
            node->Error = CO_ERR_TMR_DELETE;          /* set error                                */
            return (CO_ERR_TMR_DELETE);               /* abort function with error indication     */
        }
        node->Nmt.Tmr = -1;                           /* clear timer identifier                   */
    }
    if (cycTime > 0) {                                /* see, if new cycle time is valid          */
        node->Nmt.Tmr = COTmrCreate(&node->Tmr,       /* create heartbeat timer                   */
                                     CO_TMR_TICKS(cycTime),
                                     CO_TMR_TICKS(cycTime),
                                     CO_NmtHbProdSend,
                                    &node->Nmt);
        if (node->Nmt.Tmr < 0) {                      /* see, if an error is detected             */
            node->Error = CO_ERR_TMR_CREATE;          /* set error indication                     */
        }
    } else {                                          /* otherwise: heartbeat is disabled         */
        node->Nmt.Tmr = -1;                           /* set heartbeat timer to invalid value     */
    }

    result = CO_ObjWrDirect(obj,                      /* write value to object data element       */
                            (void *)&cycTime,
                            CO_WORD);
    return (result);                                  /* return function result                   */
}
