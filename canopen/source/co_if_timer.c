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

#include "co_core.h"

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*
* see function definition
*/
void COIfTimerReload(CO_IF *cif, uint32_t reload)
{
    const CO_IF_TIMER_DRV *timer = cif->Drv->Timer;
    timer->Reload(reload);    
}

/*
* see function definition
*/
uint32_t COIfTimerDelay(CO_IF *cif)
{
    const CO_IF_TIMER_DRV *timer = cif->Drv->Timer;
    uint32_t delay               = timer->Delay();    
    return (delay);
}

/*
* see function definition
*/
void COIfTimerStop(CO_IF *cif)
{
    const CO_IF_TIMER_DRV *timer = cif->Drv->Timer;
    timer->Stop();
}

/*
* see function definition
*/
void COIfTimerStart(CO_IF *cif)
{
    const CO_IF_TIMER_DRV *timer = cif->Drv->Timer;
    timer->Start();
}

/*
* see function definition
*/
uint8_t COIfTimerUpdate(CO_IF *cif)
{
    const CO_IF_TIMER_DRV *timer = cif->Drv->Timer;
    uint8_t result               = timer->Update();
    return (result);
}
