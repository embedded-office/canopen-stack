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

#ifndef CO_DOMAIN_H_
#define CO_DOMAIN_H_

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

#define CO_TDOMAIN  ((const CO_OBJ_TYPE *)&COTDomain)

/******************************************************************************
* PUBLIC TYPE DEFINITION
******************************************************************************/

/*! \brief DOMAIN MANAGEMENT STRUCTURE
*
*    This structure holds all data, which are needed for the domain object
*    management within the object dictionary.
*/
typedef struct CO_OBJ_DOM_T {
    uint32_t  Offset;                  /*!< Internal offset information      */
    uint32_t  Size;                    /*!< Domain size information          */
    uint8_t  *Start;                   /*!< Domain start address             */

} CO_OBJ_DOM;

/******************************************************************************
* PUBLIC CONSTANTS
******************************************************************************/

/*! \brief OBJECT TYPE DOMAIN
*
*    This type is responsible for the access to domain memory areas. It is
*    assumed, that the memory is declared in random accessible memory
*    (e.g. RAM, FLASH, etc..) and the domain management structure is stored
*    in the object entry member 'Data'.
*
* \note
*    This exemplary implementation is usable for reading and writing in
*    direct accessible memory (e.g. RAM). The reading from FLASH memory is
*    no problem, but for the write access to FLASH memory, there should be
*    a special DOMAIN implementation for each media.
*/
extern const CO_OBJ_TYPE COTDomain;

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_DOMAIN_H_ */
