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

#ifndef CO_SDO_H_
#define CO_SDO_H_

#if defined __cplusplus
extern "C"{
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"
#include "co_cfg.h"
#include "co_obj.h"

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

#define CO_SDO_ID_OFF      ((uint32_t)1<<31)  /*!< Disabled SDO server/COBID */

/*! \brief SDO server/client COB-ID parameter
*
*    These macros constructs the COB-ID for usage in object entries
*    index 1200..12ff: COB-ID for SDO servers and clients. The standard
*    defines the following encoding:
*    - bit31: valid: SDO exists (0) or not (1)
*    - bit30: dyn: value is static (0) or dynamic (1)
*    - bit29: frame = 11bit CAN-ID (0) or 29bit CAN-ID (1)
*    - bit28-0: CAN-ID
*
* \param valid
*    EMCY exists (1), or not (0)
*
* \param dynamic
*    the value is static (0) or dynamic (1)
*
* \param id
*    the CAN-ID (standard or extended format)
*
* \note: the macro inverts the given value for valid in comparison to the
*        standard definition to avoid negated logic
* \{
*/
#define CO_COBID_SDO_STD(valid, dynamic, id)                 \
    (uint32_t)(((uint32_t)(id) & 0x7ffuL)                  | \
               (((uint32_t)(dynamic) & 0x1u) << 30u)       | \
               ((uint32_t)(1uL - ((valid) & 0x1u)) << 31u))

#define CO_COBID_SDO_EXT(valid, dynamic, id)                 \
    (uint32_t)(((uint32_t)(id) & 0x1fffffffuL)             | \
               ((uint32_t)0x1u << 29u)                     | \
               (((uint32_t)(dynamic) & 0x1u) << 30u)       | \
               ((uint32_t)(1uL - ((valid) & 0x1u)) << 31u))
/*! \} */

/*! \brief SDO Default Connection
*
*    The CANopen standard defines a mandatory connection for SDO server #0.
*/
#define CO_COBID_SDO_REQUEST()    CO_COBID_SDO_STD(1u,0u,0x600uL)
#define CO_COBID_SDO_RESPONSE()   CO_COBID_SDO_STD(1u,0u,0x580uL)

#define CO_SDO_ERR_TBIT         0x05030000    /*!< Toggle bit not alternated              */
#define CO_SDO_ERR_TIMEOUT      0x05040000    /*!< SDO protocol timed out                 */
#define CO_SDO_ERR_CMD          0x05040001    /*!< SDO command specifier invalid/unknown  */
#define CO_SDO_ERR_BLK_SIZE     0x05040002    /*!< Invalid block size (block mode)        */
#define CO_SDO_ERR_SEQ_NUM      0x05040003    /*!< Invalid Sequence number (block mode)   */
#define CO_SDO_ERR_CRC          0x05040004    /*!< Invalid CRC (block mode)               */
#define CO_SDO_ERR_MEM          0x05040005    /*!< Out of memory                          */
#define CO_SDO_ERR_UNSUP        0x06010000    /*!< Unsupported access to an object        */
#define CO_SDO_ERR_RD           0x06010001    /*!< Attempt to read a write only object    */
#define CO_SDO_ERR_WR           0x06010002    /*!< Attempt to write a read only object    */
#define CO_SDO_ERR_OBJ          0x06020000    /*!< Object doesn't exist in dictionary     */
#define CO_SDO_ERR_HW_ACCESS    0x06060000    /*!< Access failed due to an hardware error */
#define CO_SDO_ERR_LEN          0x06070010    /*!< Length of parameter does not match     */
#define CO_SDO_ERR_LEN_HIGH     0x06070012    /*!< Length of parameter too high           */
#define CO_SDO_ERR_LEN_SMALL    0x06070013    /*!< Length of parameter too small          */
#define CO_SDO_ERR_SUB          0x06090011    /*!< Subindex doesn't exist in dictionary   */
#define CO_SDO_ERR_RANGE        0x06090030    /*!< Value range of parameter exceeded      */
#define CO_SDO_ERR_TOS          0x08000020    /*!< Data can't be transfered or stored     */
#define CO_SDO_ERR_TOS_STATE    0x08000022    /*!< Data can't be transfered or stored,    */
                                              /*   because of the present device state    */
#define CO_SDO_ERR_OBJ_MAP      0x06040041    /*!< Object cannot be mapped to the PDO     */
#define CO_SDO_ERR_OBJ_MAP_N    0x06040042    /*!< Number and length exceed PDO           */
#define CO_SDO_ERR_PARA_INCOMP  0x06040043    /*!< parameter incompatibility reason       */
#define CO_SDO_ERR_GENERAL      0x08000000    /*!< General error                          */

#define CO_SDO_BUF_SEG     127
#define CO_SDO_BUF_BYTE    (CO_SDO_BUF_SEG*7) /*!< transfer buffer size in byte           */

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

/*! \brief SDO TRANSFER BUFFER
*
*    This structure holds the data, which are needed for the SDO transfer
*    buffer.
*/
typedef struct CO_SDO_BUF_T {
    uint32_t  Num;               /*!< Number of bytes in transfer buffer     */
    uint8_t  *Start;             /*!< Pointer to start of transfer buffer    */
    uint8_t  *Cur;               /*!< Pointer to next free buffer location   */

} CO_SDO_BUF;

#if defined __cplusplus
}
#endif

#endif /* CO_SDO_H_ */
