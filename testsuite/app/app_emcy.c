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

#include "app_emcy.h"
#include "app_env.h"

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

static CO_EMCY_TBL  EmcyCode[EMCY_CODE_MAX];
static uint8_t      EmcyNum = 0;
static uint32_t     EmcyHist[EMCY_HIST_MAX];
static uint32_t     EmcyId = 0;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

void EmcyResetTable(void)
{
    uint32_t idx;

    for (idx = 0; idx < EMCY_CODE_MAX; idx++) {
        EmcyCode[idx].Code = 0;
        EmcyCode[idx].Reg  = 0;
    }

    for (idx = 0; idx < EMCY_HIST_MAX; idx++) {
        EmcyHist[idx] = 0;
    }

    EmcyNum = 0;
    EmcyId  = 0;
}

CO_EMCY_TBL *EmcyGetTable(void)
{
    return (&EmcyCode[0]);
}

uint32_t EmcyAddCode(int16_t code, uint8_t reg)
{
    uint32_t n;

    for (n = 0; n < EMCY_CODE_MAX; n++) {
        if (EmcyCode[n].Code == 0) {
            EmcyCode[n].Code = code;
            EmcyCode[n].Reg  = reg;
            break;
        }
    }

    return (n);
}

void EmcyObjDir(uint32_t len)
{
    uint32_t n;

    /* mandatory if emcy history is supported */
    TS_ODAdd(CO_KEY(0x1003, 0, CO_UNSIGNED8 |CO_OBJ____RW), CO_TEMCY, (uint32_t)&EmcyNum);
    for (n = 1; n <= len; n++) {
        TS_ODAdd(CO_KEY(0x1003, n, CO_UNSIGNED32|CO_OBJ____R_), CO_TEMCY, (uint32_t)&EmcyHist[n]);
    }

    /* mandatory if emcy is supported */
    TS_ODAdd(CO_KEY(0x1014, 0, CO_UNSIGNED32|CO_OBJ__N_RW), 0, (uint32_t)&EmcyId);
    EmcyId = 0x00000080;
}
