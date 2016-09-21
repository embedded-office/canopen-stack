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
* \file     co_hbcons.c
*
* \brief    HEARTBEAT CONSUMER
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_hbcons.c#2 $
*
*           This source file implements the CANopen heartbeat consumer handling.
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

static void       CO_NmtHbConsMonitor (void *parg);

static int16_t CO_TNmtHbConsWrite  (CO_OBJ *obj, void *buf, uint32_t size);
static int16_t CO_TNmtHbConsRead   (CO_OBJ *obj, void *buf, uint32_t len);

/*
****************************************************************************************************
*                                       GLOBAL TYPE STRUCTURE
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief OBJECT TYPE HEARTBEAT CONSUMER
*
* \ingroup NMT
*
*          This object type specializes the general handling of objects for the object directory
*          entry 0x1016. This entries is designed to provide the heartbeat consumer monitor times.
*/
/*------------------------------------------------------------------------------------------------*/
CO_OBJ_TYPE COTNmtHbCons = { 0, 0, 0, 0, CO_TNmtHbConsRead, CO_TNmtHbConsWrite };

/*
****************************************************************************************************
*                                           FUNCTIONS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief  GET NUMBER OF HEARTBEAT EVENTS
*
* \ingroup  API
*
*           This function retuns the number of heartbeat (miss-)events, which are detected since
*           the last call of this function or initializing the node.
*
* \param[in,out]   nmt             reference to NMT structure
*
* \param[in]       nodeId          node ID of monitored node (or 0 for master node)
*
* \retval          >=0             number of detected heartbeat events for given node ID
* \retval           <0             error detected (e.g. node ID is not monitored)
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CONmtGetHbEvents (CO_NMT *nmt, uint8_t nodeId)
{
    int16_t  result = -1;                          /* Local: function result                   */
    CO_HBCONS  *hbc;                                  /* Local: heartbeat consumer                */
                                                      /*------------------------------------------*/
    if (nmt == 0) {                                   /* see, if any ptr parameters are invalid   */
        CO_NodeFatalError();                          /* inform user                              */
        return (result);                              /* abort further initializations            */
    }

    hbc = nmt->HbCons;                                /* get first element in heartbeat chain     */
    while (hbc != 0) {                                /* loop through active heartbeat chain      */
        if (nodeId == hbc->NodeId) {                  /* see, if nodeId matches consumer          */
            result     = (int16_t)hbc->Event;      /* yes: add event of given nodeId           */
            hbc->Event = 0;                           /* clear event counter                      */
        }
        hbc = hbc->Next;                              /* switch to next heartbeat consumer        */
    }
                                                      /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  GET LAST RECEIVED HEARTBEAT STATE
*
* \ingroup  API
*
*           This function returns the last received heartbeat state of a given node.
*
* \param[in,out]   nmt             reference to NMT structure
*
* \param[in]       nodeId          node ID of monitored node
*
* \retval      !=CO_INVALID        last detected heartbeat state for given node ID
* \retval       =CO_INVALID        error detected (e.g. node ID is not monitored)
*/
/*------------------------------------------------------------------------------------------------*/
CO_MODE CONmtLastHbState (CO_NMT *nmt, uint8_t nodeId)
{
    CO_MODE     result = CO_INVALID;                  /* Local: function result                   */
    CO_HBCONS  *hbc;                                  /* Local: heartbeat consumer                */
                                                      /*------------------------------------------*/
    if (nmt == 0) {                                   /* see, if any ptr parameters are invalid   */
        CO_NodeFatalError();                          /* inform user                              */
        return (result);                              /* abort further activities                 */
    }

    hbc = nmt->HbCons;                                /* get first element in heartbeat chain     */
    while (hbc != 0) {                                /* loop through active heartbeat chain      */
        if (nodeId == hbc->NodeId) {                  /* see, if nodeId matches consumer          */
            result = hbc->State;                      /* yes: return received state               */
        }
        hbc = hbc->Next;                              /* switch to next heartbeat consumer        */
    }
                                                      /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}

/*
****************************************************************************************************
*                                       INTERNAL FUNCTIONS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief  HEARTBEAT CONSUMER INITIALIZATION
*
* \ingroup  INTERNAL
*
*           This function initializes the CANopen heartbeat consumer.
*
* \param[in,out]   nmt             reference to NMT structure
*
* \param[in]       node            pointer to parent node structure
*/
/*------------------------------------------------------------------------------------------------*/
void CO_NmtHbConsInit (CO_NMT *nmt)
{
    uint8_t  num;                                  /* Local: number of heartbeat consumer      */
    int16_t  err;                                  /* Local: error variable                    */
    CO_OBJ     *obj;
    CO_HBCONS  *hbc;
    CO_NODE    *node;
    CO_ERR      act;
                                                      /*------------------------------------------*/
    if (nmt == 0) {                                   /* see, if any ptr parameters are invalid   */
        CO_NodeFatalError();                          /* inform user                              */
        return;                                       /* abort further initializations            */
    }
    nmt->HbCons = 0;                                  /* reset used heartbeat consumer chain      */

    node = nmt->Node;
    obj  = CODirFind(&node->Dir, CO_DEV(0x1016, 0));
    if (obj == 0) {
        node->Error = CO_ERR_NONE;                    /* remove error, because optional entry     */
        return;
    }
    err = COObjRdValue(obj, &num, CO_BYTE, 0);        /* read number of heartbeat consumers       */
    if ((err != CO_ERR_NONE) || (num < 1)) {          /* see, if value is allowed                 */
        node->Error = CO_ERR_CFG_1016;                /* if entry exists, the value must be valid */
        return;
    }
    while (num > 0) {
        obj = CODirFind(&node->Dir,
                        CO_DEV(0x1016, num));
        if (obj == 0) {
            node->Error = CO_ERR_CFG_1016;            /* if entry exists, the value must be valid */
            break;
        }
        hbc = (CO_HBCONS *)obj->Data;
        if (hbc == 0) {
            node->Error = CO_ERR_CFG_1016;            /* if entry exists, the value must be valid */
            break;
        }
        act = CO_NmtHbConsActivate(nmt,               /* try to activate heartbeat consumer       */
                                   hbc,
                                   hbc->Time,
                                   hbc->NodeId);
        if (act != CO_ERR_NONE) {
            node->Error = act;
        }
        num--;
    }
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  HEARTBEAT CONSUMER ACTIVATION
*
* \ingroup  INTERNAL
*
*           This function activates a single heartbeat consumer.
*
* \param[in,out]   nmt             reference to NMT structure
*
* \param[in,out]   hbc             reference to heartbeat consumer structure
*
* \retval    =CO_ERR_NONE          successfull activated, or consumer is deactivated by command
* \retval   !=CO_ERR_NONE          error detected (double activation, timer delete problem)
*/
/*------------------------------------------------------------------------------------------------*/
CO_ERR CO_NmtHbConsActivate (CO_NMT *nmt, CO_HBCONS *hbc, uint16_t time, uint8_t nodeid)
{
    CO_ERR      result  = CO_ERR_NONE;
    int16_t  err;                                  /* Local: error variable                    */
    CO_HBCONS  *act;
    CO_HBCONS  *prev;
    CO_HBCONS  *found = 0;

    prev = 0;
    act  = nmt->HbCons;                               /* get root of heartbeat consumer chain     */
    while (act != 0) {                                /* loop through all active consumers        */
        if (act->NodeId == nodeid) {                  /* see, if nodeId is already active         */
            found = act;                              /* already in active list                   */
            break;
        }
        prev = act;                                   /* store previous active consumer           */
        act  = act->Next;                             /* switch to next consumer                  */
    }

    if (found != 0) {                                 /* see, if consumer is in active chain      */
        if (time > 0) {                               /* see, if we want to enable/change time    */
            result = CO_ERR_OBJ_INCOMPATIBLE;         /* not allowed to change during active      */

        } else {                                      /* otherwise: we want disable consumer      */
            if (hbc->Tmr >= 0) {                      /* see, if monitor is already active        */
                err = COTmrDelete(&nmt->Node->Tmr, hbc->Tmr); /* abort monitor                    */
                if (err < 0) {
                    result = CO_ERR_TMR_DELETE;       /* error indication                         */
                }
            }
            hbc->Time   = time;
            hbc->NodeId = nodeid;
            hbc->Tmr    = -1;                         /* set monitor timer to invalid value       */
            hbc->Event  = 0;                          /* reset event counter                      */
            hbc->State  = CO_INVALID;                 /* reset state to 'invalid mode'            */
            hbc->Node   = nmt->Node;                  /* link to parent node                      */
            if (prev == 0) {                          /* see, if consumer is first in chain       */
                nmt->HbCons = hbc->Next;              /* yes: remove first consumer in chain      */
            } else {                                  /* otherwise: consumer is within list       */
                prev->Next  = hbc->Next;              /* remove consumer within chain             */
            }
            hbc->Next   = 0;                          /* clear next pointer in disabled consumer  */
        }

    } else {                                          /* otherwise: consumer not in active chain  */
        hbc->Time   = time;
        hbc->NodeId = nodeid;
        hbc->Tmr    = -1;                             /* set monitor timer to invalid value       */
        hbc->Event  = 0;                              /* reset event counter                      */
        hbc->State  = CO_INVALID;                     /* reset state to 'invalid mode'            */
        hbc->Node   = nmt->Node;                      /* link to parent node                      */
                                                      /*------------------------------------------*/
        if (time > 0) {                               /* see, if consumer is enabled              */
            hbc->Next   = nmt->HbCons;                /* add in consumer chain                    */
            nmt->HbCons = hbc;
        } else {                                      /* otherwise: time changed while disabled   */
            hbc->Next   = 0;                          /* clear next pointer in disabled consumer  */
        }
    }

    return (result);
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  HEARTBEAT CONSUMER CHECK
*
* \ingroup  INTERNAL
*
*           This function checks a received CAN frame against all heartbeat consumers and
*           increments the receive counter in the specific consumer monitor.
*
* \note     We allow the nodeID 0 for heartbeat consuming, because CANopen master heartbeat
*           may come with this nodeID.
*
* \param[in]       frm             reference to CAN frame structure
*
* \param[in]       nmt             pointer to network management structure
*
* \retval    <0    CAN message is not a matching heartbeat message
*           >=0    CAN message is a matching (and consumed) heartbeat message. The return value
*                  is equal to the consumer node-ID
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_NmtHbConsCheck (CO_NMT *nmt, CO_IF_FRM *frm)
{
    int16_t  result = -1;                          /* Local: function result                   */
    uint32_t  cobid;                                /* Local: CAN identifier                    */
    uint8_t  nodeid;                               /* Local: heartbeat producer node-ID        */
    CO_MODE     state;                                /* Local: received state                    */
    CO_HBCONS  *hbc;                                  /* Local: active heartbeat consumer chain   */
                                                      /*------------------------------------------*/
    cobid  = frm->Identifier;                         /* get COBID of the CAN frame               */
    hbc    = nmt->HbCons;                             /* get first heartbeat consumer element     */
    if (hbc == 0) {                                   /* see, if no heartbeat consumer active     */
        return (result);                              /* yes: finished                            */
    }
    if ((cobid >= 1792      ) &&                      /* see, if CAN frame is a heartbeat frame   */
        (cobid <= 1792 + 127)) {
        nodeid = (uint8_t)(cobid - 1792);          /* yes: calculate node-ID out of COBID      */
    } else {                                          /* otherwise: no heartbeat message          */
        return (result);                              /* finished.                                */
    }
    while (hbc != 0) {                                /* loop through all active consumers        */
        if (hbc->NodeId != nodeid) {                  /* see, if node-ID is not for this consumer */
            hbc = hbc->Next;                          /* yes: switch to next consumer             */
        } else {                                      /* otherwise: message is for this consumer  */
            if (hbc->Tmr >= 0) {                      /* see, if consumer is waiting              */
                COTmrDelete(&nmt->Node->Tmr, hbc->Tmr);
            } 
            hbc->Tmr = COTmrCreate(&nmt->Node->Tmr,   /* start the consumer monitor timeout       */
                                    CO_TMR_TICKS(hbc->Time),
                                    0,
                                    CO_NmtHbConsMonitor,
                                    hbc);
            if (hbc->Tmr < 0) {                       /* problem while creating a timer           */
                nmt->Node->Error = CO_ERR_TMR_CREATE; /* set error indication                     */
            }
            state = CONmtModeDecode(frm->Data[0]);    /* decode received state                    */
#if CO_CB_HBC_CHANGE_EN > 0
            if (hbc->State != state) {
                CO_NmtHbConsChange(nmt, hbc->NodeId, state);
            }
#endif
            hbc->State = state;                       /* set received state                       */
            result     = (int16_t)hbc->NodeId;     /* 'consume' given CAN frame                */
            break;
        }
    }
    return (result);                                  /* return function result                   */
}

/*
****************************************************************************************************
*                                           LOCAL FUNCTIONS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief  HEARTBEAT CONSUMER TIMEOUT
*
* \ingroup  INTERNAL
*
*           This timer callback function checks that at least one received heartbeat 
*           is detected for this heartbeat consumer.
*
* \param[in]       parg            heartbeat consumer structure
*/
/*------------------------------------------------------------------------------------------------*/
static void CO_NmtHbConsMonitor (void *parg)
{
    CO_NODE   *node;                                  /* Local: reference to parent node          */
    CO_HBCONS *hbc;                                   /* Local: reference to consumer structure   */
                                                      /*------------------------------------------*/
    hbc  = (CO_HBCONS *)parg;                         /* get reference to the consumer structure  */
    node = hbc->Node;                                 /* get parent node for this consumer        */

    hbc->Tmr = COTmrCreate(&node->Tmr,                /* start next consumer monitor timeout      */
                            CO_TMR_TICKS(hbc->Time),
                            0,
                            CO_NmtHbConsMonitor,
                            hbc);
    if (hbc->Tmr < 0) {                               /* problem while creating a timer           */
        node->Error = CO_ERR_TMR_CREATE;              /* set error indication                     */
    }
    if (hbc->Event < 0xFFu) {                         /* increment event counter up to 0xFF       */
        hbc->Event++;
    }
#if CO_CB_HBC_EVENT_EN > 0
    CO_NmtHbConsEvent(&node->Nmt, hbc->NodeId);       /* call heartbeat consumer event callback   */
#endif
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  WRITE HEARTBEAT CONSUMER CONFIG
*
* \ingroup  INTERNAL
*
*           This function is a typed object write function, special for object entry 1016.
*           This entry defines the expected rate of the heartbeat message of a specific node.
*
* \param[in,out]   obj               ptr to accessed object entry
*
* \param[in]       buf               ptr to write value
*
* \param[in]       size              size of write value
*
* \retval   CO_ERR_NONE         heartbeat consumer config is written
* \retval   CO_ERR_TYPE_WR      an error is detected and function aborted
*/
/*------------------------------------------------------------------------------------------------*/
static int16_t CO_TNmtHbConsWrite (CO_OBJ* obj, void *buf, uint32_t size)
{
    CO_NODE    *node;                                 /* Local: ptr to parent node                */
    CO_DIR     *cod;                                  /* Local: ptr to object directory           */
    CO_HBCONS  *hbc;                                  /* Local: ptr to the HB consumer structure  */
    int16_t  result = CO_ERR_TYPE_WR;              /* Local: function result                   */
    uint32_t  value  = 0;                           /* Local: encoded heartbeat consumer value  */
    uint16_t  time;                                 /* Local: time to write                     */
    uint8_t  nodeid;                               /* Local: nodeid to monitor                 */
                                                      /*------------------------------------------*/
    cod  = obj->Type->Dir;                            /* get ptr to object directory              */
    node = cod->Node;                                 /* get ptr to parent node                   */
    hbc  = (CO_HBCONS *)obj->Data;
    if (hbc == 0) {
        node->Error = CO_ERR_CFG_1016;                /* if entry exists, the value must be valid */
        return (result);
    }
    if (size != CO_LONG) {
        node->Error = CO_ERR_CFG_1016;                /* if entry exists, the value must be valid */
        return (result);
    }
    value  = *((uint32_t *)buf);
    time   = (uint16_t)value;
    nodeid = (uint8_t)(value >> 16);

    result = CO_NmtHbConsActivate(&node->Nmt,         /* try to activate heartbeat consumer       */
                                   hbc,
                                   time,
                                   nodeid);
                                                      /*------------------------------------------*/
    return (result);
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief READ HEARTBEAT CONSUMER CONFIG
*
* \ingroup INTERNAL
*
*          This function is a typed object read function, special for object entry 1016.
*          This entry defines the expected rate of the heartbeat message of a specific node.
*
* \param[in,out]   obj          ptr to accessed object entry
*
* \param[in,out]   buf          ptr to write value
*
* \param[in]       len          size of write value
*
* \retval         =CO_ERR_NONE  Successfully operation
* \retval        !=CO_ERR_NONE  An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
static int16_t CO_TNmtHbConsRead (CO_OBJ *obj, void *buf, uint32_t len)
{
    CO_HBCONS  *hbc;                                  /* Local: ptr to the HB consumer structure  */
    int16_t  result = CO_ERR_NONE;                 /* Local: function result                   */
    uint32_t  value;                                /* Local: encoded value for this entry      */
    uint8_t *src;                                  /* Local: pointer within RAM domain         */
    uint8_t *dst;                                  /* Local: pointer to buffer                 */
    uint32_t  num;                                  /* Local: remaining bytes in domain         */
                                                      /*------------------------------------------*/
    hbc    = (CO_HBCONS *)(obj->Data);                /* get HB consumer structure                */
    value  = (uint32_t)(hbc->Time);                 /* set HB consumer time in value            */
    value |= ((uint32_t)(hbc->NodeId)) << 16;       /* add HB consumer node                     */
    num    = CO_LONG;                                 /* set size of object entry                 */
    src    = (uint8_t *)&value;                    /* set encoded value as source              */
    dst    = (uint8_t *)buf;                       /* get buffer pointer                       */
    while ((len > 0) && (num > 0)) {                  /* loop through remaining domain            */
        *dst = *src;                                  /* copy domain into buffer location         */
        src++;                                        /* switch to next domain byte               */
        dst++;                                        /* switch to next buffer location           */
        len--;                                        /* decrement remaining space in buffer      */
        num--;                                        /* update remaining bytes of value          */
    }
                                                      /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}

/*
****************************************************************************************************
*                                         CALLBACK FUNCTIONS
****************************************************************************************************
*/

#if CO_CB_HBC_EVENT_EN == 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief HEARTBEAT CONSUMER EVENT CALLBACK
*
* \ingroup CALLBACK
*
*          This function is called when a heartbeat consumer monitor timer elapses, before
*          receiving the corresponding heartbeat message.
*
* \note    This implementation is an example implementation, which will do nothing. This function
*          is optional and application specific. The function can be implemented somewhere in the
*          in the application code. The activation of the application callback function is done
*          with \ref CO_CB_HBC_EVENT_EN.
*
* \note    When disabling the application callback function, this example implementation is
*          enabled, but not called. In fact: disabling the application function will remove the
*          callback function call in the consumer monitor processing.
*
* \note    The node pointer is checked to be valid before calling this function.
*
* \param[in]       nmt                reference to NMT structure
*
* \param[in]       nodeId             The nodeId of the missed heartbeat message
*/
/*------------------------------------------------------------------------------------------------*/
void CO_NmtHbConsEvent (CO_NMT *nmt, uint8_t nodeId)
{
    (void)nmt;
    (void)nodeId;
}
#endif

#if CO_CB_HBC_CHANGE_EN == 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief HEARTBEAT CONSUMER STATE CHANGE CALLBACK
*
* \ingroup CALLBACK
*
*          This function is called when a heartbeat consumer monitor detects a state change, of
*          a monitored node.
*
* \note    This implementation is an example implementation, which will do nothing. This function
*          is optional and application specific. The function can be implemented somewhere in the
*          in the application code. The activation of the application callback function is done
*          with \ref CO_CB_HBC_CHANGE_EN.
*
* \note    When disabling the application callback function, this example implementation is
*          enabled, but not called. In fact: disabling the application function will remove the
*          callback function call in the consumer monitor processing.
*
* \note    The node pointer is checked to be valid before calling this function.
*
* \param[in]       nmt                reference to NMT structure
*
* \param[in]       nodeId             The nodeId of the monitored node
*
* \param[in]       mode               The new received node state of the monitored node
*/
/*------------------------------------------------------------------------------------------------*/
void CO_NmtHbConsChange(CO_NMT *nmt, uint8_t nodeId, CO_MODE mode)
{
    (void)nmt;
    (void)nodeId;
    (void)mode;
}
#endif
