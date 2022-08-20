/******************************************************************************
   Copyright 2020 Embedded Office GmbH & Co. KG

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
******************************************************************************/

#ifndef CO_OBJ_H_
#define CO_OBJ_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"
#include "co_err.h"

/******************************************************************************
* DEFINES
******************************************************************************/

/*             .---------------- D: Direct Storage            (bit 7)        */
/*             |.--------------- N: Consider Node-Id          (bit 6)        */
/*             ||.-------------- A: Asynchronous PDO Trigger  (bit 3)        */
/*             |||.------------- P: PDO Mappable              (bit 2)        */
/*             ||||.------------ R: Read Access for CAN       (bit 1)        */
/*             |||||.----------- W: Write Access for CAN      (bit 0)        */
/*             ||||||                                                        */
#define CO_OBJ______W     0x01
#define CO_OBJ_____R_     0x02
#define CO_OBJ____P__     0x04
#define CO_OBJ___A___     0x08
#define CO_OBJ__N____     0x40
#define CO_OBJ_D_____     0x80

/* some popular collection of object entry configurations: */
#define CO_OBJ_____RW     (CO_OBJ_____R_ | CO_OBJ______W)
#define CO_OBJ____PR_     (CO_OBJ____P__ | CO_OBJ_____R_)
#define CO_OBJ____P_W     (CO_OBJ____P__ | CO_OBJ______W)
#define CO_OBJ____PRW     (CO_OBJ____P__ | CO_OBJ_____R_ | CO_OBJ______W)
#define CO_OBJ___APR_     (CO_OBJ___A___ | CO_OBJ____P__ | CO_OBJ_____R_)
#define CO_OBJ___AP_W     (CO_OBJ___A___ | CO_OBJ____P__ | CO_OBJ______W)
#define CO_OBJ___APRW     (CO_OBJ___A___ | CO_OBJ____P__ | CO_OBJ_____R_ | CO_OBJ______W)
#define CO_OBJ__N__R_     (CO_OBJ__N____ | CO_OBJ_____R_)
#define CO_OBJ__N___W     (CO_OBJ__N____ | CO_OBJ______W)
#define CO_OBJ__N__RW     (CO_OBJ__N____ | CO_OBJ_____R_ | CO_OBJ______W)
#define CO_OBJ__N_PR_     (CO_OBJ__N____ | CO_OBJ____P__ | CO_OBJ_____R_)
#define CO_OBJ__N_P_W     (CO_OBJ__N____ | CO_OBJ____P__ | CO_OBJ______W)
#define CO_OBJ__N_PRW     (CO_OBJ__N____ | CO_OBJ____P__ | CO_OBJ_____R_ | CO_OBJ______W)
#define CO_OBJ_D___R_     (CO_OBJ_D_____ | CO_OBJ_____R_)
#define CO_OBJ_D____W     (CO_OBJ_D_____ | CO_OBJ______W)
#define CO_OBJ_D___RW     (CO_OBJ_D_____ | CO_OBJ_____R_ | CO_OBJ______W)
#define CO_OBJ_DN__R_     (CO_OBJ_D_____ | CO_OBJ__N____ | CO_OBJ_____R_)
#define CO_OBJ_DN___W     (CO_OBJ_D_____ | CO_OBJ__N____ | CO_OBJ______W)
#define CO_OBJ_DN__RW     (CO_OBJ_D_____ | CO_OBJ__N____ | CO_OBJ_____R_ | CO_OBJ______W)

#define CO_OBJ_T0         (CO_OBJ___A___)    /*!< Type specific control: T0  (A)         */
#define CO_OBJ_T1         0x10               /*!< Type specific control: T1              */
#define CO_OBJ_T2         0x20               /*!< Type specific control: T2              */
#define CO_OBJ_T3         (CO_OBJ__N____)    /*!< Type specific control: T3  (N)         */

/*! \brief OBJECT DICTIONARY ENDMARKER
*
*    This define may be used in object dictionary definitions. It marks the
*    first unused object entry.
*/
#define CO_OBJ_DICT_ENDMARK   { (uint32_t)0, (CO_OBJ_TYPE *)0, (CO_DATA)0 }

/******************************************************************************
* PUBLIC MACROS
******************************************************************************/

/*! \brief OBJECT KEY
*
*    This macro helps to build the CANopen object member variable 'key'.
*    This variable is used to hold the unique address of the object entry
*    with index:subindex. Furthermore the lower 8bits of the key are used
*    to describe the way of accessing this object entry.
*
* \param idx
*    CANopen object index [0x0000..0xFFFF]
*
* \param sub
*    CANopen object subindex [0x00..0xFF]
*
* \param flags
*    The additional object property flags
*/
#define CO_KEY(idx,sub,flags)    \
    (uint32_t)( (((uint32_t)(  idx) & 0xFFFF)<<16) | \
                (((uint32_t)(  sub) &   0xFF)<< 8) | \
                 ((uint32_t)(flags) &   0xFF) )

/*! \brief OBJECT DEVICE
*
*    This macro helps to build the CANopen object member device identifier.
*    This identifier is used to search the unique address of the object
*    entry (index:subindex).
*
* \param idx
*    CANopen object index [0x0000..0xFFFF]
*
* \param sub
*    CANopen object subindex [0x00..0xFF]
*/
#define CO_DEV(idx,sub)    \
    (uint32_t)( (((uint32_t)(idx) & 0xFFFF)<<16) | \
                (((uint32_t)(sub) &   0xFF)<<8 ) )

/*! \brief EXTRACT DEVICE
*
*    This macro helps to extract the index and subindex out of the CANopen
*    object entry member 'key'.
*
* \param key
*    CANopen object member variable 'key'.
*/
#define CO_GET_DEV(key)    \
    (uint32_t)((uint32_t)(key) & 0xFFFFFF00L)

/*! \brief EXTRACT SUBINDEX
*
*    This macro helps to extract the subindex out of the CANopen object
*    entry member 'key'.
*
* \param key
*    CANopen object member variable 'key'.
*/
#define CO_GET_SUB(key)    \
    (uint8_t)(((uint32_t)(key) & 0x0000FF00L) >> 8)

/*! \brief EXTRACT INDEX
*
*    This macro helps to extract the index out of the CANopen object entry
*    member 'key'.
*
* \param key
*    CANopen object member variable 'key'.
*/
#define CO_GET_IDX(key)    \
    (uint16_t)((uint32_t)(key) >> 16)

/*! \brief CHECK IF OBJECT IS PDO MAPPABLE
*
*    This macro helps to determine, if the object entry is PDO mappable.
*    The macro masks the corresponding bit from the flags bitfield.
*
* \param key
*    CANopen object member variable 'key'.
*/
#define CO_IS_PDOMAP(key)    \
    (uint32_t)((uint32_t)(key) & CO_OBJ____P__)

/*! \brief CHECK IF OBJECT TRIGGERS TPDO ASYNCHRONOUSLY
*
*    This macro helps to determine, if the object entry triggers the TPDO
*    transmission when changing the value.
*    The macro masks the corresponding bit from the flags bitfield.
*
* \param key
*    CANopen object member variable 'key'.
*/
#define CO_IS_ASYNC(key)    \
    (uint32_t)((uint32_t)(key) & CO_OBJ___A___)

/*! \brief CHECK IF OBJECT DEPENDS ON NODE-ID
*
*    This macro helps to determine, if the object entry value depends on
*    the node-ID. The macro masks the corresponding bit from the flags
*    bit field.
*
* \param key
*    CANopen object member variable 'key'.
*/
#define CO_IS_NODEID(key)    \
    (uint32_t)((uint32_t)(key) & CO_OBJ__N____)

/*! \brief CHECK IF OBJECT IS A DIRECT
*
*    This macro helps to determine, if the object entry value is a direct
*    value. The macro masks the corresponding bit from the flags bitfield.
*
* \param key
*    CANopen object member variable 'key'.
*/
#define CO_IS_DIRECT(key)    \
    (uint32_t)((uint32_t)(key) & CO_OBJ_D_____)

/*! \brief CHECK IF OBJECT IS READABLE
*
*    This macro helps to determine, if the object entry value is readable.
*    The macro masks the corresponding bit from the flags bitfield.
*
* \param key
*    CANopen object member variable 'key'.
*/
#define CO_IS_READ(key)    \
    (uint32_t)((uint32_t)(key) & CO_OBJ_____R_)

/*! \brief CHECK IF OBJECT IS WRITEABLE
*
*    This macro helps to determine, if the object entry value is writeable.
*    The macro masks the corresponding bit from the flags bitfield.
*
* \param key
*    CANopen object member variable 'key'.
*/
#define CO_IS_WRITE(key)    \
    (uint32_t)((uint32_t)(key) & CO_OBJ______W)

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

struct CO_NODE_T;              /* Declaration of canopen node structure      */
struct CO_OBJ_TYPE_T;          /* Declaration of object type structure       */
struct CO_DICT_T;              /* Declaration of object dictionary structure */

/*! \brief OBJECT DATA TYPE
*
*    This type holds a pointer to the object entrys data. To allow storage of
*    constant values directly within the object dictionary, this type must be
*    able to hold a complete 32bit value.
*
* \note  This is required for 8bit and 16bit controllers, where pointers may
*        represent 24bit only.
*/
typedef uintptr_t CO_DATA;

/*! \brief OBJECT ENTRY
*
*    This structure holds all data, needed for managing a single object
*    entry.
*/
typedef struct CO_OBJ_T {
    uint32_t                    Key;   /*!< 16Bit Index, 8Bit Subindex, 8Bit Flags */
                                       /*   - 0: 1=rd access allowed               */
                                       /*   - 1: 1=wr access allowed               */
                                       /*   - 2: 1=pdo mappable                    */
                                       /*   - 3: 1=signed value                    */
                                       /*   - 4: |- valid bytes in 2^n; used if    */
                                       /*   - 5: |  Type=0, or Type->Size ptr=0    */
                                       /*   - 6: 1=+/- node-id on read/write       */
                                       /*   - 7: 1=direct (ptr is value if Type=0) */
    const struct CO_OBJ_TYPE_T *Type;  /*!< ==0: value access via Data-Ptr,        */
                                       /*   !=0: access via type structure         */
    CO_DATA                     Data;  /*!< Address of value or data structure     */
                                       /*   or data value for direct access        */
} CO_OBJ;

/*!< Size type function prototype */
typedef uint32_t (*CO_OBJ_SIZE_FUNC) (struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t width);
/*!< Initialze type function prototype */
typedef CO_ERR   (*CO_OBJ_INIT_FUNC) (struct CO_OBJ_T *obj, struct CO_NODE_T *node);
/*!< Read type function prototype */
typedef CO_ERR   (*CO_OBJ_READ_FUNC) (struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
/*!< Write type function prototype */
typedef CO_ERR   (*CO_OBJ_WRITE_FUNC)(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buffer, uint32_t size);
/*!< Reset type function prototype */
typedef CO_ERR   (*CO_OBJ_RESET_FUNC)(struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t para);

/*! \brief OBJECT TYPE
*
*    This structure holds all data, needed for managing a special object
*    entry type.
*/
typedef struct CO_OBJ_TYPE_T {
    CO_OBJ_SIZE_FUNC   Size;           /*!< Get type size function           */
    CO_OBJ_INIT_FUNC   Init;           /*!< Init type module function        */
    CO_OBJ_READ_FUNC   Read;           /*!< Read type value function         */
    CO_OBJ_WRITE_FUNC  Write;          /*!< Write type value function        */
    CO_OBJ_RESET_FUNC  Reset;          /*!< Reset type value function        */

} CO_OBJ_TYPE;

/******************************************************************************
* PROTECTED API FUNCTIONS
******************************************************************************/

/*! \brief  INITIALIZE OBJECT ENTRY
*
*    This function initializes the object dictionary entry and the underlying
*    type module.
*
* \param obj
*    pointer to the object dictionary entry
*
* \param node
*    reference to parent node
*
* \retval   =CO_ERR_NONE    Successfully operation
* \retval  !=CO_ERR_NONE    An error is detected
*/
CO_ERR COObjInit (struct CO_OBJ_T *obj, struct CO_NODE_T *node);

/*! \brief  RESET OBJECT ENTRY
*
*    This function resets the object dictionary entry and the underlying
*    type module.
*
* \param obj
*    pointer to the object dictionary entry
*
* \param node
*    reference to parent node
*
* \param para
*    reset parameter (type specific use)
*
* \retval   =CO_ERR_NONE    Successfully operation
* \retval  !=CO_ERR_NONE    An error is detected
*/
CO_ERR COObjReset (struct CO_OBJ_T *obj, struct CO_NODE_T *node, uint32_t para);

/*! \brief  START READ BUFFER FROM OBJECT ENTRY
*
*    This function starts the read operation at the beginning of the byte
*    stream from the given object dictionary entry into the given destination
*    buffer.
*
* \param obj
*    pointer to the object dictionary entry
*
* \param node
*    reference to parent node
*
* \param buffer
*    pointer to the destination buffer
*
* \param len
*    length of destination buffer
*
* \retval   =CO_ERR_NONE    Successfully operation
* \retval  !=CO_ERR_NONE    An error is detected
*/
CO_ERR COObjRdBufStart(CO_OBJ *obj, struct CO_NODE_T *node, uint8_t *buffer, uint32_t len);

/*! \brief  CONTINUE READ BUFFER FROM OBJECT ENTRY
*
*    This function continues the read operation at the current offset of
*    the byte stream from the given object dictionary entry into the given
*    destination buffer.
*
* \param obj
*    pointer to the object dictionary entry
*
* \param node
*    reference to parent node
*
* \param buffer
*    pointer to the destination buffer
*
* \param len
*    length of destination buffer
*
* \retval   =CO_ERR_NONE    Successfully operation
* \retval  !=CO_ERR_NONE    An error is detected
*/
CO_ERR COObjRdBufCont(CO_OBJ *obj, struct CO_NODE_T *node, uint8_t *buffer, uint32_t len);

/*! \brief  START WRITE BUFFER TO OBJECT ENTRY
*
*    This function starts the write operation at the beginning of the
*    given object dictionary entry from the given source buffer.
*
* \param obj
*    pointer to the object dictionary entry
*
* \param node
*    reference to parent node
*
* \param buffer
*    pointer to the source buffer
*
* \param len
*    length of source buffer
*
* \retval   =CO_ERR_NONE    Successfully operation
* \retval  !=CO_ERR_NONE    An error is detected
*/
CO_ERR COObjWrBufStart(CO_OBJ *obj, struct CO_NODE_T *node, uint8_t *buffer, uint32_t len);

/*! \brief  CONTINUE WRITE BUFFER TO OBJECT ENTRY
*
*    This function continues the write operation at the current offset of
*    the byte stream from the given source buffer to the object dictionary
*    entry.
*
* \param obj
*    pointer to the object dictionary entry
*
* \param node
*    reference to parent node
*
* \param buffer
*    pointer to the source buffer
*
* \param len
*    length of source buffer
*
* \retval   =CO_ERR_NONE    Successfully operation
* \retval  !=CO_ERR_NONE    An error is detected
*/
CO_ERR COObjWrBufCont(CO_OBJ *obj, struct CO_NODE_T *node, uint8_t *buffer, uint32_t len);

/*! \brief TYPE SPECIFIC SDO ABORT CODE
*
*    This function is responsible to set a user defined SDO abort code
*    for the current transfer of an SDO server. This function is useful,
*    when detecting an error within a type read or write function during
*    an SDO transfer. If no SDO transfer is active, this function will
*    do nothing.
*
* \param obj
*    String object entry reference
*
* \param node
*    reference to parent node
*
* \param abort
*    Requested abort code (or 0 for standardized abort code)
*/
void COObjTypeUserSDOAbort(CO_OBJ *obj, struct CO_NODE_T *node, uint32_t abort);

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*! \brief  GET SIZE OF OBJECT ENTRY
*
*    This function returns the size of the given object dictionary entry.
*
* \param obj
*    pointer to the dictionary entry
*
* \param node
*    reference to parent node
*
* \param width
*    Expected object size in byte (or 0 if unknown)
*
* \retval  >0    Object entry size in bytes
* \retval  =0    An error is detected
*/
uint32_t COObjGetSize(CO_OBJ *obj, struct CO_NODE_T *node, uint32_t width);

/*! \brief  READ VALUE FROM OBJECT ENTRY
*
*    This function reads a value from the given object dictionary entry.
*
* \param obj
*    pointer to the dictionary entry
*
* \param node
*    reference to parent node
*
* \param value
*    pointer to the result variable
*
* \param width
*    width of result variable (must match the type size)
*
* \retval   =CO_ERR_NONE    Successfully operation
* \retval  !=CO_ERR_NONE    An error is detected
*/
CO_ERR COObjRdValue(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *value, uint8_t width);

/*! \brief  WRITE VALUE TO OBJECT ENTRY
*
*    This function writes a value to the given object dictionary entry.
*
* \param obj
*    pointer to the object dictionary entry
*
* \param node
*    reference to parent node
*
* \param value
*    pointer to the source variable
*
* \param width
*    width of result variable (must match the type size)
*
* \retval   =CO_ERR_NONE    Successfully operation
* \retval  !=CO_ERR_NONE    An error is detected
*/
CO_ERR COObjWrValue(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *value, uint8_t width);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_OBJ_H_ */
