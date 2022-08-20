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

#ifndef CO_PDO_EVENT_H_
#define CO_PDO_EVENT_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"
#include "co_err.h"
#include "co_obj.h"
#include "co_pdo.h"

/******************************************************************************
* DEFINES
******************************************************************************/

#define CO_TPDO_EVENT  ((const CO_OBJ_TYPE *)&COTPdoEvent)

/******************************************************************************
* PUBLIC CONSTANTS
******************************************************************************/

/*! \brief OBJECT TYPE: TRANSMIT PDO EVENT TIMER
*
*    This object type specializes the general handling of the event timer
*    object within the TPDO communication profile.
*/
extern const CO_OBJ_TYPE COTPdoEvent;       /*!< PDO Event Type              */

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_PDO_EVENT_H_ */
