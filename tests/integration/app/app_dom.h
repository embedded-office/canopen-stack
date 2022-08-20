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

#ifndef APP_DOM_H_
#define APP_DOM_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_core.h"

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

/*---------------------------------------------------------------------------*/
/*! \brief MEMORY AREA
*
* \details The size in byte of physical memory area for all domain entries.
*/
/*---------------------------------------------------------------------------*/
#define MEM_BLOCK   1024

/*---------------------------------------------------------------------------*/
/*! \brief NUMBER OF DOMAINS
*
* \details The number of domains within the physical memory area.
*/
/*---------------------------------------------------------------------------*/
#define MAX_DOM   2

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*---------------------------------------------------------------------------*/
/*! \brief INIT DOMAIN TEST POOL
*
* \details Setup a tiny memory management for dynamic domain memory areas,
*          which are usable for test purposes.
*/
/*---------------------------------------------------------------------------*/
void DomInit(void);

/*---------------------------------------------------------------------------*/
/*! \brief CREATE TEST DOMAIN
*
* \details Add an object dictionary entry of type domain with the given index,
*          subindex, access mode and size.
*
* \note    The limits are: maximum memory size is MEM_BLOCK bytes and
*          maximal MAX_DOM domain object entries within object dictionary.
*          There is no limit by the CANopen stack itself, these limits are
*          coming from the goal to keep the testsuite as small as possible.
*/
/*---------------------------------------------------------------------------*/
CO_OBJ_DOM *DomCreate(uint16_t idx, uint8_t sub, uint8_t access, uint32_t size);

/*---------------------------------------------------------------------------*/
/*! \brief CLEAR TEST DOMAIN
*
* \details The memory area of an object dictionary entry of type domain
*          is cleared with value 0xff (simulating an empty FLASH area).
*/
/*---------------------------------------------------------------------------*/
void DomClear(CO_OBJ_DOM *dom);

/*---------------------------------------------------------------------------*/
/*! \brief FILL TEST DOMAIN
*
* \details The memory area of an object dictionary entry of type domain
*          is filled with upcounting byte values, starting with given value.
*/
/*---------------------------------------------------------------------------*/
void DomFill(CO_OBJ_DOM *dom, uint8_t start);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif
