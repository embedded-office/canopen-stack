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
* \file     co_obj.h
*
* \brief    OBJECT DIRECTORY ENTRY DEFINITIONS
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_obj.h#3 $
*
*           This include file holds the definitions for the CANopen object directory entries.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_OBJ_H_
#define CO_OBJ_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
****************************************************************************************************
*                                             INCLUDES
****************************************************************************************************
*/

#include "cpu.h"                                      /* CPU definitions                          */
#include "co_cfg.h"                                   /* CANopen configuration                    */

/*
****************************************************************************************************
*                                             DEFINES
****************************************************************************************************
*/

#define CO_OBJ____R_     0x01                         /*!< Read Only                              */
#define CO_OBJ_____W     0x02                         /*!< Write Only                             */
#define CO_OBJ____RW     0x03                         /*!< Read/Write                             */
#define CO_OBJ___P__     0x04                         /*!< PDO Map                                */
#define CO_OBJ___PR_     0x05                         /*!< Read Only, PDO Map                     */
#define CO_OBJ___P_W     0x06                         /*!< Write Only, PDO Map                    */
#define CO_OBJ___PRW     0x07                         /*!< Read/Write, PDO Map                    */
#define CO_OBJ__N___     0x40                         /*!< Consider Node-Id                       */
#define CO_OBJ__N_R_     0x41                         /*!< Read Only, + Node-Id                   */
#define CO_OBJ__N__W     0x42                         /*!< Write Only, - Node-Id                  */
#define CO_OBJ__N_RW     0x43                         /*!< Read/Write, +/- Node-Id                */
#define CO_OBJ__NPR_     0x45                         /*!< Read Only, PDO Map, + Node-Id          */
#define CO_OBJ__NP_W     0x46                         /*!< Write Only, PDO Map, - Node-Id         */
#define CO_OBJ__NPRW     0x47                         /*!< Read/Write, PDO Map, +/- Node-Id       */
#define CO_OBJ_D____     0x80                         /*!< Direct Access (Ptr = Value)            */
#define CO_OBJ_D__R_     0x81                         /*!< Read Only, Direct Access               */
#define CO_OBJ_D___W     0x82                         /*!< Write Only, Direct Access              */
#define CO_OBJ_D__RW     0x83                         /*!< Read/Write, Direct Access              */
#define CO_OBJ_DN_R_     0xC1                         /*!< Read Only, + Node-Id, Direct Access    */
#define CO_OBJ_DN__W     0xC2                         /*!< Write Only, - Node-Id, Direct Access   */
#define CO_OBJ_DN_RW     0xC3                         /*!< Read/Write, +/- Node-Id, Direct Access */

#define CO_OBJ_SGN       0x08                         /*!< Signed number                          */
#define CO_OBJ_SZ1       0x00                         /*!< Size 2^0 = 1 Byte                      */
#define CO_OBJ_SZ2       0x10                         /*!< Size 2^1 = 2 Byte                      */
#define CO_OBJ_SZ4       0x20                         /*!< Size 2^2 = 4 Byte                      */
#define CO_OBJ_SZ8       0x30                         /*!< Size 2^3 = 8 Byte                      */

#define CO_OBJ_FLG_MSK   0xC7                         /*!< Mask for flags                         */
#define CO_OBJ_SZ_MSK    0x30                         /*!< Mask for fixed object size             */
#define CO_OBJ_FMT_MSK   0x38                         /*!< Mask for format                        */

#define CO_UNSIGNED8     CO_OBJ_SZ1                   /*!< CANopen Datatype: UNSIGNED8            */
#define CO_UNSIGNED16    CO_OBJ_SZ2                   /*!< CANopen Datatype: UNSIGNED16           */
#define CO_UNSIGNED32    CO_OBJ_SZ4                   /*!< CANopen Datatype: UNSIGNED32           */
#define CO_UNSIGNED64    CO_OBJ_SZ8                   /*!< CANopen Datatype: UNSIGNED64           */
#define CO_SIGNED8       (CO_OBJ_SGN|CO_OBJ_SZ1)      /*!< CANopen Datatype: SIGNED8              */
#define CO_SIGNED16      (CO_OBJ_SGN|CO_OBJ_SZ2)      /*!< CANopen Datatype: SIGNED16             */
#define CO_SIGNED32      (CO_OBJ_SGN|CO_OBJ_SZ4)      /*!< CANopen Datatype: SIGNED32             */
#define CO_FLOAT         0                            /*!< CANopen Datatype: FLOAT                */
#define CO_DOMAIN        0                            /*!< CANopen Datatype: DOMAIN               */
#define CO_STRING        0                            /*!< CANopen Datatype: STRING               */

#define CO_BYTE          1                            /*!< Number of bytes for a byte             */
#define CO_WORD          2                            /*!< Number of bytes for a word             */
#define CO_LONG          4                            /*!< Number of bytes for a long             */

                                                      /*--- Type Control Function Code -----------*/
#define CO_CTRL_SET_OFF     1                         /*!< Set read/write offset for next access  */

/*------------------------------------------------------------------------------------------------*/
/*! \brief OBJECT DIRECTORY ENDMARKER
*
* \ingroup OBJ
*
*          This define may be used in object directory definitions. It marks the first unused
*          object entry.
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_OBJ_DIR_ENDMARK   { (CPU_INT32U)0, (CO_OBJ_TYPE *)0, (CPU_INT32U)0 }

/*
****************************************************************************************************
*                                             MACROS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief OBJECT KEY
*
* \ingroup OBJ
*
*          This macro helps to build the CANopen object member variable 'key'. This variable
*          is used to hold the unique address of the object entry with index:subindex.
*          Furthermore the lower 8bits of the key are used to describe the way of accessing
*          this object entry.
*
* \param   idx      CANopen object index [0x0000..0xFFFF]
*
* \param   sub      CANopen object subindex [0x00..0xFF]
*
* \param   flags    The additional object property flags
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_KEY(idx,sub,flags)              \
    ( (CPU_INT32U)((  idx) & 0xFFFF)<<16 | \
      (CPU_INT32U)((  sub) &   0xFF)<< 8 | \
      (CPU_INT32U)((flags) &   0xFF)     )

/*------------------------------------------------------------------------------------------------*/
/*! \brief OBJECT DEVICE
*
* \ingroup OBJ
*
*          This macro helps to build the CANopen object member device identifier. This identifier
*          is used to search the unique address of the object entry (index:subindex).
*
* \param   idx      CANopen object index [0x0000..0xFFFF]
*
* \param   sub      CANopen object subindex [0x00..0xFF]
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_DEV(idx,sub)                  \
    ( (CPU_INT32U)((idx) & 0xFFFF)<<16 | \
      (CPU_INT32U)((sub) &   0xFF)<<8  )

/*------------------------------------------------------------------------------------------------*/
/*! \brief OBJECT MAPPING LINK
*
* \ingroup OBJ
*
*          This macro helps to build the CANopen object entry for a PDO mapping. This entry
*          is used to specify the linked signal within a PDO.
*
* \param   idx      CANopen object index [0x0000..0xFFFF]
*
* \param   sub      CANopen object subindex [0x00..0xFF]
*
* \param   bit      Length of mapped signal in bits [8,16 or 32]
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_LINK(idx,sub,bit)             \
    ( (CPU_INT32U)((idx) & 0xFFFF)<<16 | \
      (CPU_INT32U)((sub) &   0xFF)<<8  | \
      (CPU_INT32U)((bit) &   0xFF)     )

/*------------------------------------------------------------------------------------------------*/
/*! \brief EXTRACT DEVICE
*
* \ingroup OBJ
*
*          This macro helps to extract the index and subindex out of the CANopen object entry
*          member 'key'.
*
* \param   key      CANopen object member variable 'key'.
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_GET_DEV(key)              \
    (CPU_INT32U)(((key) & 0xFFFFFF00))

/*------------------------------------------------------------------------------------------------*/
/*! \brief EXTRACT SUBINDEX
*
* \ingroup OBJ
*
*          This macro helps to extract the subindex out of the CANopen object entry member 'key'.
*
* \param   key      CANopen object member variable 'key'.
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_GET_SUB(key)                   \
    (CPU_INT08U)(((key) & 0x0000FF00) >> 8)

/*------------------------------------------------------------------------------------------------*/
/*! \brief EXTRACT INDEX
*
* \ingroup OBJ
*
*          This macro helps to extract the index out of the CANopen object entry member 'key'.
*
* \param   key      CANopen object member variable 'key'.
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_GET_IDX(key)                   \
    (CPU_INT16U)((key) >> 16)

/*------------------------------------------------------------------------------------------------*/
/*! \brief EXTRACT SIZE
*
* \ingroup OBJ
*
*          This macro helps to extract the object entry size in bytes out of the CANopen object
*          entry member 'key'. If this result is 0, the size must be calculated with the function
*          Size(), referenced in the linked type structure.
*
* \param   key      CANopen object member variable 'key'.
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_GET_SIZE(key)                             \
    (CPU_INT32U)(1L << (((key) & CO_OBJ_SZ_MSK) >> 4))

/*------------------------------------------------------------------------------------------------*/
/*! \brief CHECK IF OBJECT IS PDO MAPPABLE
*
* \ingroup OBJ
*
*          This macro helps to determine, if the object entry is PDO mappable. The macro masks the
*          corresponding bit from the flags bitfield.
*
* \param   key      CANopen object member variable 'key'.
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_IS_PDOMAP(key)                 \
    (CPU_INT32U)(key & CO_OBJ___P__)

/*------------------------------------------------------------------------------------------------*/
/*! \brief CHECK IF OBJECT DEPENDS ON NODE-ID
*
* \ingroup OBJ
*
*          This macro helps to determine, if the object entry value depends on the node-ID. The
*          macro masks the corresponding bit from the flags bitfield.
*
* \param   key      CANopen object member variable 'key'.
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_IS_NODEID(key)                 \
    (CPU_INT32U)(key & CO_OBJ__N___)

/*------------------------------------------------------------------------------------------------*/
/*! \brief CHECK IF OBJECT IS A DIRECT
*
* \ingroup OBJ
*
*          This macro helps to determine, if the object entry value is a direct value. The
*          macro masks the corresponding bit from the flags bitfield.
*
* \param   key      CANopen object member variable 'key'.
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_IS_DIRECT(key)                 \
    (CPU_INT32U)(key & CO_OBJ_D____)

/*------------------------------------------------------------------------------------------------*/
/*! \brief CHECK IF OBJECT IS READABLE
*
* \ingroup OBJ
*
*          This macro helps to determine, if the object entry value is readable. The
*          macro masks the corresponding bit from the flags bitfield.
*
* \param   key      CANopen object member variable 'key'.
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_IS_READ(key)                 \
    (CPU_INT32U)(key & CO_OBJ____R_)

/*------------------------------------------------------------------------------------------------*/
/*! \brief CHECK IF OBJECT IS WRITEABLE
*
* \ingroup OBJ
*
*          This macro helps to determine, if the object entry value is writeable. The
*          macro masks the corresponding bit from the flags bitfield.
*
* \param   key      CANopen object member variable 'key'.
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_IS_WRITE(key)                 \
    (CPU_INT32U)(key & CO_OBJ_____W)


/*
****************************************************************************************************
*                                           DATA TYPES
****************************************************************************************************
*/

struct CO_OBJ_TYPE_T;                                 /* Declaration of object type structure     */
struct CO_DIR_T;                                      /* Declaration of object directory structure*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief OBJECT ENTRY
*
* \ingroup OBJ
*
*          This structure holds all data, needed for managing a single object entry.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_OBJ_T {
    CPU_INT32U            Key;                        /*!< 16Bit Index, 8Bit Subindex, 8Bit Flags */
                                                      /*   - 0: 1=rd access allowed               */
                                                      /*   - 1: 1=wr access allowed               */
                                                      /*   - 2: 1=pdo mappable                    */
                                                      /*   - 3: 1=signed value                    */
                                                      /*   - 4: |- valid bytes in 2^n; used if    */
                                                      /*   - 5: |  Type=0, or Type->Size ptr=0    */
                                                      /*   - 6: 1=+/- node-id on read/write       */
                                                      /*   - 7: 1=direct (ptr is value if Type=0) */
    struct CO_OBJ_TYPE_T *Type;                       /*!< ==0: value access via Data-Ptr,        */
                                                      /*   !=0: access via type structure         */
    CPU_INT32U            Data;                       /*!< Address of value or data structure     */
                                                      /*   or data value for direct access        */
} CO_OBJ;

typedef CPU_INT32U (*CO_OBJ_SIZE_FUNC)(CO_OBJ *,      /*!< Size type function prototype           */
                                       CPU_INT32U);
typedef CPU_INT16S (*CO_OBJ_CTRL_FUNC)(CO_OBJ *,
                                       CPU_INT16U,
                                       CPU_INT32U);   /*!< Control type function prototype        */
typedef CPU_INT16S (*CO_OBJ_READ_FUNC)(CO_OBJ *,   
                                       void*,
                                       CPU_INT32U);   /*!< Read type function prototype           */
typedef CPU_INT16S (*CO_OBJ_WRITE_FUNC)(CO_OBJ *,  
                                       void*,
                                       CPU_INT32U);   /*!< Write type function prototype          */

/*------------------------------------------------------------------------------------------------*/
/*! \brief OBJECT TYPE
*
* \ingroup OBJ
*
*          This structure holds all data, needed for managing a special object entry type.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_OBJ_TYPE_T {
    struct CO_DIR_T   *Dir;                           /*!< Link to parent object directory        */
    CPU_INT32U         Offset;                        /*!< Offset within the data memory          */
    CO_OBJ_SIZE_FUNC   Size;                          /*!< Get size of type function              */
    CO_OBJ_CTRL_FUNC   Ctrl;                          /*!< Special type control function          */
    CO_OBJ_READ_FUNC   Read;                          /*!< Read function                          */
    CO_OBJ_WRITE_FUNC  Write;                         /*!< Write function                         */

} CO_OBJ_TYPE;

/*
****************************************************************************************************
*                                       FUNCTION PROTOTYPES
****************************************************************************************************
*/

CPU_INT32U  COObjGetSize    (CO_OBJ *obj, CPU_INT32U width);
CPU_INT16S  COObjRdValue    (CO_OBJ *obj, void *value, CPU_INT08U width, CPU_INT08U nodeid);
CPU_INT16S  COObjWrValue    (CO_OBJ *obj, void *value, CPU_INT08U width, CPU_INT08U nodeid);
CPU_INT16S  COObjRdBufStart (CO_OBJ *obj, CPU_INT08U *buffer, CPU_INT32U len);
CPU_INT16S  COObjRdBufCont  (CO_OBJ *obj, CPU_INT08U *buffer, CPU_INT32U len);
CPU_INT16S  COObjWrBufStart (CO_OBJ *obj, CPU_INT08U *buffer, CPU_INT32U len);
CPU_INT16S  COObjWrBufCont  (CO_OBJ *obj, CPU_INT08U *buffer, CPU_INT32U len);

void        CO_ObjInit      (CO_OBJ *obj);
CPU_INT16S  CO_ObjCmp       (CO_OBJ *obj, void *val);
CPU_INT16S  CO_ObjRdDirect  (CO_OBJ *obj, void *val, CPU_INT32U len);
CPU_INT16S  CO_ObjWrDirect  (CO_OBJ *obj, void *val, CPU_INT32U len);
CPU_INT16S  CO_ObjRdType    (CO_OBJ *obj, void *dst, CPU_INT32U len, CPU_INT32U off);
CPU_INT16S  CO_ObjWrType    (CO_OBJ *obj, void *dst, CPU_INT32U len, CPU_INT32U off);

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_OBJ_H_ */
