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
* \file     co_fpdo.c
*
* \brief    FULL PDO HANDLING
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_fpdo.c#4 $
*
*           This source file implements the missing parts to get a fully compliant PDO producer
*           and consumer.
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

#if CO_RPDO_DYN_MAP_EN > 0 || CO_TPDO_DYN_MAP_EN > 0
static CPU_INT16S CO_TPdoMapNumWrite  (CO_OBJ* obj, void *buf, CPU_INT32U size);
static CPU_INT16S CO_TPdoMapWrite     (CO_OBJ* obj, void *buf, CPU_INT32U size);
#endif

#if CO_RPDO_DYN_COM_EN > 0 || CO_TPDO_DYN_COM_EN > 0
static CPU_INT16S CO_TPdoComIdWrite   (CO_OBJ* obj, void *buf, CPU_INT32U size);
static CPU_INT16S CO_TPdoComTypeWrite (CO_OBJ* obj, void *buf, CPU_INT32U size);
#endif

/*
****************************************************************************************************
*                                       GLOBAL TYPE STRUCTURE
****************************************************************************************************
*/

#if CO_RPDO_DYN_MAP_EN > 0 || CO_TPDO_DYN_MAP_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief OBJECT TYPE: NUMBER OF PDO MAPPINGS
*
* \ingroup OBJ
*
*          This type is responsible for the access to the number of PDO mapping entries. Due to
*          the special handling of PDO mapping change accesses, only the write access needs to
*          be handled via the type structure; the read operation can be performed directly.
*/
/*------------------------------------------------------------------------------------------------*/
CO_OBJ_TYPE COTPdoMapN = { 0, 0, 0, 0, 0, CO_TPdoMapNumWrite };
#endif

#if CO_RPDO_DYN_MAP_EN > 0 || CO_TPDO_DYN_MAP_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief OBJECT TYPE: PDO MAPPING
*
* \ingroup OBJ
*
*          This type is responsible for the access to the PDO mapping entries. Due to the
*          special handling of PDO mapping change accesses, only the write access needs to
*          be handled via the type structure; the read operation can be performed directly.
*/
/*------------------------------------------------------------------------------------------------*/
CO_OBJ_TYPE COTPdoMap = { 0, 0, 0, 0, 0, CO_TPdoMapWrite };
#endif

#if CO_RPDO_DYN_COM_EN > 0 || CO_TPDO_DYN_COM_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief OBJECT TYPE: PDO IDENTIFIER
*
* \ingroup OBJ
*
*          This type is responsible for the access to the PDO communication entries. Due to the
*          special handling of PDO communication change accesses, only the write access needs to
*          be handled via the type structure; the read operation can be performed directly.
*/
/*------------------------------------------------------------------------------------------------*/
CO_OBJ_TYPE COTPdoId = { 0, 0, 0, 0, 0, CO_TPdoComIdWrite };
#endif

#if CO_RPDO_DYN_COM_EN > 0 || CO_TPDO_DYN_COM_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief OBJECT TYPE: PDO TRANSMISSION TYPE
*
* \ingroup OBJ
*
*          This type is responsible for the access to the PDO communication entries. Due to the
*          special handling of PDO communication change accesses, only the write access needs to
*          be handled via the type structure; the read operation can be performed directly.
*/
/*------------------------------------------------------------------------------------------------*/
CO_OBJ_TYPE COTPdoType = { 0, 0, 0, 0, 0, CO_TPdoComTypeWrite };
#endif

/*
****************************************************************************************************
*                                      OBJECT ACCESS FUNCTIONS
****************************************************************************************************
*/

#if CO_RPDO_DYN_MAP_EN > 0 || CO_TPDO_DYN_MAP_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  WRITE NUMPER OF PDO MAPPINGS
*
* \ingroup  INTERNAL
*
*           This function allows the write access to the number of PDO mappings within
*           the PDO mapping profile.
*
* \param[in,out]   obj          Pointer to PDO number of mappings object
*
* \param[in]       buf          Pointer to write data value
*
* \param[in]       size         Object size in byte
*
* \retval         =CO_ERR_NONE  Successfully operation
* \retval        !=CO_ERR_NONE  An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
static CPU_INT16S CO_TPdoMapNumWrite(CO_OBJ* obj, void *buf, CPU_INT32U size)
{
    CO_DIR      *cod;                                 /* Local: pointer to object directory       */
    CPU_INT32U   id;                                  /* Local: current identifier                */
    CPU_INT32U   mapentry;                            /* Local: mapping entry                     */
    CPU_INT16S   result = CO_ERR_NONE;                /* Local: function result                   */
    CPU_INT16U   pmapidx;                             /* Local: object mapping entry index        */
    CPU_INT16U   pcomidx;                             /* Local: object communication entry index  */
    CPU_INT08U   mapbytes;                            /* Local: number of mapped bytes            */
    CPU_INT08U   mapnum;                              /* Local: new number of maps to be written  */
    CPU_INT08U   i;                                   /* Local: counter                           */
                                                      /*------------------------------------------*/
    if ((obj == 0) ||                                 /* see, if any parameter pointer is invalid */
        (buf == 0) || (size != CO_LONG)) {
        return (CO_ERR_BAD_ARG);                      /* indicate error to caller                 */
    }
    if (CO_GET_SUB(obj->Key) != 0) {                  /* see, if subidx is not number of mapping  */
        return (CO_ERR_TPDO_MAP_OBJ);                 /* indicate error to caller                 */
    }                                                 /*------------------------------------------*/
    mapnum  = (CPU_INT08U)(*(CPU_INT32U *)buf);       /* get new number of mapping                */
    if (mapnum > 8) {                                 /* see, if number is out of range           */
        return (CO_ERR_OBJ_MAP_LEN);                  /* indicate error to caller                 */
    }

    cod     = obj->Type->Dir;                         /* get pointer to object directory          */
    pmapidx = CO_GET_IDX(obj->Key);                   /* get index of object                      */
    if ( (pmapidx >= 0x1600) &&                       /* see, if index is in RPDO communication   */
         (pmapidx <= 0x17FF) ) {

    } else if ( (pmapidx >= 0x1A00) &&                /* otherwise, see, if in TPDO communication */
                (pmapidx <= 0x1BFF) ) {

    } else {                                          /* otherwise, not in any PDO communication  */
        return (CO_ERR_TPDO_MAP_OBJ);                 /* indicate error to caller                 */
    }

    pcomidx = pmapidx - 0x200;                        /* calculate corresponding PDO com. profile */
    (void)CODirRdLong(cod, CO_DEV(pcomidx,1), &id);   /* get current identifier value             */
    if ((id & CO_TPDO_COBID_OFF) == 0) {              /* see, if current id indicates PDO valid   */
        result = CO_ERR_OBJ_ACC;                      /* indicate error; access not allowed       */
    } else {                                          /* otherwise: current PDO is invalid        */

                                                      /* check for number of mapped bytes         */
        mapbytes = 0;                                 /* clear byte counter                       */

        for (i = 1; i <= mapnum; i++) {               /* loop through entries to be mapped        */

            result = CODirRdLong(cod,                 /* get mapping entry                        */
                                 CO_DEV(pmapidx,i),
                                 &mapentry);

            if (result != CO_ERR_NONE) {              /* if mapping parameter not readable        */
                return (CO_ERR_OBJ_MAP_TYPE);         /* incicate error                           */
            }

            mapbytes += CO_GET_SIZE(mapentry);        /* get size and add                         */
        }

        if (mapbytes > 8) {                           /* if more than 8 bytes to be mapped        */
            return (CO_ERR_OBJ_MAP_LEN);              /* indicate error to caller; cannot be mapped */
        }                                             /*------------------------------------------*/

        result = CO_ObjWrDirect(obj, &mapnum,         /* set new mapping value                    */
                                CO_LONG);
    }
    return (result);
}
#endif

#if CO_RPDO_DYN_MAP_EN > 0 || CO_TPDO_DYN_MAP_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  WRITE PDO MAPPING
*
* \ingroup  INTERNAL
*
*           This function allows the write access to the PDO mapping within
*           the PDO mapping profile.
*
* \param[in,out]   obj          Pointer to PDO mapping object
*
* \param[in]       buf          Pointer to write data value
*
* \param[in]       size         Object size in byte
*
* \retval         =CO_ERR_NONE  Successfully operation
* \retval        !=CO_ERR_NONE  An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
static CPU_INT16S CO_TPdoMapWrite(CO_OBJ *obj, void *buf, CPU_INT32U size)
{
    CO_DIR      *cod;                                 /* Local: pointer to object directory       */
    CO_OBJ      *objm;                                /* Local: ptr to map object entry           */
    CPU_INT32U   map;                                 /* Local: new type to be written            */
    CPU_INT32U   id;                                  /* Local: current identifier                */
    CPU_INT32U   maps;                                /* Local: buffer for supported features     */
    CPU_INT16S   result = CO_ERR_NONE;                /* Local: function result                   */
    CPU_INT16U   pmapidx;                             /* Local: object mapping entry index        */
    CPU_INT16U   pcomidx;                             /* Local: object communication entry index  */
    CPU_INT08U   mapn;                                /* Local: number of mapping entries         */
                                                      /*------------------------------------------*/
    (void)size;                                       /* prevent compiler warning                 */
                                                      /*------------------------------------------*/
    if ((obj == 0) || (buf == 0)) {                   /* see, if any parameter pointer is invalid */
        return (CO_ERR_BAD_ARG);                      /* indicate error to caller                 */
    }
    if (CO_GET_SUB(obj->Key) < 1) {                   /* see, if subindex is not a mapping subidx */
        return (CO_ERR_TPDO_MAP_OBJ);                 /* indicate error to caller                 */
    }                                                 /*------------------------------------------*/

    cod     = obj->Type->Dir;                         /* get pointer to object directory          */
    pmapidx = CO_GET_IDX(obj->Key);                   /* get index of object                      */
    if ( (pmapidx >= 0x1600) &&                       /* see, if index is in RPDO mapping         */
         (pmapidx <= 0x17FF) ) {

    } else if ( (pmapidx >= 0x1A00) &&                /* otherwise, see, if in TPDO mapping       */
                (pmapidx <= 0x1BFF) ) {

    } else {                                          /* otherwise, not in any PDO mapping        */
        return (CO_ERR_TPDO_MAP_OBJ);                 /* indicate error to caller                 */
    }

    pcomidx = pmapidx - 0x200;                        /* calculate corresponding PDO com. profile */
    map     = *(CPU_INT32U*)buf;                      /* get new mapping                          */
    (void)CODirRdLong(cod, CO_DEV(pcomidx,1), &id);   /* get current identifier value             */
    if ((id & CO_TPDO_COBID_OFF) == 0) {              /* see, if current id indicates PDO valid   */
        result = CO_ERR_OBJ_ACC;                      /* indicate error; access not allowed       */
    } else {                                          /* otherwise: current PDO is invalid        */

        (void)CODirRdByte(cod,                        /* get current number of mapping entries    */
                          CO_DEV(pmapidx,0),
                          &mapn);

        if (mapn != 0) {                              /* mapping not deactivated                  */
            return (CO_ERR_OBJ_ACC);                  /* indicate error to caller                 */
        }                                             /*------------------------------------------*/

        objm = CODirFind(cod, map);                   /* try to find the entry to be mapped       */

        if (objm == (CO_OBJ*)0) {                     /* entry not found                          */
            return (CO_ERR_OBJ_MAP_TYPE);             /* indicate error to caller                 */
        }                                             /*------------------------------------------*/

        maps = CO_IS_PDOMAP(objm->Key);               /* check if mappable                        */
        if (maps == 0) {                              /* not mappable                             */
            return (CO_ERR_OBJ_MAP_TYPE);             /* indicate error to caller                 */
        }                                             /*------------------------------------------*/

        if ( (pmapidx >= 0x1600) &&                   /* see, if index is in RPDO mapping         */
             (pmapidx <= 0x17FF) ) {

            maps = CO_IS_WRITE(objm->Key);            /* check if writeable                       */
            if (maps == 0) {                          /* not writeable                            */
                return (CO_ERR_OBJ_MAP_TYPE);         /* indicate error to caller                 */
            }                                         /*------------------------------------------*/

        } else if ( (pmapidx >= 0x1A00) &&            /* otherwise, see, if in TPDO mapping       */
                    (pmapidx <= 0x1BFF) ) {

            maps = CO_IS_READ(objm->Key);             /* check if readable                        */
            if (maps == 0) {                          /* not readable                             */
                return (CO_ERR_OBJ_MAP_TYPE);         /* indicate error to caller                 */
            }                                         /*------------------------------------------*/
        }

        result = CO_ObjWrDirect(obj, &map, CO_LONG);  /* set new mapping value                    */
    }
    return (result);
}
#endif

#if CO_RPDO_DYN_COM_EN > 0 || CO_TPDO_DYN_COM_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  WRITE PDO IDENTIFIER
*
* \ingroup  INTERNAL
*
*           This function allows the write access to the PDO identifier within
*           the PDO communication profile.
*
* \param[in,out]   obj          Pointer to PDO identifier object
*
* \param[in]       buf          Pointer to write data value
*
* \param[in]       size         Object size in byte
*
* \retval         =CO_ERR_NONE  Successfully operation
* \retval        !=CO_ERR_NONE  An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
static CPU_INT16S CO_TPdoComIdWrite(CO_OBJ* obj, void *buf, CPU_INT32U size)
{
    CO_DIR      *cod;                                 /* Local: pointer to object directory       */
    CO_NMT      *nmt;                                 /* Local: pointer to NMT management         */
#if CO_TPDO_N > 0
    CO_TPDO     *tpdo = 0;                            /* Local: pointer to transmit PDO object    */
#endif
#if CO_RPDO_N > 0
    CO_RPDO     *rpdo = 0;                            /* Local: pointer to receive PDO object     */
#endif
    CPU_INT32U   nid;                                 /* Local: new identifier to be written      */
    CPU_INT32U   oid;                                 /* Local: old identifier to be over-written */
    CPU_INT16S   result = CO_ERR_NONE;                /* Local: function result                   */
    CPU_INT16U   pcomidx;                             /* Local: object entry index                */
    CPU_INT16U   num;                                 /* Local: PDO number                        */
                                                      /*------------------------------------------*/
    (void)size;                                       /* prevent compiler warning                 */
                                                      /*------------------------------------------*/
    if ((obj == 0) || (buf == 0)) {                   /* see, if any parameter pointer is invalid */
        return (CO_ERR_BAD_ARG);                      /* indicate error to caller                 */
    }
    if (CO_GET_SUB(obj->Key) != 1) {                  /* see, if subindex is not a PDO id         */
        return (CO_ERR_TPDO_COM_OBJ);                 /* indicate error to caller                 */
    }                                                 /*------------------------------------------*/

    nid = *(CPU_INT32U*)buf;                          /* get new identifier                       */
    if ((nid & CO_TPDO_COBID_EXT) != 0) {             /* see, if new identifier is extended ID    */
        return (CO_ERR_OBJ_RANGE);                    /* yes: abort write access (not supported)  */
    }

    cod     = obj->Type->Dir;                         /* get pointer to object directory          */
    nmt     = &cod->Node->Nmt;                        /* get pointer to NMT management            */
    pcomidx = CO_GET_IDX(obj->Key);                   /* get index of object                      */
    if ( (pcomidx >= 0x1400) &&                       /* see, if index is in RPDO communication   */
         (pcomidx <= 0x15FF) ) {
#if CO_RPDO_N > 0
        rpdo = cod->Node->RPdo;                       /* get pointer to receive PDO object        */
        num  = pcomidx & 0x1FF;                       /* get PDO number                           */
#else
        return (CO_ERR_RPDO_COM_OBJ);                 /* indicate error to caller                 */
#endif
    } else if ( (pcomidx >= 0x1800) &&                /* otherwise, see, if in TPDO communication */
                (pcomidx <= 0x19FF) ) {
#if CO_TPDO_N > 0
        if((nid & CO_TPDO_COBID_REMOTE) == 0) {       /* see, if new identifier allows RTR        */
            return (CO_ERR_OBJ_RANGE);                /* yes: abort write access (not supported)  */
        }

        tpdo = cod->Node->TPdo;                       /* get pointer to transmit PDO object       */
        num  = pcomidx & 0x1FF;                       /* get PDO number                           */
#else
        return (CO_ERR_TPDO_COM_OBJ);                 /* indicate error to caller                 */
#endif
    } else {                                          /* otherwise, not in any PDO communication  */
        return (CO_ERR_TPDO_COM_OBJ);                 /* indicate error to caller                 */
    }

    (void)CO_ObjRdDirect(obj, &oid, CO_LONG);         /* get old identifier value                 */
    if ((oid & CO_TPDO_COBID_OFF) == 0) {             /* see, if current id indicates PDO valid   */
        if ((nid & CO_TPDO_COBID_OFF) != 0) {         /* see, if new id indicates invalid PDO     */
            result = CO_ObjWrDirect(obj, &nid,        /* set new identifier value                 */
                                    CO_LONG);
            if (nmt->Mode == CO_OPERATIONAL) {        /* see, if device is in OPERATIONAL state   */
#if CO_TPDO_N > 0
                if (tpdo != 0) {                      /* see, if an TPDO should be rescanned      */
                    CO_TPdoReset(tpdo, num);          /* rescan TPDO from object directory        */
                }
#endif
#if CO_RPDO_N > 0
                if (rpdo != 0) {                      /* see, if an RPDO should be rescanned      */
                    CO_RPdoReset(rpdo, num);          /* rescan RPDO from object directory        */
                }
#endif
            }
        } else {                                      /* otherwise: new id is valid PDO id        */
            result = CO_ERR_OBJ_RANGE;                /* indicate error; access not allowed       */
        }
    } else {                                          /* otherwise: current PDO is invalid        */
        if ((nid & CO_TPDO_COBID_OFF) != 0) {         /* see, if new id indicates invalid PDO     */
            result = CO_ObjWrDirect(obj, &nid,        /* set new identifier value                 */
                                    CO_LONG);
        } else {                                      /* otherwise: new id is valid PDO id        */
            result = CO_ObjWrDirect(obj, &nid,        /* set new identifier value                 */
                                    CO_LONG);
            if (nmt->Mode == CO_OPERATIONAL) {        /* see, if device is in OPERATIONAL state   */
#if CO_TPDO_N > 0
                if (tpdo != 0) {                      /* see, if an TPDO should be rescanned      */
                    CO_TPdoReset(tpdo, num);          /* rescan TPDO from object directory        */
                }                                     /* otherwise an RPDO should be rescanned    */
#endif
#if CO_RPDO_N > 0
                if (rpdo != 0) {                      /* see, if an RPDO should be rescanned      */
                    CO_RPdoReset(rpdo, num);          /* rescan RPDO from object directory        */
                }
#endif
            }
        }
    }
    return (result);
}
#endif

#if CO_RPDO_DYN_COM_EN > 0 || CO_TPDO_DYN_COM_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief  WRITE PDO IDENTIFIER
*
* \ingroup  INTERNAL
*
*           This function allows the write access to the PDO transmission type within
*           the PDO communication profile.
*
* \param[in,out]   obj          Pointer to PDO transmission type object
*
* \param[in]       buf          Pointer to write data value
*
* \param[in]       size         Object size in byte
*
* \retval         =CO_ERR_NONE  Successfully operation
* \retval        !=CO_ERR_NONE  An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
static CPU_INT16S CO_TPdoComTypeWrite (CO_OBJ* obj, void *buf, CPU_INT32U size)
{
    CO_DIR      *cod;                                 /* Local: pointer to object directory       */
    CPU_INT08U   type;                                /* Local: new type to be written            */
    CPU_INT32U   id;                                  /* Local: current identifier                */
    CPU_INT16S   result = CO_ERR_NONE;                /* Local: function result                   */
    CPU_INT16U   pcomidx;                             /* Local: object entry index                */
                                                      /*------------------------------------------*/
    if ((obj == 0) ||                                 /* see, if any parameter pointer is invalid */
        (buf == 0) || (size != CO_LONG)) {
        return (CO_ERR_BAD_ARG);                      /* indicate error to caller                 */
    }
    if (CO_GET_SUB(obj->Key) != 2) {                  /* see, if subindex is not a PDO type       */
        return (CO_ERR_PARA_IDX);                     /* indicate error to caller                 */
    }                                                 /*------------------------------------------*/

    type = *(CPU_INT08U*)buf;                         /* get new type                             */
#if CO_SYNC_EN == 0
    if (type < 254) {                                 /* see, if new type is invalid              */
        return (CO_ERR_OBJ_ACC);                      /* yes: abort write access (not supported)  */
    }
#endif

    cod     = obj->Type->Dir;                         /* get pointer to object directory          */
    pcomidx = CO_GET_IDX(obj->Key);                   /* get index of object                      */
    if ( (pcomidx >= 0x1400) &&                       /* see, if index is in RPDO communication   */
         (pcomidx <= 0x15FF) ) {

    } else if ( (pcomidx >= 0x1800) &&                /* otherwise, see, if in TPDO communication */
                (pcomidx <= 0x19FF) ) {

    } else {                                          /* otherwise, not in any PDO communication  */
        return (CO_ERR_PARA_IDX);                     /* indicate error to caller                 */
    }

    (void)CODirRdLong(cod, CO_DEV(pcomidx,1), &id);   /* get current identifier value             */
    if ((id & CO_TPDO_COBID_OFF) == 0) {              /* see, if current id indicates PDO valid   */
        result = CO_ERR_OBJ_RANGE;                    /* indicate error; access not allowed       */
    } else {                                          /* otherwise: current PDO is invalid        */
        result = CO_ObjWrDirect(obj, &type, CO_LONG); /* set new type value                       */
    }
    return (result);
}
#endif

