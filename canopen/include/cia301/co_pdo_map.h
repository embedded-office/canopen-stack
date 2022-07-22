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

#ifndef CO_PDO_MAP_H_
#define CO_PDO_MAP_H_

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

#define CO_TPDO_MAP  ((const CO_OBJ_TYPE *)&COTPdoMap)

/******************************************************************************
* PUBLIC CONSTANTS
******************************************************************************/

/*! \brief OBJECT TYPE: PDO MAPPING ENTRY
*
*    This type is responsible for the access to the PDO mapping entries. Due
*    to the special handling of PDO mapping change accesses, only the write
*    access needs to be handled via the type structure; the read operation
*    can be performed directly.
*/
extern const CO_OBJ_TYPE COTPdoMap;         /*!< PDO Mapping Type            */

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_PDO_MAP_H_ */
