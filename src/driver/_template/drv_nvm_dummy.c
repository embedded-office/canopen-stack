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

/* TODO: rename the include file name to match the naming convention:
 *   co_nvm_<device>.h
 */
#include "co_nvm_dummy.h"

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

/* TODO: place here your timer register definitions */

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void     DrvNvmInit  (void);
static uint32_t DrvNvmRead  (uint32_t start, uint8_t *buffer, uint32_t size);
static uint32_t DrvNvmWrite (uint32_t start, uint8_t *buffer, uint32_t size);


/******************************************************************************
* PUBLIC VARIABLE
******************************************************************************/

/* TODO: rename the variable to match the naming convention:
 *   <device>NvmDriver
 */
const CO_IF_NVM_DRV DummyNvmDriver = {
    DrvNvmInit,
    DrvNvmRead,
    DrvNvmWrite
};

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void DrvNvmInit(void)
{
    /* TODO: initialize the non-volatile memory */
}

static uint32_t DrvNvmRead(uint32_t start, uint8_t *buffer, uint32_t size)
{
    (void)start;
    (void)buffer;
    (void)size;

    /* TODO: read a memory block from non-volatile memory into given buffer */
    return (0u);
}

static uint32_t DrvNvmWrite(uint32_t start, uint8_t *buffer, uint32_t size)
{
    (void)start;
    (void)buffer;
    (void)size;

    /* TODO: write content of given buffer into non-volatile memory */
    return (0u);
}
