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

/* allocate memory for the emergency code mapping table */
static CO_EMCY_TBL EmcyCode[EMCY_CODE_MAX];
/* allocate memory for the number of registered emergency codes */
static uint8_t EmcyNum;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*---------------------------------------------------------------------------*/
/*! \brief REQ-TEM-0100
*
* \details The test emergency code definition table is hold in RAM to get
*          most flexibility for tests. We initialise all emergency object 
*          variables with 0 to operate like a powerup with BSS clearing.
*/
/*---------------------------------------------------------------------------*/
void EmcyResetTable(void)
{
    uint32_t idx;

    for (idx = 0; idx < EMCY_CODE_MAX; idx++) {
        EmcyCode[idx].Code = 0;
        EmcyCode[idx].Reg  = 0;
    }
    EmcyNum = 0;
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-TEM-0110
*
* \details Return the pointer to the first entry of the emergency map table.
*/
/*---------------------------------------------------------------------------*/
CO_EMCY_TBL *EmcyGetTable(void)
{
    CO_EMCY_TBL *result = NULL;

    if (EmcyNum > 0) {
        result = &EmcyCode[0];
    }
    return (result);
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-TEM-0120
*
* \details Fill the emergency map table from start to end without sorting.
*          If table is full, return the table size as indication.
*/
/*---------------------------------------------------------------------------*/
uint32_t EmcyAddCode(int16_t code, uint8_t reg)
{
    uint32_t n;

    for (n = 0; n < EMCY_CODE_MAX; n++) {
        if (EmcyCode[n].Code == 0) {
            EmcyCode[n].Code = code;
            EmcyCode[n].Reg  = reg;
            EmcyNum++;
            break;
        }
    }

    return (n);
}
