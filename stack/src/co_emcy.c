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
* \file     co_emcy.c
*
* \brief    EMCY PRODUCER
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_emcy.c#3 $
*
*           This source file implements the EMCY producer.
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

#if CO_EMCY_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  SET EMCY ERROR
*
* \ingroup  API
*
*           This function checks the current error state and updates the object directory.
*           The EMCY message is transmitted, if the error is detected for the first time.
*           The given manufacturer specific fields are optional, e.g. the ptr may be 0 to
*           set all manufacturer specific values to 0.
*
* \param[in,out]   emcy          pointer to the EMCY object
*
* \param[in]       err           EMCY error identifier in User EMCY table
*
* \param[in]       usr           manufacturer specific fields in EMCY history and/or EMCY message
*/
/*------------------------------------------------------------------------------------------------*/
void COEmcySet(CO_EMCY *emcy, CPU_INT08U err, CO_EMCY_USR *usr)
{
    CPU_INT16S chk;                                   /* Local: result of parameter check         */
    CPU_INT16S change;                                /* Local: current error change indication   */
                                                      /*------------------------------------------*/
    chk = CO_EmcyCheck(emcy);                         /* check parameter emcy to be valid         */
    if (chk < 0) {                                    /* see, if parameter emcy is bad            */
        return;                                       /* error is registered (if possible)        */
    }
    change = CO_EmcySetErr(emcy, err, 1);             /* set error detection marker               */
    if (change > 0) {                                 /* see, if a error change is detected       */
        CO_EmcyUpdate(emcy, err, usr, 1);             /* update object directory entries          */
        CO_EmcySend  (emcy, err, usr, 1);             /* send EMCY message                        */
    }
}
#endif

#if CO_EMCY_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  CLEAR EMCY ERROR
*
* \ingroup  API
*
*           This function checks the current error state and updates the object directory.
*           The EMCY message is transmitted, if the error was previously detected.
*
* \param[in,out]   emcy          pointer to the EMCY object
*
* \param[in]       err           EMCY error identifier in User EMCY table
*/
/*------------------------------------------------------------------------------------------------*/
void COEmcyClr(CO_EMCY *emcy, CPU_INT08U err)
{
    CPU_INT16S chk;                                   /* Local: result of parameter check         */
    CPU_INT16S change;                                /* Local: current error change indication   */
                                                      /*------------------------------------------*/
    chk = CO_EmcyCheck(emcy);                         /* check parameter emcy to be valid         */
    if (chk < 0) {                                    /* see, if parameter emcy is bad            */
        return;                                       /* error is registered (if possible)        */
    }
    change = CO_EmcySetErr(emcy, err, 0);             /* clear error detection marker             */
    if (change > 0) {                                 /* see, if a error change is detected       */
        CO_EmcyUpdate(emcy, err, 0, 0);               /* update object directory entries          */
        CO_EmcySend  (emcy, err, 0, 0);               /* send EMCY message                        */
    }
}
#endif

#if CO_EMCY_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  GET EMCY ERROR STATUS
*
* \ingroup  API
*
*           This function returns the current EMCY error status.
*
* \param[in,out]   emcy          pointer to the EMCY object
*
* \param[in]       err           EMCY error identifier in User EMCY table
*
* \retval   =0     the error is not detected
* \retval   =1     the error was detected before
* \retval   <0     an error is detected inside of this function
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S COEmcyGet(CO_EMCY *emcy, CPU_INT08U err)
{
    CPU_INT16S chk;                                   /* Local: result of parameter check         */
    CPU_INT16S cur;                                   /* Local: current error detection marker    */
                                                      /*------------------------------------------*/
    chk = CO_EmcyCheck(emcy);                         /* check parameter emcy to be valid         */
    if (chk < 0) {                                    /* see, if parameter emcy is bad            */
        return (-1);                                  /* error is registered (if possible)        */
    }
    cur = CO_EmcyGetErr(emcy, err);                   /* get current error detection marker       */
                                                      /*------------------------------------------*/
    return (cur);                                     /* return function result                   */
}
#endif

#if CO_EMCY_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  COUNT DETECTED EMCY ERRORS
*
* \ingroup  API
*
*           This function returns the number of currently detected EMCY errors.
*
* \param[in,out]   emcy          pointer to the EMCY object
*
* \retval   >=0    the number of detected EMCY errors
* \retval   <0     an error is detected inside of this function
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S COEmcyCnt(CO_EMCY *emcy)
{
    CPU_INT16S chk;                                   /* Local: result of parameter check         */
    CPU_INT16S result = -1;                           /* Local: function result                   */
    CPU_INT08U n;                                     /* Local: loop through error classes        */
                                                      /*------------------------------------------*/
    chk = CO_EmcyCheck(emcy);                         /* check parameter emcy to be valid         */
    if (chk < 0) {                                    /* see, if parameter emcy is bad            */
        return (result);                              /* error is registered (if possible)        */
    }
    result = 0;                                       /* reset error counter                      */
    for (n=0; n < CO_EMCY_REG_NUM; n++) {             /* loop through all error classes           */
        result += emcy->Cnt[n];                       /* add number of pending errors             */
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return sum of pending errors             */
}
#endif

#if CO_EMCY_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  RESET ALL EMCY ERRORS
*
* \ingroup  API
*
*           This function clears all EMCY errors. The EMCY message transmission can be
*           suppressed by setting the parameter silent to 1.
*
* \param[in,out]   emcy          pointer to the EMCY object
*
* \param[in]       silent        Disables the EMCY message transmission for the state changes,
*                                made by this function
*/
/*------------------------------------------------------------------------------------------------*/
void COEmcyReset(CO_EMCY *emcy, CPU_INT08U silent)
{
    CPU_INT16S change;                                /* Local: current error change indication   */
    CPU_INT16S chk;                                   /* Local: result of parameter check         */
    CPU_INT08U n;                                     /* Local: loop counter                      */
                                                      /*------------------------------------------*/
    chk = CO_EmcyCheck(emcy);                         /* check parameter emcy to be valid         */
    if (chk < 0) {                                    /* see, if parameter emcy is bad            */
        return;                                       /* error is registered (if possible)        */
    }
    for (n=0; n < CO_EMCY_N; n++) {                   /* loop through all EMCY errors             */
        if (silent == 0) {                            /* see, if not silent operation requested   */
            COEmcyClr(emcy, n);                       /* clear EMCY errors                        */
        } else {                                      /* otherwise: clear without sending changes */
            change = CO_EmcySetErr(emcy, n, 0);       /* clear error detection marker             */
            if (change > 0) {                         /* see, if a error change is detected       */
                CO_EmcyUpdate(emcy, n, 0, 0);         /* update object directory entries          */
            }
        }
    }
}
#endif

/*
****************************************************************************************************
*                                        INTERNAL FUNCTIONS
****************************************************************************************************
*/

#if CO_EMCY_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  INIT EMCY MANAGEMENT
*
* \ingroup  INTERNAL
*
*           This function initializes the EMCY object and the internal error storage memory.
*           Additionally, the links to the given user EMCY error codes to the given node are set.
*
* \param[in,out]   emcy          pointer to the EMCY object
*
* \param[in,out]   node          pointer to the CANopen device node information structure
*
* \param[in]       root          pointer to the start of the User EMCY table
*/
/*------------------------------------------------------------------------------------------------*/
void CO_EmcyInit(CO_EMCY *emcy, CO_NODE *node, CO_EMCY_TBL *root)
{
    CO_OBJ     *obj;                                  /* Local: pointer to object entry           */
    CPU_INT16U  n;                                    /* Local: loop counter                      */
    CPU_INT32U  size;
                                                      /*------------------------------------------*/
    if ((root == 0) || (node == 0) || (emcy == 0)) {  /* see, if any parameter pointer is invalid */
        CO_NodeFatalError();                          /* inform user                              */
        return;                                       /* abort EMCY initialization                */
    }                                                 /*------------------------------------------*/
    emcy->Root = root;                                /* store appl. EMCY code table reference    */
    emcy->Node = node;                                /* link parent node to EMCY structure       */

    for (n=0; n < CO_EMCY_STORAGE; n++) {             /* clear detailed error storage memory      */
        emcy->Err[n] = 0;
    }
    for (n=0; n < CO_EMCY_REG_NUM; n++) {             /* clear all register bit counters          */
        emcy->Cnt[n] = 0;
    }                                                 /*------------------------------------------*/
    obj = CODirFind(&node->Dir, CO_DEV(0x1014,0));    /* check existance of EMCY identifier       */
    if (obj == 0) {                                   /* see, if an error is detected             */
        node->Error = CO_ERR_CFG_1014_0;              /* yes: set error code                      */
    } else {
        size = COObjGetSize(obj, CO_LONG);            /* check object size matches expectation    */
        if (size == 0) {                              /* see, if an error is detected             */
            node->Error = CO_ERR_CFG_1014_0;          /* yes: set error code                      */
        }
    }                                                 /*------------------------------------------*/
    obj = CODirFind(&node->Dir, CO_DEV(0x1001,0));    /* check existance of error register        */
    if (obj == 0) {                                   /* see, if an error is detected             */
        node->Error = CO_ERR_CFG_1001_0;              /* yes: set error code                      */
    } else {
        size = COObjGetSize(obj, CO_BYTE);            /* check object size matches expectation    */
        if (size == 0) {                              /* see, if an error is detected             */
            node->Error = CO_ERR_CFG_1001_0;          /* yes: set error code                      */
        }
    }                                                 /*------------------------------------------*/

#if CO_EMCY_HIST_EN > 0
    CO_EmcyHistInit(emcy);                            /* initialize EMCY history object entries   */
#endif
}
#endif

#if CO_EMCY_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  CHECK EMCY PARAMETER STRUCTURE
*
* \ingroup  INTERNAL
*
*           This function helps to centralize the parameter checking of the emcy structure.
*
* \param[in,out]   emcy          pointer to the EMCY object
*
* \retval   =0     the given parameter emcy is ok
* \retval   <0     an error is detected in parameter emcy
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CO_EmcyCheck(CO_EMCY *emcy)
{
    CO_NODE    *node;                                 /* Local: ptr to parent node                */
    CPU_INT16S  result = -1;                          /* Local: function result                   */
                                                      /*------------------------------------------*/
    if (emcy == 0) {                                  /* see, if parameter pointer is invalid     */
        CO_NodeFatalError();                          /* inform user                              */
        return (result);                              /* abort function                           */
    }
    node = emcy->Node;                                /* get ptr to parent node                   */
    if (node == 0) {                                  /* see, if node pointer is invalid          */
        CO_NodeFatalError();                          /* inform user                              */
        return (result);                              /* abort function                           */
    }
    if (emcy->Root == 0) {                            /* see, if root pointer is invalid          */
        node->Error = CO_ERR_EMCY_BAD_ROOT;           /* set error indication                     */
        return (result);                              /* abort function                           */
    }                                                 /*------------------------------------------*/
    result = 0;                                       /* all checks passed                        */
    return (result);                                  /* return function result                   */
}
#endif

#if CO_EMCY_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  GET INTERNAL ERROR STATE
*
* \ingroup  INTERNAL
*
*           This function gets the error state of the given error identifier out of the internal
*           error storage memory.
*
* \note     The parameter emcy shall be checked by the calling function.
*
* \param[in]       emcy          pointer to the EMCY object
*
* \param[in]       err           user error identifier
*
* \retval   =0     this error is not present
* \retval   =1     the error is already detected
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CO_EmcyGetErr(CO_EMCY *emcy, CPU_INT08U err)
{
    CPU_INT08U result;                                /* Local: function result                   */
    CPU_INT08U byte;                                  /* Local: calculated byte in error memory   */
    CPU_INT08U mask;                                  /* Local: calculated bit mask in the byte   */
                                                      /*------------------------------------------*/
    if (err >= CO_EMCY_N) {                           /* see, if error identifier is invalid      */
        err = CO_EMCY_N - 1;                          /* limit to last entry in user table        */
    }                                                 /*------------------------------------------*/
    byte = err >> 3;                                  /* calculate byte, holding the given error  */
    mask = (CPU_INT08U)1 << (err & 0x7);              /* calculate mask, representing the error   */
    if ((emcy->Err[byte] & mask) == 0) {              /* see, if this error is currently not set  */
        result = 0;                                   /* set result to 'error not detected'       */
    } else {                                          /* otherwise, this error is currently set   */
        result = 1;                                   /* set result to 'error present'            */
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}
#endif

#if CO_EMCY_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  SET INTERNAL ERROR STATE
*
* \ingroup  INTERNAL
*
*           This function sets the error state of the given error identifier into the internal
*           error storage memory.
*
* \note     The parameter emcy shall be checked by the calling function.
*
* \param[in,out]   emcy          pointer to the EMCY object
*
* \param[in]       err           user error identifier
*
* \param[in]       state         error state (=0: no error, !=0: error)
*
* \retval   =0     error state is unchanged
* \retval   =1     the error state has changed
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CO_EmcySetErr(CO_EMCY *emcy, CPU_INT08U err, CPU_INT08U state)
{
    CPU_INT08U result;                                /* Local: function result                   */
    CPU_INT08U byte;                                  /* Local: calculated byte in error memory   */
    CPU_INT08U mask;                                  /* Local: calculated bit mask in the byte   */
                                                      /*------------------------------------------*/
    if (err >= CO_EMCY_N) {                           /* see, if error identifier is invalid      */
        err = CO_EMCY_N - 1;                          /* limit to last entry in user table        */
    }                                                 /*------------------------------------------*/
    byte = err >> 3;                                  /* calculate byte, holding the given error  */
    mask = (CPU_INT08U)1 << (err & 0x7);              /* calculate mask, representing the error   */
    if ((emcy->Err[byte] & mask) == 0) {              /* see, if this error is currently not set  */
        if (state != 0) {                             /* yes: see, if new error state is set      */
            emcy->Err[byte] |= mask;                  /* yes: mark error detected in memory       */
            result           = 1;                     /* indicate error state change              */
        } else {                                      /* otherwise: both states indicates not set */
            result           = 0;                     /* indicate same error state as before      */
        }
    } else {                                          /* otherwise: error is currently set        */
        if (state == 0) {                             /* yes: see, if new error state is cleared  */
            emcy->Err[byte] &= ~mask;                 /* yes: clear detected error in memory      */
            result           = 1;                     /* indicate error state change              */
        } else {                                      /* otherwise: both states indicates set     */
            result           = 0;                     /* indicate same error state as before      */
        }
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}
#endif

#if CO_EMCY_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  SEND EMCY MESSAGE
*
* \ingroup  INTERNAL
*
*           This function transmits the EMCY message to the configured CANbus.
*
* \note     The parameter emcy shall be checked by the calling function.
*
* \param[in,out]   emcy          pointer to the EMCY object
*
* \param[in]       err           user error identifier
*
* \param[in]       usr           manufacturer specific fields in EMCY message and/or EMCY message
*                                (only used, when \ref CO_EMCY_EMCY_MAN_EN is set to 1)
*
* \param[in]       state         error state (=0: no error, !=0: error)
*/
/*------------------------------------------------------------------------------------------------*/
void CO_EmcySend(CO_EMCY *emcy, CPU_INT08U err, CO_EMCY_USR *usr, CPU_INT08U state)
{
    CO_IF_FRM    frm;                                 /* Local: CAN frame for transmission        */
    CO_NODE     *node;                                /* Local: pointer to node informations      */
    CO_DIR      *dir;                                 /* Local: pointer to object directory       */
    CO_EMCY_TBL *data;                                /* Local: pointer to user error field table */
    CPU_INT08U   n;                                   /* Local: loop through user error field     */
                                                      /*------------------------------------------*/
#if CO_EMCY_EMCY_MAN_EN == 0
    (void)usr;                                        /* unused, prevent compiler warning         */
#endif

    node = emcy->Node;                                /* get node pointer                         */
    if ((node->Nmt.Allowed & CO_EMCY_ALLOWED) == 0) { /* see, if EMCY messages are not allowed    */
       return;                                        /* exit function without error              */
    }
    if (err >= CO_EMCY_N) {                           /* see, if error identifier is invalid      */
        err = CO_EMCY_N - 1;                          /* limit to last entry in user table        */
    }                                                 /*------------------------------------------*/
    dir  = &node->Dir;                                /* get link to object directory             */
    data = &emcy->Root[err];                          /* get entry of addressed user error field  */
                                                      /*------------------------------------------*/
    (void)CODirRdLong(dir, CO_DEV(0x1014,0),          /* read EMCY identifier (existance checked) */
                      &frm.Identifier);
    frm.DLC = 8;                                      /* fixed DLC (8 bytes)                      */
    if (state == 1) {                                 /* see, if new error is detected            */
        frm.Data[0] = (CPU_INT08U)(data->Code);       /* set pre-defined error code in message    */
        frm.Data[1] = (CPU_INT08U)(data->Code >> 8);
    } else {
        frm.Data[0] = (CPU_INT08U)0;                  /* set error code 'no error' in message     */
        frm.Data[1] = (CPU_INT08U)0;
    }
    (void)CODirRdByte(dir, CO_DEV(0x1001,0),          /* set error register (existance checked)   */
                      &frm.Data[2]);
    for (n=0; n<5; n++) {                             /* loop through manufacturer specific field */
        frm.Data[3+n] = 0;                            /* clear user error codes in message        */
    }
#if CO_EMCY_EMCY_MAN_EN > 0
    if (usr != 0) {                                   /* see, if additional infos are given       */
        for (n=0; n<5; n++) {                         /* loop through manufacturer specific field */
            frm.Data[3+n] = usr->Emcy[n];             /* copy user error codes in message         */
        }
    }                                                 /*------------------------------------------*/
#endif
    (void)COIfSend(&emcy->Node->If, &frm);            /* transmit EMCY message                    */
                                                      /* possible error registered in node        */
}
#endif

#if CO_EMCY_N > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  UPDATE EMCY OBJECT ENTRIES
*
* \ingroup  INTERNAL
*
*           This function updates the EMCY object directory entries with the given error
*           code change.
*
* \note     The parameter emcy shall be checked by the calling function.
*
* \param[in,out]   emcy          pointer to the EMCY object
*
* \param[in]       err           user error identifier
*
* \param[in]       usr           manufacturer specific fields in EMCY history and/or EMCY message
*                                (only used, when \ref CO_EMCY_HIST_EN is set to 1)
*
* \param[in]       state         error state (=0: no error, !=0: error)
*/
/*------------------------------------------------------------------------------------------------*/
void CO_EmcyUpdate(CO_EMCY *emcy, CPU_INT08U err, CO_EMCY_USR *usr, CPU_INT08U state)
{
    CO_DIR     *dir;                                  /* Local: pointer to object directory       */
    CPU_INT08U  regbit;                               /* Local: bit in error register             */
    CPU_INT08U  regmask;                              /* Local: bit mask for error register       */
    CPU_INT08U  reg;                                  /* Local: error register value              */
                                                      /*------------------------------------------*/
#if CO_EMCY_HIST_EN == 0
    (void)usr;                                        /* unused, prevent compiler warning         */
#endif

    if (err >= CO_EMCY_N) {                           /* see, if error identifier is invalid      */
        err = CO_EMCY_N - 1;                          /* limit to last entry in user table        */
    }                                                 /*------------------------------------------*/
    dir     = &emcy->Node->Dir;                       /* get link to object directory             */
    regbit  =  emcy->Root[err].Reg;                   /* get corresponding bit in error register  */
    regmask =  (CPU_INT08U)(1u << regbit);            /* calculate register mask                  */
                                                      /*------------------------------------------*/
    (void)CODirRdByte(dir, CO_DEV(0x1001,0), &reg);   /* get error register (existance checked)   */
                                                      /*------------------------------------------*/
    if (state != 0) {                                 /*                SET ERROR                 */
                                                      /*------------------------------------------*/
                                                      /*lint -e{644} : reg set in CODirRdByte()   */
        if ((reg & regmask) == 0) {                   /* see, if this error class is not set      */
            reg |= regmask;                           /* yes: set error class in error register   */
            reg |= 0x01;                              /* generic error is set for any error       */
        }
#if CO_EMCY_HIST_EN > 0
        CO_EmcyHistAdd(emcy, err, usr);               /* add this EMCY to EMCY history            */
#endif
        emcy->Cnt[regbit]++;                          /* count occurance of this error class      */
                                                      /*------------------------------------------*/
    } else {                                          /*               CLEAR ERROR                */
                                                      /*------------------------------------------*/
        emcy->Cnt[regbit]--;                          /* decrement number of occurance            */
        if (emcy->Cnt[regbit] == 0) {                 /* see, if error was the last in this class */
            reg &= ~regmask;                          /* yes: clear error class in error register */
#if CO_EMCY_REG_CLASS_EN > 0
            if ((reg & ~0x01) != 0) {                 /* see, if not all error classes are cleared*/
                reg |= 0x01;                          /* ensure that generic error stays till end */
            } else {                                  /* otherwise: all error classes are cleared */
                if (emcy->Cnt[0] == 0) {              /* see, if no generic error is pending      */
                    reg &= ~0x01;                     /* yes: clear generic error register bit    */
                }
            }
#endif
        }
    }
    (void)CODirWrByte(dir, CO_DEV(0x1001,0), reg);    /* set error register (existance checked)   */
}
#endif
