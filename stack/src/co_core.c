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
* \file     co_core.c
*
* \brief    CORE
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_core.c#7 $
*
*           This source file implements the core of the CANopen protocol stack.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

/*
****************************************************************************************************
*                                             INCLUDES
****************************************************************************************************
*/

#include "co_core.h"                                  /* CANopen PDO handling functions           */

/*
****************************************************************************************************
*                                            FUNCTIONS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief  CANOPEN STACK INITIALIZATION
*
* \ingroup  API
*
*           This function initializes the internals of the CANopen stack. The specification of
*           the CANopen node, and the CANopen node object itself is given as parameter.
*
* \note     The node is still in INIT mode after this function call. To finalize the initialization
*           phase (e.g. profile specific or application actions, etc..), see \ref CONodeStart().
*
* \param[in,out]   node         pointer to the CANopen node object
*
* \param[in]       spec         pointer to the configuration collection of config aspects (specification)
*/
/*------------------------------------------------------------------------------------------------*/
void CONodeInit(CO_NODE *node, CO_NODE_SPEC *spec)
{
    int16_t   err;                                 /* Local: error code                        */
                                                      /*------------------------------------------*/
                                                      /*              NODE VARIABLES              */
                                                      /*------------------------------------------*/
    node->If.Drv   = spec->CanDrv;                    /* link specified CAN bus driver            */
#if CO_SDO_SEG_EN > 0 || CO_SDO_BLK_EN > 0
    node->SdoBuf   = spec->SdoBuf;                    /* link specified SDO buffer memory         */
#endif
    node->Baudrate = spec->Baudrate;                  /* set default CAN baudrate                 */
    node->NodeId   = spec->NodeId;                    /* set default Node-Id                      */
    node->Error    = CO_ERR_NONE;                     /* clear internal error indication          */
    node->Nmt.Tmr  = -1;                              /* invalidate timer entry                   */
                                                      /*------------------------------------------*/
                                                      /*            LSS CONFIGURATION             */
                                                      /*------------------------------------------*/
#if CO_LSS_EN > 0
    err = CO_LssLoad(&node->Baudrate, &node->NodeId); /* Load LSS configured values               */
    if (err != CO_ERR_NONE) {                         /* see, if error while reading media occurs */
        node->Error = CO_ERR_LSS_LOAD;                /* set error indication                     */
        return;                                       /* abort initialization                     */
    }
#endif
                                                      /*------------------------------------------*/
                                                      /*              TIMER MODULE                */
                                                      /*------------------------------------------*/
    CO_TmrInit(&node->Tmr, node,                      /* Initialize Highspeed-Timer               */
                spec->TmrMem, spec->TmrNum);
                                                      /*------------------------------------------*/
                                                      /*                CAN LAYER                 */
                                                      /*------------------------------------------*/
    CO_IfInit(&node->If, node);                       /* Initialize CAN bus interface             */
    CO_IfEnable(&node->If, node->Baudrate);           /* Enable CAN bus interface                 */
                                                      /*------------------------------------------*/
                                                      /*                NODE OBJECTS              */
                                                      /*------------------------------------------*/
    err = CO_DirInit(&node->Dir, node,                /* initialize object directory              */
                     spec->Dir, spec->DirLen);
    if (err < 0) {                                    /* see, if a parameter error is detected    */
        return;                                       /* abort further initializations            */
    }
#if CO_OBJ_PARA_EN > 0
    CONodeParaLoad(node, CO_RESET_COM);               /* get all parameters within 1000h..1FFFh   */
    CONodeParaLoad(node, CO_RESET_NODE);              /* get all parameters within 2000h..9FFFh   */
#endif
    CO_NmtInit(&node->Nmt, node);                     /* Initialize NMT slave                     */
    CO_SdoInit( node->Sdo, node);                     /* Initialize SDO server                    */
                                                      /*------------------------------------------*/
                                                      /*           OPTIONAL NODE OBJECTS          */
                                                      /*------------------------------------------*/
#if CO_TPDO_N > 0
    CO_TPdoClear(node->TPdo, node);                   /* Clear TPDO tables                        */
#endif
#if CO_RPDO_N > 0
    CO_RPdoClear(node->RPdo, node);                   /* Clear RPDO tables                        */
#endif
#if CO_EMCY_N > 0
    if (spec->EmcyCode != 0) {
        CO_EmcyInit(&node->Emcy, node,                /* Initialize EMCY tables                   */
                    spec->EmcyCode); 
    }
#endif
#if CO_SYNC_EN > 0
    CO_SyncInit(&node->Sync, node);                   /* Initialize SYNC tables                   */
#endif
#if CO_LSS_EN > 0
    CO_LssInit(&node->Lss, node);                     /* Initialize LSS slave                     */
#endif
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  START NODE
*
* \ingroup  API
*
*           This function will finish the initialization phase. The node will change into
*           PRE-OPERATIONAL state and is ready for communication.
*
* \param[in,out]   node         pointer to the CANopen node object
*/
/*------------------------------------------------------------------------------------------------*/
void CONodeStart(CO_NODE *node)
{
    CO_MODE mode;                                     /* Local: NMT mode of CANopen node          */
                                                      /*------------------------------------------*/
    mode = CONmtGetMode(&node->Nmt);                  /* get current mode of CANopen node         */
    if (mode == CO_INIT) {                            /* see, if mode indicates INIT mode         */
        CO_NmtBootup(&node->Nmt);                     /* leave init mode with final boot-up       */
    }
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  STOP NODE
*
* \ingroup  API
*
*           This function will stop all communication activities and removes the CANopen node
*           from the CAN bus interface.
*
* \note     To reactivate a stopped CANopen node, the functions \ref CONodeInit() and
*           \ref CONodeStart() must be called again.
*
* \param[in,out]   node         pointer to the CANopen node object
*/
/*------------------------------------------------------------------------------------------------*/
void CONodeStop(CO_NODE *node)
{
    CO_TmrClear (&node->Tmr);                         /* clear all CANopen highspeed timers       */
    CONmtSetMode(&node->Nmt, CO_INVALID);             /* set CANopen node mode to INVALID         */
    COIfClose   (&node->If);                          /* close CAN bus connection                 */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  GET NODE ERROR
*
* \ingroup  API
*
*           This function returns the current error status of the given node. If an error was
*           detected, the error is cleared with this function call.
*
* \param[in,out]   node         pointer to the CANopen node object
*
* \return          One of the error codes in \ref CO_ERR
*/
/*------------------------------------------------------------------------------------------------*/
CO_ERR CONodeGetErr(CO_NODE *node)
{
    CO_ERR result;                                    /* Local: function result                   */
                                                      /*------------------------------------------*/
    result      = node->Error;                        /* get current error status                 */
    node->Error = CO_ERR_NONE;                        /* ensure, that error is cleared            */
                                                      /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}


#if CO_OBJ_PARA_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief LOAD PARAMETER FROM NVM
*
* \ingroup API
*
*          This function is responsible for the loading of all parameter groups with the given
*          type. The single parameter group(s) will be loaded from NVM by calling the user
*          application callback function \ref CO_ParaLoad().
*
* \note    This function considers all parameter groups, which are linked to the parameter
*          store index (1010h) within the object directory. Every not linked parameter group
*          is not scope of this function and must be handled within the application.
*
* \param[in,out]   node        Ptr to node info
*
* \param[in]       type        Reset type, e.g. CO_RESET_COM or CO_RESET_NODE
*
* \retval          =0          loading successful
* \retval          <0          an error is detected and function aborted
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CONodeParaLoad(CO_NODE *node, CO_NMT_RESET type)
{
    CO_DIR     *cod;                                  /* Local: ptr to object directory           */
    CO_OBJ     *obj;                                  /* Local: ptr to object entry               */
    CO_PARA    *pg;                                   /* Local: ptr to parameter group info       */
    int16_t  err;                                  /* Local: function error indication         */
    int16_t  result = 0;                           /* Local: function result                   */
    uint8_t  num    = 0;                           /* Local: highest subindex in index 1010h   */
    uint8_t  sub;                                  /* Local: current working subindex in loop  */
                                                      /*------------------------------------------*/
    cod = &node->Dir;                                 /* get ptr to object directory              */
    err = CODirRdByte(cod, CO_DEV(0x1010,0), &num);   /* get highest subindex entry               */
    if (err != CO_ERR_NONE) {                         /* see, if an error is detected             */
        node->Error = CO_ERR_NONE;                    /* abort with no error: entry is optional   */
        return (result);
    }                                                 /*------------------------------------------*/
                                                      /*lint -e{644} : num set in CODirRdByte()   */
    for (sub = 1; sub <= num; sub++) {                /* loop through storage subindex entries    */
        obj = CODirFind(cod, CO_DEV(0x1010,sub));     /* get object entry                         */
        if (obj != 0) {                               /* see, if object entry is available        */
            pg = (CO_PARA *)obj->Data;                /* get pointer to parameter group info      */
            if (pg->Type == type) {                   /* see, if parameter reset type is matching */
                err = CO_ParaLoad(pg);                /* yes: call application callback function  */
                if (err != CO_ERR_NONE) {             /* see, if user detects an error            */
                    node->Error = CO_ERR_PARA_LOAD;   /* write error code into node error info    */
                    result      = -1;                 /* indicate an detected error in function   */
                }
            }
        }
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}
#endif

/*------------------------------------------------------------------------------------------------*/
/*! \brief  CAN RECEIVE PROCESSING
*
* \ingroup  INTERNAL
*
*           This function processes one received CAN frame from the given CAN node and initiates
*           the specific protocol activity. If the CAN frame is not handled by the stack, the
*           user will get this CAN frame into the (optional) callback function \see CO_IfReceive()
*
* \param[in,out]   node        Ptr to node info
*/
/*------------------------------------------------------------------------------------------------*/
void CONodeProcess(CO_NODE *node)
{
    CO_IF_FRM    frm;                                 /* Local: memory for one CAN frame          */
    CO_SDO      *srv;                                 /* Local: SDO server                        */
    int16_t   err;                                 /* Local: error variable                    */
    uint8_t   allowed;                             /* Local: allowed communication objects     */
#if CO_RPDO_N > 0
    int16_t   num;                                 /* Local: number of RPDO                    */
#endif
                                                      /*------------------------------------------*/
    err = COIfRead(&node->If, &frm);                  /* check for a CAN frame on bus             */
    if (err < 0) {                                    /* if error in can message                  */
        allowed = 0;                                  /* set allowed objects to none              */
    } else {                                          /* otherwise: valid can frame received      */
        allowed = node->Nmt.Allowed;                  /* set allowed objects with mode encoding   */
    }                                                 /*------------------------------------------*/
#if CO_LSS_EN > 0
    err = CO_LssCheck(&node->Lss, &frm);              /* check for LSS request                    */
    if (err != 0) {                                   /* see, if frame is a LSS request           */
        if (err > 0) {                                /* see, if a response is generated          */
            (void)COIfSend(&node->If, &frm);          /* send SDO response at once                */
                                                      /* possible error registered in node        */
        }                                             /* otherwise: ignore LSS request (err < 0)  */
        allowed = 0;                                  /* no further objects allowed (in 1 CAN id) */
    }                                                 /*------------------------------------------*/
#endif
    if ((allowed & CO_SDO_ALLOWED) != 0) {            /* see, if SDO communication is allowed     */
        srv = CO_SdoCheck(node->Sdo, &frm);           /* yes: see, if frame is a SDO request      */
        if (srv != 0) {                               /* see, if a SDO server is addressed        */
            err = CO_SdoResponse(srv);                /* yes: generate SDO response               */
            if (err >= -1) {                          /* on success or abort message              */
                (void)COIfSend(&node->If, &frm);      /* send SDO response at once                */
                                                      /* possible error registered in node        */
            }
            allowed = 0;                              /* no further objects allowed (in 1 CAN id) */
        }
    }                                                 /*------------------------------------------*/
    if ((allowed & CO_NMT_ALLOWED) != 0) {            /* see, if NMT communication is allowed     */
        if (CO_NmtCheck(&node->Nmt, &frm) >= 0) {     /* yes: see, if frame is a NMT command      */
            allowed = 0;                              /* no further objects allowed (in 1 CAN id) */
        } else if (CO_NmtHbConsCheck(&node->Nmt,      /* see, if frame is a heartbeat             */
                                     &frm) >= 0) {
            allowed = 0;                              /* no further objects allowed (in 1 CAN id) */
        }
    }                                                 /*------------------------------------------*/
#if CO_RPDO_N > 0
    if ((allowed & CO_PDO_ALLOWED) != 0) {            /* see, if PDO communication is allowed     */
        num = CO_RPdoCheck(node->RPdo, &frm);         /* yes: see, if frame is a RPDO message     */
        if (num >= 0) {                               /* if frame is a PDO command                */
            CO_RPdoRx(node->RPdo, num, &frm);         /* distribute RPDO                          */
            allowed = 0;                              /* no further objects allowed (in 1 CAN id) */
        }
    }                                                 /*------------------------------------------*/
#endif
#if CO_SYNC_EN > 0
    if ((allowed & CO_SYNC_ALLOWED) != 0) {           /* see, if SYNC is allowed                  */
#if CO_TPDO_N > 0
        err = CO_SyncUpdate(&node->Sync, &frm);       /* update sync management for TPDOs         */
        if (err >= 0) {
#endif
            CO_SyncHandler(&node->Sync);              /* handling of all synchronized PDOs        */
            allowed = 0;                              /* no further objects allowed (in 1 CAN id) */
#if CO_TPDO_N > 0
        }
#endif
    }                                                 /*------------------------------------------*/
#endif
#if CO_CB_IF_RECEIVE_EN > 0
    if (allowed != 0) {                               /* if message not handled yet               */
        CO_IfReceive(&frm);                           /* execute application receive callback     */
    }
#endif
}

/*
****************************************************************************************************
*                                        INTERNAL FUNCTIONS
****************************************************************************************************
*/

#if CO_CB_FATAL_ERROR_EN == 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  FATAL ERROR CALLBACK
*
* \ingroup  CALLBACK
*
*           This function is called, after detecting a fatal error within the stack - and
*           no way out of the situation - panic -.
*
*           The function is intended to allow the implementation of a pre-defined shutdown
*           sequence and setting the device in a safety state.
*
* \note     This function should be implemented within the application. This function is a
*           placeholder only. The application implementation of this function is activated
*           with the configuration define \ref CO_CB_FATAL_ERROR_EN.
*/
/*------------------------------------------------------------------------------------------------*/
void CO_NodeFatalError(void)
{
    for (;;) { }                                      /* default: stop on fatal error             */
}
#endif
