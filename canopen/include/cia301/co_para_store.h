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

#ifndef CO_PARA_STORE_H_
#define CO_PARA_STORE_H_

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

#define CO_TPARA_STORE  ((const CO_OBJ_TYPE *)&COTParaStore)

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
extern const CO_OBJ_TYPE COTParaStore;

/******************************************************************************
* PROTECTED API FUNCTIONS
******************************************************************************/

/*! \brief LOAD PARAMETER FROM NVM
*
*    This function is responsible for the loading of all parameter groups
*    with the given type. The single parameter group(s) will be loaded from
*    NVM by calling the nvm driver function for reading data.
*
* \note
*    This function considers all parameter groups, which are linked to the
*    parameter store index (1010h) within the object dictionary. Every not
*    linked parameter group is not scope of this function and must be handled
*    within the application.
*
* \param node
*    Ptr to node info
*
* \param type
*    Reset type, e.g. CO_RESET_COM or CO_RESET_NODE
*
* \retval  ==CO_ERR_NONE    loading successful
* \retval  !=CO_ERR_NONE    an error is detected and function aborted
*/
CO_ERR CONodeParaLoad(struct CO_NODE_T *node, CO_NMT_RESET type);

/******************************************************************************
* PUBLIC API FUNCTIONS
******************************************************************************/

/*! \brief PARAMETER STORE
*
*    This function is responsible for the storing activities of the given
*    parameter group. The whole parameter group will be stored in NVM by
*    calling the nvm driver function.
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
CO_ERR COParaStore(CO_PARA *pg, struct CO_NODE_T *node);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_PARA_STORE_H_ */
