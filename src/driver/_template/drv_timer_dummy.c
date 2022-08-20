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
 *   co_timer_<device>.h
 */
#include "co_timer_dummy.h"

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

/* TODO: place here your timer register definitions */

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

/* TODO: rename the variable to match the naming convention:
 *   <device>TimerDriver
 */
const CO_IF_TIMER_DRV DummyTimerDriver = {
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

    /* TODO: initialize timer, clear counter and keep timer stopped */
}

static void DrvTimerStart(void)
{
    /* TODO: start hardware timer */
}

static uint8_t DrvTimerUpdate(void)
{
    /* TODO: return 1 if timer event is elapsed, otherwise 0 */
    return (0u);
}

static uint32_t DrvTimerDelay(void)
{
    /* TODO: return current timer counter value */
    return (0u);
}

static void DrvTimerReload(uint32_t reload)
{
    (void)reload;

    /* TODO: reload timer counter value with given reload value */
}

static void DrvTimerStop(void)
{
    /* TODO: stop timer and clear counter value */
}
