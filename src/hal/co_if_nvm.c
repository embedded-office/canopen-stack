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
uint32_t COIfNvmRead(struct CO_IF_T *cif, uint32_t start, uint8_t *buffer, uint32_t size)
{
    const CO_IF_NVM_DRV *nvm = cif->Drv->Nvm;
    uint32_t num = nvm->Read(start, buffer, size);
    return (num);
}

/*
* see function definition
*/
uint32_t COIfNvmWrite(struct CO_IF_T *cif, uint32_t start, uint8_t *buffer, uint32_t size)
{
    const CO_IF_NVM_DRV *nvm = cif->Drv->Nvm;
    uint32_t num = nvm->Write(start, buffer, size);
    return (num);
}
