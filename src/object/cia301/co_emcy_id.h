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

#ifndef CO_EMCY_ID_H_
#define CO_EMCY_ID_H_

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

#define CO_TEMCY_ID  ((const CO_OBJ_TYPE *)&COTEmcyId)

#define CO_EMCY_COBID_OFF    ((uint32_t)1 << 31)    /*!< marked as unused    */
#define CO_EMCY_COBID_EXT    ((uint32_t)1 << 29)    /*!< extended format     */
#define CO_EMCY_COBID_MASK   ((uint32_t)0x1FFFFFFF) /*!< identifier mask     */

/*! \brief COB-ID EMCY
*
*    These macros constructs the COB-ID for usage in object entry
*    index 1014: COB-ID EMCY. The standard defines the following encoding:
*    - bit31: valid: EMCY exists (0) or not (1)
*    - bit30: fixed to 0
*    - bit29: frame = 11bit CAN-ID (0) or 29bit CAN-ID (1)
*    - bit28-0: CAN-ID
*
* \param valid
*    EMCY exists (1), or not (0)
*
* \param id
*    the CAN-ID (standard or extended format)
*
* \note: the macro inverts the given value for valid in comparison to the
*        standard definition to avoid negated logic
* \{
*/
#define CO_COBID_EMCY_STD(valid, id)                        \
    (uint32_t)(((uint32_t)(id) & 0x7ffuL)                 | \
               ((uint32_t)(1u - ((valid) & 0x1u)) << 31u))

#define CO_COBID_EMCY_EXT(valid, id)                        \
    (uint32_t)(((uint32_t)(id) & 0x1fffffffuL)            | \
               ((uint32_t)0x1uL << 29u)                   | \
               ((uint32_t)(1u - ((valid) & 0x1u)) << 31u))
/*! \} */

/******************************************************************************
* PUBLIC CONSTANTS
******************************************************************************/

/*! \brief OBJECT TYPE EMCY IDENTIFIER
*
*    This object type specializes the general handling of objects for the
*    object dictionary entries holding a EMCY identifier. This entry is
*    designed to provide the feature of changing a EMCY identifier.
*/
extern const CO_OBJ_TYPE COTEmcyId;

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_EMCY_ID_H_ */
