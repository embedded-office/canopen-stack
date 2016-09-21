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
* \file     co_para.c
*
* \brief    PARAMETER GROUP
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_para.c#2 $
*
*           This source file implements the parameter object groups.
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
*                                             DEFINES
****************************************************************************************************
*/

#if CO_OBJ_PARA_EN > 0
#define CO_PARA_STORE_SIGNATURE    0x65766173         /*!< Signature for storing parameter        */
#define CO_PARA_RESTORE_SIGNATURE  0x64616F6c         /*!< Signature for restoring parameter      */
#endif

/*
****************************************************************************************************
*                                     LOCAL FUNCTION PROTOTYPES
****************************************************************************************************
*/

#if CO_OBJ_PARA_EN > 0
static int16_t CO_ParaCheck  (CO_OBJ* obj, void *buf, uint32_t size);
static int16_t CO_TParaRead  (CO_OBJ* obj, void *buf, uint32_t size);
static int16_t CO_TParaWrite (CO_OBJ* obj, void *buf, uint32_t size);
#endif

/*
****************************************************************************************************
*                                       GLOBAL TYPE STRUCTURE
****************************************************************************************************
*/

#if CO_OBJ_PARA_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief OBJECT TYPE PARAMETER
*
* \ingroup PARA
*
*          This object type specializes the general handling of objects for the object directory
*          entries 0x1010 and 0x1011. These entries are designed to provide the store and restore
*          feature of a configurable parameter group.
*/
/*------------------------------------------------------------------------------------------------*/
CO_OBJ_TYPE COTPara = { 0, 0, 0, 0, CO_TParaRead, CO_TParaWrite };
#endif

/*
****************************************************************************************************
*                                             FUNCTIONS
****************************************************************************************************
*/

#if CO_OBJ_PARA_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief PARAMETER STORE
*
* \ingroup API
*
*          This function is responsible for the storing activities of the given parameter group.
*          The whole parameter group will be stored in NVM by calling the user callback function
*          \ref CO_ParaSave().
*
* \param[in]       pg          Ptr to parameter group info
*
* \param[in,out]   node        Ptr to node info
*/
/*------------------------------------------------------------------------------------------------*/
void COParaStore(CO_PARA *pg, CO_NODE *node)
{
    int16_t err;                                   /* Local: error code                        */
                                                      /*------------------------------------------*/
    if ((pg == 0) || (node == 0)) {                   /* see, if one of the argument ptr is bad   */
        return;                                       /* abort function                           */
    }
    if ((pg->Value & CO_PARA___E) != 0) {             /* see, if parameter group is enabled       */
        err = CO_ParaSave(pg);                        /* call application callback function       */
        if (err != CO_ERR_NONE) {                     /* see, if user detects an error            */
            node->Error = CO_ERR_PARA_STORE;          /* write error code into node error info    */
        }
    }
}
#endif

#if CO_OBJ_PARA_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief PARAMETER RESTORE DEFAULT
*
* \ingroup API
*
*          This function is responsible for removing the changes on the parameter values of the
*          given parameter group. The changes in NVM of the given parameter group will be replaced
*          with the default values by calling the user callback function \ref CO_ParaDefault().
*
* \param[in]       pg          Ptr to parameter group info
*
* \param[in,out]   node        Ptr to node info
*/
/*------------------------------------------------------------------------------------------------*/
void COParaRestore(CO_PARA *pg, CO_NODE *node)
{
    int16_t err;                                   /* Local: error code                        */
                                                      /*------------------------------------------*/
    if ((pg == 0) || (node == 0)) {                   /* see, if one of the argument ptr is bad   */
        return;                                       /* abort function                           */
    }
    if ((pg->Value & CO_PARA___E) != 0) {             /* see, if parameter group is enabled       */
        err = CO_ParaDefault(pg);                     /* call application callback function       */
        if (err != CO_ERR_NONE) {                     /* see, if user detects an error            */
            node->Error = CO_ERR_PARA_RESTORE;        /* write error code into node error info    */
        }
    }
}
#endif

/*
****************************************************************************************************
*                                           LOCAL FUNCTIONS
****************************************************************************************************
*/

#if CO_OBJ_PARA_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief CHECK PARAMETER OBJECT ACCESS
*
* \ingroup INTERNAL
*
*          This function is responsible for checking the access to parameter storage (0x1010) and
*          parameter restore default (0x1011) object entries. There are some plausibility checks
*          of given parameter and configuration. If all checks are passed, the requested activity
*          is returned with the encoding: 0 = restore parameter, 1 = store parameter.
*
* \param[in,out]   obj         Ptr to addressed object entry
*
* \param[in]       buf         Ptr to data buffer
*
* \param[in]       size        Size of given data in buffer
*
* \retval  =0      restore activity
* \retval  >0      store activity
* \retval  <0      error is detected and function aborted
*/
/*------------------------------------------------------------------------------------------------*/
static int16_t CO_ParaCheck(CO_OBJ* obj, void *buf, uint32_t size)
{
    uint32_t  signature;                            /* Local: written signature to the entry    */
    int16_t  result = CO_ERR_PARA_IDX;             /* Local: function result                   */
    CO_DIR     *cod;                                  /* Local: cached ptr to object directory    */
    int16_t  err;                                  /* Local: error code                        */
    uint8_t  num;                                  /* Local: number of entries in storage idx  */
    uint8_t  sub;                                  /* Local: addressed subindex                */
                                                      /*------------------------------------------*/
    if ((obj == 0) || (buf == 0) || (size != 4)) {    /* see, if one of the arguments is invalid  */
        return (CO_ERR_BAD_ARG);                      /* yes: abort with error indication         */
    }
    cod = obj->Type->Dir;                             /* get ptr to related object directory      */

                                                      /*------------------------------------------*/
    if (CO_GET_IDX(obj->Key) == 0x1010) {             /*       S T O R E   P A R A M E T E R      */
                                                      /*------------------------------------------*/
        err = CODirRdByte(cod,                        /* get highest number of parameter group    */
                          CO_DEV(0x1010,0),
                          &num);
        if (err != CO_ERR_NONE) {                     /* see, if an error is detected             */
            cod->Node->Error = CO_ERR_CFG_1010_0;     /* yes: set error code                      */
            return (err);                             /* abort with error indication              */
        }
                                                      /*lint -e{644} : num set in CODirRdByte     */
        if (num > 0x7F) {                             /* see, if number is out of range           */
            return (CO_ERR_CFG_1010_0);               /* abort with error indication              */
        }
        sub = CO_GET_SUB(obj->Key);                   /* get addressed subindex                   */
        if ((sub < 1) || (sub > num)) {               /* see, if addressed subidx is invalid      */
            return (CO_ERR_BAD_ARG);                  /* abort with error indication              */
        }
        signature = *((uint32_t *)buf);             /* get signature value                      */
        if (signature != CO_PARA_STORE_SIGNATURE) {   /* see, if signature is wrong               */
            return (CO_ERR_OBJ_ACC);                  /* abort with error indication              */
        }
        result = CO_ERR_NONE;                         /* indicate 'store parameter'               */
    }
                                                      /*------------------------------------------*/
    if (CO_GET_IDX(obj->Key) == 0x1011) {             /*     R E S T O R E   P A R A M E T E R    */
                                                      /*------------------------------------------*/
        err = CODirRdByte(cod,                        /* get number of parameter groups           */
                          CO_DEV(0x1011,0),
                          &num);
        if (err != CO_ERR_NONE) {                     /* see, if an error is detected             */
            cod->Node->Error = CO_ERR_CFG_1011_0;     /* yes: set error code                      */
            return (err);                             /* abort with error indication              */
        }
        if (num > 0x7F) {                             /* see, if highest subidx is out of range   */
            return (CO_ERR_CFG_1011_0);               /* abort with error indication              */
        }
        sub = CO_GET_SUB(obj->Key);                   /* get addressed subindex                   */
        if ((sub < 1) || (sub > num)) {               /* see, if addressed subidx is invalid      */
            return (CO_ERR_BAD_ARG);                  /* abort with error indication              */
        }
        signature = *((uint32_t *)buf);             /* get signature value                      */
        if (signature != CO_PARA_RESTORE_SIGNATURE) { /* see, if signature is wrong               */
            return (CO_ERR_OBJ_ACC);                  /* abort with error indication              */
        }
        result = CO_ERR_NONE;                         /* indicate 'restore parameter'             */
    }
                                                      /*------------------------------------------*/
    if (result != CO_ERR_NONE) {                      /*     B A D   P A R A M E T E R   I D X    */
                                                      /*------------------------------------------*/
        cod->Node->Error = CO_ERR_PARA_IDX;           /* yes: set error code                      */
    }
    return (result);                                  /* return function result                   */
}
#endif

#if CO_OBJ_PARA_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief PARAMETER OBJECT READ ACCESS
*
* \ingroup INTERNAL
*
*          This function is responsible for the delivery of the store- and restore-feature
*          encoding during a read access of the parameter object entry. The feature encoding
*          is static configured within the parameter group structure (\ref CO_PARA).
*
* \param[in,out]   obj         Ptr to addressed parameter object entry
*
* \param[in]       buf         Ptr to data buffer
*
* \param[in]       size        Size of given data in buffer
*
* \retval   >0     store / restore successful
* \retval  <=0     error is detected and function aborted
*/
/*------------------------------------------------------------------------------------------------*/
static int16_t CO_TParaRead(CO_OBJ* obj, void *buf, uint32_t size)
{
    CO_PARA *pg;                                      /* Local: ptr to parameter group info       */
                                                      /*------------------------------------------*/
    if ((obj == 0) || (buf == 0)) {                   /* see, if one of the arguments is invalid  */
        return (CO_ERR_BAD_ARG);                      /* yes: abort with error indication         */
    }
    if (size != CO_LONG) {                            /* see, if size is incorrect                */
        return (CO_ERR_BAD_ARG);                      /* abort function with error indication     */
    }
    pg = (CO_PARA *)obj->Data;                        /* get configured parameter group           */
    if (pg == 0) {                                    /* see, if configuration is invalid         */
        return (CO_ERR_OBJ_READ);                     /* yes: abort with error indication         */
    }
    *(uint32_t *)buf = pg->Value;                   /* set result value                         */
                                                      /*------------------------------------------*/
    return (CO_ERR_NONE);                             /* return function result                   */
}
#endif

#if CO_OBJ_PARA_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief PARAMETER OBJECT WRITE ACCESS
*
* \ingroup INTERNAL
*
*          This function is responsible for performing all neccessary activities during a write
*          access of the parameter object entry.
*
* \param[in,out]   obj         Ptr to addressed parameter object entry
*
* \param[in]       buf         Ptr to data buffer
*
* \param[in]       size        Size of given data in buffer
*
* \retval  =0      store / restore successful
* \retval  <0      error is detected and function aborted
*/
/*------------------------------------------------------------------------------------------------*/
static int16_t CO_TParaWrite(CO_OBJ* obj, void *buf, uint32_t size)
{
    CO_DIR     *cod;                                  /* Local: cached ptr to object directory    */
    CO_OBJ     *pwo;                                  /* Local: working ptr to object             */
    CO_PARA    *pg;                                   /* Local: ptr to parameter group info       */
    int16_t  select;                               /* Local: select store (1) or restore (0)   */
    uint16_t  idx;                                  /* Local: current working index             */
    uint8_t  num;                                  /* Local: number of entries in storage idx  */
    uint8_t  sub;                                  /* Local: current working subindex          */
                                                      /*------------------------------------------*/
    select = CO_ParaCheck(obj, buf, size);            /* check parameter and object config        */
    if (select != CO_ERR_NONE) {                      /* see, if an error is detected             */
        return (select);                              /* yes: abort with error indication         */
    }                                                 /*------------------------------------------*/
    cod = obj->Type->Dir;                             /* get ptr to related object directory      */
    idx = CO_GET_IDX(obj->Key);                       /* get given index                          */
    sub = CO_GET_SUB(obj->Key);                       /* get given subindex                       */
    (void)CODirRdByte(cod, CO_DEV(idx,0), &num);      /* get number of parameter groups           */

                                                      /*lint -e{644} : num set in CODirRdByte     */
    if ( (sub == 1) &&                                /* see, if special case: access _ALL_ para  */
         (num  > 1) ) {                               /*   with more than 1 para group            */
        for (sub = 2; sub < num; sub++) {             /* loop through all parameter groups        */
            pwo = CODirFind(cod, CO_DEV(idx, sub));   /* look for parameter group #<sub>          */
            if (pwo != 0) {                           /* see, if parameter group is found         */
                pg = (CO_PARA *)pwo->Data;            /* get configured parameter group infos     */
                if (idx == 0x1011) {                  /* yes: see, if restore is selected         */
                    COParaRestore(pg, cod->Node);     /* restore parameter                        */
                } else {                              /* otherwise: storing is selected           */
                    COParaStore(pg, cod->Node);       /* store parameter                          */
                }
            }
        }
    } else {                                          /* otherwise: save addressed para group     */
        pg = (CO_PARA *)obj->Data;                    /* get configured parameter group infos     */
        if (idx == 0x1011) {                          /* yes: see, if restore is selected         */
            COParaRestore(pg, cod->Node);             /* restore parameter                        */
        } else {                                      /* otherwise: storing is selected           */
            COParaStore(pg, cod->Node);               /* store parameter                          */
        }
    }                                                 /*------------------------------------------*/
    return (CO_ERR_NONE);                             /* return function result                   */
}
#endif

/*
****************************************************************************************************
*                                         CALLBACK FUNCTIONS
****************************************************************************************************
*/

#if CO_OBJ_PARA_EN > 0 && CO_CB_PARA_LOAD_EN == 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief LOAD PARAMETER VALUES CALLBACK
*
* \ingroup CALLBACK
*
*          This callback function will be called during reset and powerup events. The function
*          is responsible for loading the parameter from NVM into the parameter group memory.
*
* \note    This implementation is an example implementation, which will copy the given default
*          memory to the parameters. This function is application specific and must be implemented
*          somewhere in the application code. The activation of the application callback function
*          is done with \ref CO_CB_PARA_LOAD_EN.
*
* \note    The parameter group info pointer is checked to be valid before calling this function.
*
* \param[in]       pg          Ptr to parameter group info
*
* \retval  =0      parameter loading successful
* \retval  <0      error is detected and function aborted
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_ParaLoad(CO_PARA *pg)
{
    uint8_t *ptr;                                  /* Local: ptr within parameter memory block */
    uint8_t *src;                                  /* Local: ptr within default memory block   */
    uint32_t  num;                                  /* Local: loop counter                      */
                                                      /*------------------------------------------*/
    ptr   = pg->Start;                                /* set ptr to parameter memory block start  */
    src   = pg->Default;                              /* set src to default memory block start    */
    for (num = 0; num < pg->Size; num++) {            /* loop through whole memory block          */
        *ptr = *src;                                  /* set data in parameter memory block       */
        ptr++;                                        /* switch to next parameter memory cell     */
        src++;                                        /* switch to next default memory cell       */
    }                                                 /*------------------------------------------*/
    return (CO_ERR_NONE);                             /* return function result                   */
}
#endif

#if CO_OBJ_PARA_EN > 0 && CO_CB_PARA_SAVE_EN == 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief SAVE PARAMETER VALUES CALLBACK
*
* \ingroup CALLBACK
*
*          This callback function will be called during storing a parameter group. The function
*          is responsible for saving the current parameter group memory into NVM.
*
* \note    This implementation is an example implementation, which will do nothing. This function
*          is application specific and must be implemented somewhere in the application code. The
*          activation of the application callback function is done with \ref CO_CB_PARA_SAVE_EN.
*
* \note    The parameter group info pointer is checked to be valid before calling this function.
*
* \param[in]       pg          Ptr to parameter group info
*
* \retval  =0      parameter loading successful
* \retval  <0      error is detected and function aborted
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_ParaSave(CO_PARA *pg)
{
    (void)pg;                                         /* unused; prevent compiler warnging        */

    return (CO_ERR_NONE);                             /* return function result                   */
}
#endif

#if CO_OBJ_PARA_EN > 0 && CO_CB_PARA_DEFAULT_EN == 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief SET DEFAULT PARAMETER VALUES CALLBACK
*
* \ingroup CALLBACK
*
*          This callback function will be called during restoring the default values of a
*          parameter group. The function is responsible for setting the factory defaults in
*          the current parameter group memory.
*
* \note    This implementation is an example implementation, which will copy the given default
*          memory to the parameters. This function is application specific and must be implemented
*          somewhere in the application code. The activation of the application callback function
*          is done with \ref CO_CB_PARA_DEFAULT_EN.
*
* \note    The parameter group info pointer is checked to be valid before calling this function.
*
* \param[in]       pg          Ptr to parameter group info
*
* \retval  =0      parameter loading successful
* \retval  <0      error is detected and function aborted
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_ParaDefault(CO_PARA *pg)
{
    uint8_t *ptr;                                  /* Local: ptr within parameter memory block */
    uint8_t *src;                                  /* Local: ptr within default memory block   */
    uint32_t  num;                                  /* Local: loop counter                      */
                                                      /*------------------------------------------*/
    ptr   = pg->Start;                                /* set ptr to parameter memory block start  */
    src   = pg->Default;                              /* set src to default memory block start    */
    for (num = 0; num < pg->Size; num++) {            /* loop through whole memory block          */
        *ptr = *src;                                  /* set data in parameter memory block       */
        ptr++;                                        /* switch to next parameter memory cell     */
        src++;                                        /* switch to next default memory cell       */
    }                                                 /*------------------------------------------*/
    return (CO_ERR_NONE);                             /* return function result                   */
}
#endif
