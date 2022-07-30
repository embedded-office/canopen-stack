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

#ifndef CO_EMCY_HIST_H_
#define CO_EMCY_HIST_H_

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

#define CO_TEMCY_HIST  ((const CO_OBJ_TYPE *)&COTEmcyHist)

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

/******************************************************************************
* PUBLIC CONSTANTS
******************************************************************************/

/*! \brief OBJECT TYPE EMCY HISTORY
*
*    This type is responsible for the access to the EMCY history.
*/
extern const CO_OBJ_TYPE COTEmcyHist;  /* Link to EMCY Object Type Structure */

/******************************************************************************
* PROTECTED API FUNCTION
******************************************************************************/

/*! \brief  INIT EMCY HISTORY
*
*    This function initializes the EMCY history management. Additionally,
*    the object entries, mandatory for EMCY history are checked for existance.
*
* \note
*    The parameter emcy shall be checked by the calling function.
*
* \param emcy
*    pointer to the EMCY object
*/
void COEmcyHistInit(CO_EMCY *emcy);

/*! \brief  ADD EMCY IN HISTORY
*
*    This function appends a new EMCY entry into the history. To avoid moving
*    the object entries with each new entry, the following algorithm is
*    implemented:
*
*    -# This function will use the EMCY Entries as ringbuffer. Each
*       Add-function call will use the next higher subindex for storage of
*       the EMCY History informations.
*    -# The read/write access functions will remap the different subindex
*       entry values to the required order (latest EMCY add must appear at
*       subindex 1).
*
* \note
*    The parameter emcy shall be checked by the calling function.
*
* \param emcy
*    pointer to the EMCY object
*
* \param err
*    user error identifier
*
* \param usr
*    manufacturer specific fields in EMCY message and/or EMCY message
*/
void COEmcyHistAdd(CO_EMCY *emcy, uint8_t err, CO_EMCY_USR *usr);

/*! \brief RESET EMCY HISTORY
*
*    This function clears the EMCY history in the object dictionary.
*
* \param emcy
*    pointer to the EMCY object
*/
void COEmcyHistReset(struct CO_EMCY_T *emcy);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_EMCY_H_ */
