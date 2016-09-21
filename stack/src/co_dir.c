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
* \file     co_dir.c
*
* \brief    OBJECT DIRECTORY
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_dir.c#3 $
*
*           This source file implements the CANopen object directory management functions.
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

/*------------------------------------------------------------------------------------------------*/
/*! \brief  FIND OBJECT ENTRY IN DIRECTORY
*
* \ingroup  API
*
*           This function searches the given key within the given object directory.
*
* \param[in,out]   cod           pointer to the CANopen object directory
*
* \param[in]       key           object entry key; should be generated with the macro CO_DEV()
*
* \retval          >0    The pointer to the identified object entry
* \retval          =0    Addressed object was not found
*/
/*------------------------------------------------------------------------------------------------*/
CO_OBJ *CODirFind(CO_DIR *cod, CPU_INT32U key)
{
    CO_OBJ     *result = 0;                           /* Local: function result                   */
    CO_OBJ     *obj    = 0;                           /* Local: ptr to object entry               */
    CPU_INT32S  start  = 0;                           /* Local: start of searching                */
    CPU_INT32S  end;                                  /* Local: end of searching                  */
    CPU_INT32S  center;                               /* Local: center of searching               */
                                                      /*------------------------------------------*/
    if (cod == 0) {                                   /* see, if the object directory ptr is bad  */
        cod->Node->Error = CO_ERR_BAD_ARG;            /* set error code information               */
        return (result);                              /* abort searching                          */
    }
    if (key == 0) {                                   /* see, if key is invalid                   */
        cod->Node->Error = CO_ERR_BAD_ARG;            /* set error code information               */
        return (result);                              /* abort searching                          */
    }                                                 /*------------------------------------------*/
    key = CO_GET_DEV(key);                            /* ensure that key includes only idx/subidx */
    end = cod->Num;                                   /* set end to number of entries             */
    while (start <= end) {                            /* binary search algorithm                  */
        center = start + ((end - start) / 2);
        obj    = &(cod->Root[center]);                /* set to center object entry               */
        if (CO_GET_DEV(obj->Key) == key) {            /* see, if object device equal given device */
            result = obj;                             /* return current object                    */
            break;                                    /* break the loop                           */
        }
        if (CO_GET_DEV(obj->Key) > key) {             /* see, if wanted device is in leading half */
            end    = center - 1;                      /* next search in the leading half          */
        } else {                                      /* searching entry is in the rear half      */
            start  = center + 1;                      /* next search in the rear half             */
        }
    }                                                 /*------------------------------------------*/
    if (result == 0) {                                /* see, if object is not found              */
        cod->Node->Error = CO_ERR_OBJ_NOT_FOUND;      /* set error code information               */
    }                                                 /*------------------------------------------*/
    return(result);                                   /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  READ BYTE FROM OBJECT DIRECTORY
*
* \ingroup  API
*
*           This function reads a 8bit value from the given object directory. The object
*           entry is addressed with the given key and the value will be written to the
*           given destination pointer.
*
* \param[in,out]   cod           pointer to the CANopen object directory
*
* \param[in]       key           object entry key; should be generated with the macro CO_DEV()
*
* \param[in]       value         pointer to the value destination
*
* \retval          =CO_ERR_NONE    Successfully operation
* \retval         !=CO_ERR_NONE    An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CODirRdByte(CO_DIR *cod, CPU_INT32U key, CPU_INT08U *value)
{
    CPU_INT32U  sz;                                   /* Local: size of object entry              */
    CPU_INT16S  result = CO_ERR_OBJ_NOT_FOUND;                          /* Local: function result                   */
    CO_OBJ     *obj;                                  /* Local: ptr to object entry               */
                                                      /*------------------------------------------*/
    if ((cod == 0) || (value == 0)) {                 /* see, if argument pointer is invalid      */
        return (CO_ERR_BAD_ARG);                      /* abort function with error indication     */
    }                                                 /*------------------------------------------*/
    obj = CODirFind(cod, key);                        /* try to find key within object directory  */
    if (obj != 0) {                                   /* see, if object entry was found           */
        sz = COObjGetSize(obj, (CPU_INT32U)CO_BYTE);  /* get size of object entry                 */
        if (sz != (CPU_INT32U)CO_BYTE) {              /* see, if object size matches byte         */
            cod->Node->Error = CO_ERR_OBJ_SIZE;       /* set error code information               */
            result           = CO_ERR_OBJ_SIZE;       /* indicate error                           */
        } else {                                      /*------------------------------------------*/
            result = COObjRdValue(obj,                /* read value into given destination        */
                                  (void *)value,
                                  CO_BYTE,
                                  cod->Node->NodeId);
            if (result != CO_ERR_NONE) {              /* see, if an error was detected            */
                cod->Node->Error = CO_ERR_OBJ_READ;   /* set error code information               */
            }
        }
    }                                                 /*------------------------------------------*/
    return(result);                                   /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  READ WORD FROM OBJECT DIRECTORY
*
* \ingroup  API
*
*           This function reads a 16bit value from the given object directory. The object
*           entry is addressed with the given key and the value will be written to the
*           given destination pointer.
*
* \param[in,out]   cod           pointer to the CANopen object directory
*
* \param[in]       key           object entry key; should be generated with the macro CO_DEV()
*
* \param[in]       value         pointer to the value destination
*
* \retval          =CO_ERR_NONE    Successfully operation
* \retval         !=CO_ERR_NONE    An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CODirRdWord(CO_DIR *cod, CPU_INT32U key, CPU_INT16U *value)
{
    CPU_INT32U  sz;                                   /* Local: size of object entry              */
    CPU_INT16S  result = CO_ERR_OBJ_NOT_FOUND;        /* Local: function result                   */
    CO_OBJ     *obj;                                  /* Local: ptr to object entry               */
                                                      /*------------------------------------------*/
    if ((cod == 0) || (value == 0)) {                 /* see, if argument pointer is invalid      */
        return (CO_ERR_BAD_ARG);                      /* abort function with error indication     */
    }                                                 /*------------------------------------------*/
    obj = CODirFind(cod, key);                        /* try to find key within object directory  */
    if (obj != 0) {                                   /* see, if object entry was found           */
        sz = COObjGetSize(obj, (CPU_INT32U)CO_WORD);  /* get size of object entry                 */
        if (sz != (CPU_INT32U)CO_WORD) {              /* see, if object size matches word         */
            cod->Node->Error = CO_ERR_OBJ_SIZE;       /* set error code information               */
            result           = CO_ERR_OBJ_SIZE;       /* indicate error                           */
        } else {                                      /*------------------------------------------*/
            result = COObjRdValue(obj,                /* read value into given destination        */
                                  (void *)value,
                                  CO_WORD,
                                  cod->Node->NodeId);
            if (result != CO_ERR_NONE) {              /* see, if an error was detected            */
                cod->Node->Error = CO_ERR_OBJ_READ;   /* set error code information               */
            }
        }
    }                                                 /*------------------------------------------*/
    return(result);                                   /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  READ LONG FROM OBJECT DIRECTORY
*
* \ingroup  API
*
*           This function reads a 32bit value from the given object directory. The object
*           entry is addressed with the given key and the value will be written to the
*           given destination pointer.
*
* \param[in,out]   cod           pointer to the CANopen object directory
*
* \param[in]       key           object entry key; should be generated with the macro CO_DEV()
*
* \param[in]       value         pointer to the value destination
*
* \retval          =CO_ERR_NONE    Successfully operation
* \retval         !=CO_ERR_NONE    An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CODirRdLong(CO_DIR *cod, CPU_INT32U key, CPU_INT32U *value)
{
    CPU_INT32U  sz;                                   /* Local: size of object entry              */
    CPU_INT16S  result = CO_ERR_OBJ_NOT_FOUND;        /* Local: function result                   */
    CO_OBJ     *obj;                                  /* Local: ptr to object entry               */
                                                      /*------------------------------------------*/
    if ((cod == 0) || (value == 0)) {                 /* see, if argument pointer is invalid      */
        return (CO_ERR_BAD_ARG);                      /* abort function with error indication     */
    }                                                 /*------------------------------------------*/
    obj = CODirFind(cod, key);                        /* try to find key within object directory  */
    if (obj != 0) {                                   /* see, if object entry was found           */
        sz = COObjGetSize(obj, (CPU_INT32U)CO_LONG);  /* get size of object entry                 */
        if (sz != (CPU_INT32U)CO_LONG) {              /* see, if object size matches long         */
            cod->Node->Error = CO_ERR_OBJ_SIZE;       /* set error code information               */
            result           = CO_ERR_OBJ_SIZE;       /* indicate error                           */
        } else {                                      /*------------------------------------------*/
            result = COObjRdValue(obj,                /* read value into given destination        */
                                  (void *)value,
                                  CO_LONG,
                                  cod->Node->NodeId);
            if (result != CO_ERR_NONE) {              /* see, if an error was detected            */
                cod->Node->Error = CO_ERR_OBJ_READ;   /* set error code information               */
            }
        }
    }                                                 /*------------------------------------------*/
    return(result);                                   /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  WRITE BYTE TO OBJECT DIRECTORY
*
* \ingroup  API
*
*           This function writes a 8bit value to the given object directory. The object
*           entry is addressed with the given key and the value will be read from the
*           given source pointer.
*
* \param[in,out]   cod           pointer to the CANopen object directory
*
* \param[in]       key           object entry key; should be generated with the macro CO_DEV()
*
* \param[in]       value         the source value
*
* \retval          =CO_ERR_NONE    Successfully operation
* \retval         !=CO_ERR_NONE    An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CODirWrByte(CO_DIR *cod, CPU_INT32U key, CPU_INT08U value)
{
    CPU_INT32U  sz;                                   /* Local: size of object entry              */
    CPU_INT16S  result = CO_ERR_OBJ_NOT_FOUND;        /* Local: function result                   */
    CO_OBJ     *obj;                                  /* Local: ptr to object entry               */
                                                      /*------------------------------------------*/
    if (cod == 0) {                                   /* see, if argument pointer is invalid      */
        return (CO_ERR_BAD_ARG);                      /* abort function with error indication     */
    }                                                 /*------------------------------------------*/
    obj = CODirFind(cod, key);                        /* try to find key within object directory  */
    if (obj != 0) {                                   /* see, if object entry was found           */
        sz = COObjGetSize(obj, (CPU_INT32U)CO_BYTE);  /* get size of object entry                 */
        if (sz != (CPU_INT32U)CO_BYTE) {              /* see, if object size matches byte         */
            cod->Node->Error = CO_ERR_OBJ_SIZE;       /* set error code information               */
            result           = CO_ERR_OBJ_SIZE;       /* indicate error                           */
        } else {                                      /*------------------------------------------*/
            result = COObjWrValue(obj,                /* write value into object directory        */
                                  (void *)&value,
                                  CO_BYTE,
                                  cod->Node->NodeId);
            if (result != CO_ERR_NONE) {              /* see, if an error was detected            */
                cod->Node->Error = CO_ERR_OBJ_WRITE;  /* set error code information               */
            }
        }
    }                                                 /*------------------------------------------*/
    return(result);                                   /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  WRITE WORD TO OBJECT DIRECTORY
*
* \ingroup  API
*
*           This function writes a 16bit value to the given object directory. The object
*           entry is addressed with the given key and the value will be read from the
*           given source pointer.
*
* \param[in,out]   cod           pointer to the CANopen object directory
*
* \param[in]       key           object entry key; should be generated with the macro CO_DEV()
*
* \param[in]       value         the source value
*
* \retval          =CO_ERR_NONE    Successfully operation
* \retval         !=CO_ERR_NONE    An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CODirWrWord(CO_DIR *cod, CPU_INT32U key, CPU_INT16U value)
{
    CPU_INT32U  sz;                                   /* Local: size of object entry              */
    CPU_INT16S  result = CO_ERR_OBJ_NOT_FOUND;        /* Local: function result                   */
    CO_OBJ     *obj;                                  /* Local: ptr to object entry               */
                                                      /*------------------------------------------*/
    if (cod == 0) {                                   /* see, if argument pointer is invalid      */
        return (CO_ERR_BAD_ARG);                      /* abort function with error indication     */
    }                                                 /*------------------------------------------*/
    obj = CODirFind(cod, key);                        /* try to find key within object directory  */
    if (obj != 0) {                                   /* see, if object entry was found           */
        sz = COObjGetSize(obj, (CPU_INT32U)CO_WORD);  /* get size of object entry                 */
        if (sz != (CPU_INT32U)CO_WORD) {              /* see, if object size matches word         */
            cod->Node->Error = CO_ERR_OBJ_SIZE;       /* set error code information               */
            result           = CO_ERR_OBJ_SIZE;       /* indicate error                           */
        } else {                                      /*------------------------------------------*/
            result = COObjWrValue(obj,                /* write value into object directory        */
                                  (void *)&value,
                                  CO_WORD,
                                  cod->Node->NodeId);
            if (result != CO_ERR_NONE) {              /* see, if an error was detected            */
                cod->Node->Error = CO_ERR_OBJ_WRITE;  /* set error code information               */
            }
        }
    }                                                 /*------------------------------------------*/
    return(result);                                   /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  WRITE LONG TO OBJECT DIRECTORY
*
* \ingroup  API
*
*           This function writes a 32bit value to the given object directory. The object
*           entry is addressed with the given key and the value will be read from the
*           given source pointer.
*
* \param[in,out]   cod           pointer to the CANopen object directory
*
* \param[in]       key           object entry key; should be generated with the macro CO_DEV()
*
* \param[in]       value         the source value
*
* \retval          =CO_ERR_NONE    Successfully operation
* \retval         !=CO_ERR_NONE    An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CODirWrLong(CO_DIR *cod, CPU_INT32U key, CPU_INT32U value)
{
    CPU_INT32U  sz;                                   /* Local: size of object entry              */
    CPU_INT16S  result = CO_ERR_OBJ_NOT_FOUND;        /* Local: function result                   */
    CO_OBJ     *obj;                                  /* Local: ptr to object entry               */
                                                      /*------------------------------------------*/
    if (cod == 0) {                                   /* see, if argument pointer is invalid      */
        return (CO_ERR_BAD_ARG);                      /* abort function with error indication     */
    }                                                 /*------------------------------------------*/
    obj = CODirFind(cod, key);                        /* try to find key within object directory  */
    if (obj != 0) {                                   /* see, if object entry was found           */
        sz = COObjGetSize(obj, (CPU_INT32U)CO_LONG);  /* get size of object entry                 */
        if (sz != (CPU_INT32U)CO_LONG) {              /* see, if object size matches long         */
            cod->Node->Error = CO_ERR_OBJ_SIZE;       /* set error code information               */
            result           = CO_ERR_OBJ_SIZE;       /* indicate error                           */
        } else {                                      /*------------------------------------------*/
            result = COObjWrValue(obj,                /* write value into object directory        */
                                  (void *)&value,
                                  CO_LONG,
                                  cod->Node->NodeId);
            if (result != CO_ERR_NONE) {              /* see, if an error was detected            */
                cod->Node->Error = CO_ERR_OBJ_WRITE;  /* set error code information               */
            }
        }
    }                                                 /*------------------------------------------*/
    return(result);                                   /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  READ BUFFER FROM OBJECT DIRECTORY
*
* \ingroup  API
*
*           This function reads a buffer byte stream from the given object directory. The
*           object entry is addressed with the given key and the bytes will be read from
*           the given destination buffer of the given length.
*
* \param[in,out]   cod           pointer to the CANopen object directory
*
* \param[in]       key           object entry key; should be generated with the macro CO_DEV()
*
* \param[in,out]   buffer        pointer to the destination buffer
*
* \param[in]       len           length of destination buffer
*
* \retval          =CO_ERR_NONE    Successfully operation
* \retval         !=CO_ERR_NONE    An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CODirRdBuffer(CO_DIR *cod, CPU_INT32U key, CPU_INT08U *buffer, CPU_INT32U len)
{
    CPU_INT16S  result = CO_ERR_OBJ_NOT_FOUND;        /* Local: function result                   */
    CO_OBJ     *obj;                                  /* Local: ptr to object entry               */
                                                      /*------------------------------------------*/
    if ((cod == 0) || (buffer == 0)) {                /* see, if argument pointer is invalid      */
        return (CO_ERR_BAD_ARG);                      /* abort function with error indication     */
    }                                                 /*------------------------------------------*/
    obj = CODirFind(cod, key);                        /* try to find key within object directory  */
    if (obj != 0) {                                   /* see, if object entry was found           */
        result = COObjRdBufStart(obj,                 /* read buffer from object directory        */
                                 (void *)buffer,
                                 (CPU_INT08U)len);
        if (result != CO_ERR_NONE) {                  /* see, if an error was detected            */
            cod->Node->Error = CO_ERR_OBJ_READ;       /* set error code information               */
        }
    }                                                 /*------------------------------------------*/
    return(result);                                   /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  WRITE BUFFER TO OBJECT DIRECTORY
*
* \ingroup  API
*
*           This function writes a buffer byte stream to the given object directory. The object
*           entry is addressed with the given key and the bytes will be read from to the
*           given source buffer of the given length.
*
* \param[in,out]   cod           pointer to the CANopen object directory
*
* \param[in]       key           object entry key; should be generated with the macro CO_DEV()
*
* \param[in]       buffer        pointer to the source bytes
*
* \param[in]       len           length of byte buffer
*
* \retval          =CO_ERR_NONE    Successfully operation
* \retval         !=CO_ERR_NONE    An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CODirWrBuffer(CO_DIR *cod, CPU_INT32U key, CPU_INT08U *buffer, CPU_INT32U len)
{
    CPU_INT16S  result = CO_ERR_OBJ_NOT_FOUND;        /* Local: function result                   */
    CO_OBJ     *obj;                                  /* Local: ptr to object entry               */
                                                      /*------------------------------------------*/
    if ((cod == 0) || (buffer == 0)) {                /* see, if argument pointer is invalid      */
        return (CO_ERR_BAD_ARG);                      /* abort function with error indication     */
    }                                                 /*------------------------------------------*/
    obj = CODirFind(cod, key);                        /* try to find key within object directory  */
    if (obj != 0) {                                   /* see, if object entry was found           */
        result = COObjWrBufStart(obj,                 /* write buffer into object directory       */
                                 (void *)buffer,
                                 (CPU_INT08U)len);
        if (result != CO_ERR_NONE) {                  /* see, if an error was detected            */
            cod->Node->Error = CO_ERR_OBJ_WRITE;      /* set error code information               */
        }
    }                                                 /*------------------------------------------*/
    return(result);                                   /* return function result                   */
}

/*
****************************************************************************************************
*                                          INTERNAL FUNCTIONS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief  INIT OBJECT DIRECTORY
*
* \ingroup  INTERNAL
*
*           This function identifies the number of already configured object directory entries
*           within the given object entry array, starting at the given address (root) with the
*           given length (max).
*
*           The internal object directory information structure will be updated with the
*           identified results and linked to the given node information structure.
*
* \param[in,out]   cod          pointer to object directory which must be initialized
*
* \param[in,out]   node         pointer to the CANopen device node information structure
*
* \param[in]       root         pointer to the start of the object entry array
*
* \param[in]       max          the length of the object entry array
*
* \retval   >=0  identified number of already configured object directory entries
* \retval   <0   An argument error is detected.
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CO_DirInit(CO_DIR *cod, CO_NODE *node, CO_OBJ *root, CPU_INT16U max)
{
    CO_OBJ     *obj;                                  /* Local: ptr to working object             */
    CPU_INT16U  num = 0;                              /* Local: num of valid objects in directory */
                                                      /*------------------------------------------*/
    if ((cod == 0) || (node == 0) || (root == 0)) {   /* see, if any ptr is invalid               */
        CO_NodeFatalError();                          /* inform user                              */
        return (-1);                                  /* return error indication                  */
    }
    if (max == 0) {                                   /* see, if directory size is invalid        */
        node->Error = CO_ERR_BAD_ARG;                 /* set error code information               */
        return (-1);                                  /* return error indication                  */
    }                                                 /*------------------------------------------*/
    obj = root;                                       /* start counting at given root object      */
    while ((obj->Key != 0) && (num < max)) {          /* while object is valid & end not reached  */
        if (obj->Type != 0) {                         /* see, if a type structure is referenced   */
            obj->Type->Dir = cod;                     /* link this type to the object directory   */
        }
        num++;                                        /* count object                             */
        obj++;                                        /* goto next entry                          */
    }                                                 /*------------------------------------------*/
    cod->Root  = root;                                /* set ptr to root object                   */
    cod->Num   = num;                                 /* set number of valid objects              */
    cod->Max   = max;                                 /* set max. number of objects in directory  */
    cod->Node  = node;                                /* set link to parent CANopen node          */
                                                      /*------------------------------------------*/
    return ((CPU_INT16S)num);                         /* return function result                   */
}
