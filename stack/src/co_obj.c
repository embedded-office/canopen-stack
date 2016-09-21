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
* \file     co_obj.c
*
* \brief    OBJECT DIRECTORY ENTRY
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_obj.c#9 $
*
*           This source file implements the CANopen object directory.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

/*
****************************************************************************************************
*                                             INCLUDES
****************************************************************************************************
*/

#include "co_core.h"                                  /* Object entry functions                   */

/*
****************************************************************************************************
*                                            FUNCTIONS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief  GET SIZE OF OBJECT ENTRY
*
* \ingroup  API
*
*           This function returns the size of the given object directory entry.
*
* \param[in]       obj           pointer to the CANopen directory entry
*
* \param[in]       width         Expected object size in byte (or 0 if unknown)
*
* \retval         >0             Object entry size in bytes
* \retval         =0             An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
uint32_t COObjGetSize(CO_OBJ *obj, uint32_t width)
{
    uint32_t   result = 0;                          /* Local: function result                   */
    CO_OBJ_TYPE *type;                                /* Local: ptr to type structure             */
                                                      /*------------------------------------------*/
    if (obj == 0) {                                   /* see, if object ptr is invalid            */
        return (result);                              /* yes: return error indication             */
    }
    type = obj->Type;                                 /* get pointer to type structure            */
    if (type != 0) {                                  /* see, if type is configured               */
        if (type->Size != 0) {                        /* yes: see, if size function is configured */
            result = type->Size(obj, width);          /* yes: call type specific size function    */
        } else {
            result = CO_GET_SIZE(obj->Key);           /* return size (encoded in key)             */
        }
    } else {                                          /* otherwise: size encoded in key           */
        result = CO_GET_SIZE(obj->Key);               /* return size (encoded in key)             */
    }
                                                      /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  READ VALUE FROM OBJECT ENTRY
*
* \ingroup  API
*
*           This function reads a value from the given object directory entry.
*
* \param[in]       obj           pointer to the CANopen directory entry
*
* \param[in,out]   value         pointer to the result memory
*
* \param[in]       width         width of read value (must be 1, 2 or 4 and reflecting the width
*                                of the referenced variable with given parameter pointer: value)
*
* \param[in]       nodeid        device nodeid (only used in case of node-id dependent value)
*
* \retval         =CO_ERR_NONE   Successfully operation
* \retval        !=CO_ERR_NONE   An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
int16_t COObjRdValue(CO_OBJ *obj, void *value, uint8_t width, uint8_t nodeid)
{
    CO_OBJ_TYPE *type;                                /* Local: ptr to object type                */
    int16_t   err      = 0;                        /* Local: type read function error code     */
    uint32_t   val      = 0;                        /* Local: 8-Bit value                       */
                                                      /*------------------------------------------*/
    if ((obj == 0) || (value == 0)) {                 /* see, if argument ptr is invalid          */
        return (CO_ERR_BAD_ARG);                      /* yes: return error indication             */
    }
    type = obj->Type;                                 /* get type from object entry               */
    if (type != 0) {                                  /* see, if type is configured               */
        err = CO_ObjRdType(obj,                       /* read with referenced type function       */
                           (void *)&val,              /*   the value with the                     */
                           CO_LONG, 0);               /*   starting at offset 0                   */
        if (err != CO_ERR_NONE) {                     /* see, if error is detected                */
            return(err);                              /* yes: set errorcode from read             */
        }
    } else {                                          /* otherwise: no type object structure      */
        err = CO_ObjRdDirect(obj,                     /* read value from object data element      */
                             (void *)&val,
                             CO_LONG);
        if (err != CO_ERR_NONE) {                     /* see, if error is detected                */
            return(err);                              /* yes: set errorcode from read             */
        }
    }                                                 /*------------------------------------------*/
    if (CO_IS_NODEID(obj->Key) != 0) {                /* see, if node-id shall be considered      */
                                                      /*lint -e{644} : val is set in CO_OBJRd<xx> */
        val = (val + nodeid);                         /* set value to object value + node id      */
    }                                                 /*------------------------------------------*/
    if (width == 1) {
        *((uint8_t *)value) = (uint8_t)val;     /* set value assuming via byte pointer      */
    } else if (width == 2) {
        *((uint16_t *)value) = (uint16_t)val;     /* set value assuming via word pointer      */
    } else if (width == 4) {
        *((uint32_t *)value) = (uint32_t)val;     /* set value assuming via long pointer      */
    }
    return(CO_ERR_NONE);                              /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  WRITE VALUE TO OBJECT ENTRY
*
* \ingroup  API
*
*           This function writes a value to the given object directory entry.
*
* \param[in,out]   obj           pointer to the CANopen object directory entry
*
* \param[in]       value         pointer to the source memory
*
* \param[in]       width         width of write value (must be 1, 2 or 4 and reflecting the width
*                                of the referenced variable with given parameter pointer: value)
*
* \param[in]       nodeid        device nodeid (only used in case of node-id dependent value)
*
* \retval         =CO_ERR_NONE   Successfully operation
* \retval        !=CO_ERR_NONE   An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
int16_t COObjWrValue(CO_OBJ *obj, void *value, uint8_t width, uint8_t nodeid)
{
    CO_OBJ_TYPE *type;                                /* Local: ptr to object type                */
    uint32_t   val      = 0;                        /* Local: 32-Bit value                      */
    int16_t   result   = CO_ERR_NONE;              /* Local: function result                   */
#if CO_TPDO_N > 0
    int16_t   status   = 0;                        /* Local: compare status                    */
#endif
                                                      /*------------------------------------------*/
    if ((obj == 0) || (value == 0)) {                 /* see, if argument ptr is invalid          */
        return (CO_ERR_BAD_ARG);                      /* yes: return error indication             */
    }                                                 /*------------------------------------------*/
    if (width == 1) {                                 /* see, if given pointer ref. to a byte     */
        val = *((uint8_t *)value) & 0xFF;          /* cast referenced value to a byte          */
    } else if (width == 2) {                          /* otherwise: see, if ref. to a word        */
        val = *((uint16_t *)value) & 0xFFFF;        /* cast referenced value to a word          */
    } else if (width == 4) {                          /* otherwise: see, if ref. to a long        */
        val = *((uint32_t *)value);                 /* cast referenced value to a long          */
    } else {                                          /* otherwise: invalid width                 */
        return (CO_ERR_BAD_ARG);                      /* return error indication                  */
    }                                                 /*------------------------------------------*/
    if (CO_IS_NODEID(obj->Key) != 0) {                /* see, if node-id shall be considered      */
        val = (val - nodeid);                         /* set object value to value - node id      */
    }                                                 /*------------------------------------------*/
#if CO_TPDO_N > 0
    if (CO_IS_PDOMAP(obj->Key) != 0) {                /* see, if PDO mappable object entry        */
        status = CO_ObjCmp(obj,                       /* compare existing value with new value    */
                           (void *)&val);
    }
#endif
    type = obj->Type;                                 /* get type from object entry               */
    if (type != 0) {                                  /* see, if type is configured               */
        result = CO_ObjWrType(obj,                    /* write with referenced type function      */
                              (void *)&val,           /* the value with the                       */
                              CO_LONG, 0);            /* starting at offset 0                     */
    } else {                                          /* otherwise: no type object structure      */
        result = CO_ObjWrDirect(obj,                  /* write value to object data element       */
                                (void *)&val,
                                CO_LONG);
    }                                                 /*------------------------------------------*/
#if CO_TPDO_N > 0
    if (status != 0) {                                /* see, if signal value has changed         */
        type = obj->Type;                             /* yes: get object entry type               */
        if (type == CO_TASYNC) {                      /* see, if asynchronous trigger is defined  */
            (void)obj->Type->Ctrl(obj,                /* call TPDO asynchronous trigger function  */
                                  CO_TPDO_ASYNC,
                                  0);
                                                      /* potential errors already noted in node   */
        }
    }                                                 /*------------------------------------------*/
#endif
    return(result);                                   /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  START READ BUFFER FROM OBJECT ENTRY
*
* \ingroup  API
*
*           This function starts the read operation at the beginning of the byte stream from
*           the given object directory entry into the given destination buffer.
*
* \param[in]       obj           pointer to the CANopen object directory entry
*
* \param[in,out]   buffer        pointer to the destination buffer
*
* \param[in]       len           length of destination buffer
*
* \retval         =CO_ERR_NONE   Successfully operation
* \retval        !=CO_ERR_NONE   An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
int16_t COObjRdBufStart(CO_OBJ *obj, uint8_t *buffer, uint32_t len)
{
    CO_OBJ_TYPE *type;                                /* Local: ptr to object type                */
    int16_t   result   = CO_ERR_OBJ_ACC;           /* Local: function result                   */
                                                      /*------------------------------------------*/
    if ((obj == 0) || (buffer == 0)) {                /* see, if argument ptr is invalid          */
        return (CO_ERR_BAD_ARG);                      /* yes: return error indication             */
    }
    type = obj->Type;                                 /* get type from object entry               */
    if (type != 0) {                                  /* see, if type is configured               */
        result = CO_ObjRdType(obj,                    /* read with referenced type function       */
                              (void *)buffer,         /* the value with the                       */
                              len, 0);                /* starting at offset 0                     */
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  CONTINUE READ BUFFER FROM OBJECT ENTRY
*
* \ingroup  API
*
*           This function continues the read operation at the current offset of the byte stream
*           from the given object directory entry into the given destination buffer.
*
* \param[in]       obj           pointer to the CANopen object directory entry
*
* \param[in,out]   buffer        pointer to the destination buffer
*
* \param[in]       len           length of destination buffer
*
* \retval         =CO_ERR_NONE   Successfully operation
* \retval        !=CO_ERR_NONE   An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
int16_t COObjRdBufCont(CO_OBJ *obj, uint8_t *buffer, uint32_t len)
{
    CO_OBJ_TYPE *type;                                /* Local: ptr to object type                */
    int16_t   result   = CO_ERR_OBJ_ACC;           /* Local: function result                   */
                                                      /*------------------------------------------*/
    if ((obj == 0) || (buffer == 0)) {                /* see, if argument ptr is invalid          */
        return (CO_ERR_BAD_ARG);                      /* yes: return error indication             */
    }
    type = obj->Type;                                 /* get type from object entry               */
    if (type != 0) {                                  /* see, if type is configured               */
        if (type->Read != 0) {                        /* see, if read function is refereced       */
            result = type->Read(obj, buffer, len);    /* call referenced type read function       */
        }
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  START WRITE BUFFER TO OBJECT ENTRY
*
* \ingroup  API
*
*           This function starts the write operation at the beginning of the given object
*           directory entry from the given source buffer.
*
* \param[in,out]   obj           pointer to the CANopen object directory entry
*
* \param[in]       buffer        pointer to the source buffer
*
* \param[in]       len           length of source buffer
*
* \retval         =CO_ERR_NONE   Successfully operation
* \retval        !=CO_ERR_NONE   An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
int16_t COObjWrBufStart(CO_OBJ *obj, uint8_t *buffer, uint32_t len)
{
    CO_OBJ_TYPE *type;                                /* Local: ptr to object type                */
    int16_t   result   = CO_ERR_OBJ_ACC;           /* Local: function result                   */
                                                      /*------------------------------------------*/
    if ((obj == 0) || (buffer == 0)) {                /* see, if argument ptr is invalid          */
        return (CO_ERR_BAD_ARG);                      /* yes: return error indication             */
    }
    type = obj->Type;                                 /* get type from object entry               */
    if (type != 0) {                                  /* see, if type is configured               */
        result = CO_ObjWrType(obj,                    /* write with referenced type function      */
                              (void *)buffer,         /* the value with the                       */
                              len, 0);                /* starting at offset 0                     */
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  CONTINUE WRITE BUFFER TO OBJECT ENTRY
*
* \ingroup  API
*
*           This function continues the write operation at the current offset of the byte stream
*           from the given source buffer to the object directory entry.
*
* \param[in,out]   obj           pointer to the CANopen object directory entry
*
* \param[in]       buffer        pointer to the source buffer
*
* \param[in]       len           length of source buffer
*
* \retval         =CO_ERR_NONE   Successfully operation
* \retval        !=CO_ERR_NONE   An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
int16_t COObjWrBufCont(CO_OBJ *obj, uint8_t *buffer, uint32_t len)
{
    CO_OBJ_TYPE *type;                                /* Local: ptr to object type                */
    int16_t   result   = CO_ERR_OBJ_ACC;           /* Local: function result                   */
                                                      /*------------------------------------------*/
    if ((obj == 0) || (buffer == 0)) {                /* see, if argument ptr is invalid          */
        return (CO_ERR_BAD_ARG);                      /* yes: return error indication             */
    }
    type = obj->Type;                                 /* get type from object entry               */
    if (type != 0) {                                  /* see, if type is configured               */
        if (type->Write != 0) {                       /* see, if write function is refereced      */
            result = type->Write(obj, buffer, len);   /* call referenced type write function      */
        }
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}

/*
****************************************************************************************************
*                                          INTERNAL FUNCTIONS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief  INIT OBJECT ENTRY
*
* \ingroup  INTERNAL
*
*           This function (re-)initializes an object entry element.
*
* \param[in,out]   obj           pointer to the object entry
*/
/*------------------------------------------------------------------------------------------------*/
void CO_ObjInit (CO_OBJ *obj)
{
    if (obj == 0) {                                   /* see, if argument ptr is invalid          */
        return;                                       /* abort initialization                     */
    }
    obj->Key   = 0;                                   /* mark object to be unused                 */
    obj->Type  = 0;                                   /* basic type (no linked type structure)    */
    obj->Data  = 0;                                   /* clear data                               */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  DIRECT READ FROM DATA POINTER
*
* \ingroup  INTERNAL
*
*           This function reads the value of the entry directly from the data pointer. This
*           function checks the internal flags and reacts as configured: reading the pointer
*           address as value, or use the address and get the value from that address.
*
* \param[in]       obj           pointer to the CANopen object directory entry
*
* \param[in,out]   val           pointer to the result memory
*
* \param[in]       len           length of value in bytes
*
* \retval         =CO_ERR_NONE   Successfully operation
* \retval        !=CO_ERR_NONE   An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_ObjRdDirect(CO_OBJ *obj, void *val, uint32_t len)
{
    uint8_t sz;                                    /* Local: object entry size                 */
    int16_t result = CO_ERR_NONE;                  /* Local: function result                   */

    if ((obj == 0) || (val == 0)) {                   /* see, if argument ptr is invalid          */
        return (CO_ERR_BAD_ARG);                      /* yes: return error indication             */
    }

    if (CO_IS_DIRECT(obj->Key) != 0) {                /* see, if direct access is required        */
        if (len == CO_BYTE) {                         /* see, if target reference is a byte       */
            *((uint8_t *)val) =                    /* get value directly from member Data      */
                (uint8_t)((obj->Data)&0xff);
        } else if (len == CO_WORD) {                  /* see, if target reference is a word       */
            *((uint16_t *)val) =                    /* get value directly from member Data      */
                (uint16_t)((obj->Data)&0xffff);
        } else if (len == CO_LONG) {                  /* see, if target reference is a long       */
            *((uint32_t *)val) =                    /* get value directly from member Data      */
                (uint32_t)(obj->Data);
        } else {                                      /* otherwise: invalid data size             */
            result = CO_ERR_BAD_ARG;                  /* yes: return error indication             */
        }
    } else {                                          /* otherwise: no direct access allowed      */
        sz = CO_GET_SIZE(obj->Key);                   /* get object entry size                    */
        if (obj->Data == 0) {                         /* see, if pointer is invalid               */
            result = CO_ERR_OBJ_ACC;
        } else if (len == CO_BYTE) {                  /* see, if target reference is a byte       */
            if (sz == CO_BYTE) {                      /* see, if object reference is a byte       */
                *((uint8_t *)val) = (uint8_t)(  /* get value referenced by member Data      */
                    *((uint8_t*)(obj->Data))&0xff);
            } else if (sz == CO_WORD) {               /* see, if object reference is a word       */
                *((uint8_t *)val) = (uint8_t)(  /* get value referenced by member Data      */
                    *((uint16_t*)(obj->Data))&0xff);
            } else if (sz == CO_LONG) {               /* see, if object reference is a long       */
                *((uint8_t *)val) = (uint8_t)(  /* get value referenced by member Data      */
                    *((uint32_t*)(obj->Data))&0xff);
            } else {                                  /* otherwise: invalid data size             */
                result = CO_ERR_BAD_ARG;              /* yes: return error indication             */
            }
        } else if (len == CO_WORD) {                  /* see, if target reference is a word       */
            if (sz == CO_BYTE) {                      /* see, if object reference is a byte       */
                *((uint16_t *)val) = (uint16_t)(  /* get value referenced by member Data      */
                    *((uint8_t*)(obj->Data))&0xffff);
            } else if (sz == CO_WORD) {               /* see, if object reference is a word       */
                *((uint16_t *)val) = (uint16_t)(  /* get value referenced by member Data      */
                    *((uint16_t*)(obj->Data))&0xffff);
            } else if (sz == CO_LONG) {               /* see, if object reference is a long       */
                *((uint16_t *)val) = (uint16_t)(  /* get value referenced by member Data      */
                    *((uint32_t*)(obj->Data))&0xffff);
            } else {                                  /* otherwise: invalid data size             */
                result = CO_ERR_BAD_ARG;              /* yes: return error indication             */
            }
        } else if (len == CO_LONG) {                  /* see, if target reference is a long       */
            if (sz == CO_BYTE) {                      /* see, if object reference is a byte       */
                *((uint32_t *)val) = (uint32_t)(  /* get value referenced by member Data      */
                    *((uint8_t*)(obj->Data)));
            } else if (sz == CO_WORD) {               /* see, if object reference is a word       */
                *((uint32_t *)val) = (uint32_t)(  /* get value referenced by member Data      */
                    *((uint16_t*)(obj->Data)));
            } else if (sz == CO_LONG) {               /* see, if object reference is a long       */
                *((uint32_t *)val) = (uint32_t)(  /* get value referenced by member Data      */
                    *((uint32_t*)(obj->Data)));
            } else {                                  /* otherwise: invalid data size             */
                result = CO_ERR_BAD_ARG;              /* yes: return error indication             */
            }
        }
    }
    return (result);
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  COMPARE GIVEN VALUE WITH OBJECT ENTRY VALUE
*
* \ingroup  INTERNAL
*
*           This function compares the given value with the currently stored value within the
*           object directory.
*
* \param[in]       obj           pointer to the CANopen object directory entry
*
* \param[in]       val           new value, (casted to 32bit value)
*
* \retval   =0     newval is equal to stored value
* \retval   >0     newval is not equal to stored value
* \retval   <0     an error is detected within this function
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_ObjCmp(CO_OBJ *obj, void *val)
{
    uint32_t newval = 0;                            /* Local: new value from parameter          */
    uint32_t oldval = 0;                            /* Local: current value in object           */
    int16_t result = 0;                            /* Local: result of comparison              */
    int16_t err;                                   /* Local: error code                        */
                                                      /*------------------------------------------*/
    newval = *((uint32_t *)val);                    /* get value for comparison                 */
    err    = CO_ObjRdDirect(obj, &oldval, CO_LONG);   /* get current value from object entry      */
    if (err != CO_ERR_NONE) {                         /* see, if an error is detected             */
        return (err);                                 /* indicate error                           */
    }
                                                      /*lint -e{644} : oldval set in CO_ObjRd<xx> */
    if (newval < oldval) {                            /* see, if new value is lower than existing */
        result = -1;                                  /* yes: indicate lower value                */
    } else if (newval > oldval) {                     /* otherwise see, if new value is greater   */
        result = 1;                                   /* yes: indicate greater value              */
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  READ WITH TYPE FUNCTIONS
*
* \ingroup  INTERNAL
*
*           This function reads the value of the entry directly with the linked type-functions.
*
* \param[in]       obj           pointer to the CANopen object directory entry
*
* \param[in,out]   dst           pointer to the result memory
*
* \param[in]       len           length of value in bytes
*
* \param[in]       off           set this offset, before reading
*
* \retval         =CO_ERR_NONE   Successfully operation
* \retval        !=CO_ERR_NONE   An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_ObjRdType(CO_OBJ *obj, void *dst, uint32_t len, uint32_t off)
{
    CO_OBJ_TYPE *type;                                /* Local: ptr to object type                */
    int16_t   result = CO_ERR_OBJ_ACC;             /* Local: function result                   */
                                                      /*------------------------------------------*/
    type = obj->Type;                                 /* get type structure reference             */
    if (type != 0) {                                  /* see, if type is valid                    */
        if (type->Read != 0) {                        /* see, if read function is refereced       */
            if (type->Ctrl != 0) {                    /* yes: see, if ctrl-function is refereced  */
                result = type->Ctrl(obj,              /* use ctrl-function to set read offset     */
                                    CO_CTRL_SET_OFF, off);
                if (result != CO_ERR_NONE) {          /* see, if an error was detected            */
                    return (result);                  /* yes: return error indication             */
                }
            }
            result = type->Read(obj, dst, len);       /* call referenced type read function       */
        } else {                                      /* otherwise: no read function referenced   */
            result = CO_ObjRdDirect(obj,              /* read value from object data element      */
                                    (void *)dst,
                                    len);
        }
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  DIRECT WRITE TO DATA POINTER
*
* \ingroup  INTERNAL
*
*           This function writes the value to the entry directly to the data pointer. This
*           function checks the internal flags and reacts as configured: writing the value
*           to the pointer address, or use the address and write the value to that address.
*
* \param[in,out]   obj           pointer to the CANopen object directory entry
*
* \param[in]       val           pointer to the source memory
*
* \param[in]       len           length of value in bytes
*
* \retval         =CO_ERR_NONE   Successfully operation
* \retval        !=CO_ERR_NONE   An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_ObjWrDirect(CO_OBJ *obj, void *val, uint32_t len)
{
    uint8_t sz;                                    /* Local: object entry size                 */
    int16_t result = CO_ERR_NONE;                  /* Local: function result                   */
                                                      /*------------------------------------------*/
    if ((obj == 0) || (val == 0)) {                   /* see, if argument ptr is invalid          */
        return (CO_ERR_BAD_ARG);                      /* yes: return error indication             */
    }

    if (CO_IS_DIRECT(obj->Key) != 0) {                /* see, if direct access is required        */
        if (len == CO_BYTE) {                         /* see, if referenced value is byte         */
            obj->Data = (uint32_t)(                 /* set referenced value directly to Data    */
                *((uint8_t*)val)&0xff);
        } else if (len == CO_WORD) {                  /* see, if referenced value is word         */
            obj->Data = (uint32_t)(                 /* set referenced value directly to Data    */
                *((uint16_t*)val)&0xffff);          /* set value directly to member Data        */
        } else if (len == CO_LONG) {                  /* see, if referenced value is long         */
            obj->Data = (uint32_t)(                 /* set referenced value directly to Data    */
                *((uint32_t *)val));
        } else {                                      /* otherwise: invalid data size             */
            result = CO_ERR_BAD_ARG;                  /* yes: return error indication             */
        }
    } else {                                          /* otherwise: no direct access allowed      */
        sz = CO_GET_SIZE(obj->Key);                   /* get object entry size                    */
        if (obj->Data == 0) {                         /* see, if pointer is invalid               */
            result = CO_ERR_OBJ_ACC;
        } else if (sz == CO_BYTE) {                   /* see, if target reference is a byte       */
            if (len == CO_BYTE) {                     /* see, if object reference is a byte       */
                *((uint8_t *)(obj->Data)) =        /* set referenced value to data reference   */
                    (uint8_t)(*((uint8_t*)(val))&0xff);
            } else if (len == CO_WORD) {              /* see, if object reference is a word       */
                *((uint8_t *)(obj->Data)) =        /* set referenced value to data reference   */
                    (uint8_t)(*((uint16_t*)(val))&0xff);
            } else if (len == CO_LONG) {              /* see, if object reference is a long       */
                *((uint8_t *)(obj->Data)) =        /* set referenced value to data reference   */
                    (uint8_t)(*((uint32_t*)(val))&0xff);
            } else {                                  /* otherwise: invalid data size             */
                result = CO_ERR_BAD_ARG;              /* yes: return error indication             */
            }
        } else if (sz == CO_WORD) {                   /* see, if target reference is a word       */
            if (len == CO_BYTE) {                     /* see, if object reference is a byte       */
                *((uint16_t *)(obj->Data)) =        /* set referenced value to data reference   */
                    (uint16_t)(*((uint8_t*)(val))&0xffff);
            } else if (len == CO_WORD) {              /* see, if object reference is a word       */
                *((uint16_t *)(obj->Data)) =        /* set referenced value to data reference   */
                    (uint16_t)(*((uint16_t*)(val))&0xffff);
            } else if (len == CO_LONG) {              /* see, if object reference is a long       */
                *((uint16_t *)(obj->Data)) =        /* set referenced value to data reference   */
                    (uint16_t)(*((uint32_t*)(val))&0xffff);
            } else {                                  /* otherwise: invalid data size             */
                result = CO_ERR_BAD_ARG;              /* yes: return error indication             */
            }
        } else if (sz == CO_LONG) {                   /* see, if target reference is a long       */
            if (len == CO_BYTE) {                     /* see, if object reference is a byte       */
                *((uint32_t *)(obj->Data)) =        /* set referenced value to data reference   */
                    (uint32_t)(*((uint8_t*)(val)));
            } else if (len == CO_WORD) {              /* see, if object reference is a word       */
                *((uint32_t *)(obj->Data)) =        /* set referenced value to data reference   */
                    (uint32_t)(*((uint16_t*)(val)));
            } else if (len == CO_LONG) {              /* see, if object reference is a long       */
                *((uint32_t *)(obj->Data)) =        /* set referenced value to data reference   */
                    (uint32_t)(*((uint32_t*)(val)));
            } else {                                  /* otherwise: invalid data size             */
                result = CO_ERR_BAD_ARG;              /* yes: return error indication             */
            }
        }
    }
    return (result);
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  WRITE WITH TYPE FUNCTIONS
*
* \ingroup  INTERNAL
*
*           This function writes the value of the entry directly with the linked type-functions.
*
* \param[in,out]   obj           pointer to the CANopen object directory entry
*
* \param[in]       src           pointer to the source memory
*
* \param[in]       len           length of value in bytes
*
* \param[in]       off           set this offset, before writing
*
* \retval         =CO_ERR_NONE   Successfully operation
* \retval        !=CO_ERR_NONE   An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
int16_t CO_ObjWrType(CO_OBJ *obj, void *src, uint32_t len, uint32_t off)
{
    CO_OBJ_TYPE *type;                                /* Local: ptr to object type                */
    int16_t   result = CO_ERR_OBJ_ACC;             /* Local: function result                   */
                                                      /*------------------------------------------*/
    type = obj->Type;                                 /* get type structure reference             */
    if (type != 0) {                                  /* see, if type is valid                    */
        if (type->Write != 0) {                       /* see, if write function is refereced      */
            if (type->Ctrl != 0) {                    /* yes: see, if ctrl-function is refereced  */
                result = type->Ctrl(obj,              /* use ctrl-function to reset write offset  */
                                    CO_CTRL_SET_OFF, off);
                if (result != CO_ERR_NONE) {          /* see, if an error was detected            */
                    return (result);                  /* yes: return error indication             */
                }
            }
            result = type->Write(obj, src, len);      /* call referenced type write function      */
        } else {                                      /* otherwise: no write function referenced  */
            result = CO_ObjWrDirect(obj,              /* write value to object data element       */
                                    (void *)src,
                                    len);
        }
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}


