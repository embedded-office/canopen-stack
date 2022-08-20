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

#include "drv_timer_swcycle.h"

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

static uint32_t TimerCounter = 0u;

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void     DrvTimerInit   (uint32_t freq);
static void     DrvTimerStart  (void);
static uint8_t  DrvTimerUpdate (void);
static uint32_t DrvTimerDelay  (void);
static void     DrvTimerReload (uint32_t reload);
static void     DrvTimerStop   (void);

/******************************************************************************
* PUBLIC VARIABLE
******************************************************************************/

const CO_IF_TIMER_DRV SwCycleTimerDriver = {
    DrvTimerInit,
    DrvTimerReload,
    DrvTimerDelay,
    DrvTimerStop,
    DrvTimerStart,
    DrvTimerUpdate
};

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void DrvTimerInit(uint32_t freq)
{
    (void)freq;
    TimerCounter = 0u;
}

static void DrvTimerStart(void)
{
}

static uint8_t DrvTimerUpdate(void)
{
    uint8_t result = 0u;

    if (TimerCounter > 0u) {
        TimerCounter--;
        if (TimerCounter == 0u) {
            result = 1u;
        }
    }

    return (result);
}

static uint32_t DrvTimerDelay(void)
{
    return (TimerCounter);
}

static void DrvTimerReload(uint32_t reload)
{
    TimerCounter = reload;
}

static void DrvTimerStop(void)
{
    TimerCounter = 0u;
}
