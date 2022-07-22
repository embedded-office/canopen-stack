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

#ifndef CO_SYNC_ID_H_
#define CO_SYNC_ID_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"
#include "co_err.h"
#include "co_obj.h"
#include "co_emcy.h"

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

#define CO_TSYNC_ID  ((const CO_OBJ_TYPE *)&COTSyncId)

#define CO_SYNC_COBID_ON     ((uint32_t)1 << 30)    /*!< generator flag         */
#define CO_SYNC_COBID_EXT    ((uint32_t)1 << 29)    /*!< extended format        */
#define CO_SYNC_COBID_MASK   ((uint32_t)0x1FFFFFFF) /*!< identifier mask        */

/*! \brief COB-ID sync message
*
*    These macros constructs the COB-ID for usage in object entry
*    index 1005: COB-ID sync message. The standard defines the
*    following encoding:
*    - bit31: x = don't care
*    - bit30: gen = device generates SYNC message (1), or not (0)
*    - bit29: frame = 11bit standard CAN-ID (0) or 29bit extended CAN-ID (1)
*    - bit28-0: CAN-ID
*
* \param generate
*    1: the device produces SYNC messages
*    0: the device consumes SYNC messages
*
* \param id
*    the CAN-ID (standard or extended format)
* \{
*/
#define CO_COBID_SYNC_STD(generate, id)                \
    (uint32_t)(((uint32_t)(id) & 0x7ffuL)            | \
               ((uint32_t)(generate) & 0x1uL) << 30u)

#define CO_COBID_SYNC_EXT(generate, id)                \
    (uint32_t)(((uint32_t)(id) & 0x1fffffffuL)       | \
               ((uint32_t)0x1uL << 29u)              | \
               ((uint32_t)(generate) & 0x1uL) << 30u)
/*! \} */

/******************************************************************************
* PUBLIC CONSTANTS
******************************************************************************/

/*! \brief OBJECT TYPE SYNC COB-ID
*
*    This object type specializes the general handling of object for the
*    object dictionary entry 0x1005. This entries is designed to provide
*    the sync cob-id value and sync producer functionality.
*/
extern const CO_OBJ_TYPE COTSyncId;

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_SYNC_ID_H_ */
