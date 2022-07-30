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

#ifndef CO_STRING_H_
#define CO_STRING_H_

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
* DEFINES
******************************************************************************/

#define CO_TSTRING  ((const CO_OBJ_TYPE *)&COTString)

/******************************************************************************
* PUBLIC TYPE DEFINITION
******************************************************************************/

/*! \brief STRING MANAGEMENT STRUCTURE
*
*    This structure holds all data, which are needed for the string object
*    management within the object dictionary.
*/
typedef struct CO_OBJ_STR_T {
    uint32_t  Offset;                  /*!< Internal offset information      */
    uint8_t  *Start;                   /*!< String start address             */

} CO_OBJ_STR;

/******************************************************************************
* PUBLIC CONSTANTS
******************************************************************************/

/*! \brief OBJECT TYPE STRING
*
*    This type is responsible for the access to unlimited string constants.
*    It is assumed, that the strings are declared in nonvolatile memory
*    (e.g. FLASH) and the string management structure is stored in the
*    object entry member 'Data'.
*/
extern const CO_OBJ_TYPE COTString;

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_STRING_H_ */
