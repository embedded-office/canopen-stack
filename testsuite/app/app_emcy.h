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

#ifndef _APP_EMCY_H_
#define _APP_EMCY_H_

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_core.h"

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

#define EMCY_CODE_MAX     CO_EMCY_N
#define EMCY_HIST_MAX     4

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

void         EmcyResetTable(void);
CO_EMCY_TBL *EmcyGetTable  (void);
uint32_t     EmcyAddCode   (int16_t code, uint8_t reg);
void         EmcyObjDir    (uint32_t len);

#endif
