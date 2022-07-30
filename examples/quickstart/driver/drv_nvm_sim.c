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

#include "drv_nvm_sim.h"

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

#define NVM_SIM_SIZE     128

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

static uint8_t NvmMemory[NVM_SIM_SIZE];

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void     DrvNvmInit  (void);
static uint32_t DrvNvmRead  (uint32_t start, uint8_t *buffer, uint32_t size);
static uint32_t DrvNvmWrite (uint32_t start, uint8_t *buffer, uint32_t size);


/******************************************************************************
* PUBLIC VARIABLE
******************************************************************************/

const CO_IF_NVM_DRV SimNvmDriver = {
    DrvNvmInit,
    DrvNvmRead,
    DrvNvmWrite
};

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void DrvNvmInit(void)
{
    uint32_t idx;

    for (idx = 0; idx < NVM_SIM_SIZE; idx++) {
        NvmMemory[idx] = 0xffu;
    }
}

static uint32_t DrvNvmRead(uint32_t start, uint8_t *buffer, uint32_t size)
{
    uint32_t idx = 0;
    uint32_t pos;

    idx = 0;
    pos = start;
    while ((pos < NVM_SIM_SIZE) && (idx < size)) {
        buffer[idx] = NvmMemory[pos];
        idx++;
        pos++;
    }

    return (idx);
}

static uint32_t DrvNvmWrite(uint32_t start, uint8_t *buffer, uint32_t size)
{
    uint32_t idx = 0;
    uint32_t pos;

    idx = 0;
    pos = start;
    while ((pos < NVM_SIM_SIZE) && (idx < size)) {
        NvmMemory[pos] = buffer[idx];
        idx++;
        pos++;
    }

    return (idx);
}
