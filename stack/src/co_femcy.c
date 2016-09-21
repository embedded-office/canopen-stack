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
* \file     co_femcy.c
*
* \brief    FULL EMCY PRODUCER
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_femcy.c#4 $
*
*           This source file implements the EMCY producer history.
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
*                                     LOCAL FUNCTIONS PROTOTYPES
****************************************************************************************************
*/

#if CO_EMCY_HIST_EN > 0
static CPU_INT16S CO_TEmcyRead (CO_OBJ *obj, void *buf, CPU_INT32U len);
static CPU_INT16S CO_TEmcyWrite(CO_OBJ *obj, void *buf, CPU_INT32U len);
#endif

/*
****************************************************************************************************
*                                       GLOBAL OBJECT TYPES
****************************************************************************************************
*/

#if CO_EMCY_HIST_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief OBJECT TYPE EMCY HISTORY
*
* \ingroup EMCY
*
*          This type is responsible for the access to the EMCY history.
*/
/*------------------------------------------------------------------------------------------------*/
CO_OBJ_TYPE COTEmcy = { 0, 0, 0, 0, CO_TEmcyRead, CO_TEmcyWrite };
#endif

/*
****************************************************************************************************
*                                             FUNCTIONS
****************************************************************************************************
*/

#if CO_EMCY_HIST_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  RESET EMCY HISTORY
*
* \ingroup  API
*
*           This function clears the EMCY history in the object directory.
*
* \param[in,out]   emcy          pointer to the EMCY object
*/
/*------------------------------------------------------------------------------------------------*/
void COEmcyHistReset(CO_EMCY *emcy)
{
    CO_NODE    *node;                                 /* Local: ptr to parent node                */
    CO_DIR     *cod;                                  /* Local: ptr to object directory           */
    CO_OBJ     *obj;                                  /* Local: ptr to object entry               */
    CPU_INT32U  val32;                                /* Local: value for EMCY entry              */
    CPU_INT16S  chk;                                  /* Local: result of parameter check         */
    CPU_INT08U  sub;                                  /* Local: loop cntr through subidx          */
    CPU_INT08U  val08;                                /* Local: value for EMCY number             */
                                                      /*------------------------------------------*/
    chk = CO_EmcyCheck(emcy);                         /* check given parameter emcy               */
    if (chk < 0) {                                    /* see, if an error is detected             */
        return;                                       /* abort initialization function            */
    }
    node = emcy->Node;                                /* get ptr to parent node                   */
    cod  = &node->Dir;                                /* get ptr to object directory              */
                                                      /*------------------------------------------*/
    val08 = 0;
    obj = CODirFind(cod, CO_DEV(0x1003,0));           /* get object of EMCY number                */
    if (obj == 0) {                                   /* see, if EMCY history did not exist       */
        node->Error = CO_ERR_NONE;                    /* reset error code (history is optional)   */
        return;                                       /* abort reset function                     */
    }
    (void)CO_ObjWrDirect(obj, &val08, 1);             /* clear number of occurred EMCY            */

    val32 = 0;
    for (sub = 1; sub <= emcy->Hist.Max; sub++) {     /* loop through all EMCY storage entries    */
        obj = CODirFind(cod, CO_DEV(0x1003,sub));     /* get object of EMCY entry                 */
        (void)CO_ObjWrDirect(obj, &val32, 4);         /* clear EMCY entry                         */
    }
                                                      /*------------------------------------------*/
    emcy->Hist.Max = sub - 1;                         /* set number of available EMCY entries     */
    emcy->Hist.Off = 0;                               /* set subidx for next entry                */
    emcy->Hist.Num = 0;                               /* indicate no EMCY entry in History        */
}
#endif

/*
****************************************************************************************************
*                                         INTERNAL FUNCTIONS
****************************************************************************************************
*/

#if CO_EMCY_HIST_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  INIT EMCY HISTORY
*
* \ingroup  INTERNAL
*
*           This function initializes the EMCY history management. Additionally, the object
*           entries, mandatory for EMCY history are checked for existance.
*
* \note     The parameter emcy shall be checked by the calling function.
*
* \param[in,out]   emcy          pointer to the EMCY object
*/
/*------------------------------------------------------------------------------------------------*/
void CO_EmcyHistInit(CO_EMCY *emcy)
{
    CO_NODE    *node;                                 /* Local: ptr to parent node                */
    CO_DIR     *cod;                                  /* Local: ptr to object directory           */
    CO_OBJ     *obj;                                  /* Local: ptr to object entry               */
    CPU_INT08U  sub;                                  /* Local: loop cntr through subidx          */
                                                      /*------------------------------------------*/
    emcy->Hist.Max = 0;                               /* indicate no EMCY entry available         */
    emcy->Hist.Num = 0;                               /* indicate no EMCY entry in History        */
    emcy->Hist.Off = 0;                               /* indicate no EMCY entry writable          */
                                                      /*------------------------------------------*/
    node = emcy->Node;                                /* get ptr to parent node                   */
    cod  = &node->Dir;                                /* get ptr to object directory              */
    obj  = CODirFind(cod, CO_DEV(0x1003,0));          /* try to find index for EMCY History       */
    if (obj == 0) {                                   /* see, if EMCY history did not exist       */
        node->Error = CO_ERR_NONE;                    /* reset error code (history is optional)   */
        return;                                       /* abort initialization function            */
    }
    obj  = CODirFind(cod, CO_DEV(0x1003,1));          /* try to find one entry for EMCY History   */
    if (obj == 0) {                                   /* see, if EMCY entry did not exist         */
        node->Error = CO_ERR_CFG_1003_1;              /* set error code (first entry mandatory)   */
        return;                                       /* abort initialization function            */
    }                                                 /*------------------------------------------*/
    sub = 2;                                          /* start max entry search with 2nd entry    */
    while (obj != 0) {                                /* repeat until EMCY entry is not existing  */
        obj = CODirFind(cod, CO_DEV(0x1003,sub));     /* find EMCY entry in object directory      */
        if (obj != 0) {                               /* see, if EMCY entry is found              */
            sub++;                                    /* switch to next EMCY entry                */
        }
    }                                                 /*------------------------------------------*/
    (void)CONodeGetErr(cod->Node);                    /* just to reset error                      */

    emcy->Hist.Max = sub - 1;                         /* set number of available EMCY entries     */
    emcy->Hist.Off = 0;                               /* set subidx for next entry                */
}
#endif

#if CO_EMCY_HIST_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  ADD EMCY IN HISTORY
*
* \ingroup  INTERNAL
*
*           This function appends a new EMCY entry into the history. To avoid moving the object
*           entries with each new entry, the following algorithm is implemented:
*
*           -# This function will use the EMCY Entries as ringbuffer. Each Add-function call
*              will use the next higher subindex for storage of the EMCY History informations.
*           -# The read/write access functions will remap the different subindex entry values
*              to the required order (latest EMCY add must appear at subindex 1).
*
* \note     The parameter emcy shall be checked by the calling function.
*
* \param[in,out]   emcy          pointer to the EMCY object
*
* \param[in]       err           user error identifier
*
* \param[in]       usr           manufacturer specific fields in EMCY message and/or EMCY message
*                                (only used, when \ref CO_EMCY_HIST_MAN_EN is set to 1)
*/
/*------------------------------------------------------------------------------------------------*/
void CO_EmcyHistAdd(CO_EMCY *emcy, CPU_INT08U err, CO_EMCY_USR *usr)
{
    CO_NODE    *node;                                 /* Local: ptr to parent node                */
    CO_DIR     *cod;                                  /* Local: ptr to object directory           */
    CO_OBJ     *obj;                                  /* Local: ptr to object entry               */
    CPU_INT32U  val = 0;                              /* Local: EMCY history value                */
    CPU_INT08U  sub;                                  /* Local: loop cntr through subidx          */
                                                      /*------------------------------------------*/
#if CO_EMCY_HIST_MAN_EN == 0
    (void)usr;                                        /* unused; prevent compiler warning         */
#endif
    if (emcy->Hist.Max == 0) {                        /* see, if EMCY history is not available    */
        return;                                       /* abort add function                       */
    }
    node = emcy->Node;                                /* get ptr to parent node                   */
    cod  = &node->Dir;                                /* get ptr to object directory              */
    emcy->Hist.Off++;                                 /* move last write subidx to next entry     */
    if (emcy->Hist.Off > emcy->Hist.Max) {            /* see, if last entry is passed             */
        emcy->Hist.Off = 1;                           /* switch back to first entry               */
    }                                                 /*------------------------------------------*/
    sub = emcy->Hist.Off;                             /* get next write subidx                    */
    val = (CPU_INT32U)emcy->Root[err].Code;           /* set emcy code                            */
#if CO_EMCY_HIST_MAN_EN > 0
    if (usr != 0) {                                   /* see, if user infos are given             */
        val |= (((CPU_INT32U)usr->Hist) << 16);       /* set manufacturer specific info field     */
    }
#endif
    obj = CODirFind(cod, CO_DEV(0x1003,sub));         /* get object of ringbuffer EMCY entry      */
    CO_ObjWrDirect(obj, &val, 4);                     /* write emergency in ringbufer entry       */
                                                      /*------------------------------------------*/
    emcy->Hist.Num++;                                 /* increment number of entries in history   */
    if (emcy->Hist.Num > emcy->Hist.Max) {            /* see, if max. number is passed            */
        emcy->Hist.Num = emcy->Hist.Max;              /* limit to max. number of entries          */
    } else {
        obj = CODirFind(cod, CO_DEV(0x1003,0));       /* get object of EMCY numbers               */
        CO_ObjWrDirect(obj, &(emcy->Hist.Num), 1);    /* write number of emergency in entry       */
    }
}
#endif

/*
****************************************************************************************************
*                                     LOCAL FUNCTIONS PROTOTYPES
****************************************************************************************************
*/

#if CO_EMCY_HIST_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief EMCY OBJECT READ ACCESS
*
* \ingroup INTERNAL
*
*          This function is responsible for the correct read access within the EMCY History
*          object entry (1003h).
*
* \note    When accessing the EMCY history entries, the subidx is remapped to the correct
*          value, because the ringbuffer must be transfered into a LIFO list.
*
* \param[in,out]   obj          EMCY history object entry reference
*
* \param[in,out]   buf          Pointer to buffer memory
*
* \param[in]       len          Length of buffer memory
*
* \retval    =CO_ERR_NONE       emergency entry is read
* \retval   !=CO_ERR_NONE       an error is detected and function aborted
*/
/*------------------------------------------------------------------------------------------------*/
static CPU_INT16S CO_TEmcyRead (CO_OBJ *obj, void *buf, CPU_INT32U len)
{
    CO_NODE    *node;                                 /* Local: ptr to parent node                */
    CO_DIR     *cod;                                  /* Local: ptr to object directory           */
    CO_EMCY    *emcy;                                 /* Local: ptr to EMCY structure             */
    CPU_INT16S  result = CO_ERR_NONE;                 /* Local: function result                   */
    CPU_INT08U  sub;                                  /* Local: addressed subidx                  */
    CPU_INT08U  map;                                  /* Local: mapped subidx                     */
                                                      /*------------------------------------------*/
    cod  = obj->Type->Dir;                            /* get ptr to object directory              */
    node = cod->Node;                                 /* get ptr to parent node                   */
    emcy = &node->Emcy;                               /* get ptr to EMCY management               */
    sub  = CO_GET_SUB(obj->Key);                      /* get addressed subindex                   */
                                                      /*------------------------------------------*/
    if (sub == 0) {                                   /* see, if special case: subidx 0           */
        result = CO_ObjRdDirect(obj, buf, len);       /* read number of entries directly          */
    } else {                                          /* otherwise: read within history           */
        if (sub <= emcy->Hist.Num) {                  /* see, if subidx is in range               */
            if (sub <= emcy->Hist.Off) {              /* yes: see if mapped subidx is lower       */
                map = emcy->Hist.Off - (sub - 1);
            } else {                                  /* otherwise: mapped subidx is higher       */
                map = (emcy->Hist.Max - (sub - 1)) +
                       emcy->Hist.Off;
            }                                         /*------------------------------------------*/
            obj = CODirFind(cod, CO_DEV(0x1003,map)); /* get object of mapped EMCY entry          */
            result = CO_ObjRdDirect(obj, buf, len);   /* read number of mapped entry              */
        }
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}
#endif

#if CO_EMCY_HIST_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief EMCY OBJECT WRITE ACCESS
*
* \ingroup INTERNAL
*
*          This function is responsible for the correct write access within the EMCY History
*          object entry (1003h). The access is only allowed in subidx 0; the only writeable
*          value is 0; the result should be a complete reset of the EMCY history.
*
* \param[in,out]   obj          EMCY history object entry reference
*
* \param[in]       buf          Pointer to buffer memory
*
* \param[in]       len          Length of buffer memory
*
* \retval   CO_ERR_NONE         emergency history is cleared
* \retval   CO_ERR_TYPE_WR      an error is detected and function aborted
*/
/*------------------------------------------------------------------------------------------------*/
static CPU_INT16S CO_TEmcyWrite(CO_OBJ *obj, void *buf, CPU_INT32U len)
{
    CO_NODE    *node;                                 /* Local: ptr to parent node                */
    CO_DIR     *cod;                                  /* Local: ptr to object directory           */
    CO_EMCY    *emcy;                                 /* Local: ptr to EMCY structure             */
    CPU_INT16S  result = CO_ERR_TYPE_WR;              /* Local: function result                   */
    CPU_INT08U  val    = 0;                           /* Local: EMCY history value                */
    CPU_INT08U  sub;                                  /* Local: addressed subidx                  */
                                                      /*------------------------------------------*/
    (void)len;                                        /* unused; prevent compiler warning         */
    cod  = obj->Type->Dir;                            /* get ptr to object directory              */
    node = cod->Node;                                 /* get ptr to parent node                   */
    emcy = &node->Emcy;                               /* get ptr to EMCY management               */
    sub  = CO_GET_SUB(obj->Key);                      /* get addressed subindex                   */
                                                      /*------------------------------------------*/
    if (sub == 0) {                                   /* see, if special case: subidx 0           */
        val = (CPU_INT08U)(*(CPU_INT32U*)buf);        /* get write value                          */
        if (val == 0) {                               /* see, if write value is 0                 */
            COEmcyHistReset(emcy);                    /* reset complete EMCY history              */
            result = CO_ERR_NONE;                     /* indicate successful write operation      */
        }
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}
#endif
