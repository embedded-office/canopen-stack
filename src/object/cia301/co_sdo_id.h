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

#ifndef CO_SDO_ID_H_
#define CO_SDO_ID_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"
#include "co_err.h"
#include "co_obj.h"
#include "co_sync.h"

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

#define CO_TSDO_ID  ((const CO_OBJ_TYPE *)&COTSdoId)

/******************************************************************************
* PUBLIC CONSTANTS
******************************************************************************/

/*! \brief OBJECT TYPE SDO IDENTIFIER
*
*    This object type specializes the general handling of objects for the
*    object dictionary entries holding a SDO identifier. These entries are
*    designed to provide the feature of changing a SDO identifier.
*/
extern const CO_OBJ_TYPE COTSdoId;

/******************************************************************************
* PROTECTED HELBER FUNCTIONS
******************************************************************************/

/*! \brief ACTIVATE SYNC PRODUCER
*
*    This function is used to activate SYNC producer functionality.
*    It's called on NMT Start Pre-Operational or sync COB-ID update
*    (1005h) with bit 30 set to 1.
*
* \param sync
*    Pointer to SYNC object
*/
void COSyncProdActivate(CO_SYNC *sync);

/*! \brief ACTIVATE SYNC PRODUCER
*
*    This function is used to deactivate SYNC producer functionality.
*    It's called on sync COB-ID update (1005h) with bit 30 set to 0.
*
* \param sync
*    Pointer to SYNC object
*/
void COSyncProdDeactivate(CO_SYNC *sync);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_SDO_ID_H_ */
