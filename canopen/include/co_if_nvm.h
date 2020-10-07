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

#ifndef CO_IF_NVM_H_
#define CO_IF_NVM_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

struct CO_IF_T;                /* Declaration of interface structure         */

typedef void     (*CO_IF_NVM_INIT_FUNC )(void);
typedef uint32_t (*CO_IF_NVM_READ_FUNC )(uint32_t, uint8_t *, uint32_t);
typedef uint32_t (*CO_IF_NVM_WRITE_FUNC)(uint32_t, uint8_t *, uint32_t);

typedef struct CO_IF_NVM_DRV_T {
    CO_IF_NVM_INIT_FUNC  Init;
    CO_IF_NVM_READ_FUNC  Read;
    CO_IF_NVM_WRITE_FUNC Write;
} CO_IF_NVM_DRV;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*! \brief  READ NVM CONTENT
*
*    This function reads the content of the non-volatile memory from the
*    given address into the given buffer.
*
* \param cif
*    pointer to the interface structure
*
* \param start
*    start address in non-volatile memory
*
* \param buffer
*    pointer to destination buffer
*
* \param size
*    size of data in bytes
*
* \return
*    number of read bytes
*/
uint32_t COIfNvmRead(struct CO_IF_T *cif, uint32_t start, uint8_t *buffer, uint32_t size);

/*! \brief  WRITE NVM CONTENT
*
*    This function writes the content of the buffer to the non-volatile memory
*    at the given address.
*
* \param cif
*    pointer to the interface structure
*
* \param start
*    start address in non-volatile memory
*
* \param buffer
*    pointer to source buffer
*
* \param size
*    size of data in bytes
*
* \return
*    number of written bytes
*/
uint32_t COIfNvmWrite(struct CO_IF_T *cif, uint32_t start, uint8_t *buffer, uint32_t size);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif /* CO_IF_NVM_H_ */
