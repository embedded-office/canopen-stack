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

#ifndef CO_PARA_RESTORE_H_
#define CO_PARA_RESTORE_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"
#include "co_err.h"
#include "co_obj.h"

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

#define CO_TPARA_RESTORE  ((const CO_OBJ_TYPE *)&COTParaRestore)

/******************************************************************************
* PUBLIC CONSTANTS
******************************************************************************/

/*! \brief OBJECT TYPE PARAMETER CONTROL
*
*    This object type specializes the general handling of objects for the
*    object dictionary entries 0x1010 and 0x1011. These entries are designed
*    to provide the store and restore feature of a configurable parameter
*    group.
*/
extern const CO_OBJ_TYPE COTParaRestore;

/******************************************************************************
* PUBLIC API FUNCTIONS
******************************************************************************/

/*! \brief PARAMETER RESTORE DEFAULT
*
*    This function is responsible for removing the changes on the parameter
*    values of the given parameter group. The changes in NVM of the given
*    parameter group will be replaced with the default values by calling
*    the user callback function \ref COParaDefault().
*
* \param pg
*    Ptr to parameter group info
*
* \param node
*    Ptr to node info
*
* \retval  ==CO_ERR_NONE    loading successful
* \retval  !=CO_ERR_NONE    an error is detected and function aborted
*/
CO_ERR COParaRestore(CO_PARA *pg, struct CO_NODE_T *node);

/******************************************************************************
* CALLBACK FUNCTIONS
******************************************************************************/

/*! \brief SET DEFAULT PARAMETER VALUES CALLBACK
*
*    This callback function will be called during restoring the default values
*    of a parameter group. The function is responsible for setting the factory
*    defaults in the current parameter group memory.
*
* \note
*    The parameter group info pointer is checked to be valid before calling
*    this function.
*
* \param pg
*    Ptr to parameter group info
*
* \retval  =0    parameter loading successful
* \retval  <0    error is detected and function aborted
*/
extern int16_t COParaDefault(CO_PARA *pg);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_PARA_RESTORE_H_ */
