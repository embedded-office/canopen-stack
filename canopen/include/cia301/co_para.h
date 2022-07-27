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

#ifndef CO_PARA_H_
#define CO_PARA_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"
#include "co_nmt.h"

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

#define CO_PARA____   0x0000     /*!< disable                                */
#define CO_PARA___E   0x0001     /*!< enable  (on command)                   */
#define CO_PARA__A_   0x0002     /*!< enable  (autonomously)                 */
#define CO_PARA__AE   0x0003     /*!< enable  (autonomously and on command)  */

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

/*! \brief PARAMETER GROUP INFO
*
*    This structure holds the informations of a parameter group. The
*    parameter group is used within the special function objects in
*    an object dictionary for store and restore parameters.
*
* \note
*    This structure may be placed into ROM to reduce RAM usage.
*/
typedef struct CO_PARA_T {
    uint32_t             Offset;   /*!< Offset in non-volatile memory area   */
    uint32_t             Size;     /*!< Size of parameter memory block       */
    uint8_t             *Start;    /*!< Start of parameter memory block      */
    uint8_t             *Default;  /*!< Start of default memory block        */
    enum CO_NMT_RESET_T  Type;     /*!< Parameter reset type                 */
    void                *Ident;    /*!< Ptr to User Ident-Code               */
    uint32_t             Value;    /*!< value when reading parameter object  */

} CO_PARA;

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_PARA_H_ */
