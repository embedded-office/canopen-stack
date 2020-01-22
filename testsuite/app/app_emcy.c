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

/* object entry variable for 0x1003:0 (number of emergency errors) */
static uint8_t EmcyNum = 0;

/* object entry variables for 0x1003:1..x (the emergency history) */
static uint32_t EmcyHist[EMCY_HIST_MAX];

/* object entry variable for 0x1014:0 (COB-ID of EMCY message) */
static uint32_t EmcyId = 0;

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

    for (idx = 0; idx < EMCY_HIST_MAX; idx++) {
        EmcyHist[idx] = 0;
    }

    EmcyNum = 0;
    EmcyId  = 0;
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-TEM-0110
*
* \details Return the pointer to the first entry of the emergency map table.
*/
/*---------------------------------------------------------------------------*/
CO_EMCY_TBL *EmcyGetTable(void)
{
    return (&EmcyCode[0]);
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
            break;
        }
    }

    return (n);
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-TEM-0130
*
* \details Setup the object entries according to the standard. Note, that
*          the given depth shall be limited to the maximum possible history
*          entries EMCY_HIST_MAX.
*/
/*---------------------------------------------------------------------------*/
void EmcyObjDir(uint8_t depth)
{
    uint32_t n;

    /* number of emergency errors is mandatory */
    TS_ODAdd(CO_KEY(0x1003, 0, CO_UNSIGNED8|CO_OBJ____RW),
                    CO_TEMCY, (uint32_t)&EmcyNum);
                    
    /* emergency error history up to the given depth */
    for (n = 1; (n <= depth) && (n < EMCY_HIST_MAX); n++) {
        TS_ODAdd(CO_KEY(0x1003, n, CO_UNSIGNED32|CO_OBJ____R_),
                        CO_TEMCY, (uint32_t)&EmcyHist[n]);
    }

    /* EMCY COB-ID is mandatory, if emcy is supported */
    TS_ODAdd(CO_KEY(0x1014, 0, CO_UNSIGNED32|CO_OBJ__N_RW),
             0, (uint32_t)&EmcyId);

    /* set default COB-ID: 0x80 */
    EmcyId = 0x00000080;
}
