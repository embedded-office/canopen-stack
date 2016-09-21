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
* \file     co_nmt.c
*
* \brief    NMT SLAVE
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_nmt.c#5 $
*
*           This source file implements the CANopen NMT handling.
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
*                                            GOBAL DATA
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief NMT MODE ALLOWED OBJECTS
*
* \ingroup NMT
*
*          This constant codes are used to identify the allowed objects within the specific
*          NMT state machine mode.
*/
/*------------------------------------------------------------------------------------------------*/
static const uint8_t CONmtModeObj[CO_MODE_NUM] = {
    ( 0 ),                                            /*!< objects in invalid mode                */

    ( CO_BOOT_ALLOWED ),                              /*!< objects in initialization mode         */

    (  CO_SDO_ALLOWED |                               /*!< objects in pre-operational mode        */
      CO_SYNC_ALLOWED |
      CO_TIME_ALLOWED |
      CO_EMCY_ALLOWED |
       CO_NMT_ALLOWED ),

    (  CO_PDO_ALLOWED |                               /*!< objects in operational mode            */
       CO_SDO_ALLOWED |
      CO_SYNC_ALLOWED |
      CO_TIME_ALLOWED |
      CO_EMCY_ALLOWED |
       CO_NMT_ALLOWED ),

    (  CO_NMT_ALLOWED )                               /*!< objects in stop mode                   */
};


/*------------------------------------------------------------------------------------------------*/
/*! \brief NMT MODE ENCODING
*
* \ingroup NMT
*
*          This constant codes are used to encode the NMT state machine mode within heartbeat
*          acc. the standard.
*/
/*------------------------------------------------------------------------------------------------*/
static const uint8_t CONmtModeCode[CO_MODE_NUM] = {
    255,                                              /*!< encoding for invalid mode              */
    0,                                                /*!< encoding for initialization mode       */
    127,                                              /*!< encoding for pre-operational mode      */
    5,                                                /*!< encoding for operational mode          */
    4                                                 /*!< encoding for stopped mode              */
};

/*
****************************************************************************************************
*                                            FUNCTIONS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief  RESET DEVICE
*
* \ingroup  API
*
*           This function resets the CANopen device with the given type.
*
* \param[in,out]   nmt             reference to NMT structure
*
* \param[in]       type            the requested NMT reset type.
*/
/*------------------------------------------------------------------------------------------------*/
void CONmtReset(CO_NMT *nmt, CO_NMT_RESET type)
{
    uint8_t nobootup = 1;                          /* Local: suppress transmission of bootup   */
#if CO_LSS_EN > 0
    int16_t err;                                   /* Local: error code                        */
#endif
                                                      /*------------------------------------------*/
    if (nmt == 0) {                                   /* see, if any ptr parameters are invalid   */
        CO_NodeFatalError();                          /* inform user                              */
        return;                                       /* abort further initializations            */
    }
    if (nmt->Mode != CO_INIT) {                       /* see, if not in initialization mode       */
        CONmtSetMode(nmt, CO_INIT);                   /* yes: set mode to initialization mode     */
        nobootup = 0;                                 /* disable transmission of bootup message   */
    }
                                                      /*------------------------------------------*/
    if (type == CO_RESET_NODE) {                      /*          R E S E T   N O D E             */
                                                      /*------------------------------------------*/
#if CO_OBJ_PARA_EN > 0
        CONodeParaLoad(nmt->Node, CO_RESET_NODE);     /* get all parameters within 2000h..9FFFh  */
#endif
    }
                                                      /*------------------------------------------*/
    if (type <= CO_RESET_COM) {                       /*   C O M M U N I C A T I O N   R E S E T  */
                                                      /*------------------------------------------*/
#if CO_OBJ_PARA_EN > 0
        CONodeParaLoad(nmt->Node, CO_RESET_COM);      /* get all parameters within 1000h..1FFFh   */
#endif
#if CO_LSS_EN > 0
        err = CO_LssLoad(&nmt->Node->Baudrate,        /* Load LSS configured values               */
                         &nmt->Node->NodeId);
        if (err != CO_ERR_NONE) {                     /* see, if error while reading media occurs */
        	nmt->Node->Error = CO_ERR_LSS_LOAD;       /* set error indication                     */
            return;                                   /* abort initialization                     */
        }
        CO_LssInit(&nmt->Node->Lss, nmt->Node);       /* Initialise LSS slave                     */
#endif
        CO_TmrClear(&nmt->Node->Tmr);                 /* clear all CANopen highspeed timers       */
        CO_NmtInit (nmt, nmt->Node);                  /* reset heartbeat timer                    */
        CO_SdoInit (nmt->Node->Sdo, nmt->Node);       /* Initialise CANopen SDO tables            */
        COIfReset  (&nmt->Node->If);                  /* reset communication interface            */

#if CO_EMCY_N > 0
        COEmcyReset(&nmt->Node->Emcy, 1);             /* Reset CANopen EMCY tables                */
#endif
#if CO_SYNC_EN > 0
        CO_SyncInit(&nmt->Node->Sync, nmt->Node);     /* Initialize SYNC tables                   */
#endif
        if (nobootup == 0) {                          /* see, if bootup message is not disabled   */
            CO_NmtBootup(nmt);                        /* transmit boot-up message                 */
        }
    }
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  SET CURRENT MODE
*
* \ingroup  API
*
*           This function sets the requested CANopen NMT state machine mode.
*
* \param[in,out]   nmt             reference to NMT structure
*
* \param[in]       mode            the requested NMT mode.
*/
/*------------------------------------------------------------------------------------------------*/
void CONmtSetMode(CO_NMT *nmt, CO_MODE mode)
{
    if (nmt == 0) {                                   /* see, if any ptr parameters are invalid   */
        CO_NodeFatalError();                          /* inform user                              */
        return;                                       /* abort further initializations            */
    }
    if (mode == CO_OPERATIONAL) {                     /* see, if switching to OPERATIONAL         */
#if CO_TPDO_N > 0
        CO_TPdoInit(nmt->Node->TPdo, nmt->Node);      /* Initialise TPDO tables                   */
#endif
#if CO_RPDO_N > 0
        CO_RPdoInit(nmt->Node->RPdo, nmt->Node);      /* Initialise TPDO tables                   */
#endif
    }
#if CO_CB_NMT_CHANGE_EN > 0
    if (nmt->Mode != mode) {                          /* see, if mode has changed                 */
        CO_NmtModeChange(nmt, mode);                  /* call mode change callback function       */
    }
#endif
    nmt->Mode    = mode;                              /* set new NMT mode                         */
    nmt->Allowed = CONmtModeObj[mode];                /* set allowed objects for this mode        */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  GET CURRENT MODE
*
* \ingroup  API
*
*           This function returns the current CANopen NMT state machine mode.
*
* \param[in]       nmt             reference to NMT structure
*
* \retval   >0     The current NMT mode
* \retval   =0     An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
CO_MODE CONmtGetMode(CO_NMT *nmt)
{
    if (nmt == 0) {                                   /* see, if any ptr parameters are invalid   */
        CO_NodeFatalError();                          /* inform user                              */
        return (CO_INVALID);                          /* abort further initializations            */
    }
    return (nmt->Mode);                               /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  SET NODE-ID
*
* \ingroup  API
*
*           This function sets the requested CANopen Node-ID within the NMT module.
*
* \note     The following error are detected within this function:
*           - CO_ERR_NMT_MODE: the CANopen device is not in INIT mode
*           - CO_ERR_BAD_ARG: the given nodeId is invalid (e.g. zero)
* \note     If one of these errors is detected, this function call will change nothing.
*
* \param[in,out]   nmt             reference to NMT structure
*
* \param[in]       nodeId          the requested NMT node ID
*/
/*------------------------------------------------------------------------------------------------*/
void CONmtSetNodeId(CO_NMT *nmt, uint8_t nodeId)
{
    CO_MODE mode;                                     /* Local: current active NMT mode           */
                                                      /*------------------------------------------*/
    if (nmt == 0) {                                   /* see, if any ptr parameters are invalid   */
        CO_NodeFatalError();                          /* inform user                              */
        return;                                       /* abort further operations                 */
    }
    if (nmt->Node == 0) {                             /* see, if node ptr is valid                */
        return;                                       /* abort further operations                 */
    }
    if (nodeId == 0) {                                /* see, if given node-ID is invalid         */
        nmt->Node->Error = CO_ERR_BAD_ARG;            /* set error code                           */
        return;                                       /* abort further operations                 */
    }
    mode = nmt->Mode;                                 /* get current mode                         */
    if (mode != CO_INIT) {                            /* see, if node is not in INIT mode         */
        nmt->Node->Error = CO_ERR_NMT_MODE;           /* set error code                           */
    } else {                                          /* otherwise: during INIT mode change is ok */
        nmt->Node->NodeId = nodeId;                   /* set new NodeId                           */
    }
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  GET NODE-ID
*
* \ingroup  API
*
*           This function returns the current CANopen Node-ID of the NMT module.
*
* \param[in,out]   nmt             reference to NMT structure
*
* \retval   =0     An error is detected
* \retval   >0     The current NMT node ID
*/
/*------------------------------------------------------------------------------------------------*/
uint8_t CONmtGetNodeId(CO_NMT *nmt)
{
    uint8_t result = 0;                            /* Local: function result                   */
                                                      /*------------------------------------------*/
    if (nmt == 0) {                                   /* see, if any ptr parameters are invalid   */
        CO_NodeFatalError();                          /* inform user                              */
        return (0);                                   /* abort further initializations            */
    }
    if (nmt->Node != 0) {                             /* see, if node ptr is valid                */
        result = nmt->Node->NodeId;                   /* get current node-id from object          */
    }
                                                      /*------------------------------------------*/
    return result;                                    /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  DECODE NODE-STATE
*
* \ingroup  API
*
*           This function returns the CANopen mode to the given heartbeat state encoding.
*
* \param[in]   code             heartbeat state 
*
* \retval   =CO_INVALID   An error is detected
* \retval   >0            The corresponding NMT heartbeat state
*/
/*------------------------------------------------------------------------------------------------*/
CO_MODE CONmtModeDecode(uint8_t code)
{
    CO_MODE    result = CO_INVALID;
    uint8_t id;

    for (id = 0; id < CO_MODE_NUM; id++) {
        if (CONmtModeCode[id] == code) {
            result = (CO_MODE)id;
        }
    }
    return (result);
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  ENCODE NODE-STATE
*
* \ingroup  API
*
*           This function returns the heartbeat state code for the given CANopen mode.
*
* \param[in]   mode             CANopen mode
*
* \return   The corresponding NMT heartbeat state code.
*/
/*------------------------------------------------------------------------------------------------*/
uint8_t CONmtModeEncode(CO_MODE mode)
{
    uint8_t result = CONmtModeCode[CO_INVALID];

    if (mode < CO_MODE_NUM) {
        result = CONmtModeCode[mode];
    }
    return (result);
}

/*
****************************************************************************************************
*                                       INTERNAL FUNCTIONS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief  NMT SLAVE INITIALIZATION
*
* \ingroup  INTERNAL
*
*           This function initializes the CANopen NMT environment.
*
* \param[in,out]   nmt             reference to NMT structure
*
* \param[in]       node            pointer to parent node structure
*/
/*------------------------------------------------------------------------------------------------*/
void CO_NmtInit(CO_NMT *nmt, CO_NODE *node)
{
    if ((nmt == 0) || (node == 0)) {                  /* see, if any ptr parameters are invalid   */
        CO_NodeFatalError();                          /* inform user                              */
        return;                                       /* abort further initializations            */
    }
    nmt->Node = node;                                 /* set reference to parent node             */
    CONmtSetMode(nmt, CO_INIT);                       /* set initialization mode                  */
    CO_NmtHbProdInit(nmt);                            /* initialize heartbeat producer            */
    CO_NmtHbConsInit(nmt);                            /* initialize heartbeat consumer            */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  BOOTUP EVENT
*
* \ingroup  INTERNAL
*
*           This function performs the bootup protocol to the configured CAN bus. This protocol
*           is used to signal that a NMT slave has entered the node state PRE-OPERATIONAL after
*           the state INITIALISING.
*
* \param[in,out]   nmt             reference to NMT structure
*
* \ds301    9.2.6.2.3
*/
/*------------------------------------------------------------------------------------------------*/
void CO_NmtBootup(CO_NMT *nmt)
{
    CO_IF_FRM    frm;                                 /* Local: boot-up CAN frame                 */
                                                      /*------------------------------------------*/
    if (nmt->Mode == CO_INIT) {                       /* see, if device is in INIT mode           */
        CONmtSetMode(nmt, CO_PREOP);                  /* enter pre-operational mode               */
                                                      /*------------------------------------------*/
        CO_SET_COBID(&frm, 1792 + nmt->Node->NodeId); /* set bootup CAN identifier                */
        CO_SET_DLC  (&frm, 1);                        /* set fixed DLC (1 byte)                   */
        CO_SET_BYTE (&frm, 0, 0);                     /* set required data byte #0 to 0           */
                                                      /*------------------------------------------*/
        (void)COIfSend(&nmt->Node->If, &frm);         /* send bootup message                      */
                                                      /* possible error registered in node        */
    }
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  NMT MESSAGE CHECK
*
* \ingroup  INTERNAL
*
*           This functions checks a received frame to be a NMT message.
*
* \param[in,out]   nmt             reference to NMT structure
*
* \param[in]       frm             received CAN frame
*
* \retval   =0     check function successful
* \retval   <0     mesage is not an NMT message
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_NmtCheck(CO_NMT *nmt, CO_IF_FRM *frm)
{
    int16_t result = -1;                           /* Local: function result                   */
                                                      /*------------------------------------------*/
    if (frm->Identifier == 0) {                       /* check for nmt message                    */
        result = 0;                                   /* success, regardless if we are addressed  */
        if ( (frm->Data[1] == nmt->Node->NodeId) ||   /* see, if we are addressed (or broadcast)  */
             (frm->Data[1] == 0                ) ) {
            switch(frm->Data[0]) {                    /* select command specifier                 */
                case 1:                               /* Cmd: Start Remote Node                   */
                    CONmtSetMode(nmt, CO_OPERATIONAL);/* set node to operational                  */
                    break;                            /*------------------------------------------*/
                case 2:                               /* Cmd: Stop Remote Node                    */
                    CONmtSetMode(nmt, CO_STOP);       /* set node to operational                  */
                    break;                            /*------------------------------------------*/
                case 128:                             /* Cmd: Enter Pre-Operational Operation     */
                    CONmtSetMode(nmt, CO_PREOP);      /* set node to operational                  */
                    break;                            /*------------------------------------------*/
                case 129:                             /* Cmd: Reset Node                          */
                    CONmtReset(nmt, CO_RESET_NODE);   /* init CANOpen with reset node             */
                    break;                            /*------------------------------------------*/
                case 130:                             /* Cmd: Reset Communication                 */
                    CONmtReset(nmt, CO_RESET_COM);    /* init CANOpen with reset communication    */
                    break;                            /*------------------------------------------*/
                default:                              /* Cmd: unknown                             */
                    break;
            }
        }
    }
    return(result);
}

/*
****************************************************************************************************
*                                         CALLBACK FUNCTIONS
****************************************************************************************************
*/

#if CO_CB_NMT_CHANGE_EN == 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief NMT MODE CHANGE CALLBACK
*
* \ingroup CALLBACK
*
*          This function is called when the NMT mode is changed.
*
* \note    This implementation is an example implementation, which will do nothing. This function
*          is optional and application specific. The function can be implemented somewhere in the
*          in the application code. The activation of the application callback function is done
*          with \ref CO_CB_NMT_CHANGE_EN.
*
* \note    When disabling the application callback function, this example implementation is
*          enabled, but not called. In fact: disabling the application function will remove the
*          callback function call in the NMT mode management.
*
* \note    The nmt object pointer is checked to be valid before calling this function.
*
* \param[in]       nmt             reference to NMT structure
*
* \param[in]       mode            the new mode
*/
/*------------------------------------------------------------------------------------------------*/
void CO_NmtModeChange(CO_NMT *nmt, CO_MODE mode)
{
    (void)nmt;
    (void)mode;
}
#endif
