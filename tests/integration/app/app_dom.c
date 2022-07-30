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

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "app_dom.h"
#include "app_dict.h"
#include "app_env.h"

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

/* allocate the physical memory for all domain object entries */
static uint8_t MemBlock[MEM_BLOCK];

/* allocate the domain entry management structures */
static CO_OBJ_DOM Domain[MAX_DOM];

/* start byte in memory area for next domain */
static uint32_t MemNext = 0;

/* index of next domain entry management structure */
static uint8_t DomIdx = 0;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*---------------------------------------------------------------------------*/
/*! \brief REQ-TD-0100
*
* \details The test domains are using the same memory for each test case.
*          Therefore we start after initialization with first domain entry
*          management structure and first byte in domain memory area.
*/
/*---------------------------------------------------------------------------*/
void DomInit(void)
{
    MemNext = 0;
    DomIdx  = 0;
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-TD-0110
*
* \details Limit the size of created domains to the available physical memory
*          area and place the subsequent domain memories in order of calling
*          this function. The memory is cleared with this function and added
*          to the test object dictionary.
*/
/*---------------------------------------------------------------------------*/
CO_OBJ_DOM *DomCreate(uint16_t idx, uint8_t sub, uint8_t access, uint32_t size)
{
    CO_OBJ_DOM *result;

    if (MemNext + size > MEM_BLOCK) {
        size = MEM_BLOCK - MemNext;
    }
    Domain[DomIdx].Offset = 0;
    Domain[DomIdx].Size   = size;
    Domain[DomIdx].Start  = &MemBlock[MemNext];
    MemNext += size;

    DomClear(&Domain[DomIdx]);
    TS_ODAdd(CO_KEY(idx, sub, access), CO_TDOMAIN, (CO_DATA)(&Domain[DomIdx]));
    result = &Domain[DomIdx];
    DomIdx++;

    return (result);
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-TD-0120
*
* \details Fill the memory, which is configured in the domain management
*          structure, with 0xff.
*/
/*---------------------------------------------------------------------------*/
void DomClear(CO_OBJ_DOM *dom)
{
    uint32_t  size = dom->Size;
    uint8_t  *ptr  = dom->Start;
    uint8_t   data = 0xFF;

    while (size > 0) {
        *ptr = data;
        ptr++;
        size--;
    }
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-TD-0130
*
* \details Fill the memory, which is configured in the domain management
*          structure, with upcounting byte values starting at given start
*          value.
*/
/*---------------------------------------------------------------------------*/
void DomFill(CO_OBJ_DOM *dom, uint8_t start)
{
    uint32_t size = dom->Size;
    uint8_t *ptr  = dom->Start;
    uint8_t data  = start;

    while (size > 0) {
        *ptr = data;
        ptr++;
        data++;
        size--;
    }
}
