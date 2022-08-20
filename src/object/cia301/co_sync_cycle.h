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

#ifndef CO_SYNC_CYCLE_H_
#define CO_SYNC_CYCLE_H_

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

#define CO_TSYNC_CYCLE  ((const CO_OBJ_TYPE *)&COTSyncCycle)

/******************************************************************************
* PUBLIC CONSTANTS
******************************************************************************/

/*! \brief OBJECT TYPE SYNC COMMUNICATION CYCLE
*
*    This object type specializes the general handling of object for the
*    object dictionary entry 0x1006. This entries is designed to provide
*    the sync communication cycle value.
*/
extern const CO_OBJ_TYPE COTSyncCycle;

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_SYNC_CYCLE_H_ */
