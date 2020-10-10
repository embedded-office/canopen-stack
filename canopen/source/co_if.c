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
void COIfInit(CO_IF *cif, struct CO_NODE_T *node, uint32_t freq)
{
    const CO_IF_CAN_DRV   *can   = cif->Drv->Can;
    const CO_IF_TIMER_DRV *timer = cif->Drv->Timer;
    const CO_IF_NVM_DRV   *nvm   = cif->Drv->Nvm;

    /* initialize interface structure */
    cif->Node = node;

    /* initialize hardware via drivers */
    nvm->Init();
    timer->Init(freq);
    can->Init();
}
