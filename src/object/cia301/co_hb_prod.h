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

#ifndef CO_HB_PROD_H_
#define CO_HB_PROD_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"
#include "co_obj.h"

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

#define CO_THB_PROD  ((const CO_OBJ_TYPE *)&COTNmtHbProd)

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

typedef uint16_t  CO_OBJ_HB_PROD;         /*!< Heartbeat Producer Cycle Time */

/******************************************************************************
* PUBLIC CONSTANTS
******************************************************************************/

/*! \brief OBJECT TYPE HEARTBEAT PRODUCER
*
*    This object type specializes the general handling of objects for the
*    object dictionary entry 0x1017. This entries is designed to provide
*    the heartbeat producer cycle time.
*/
extern const CO_OBJ_TYPE COTNmtHbProd;

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_HB_PROD_H_ */
