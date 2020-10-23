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

#ifndef APP_EMCY_H_
#define APP_EMCY_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_core.h"

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

/*---------------------------------------------------------------------------*/
/*! \brief NUMBER OF EMERGENCY CODES
*
* \details The number of supported emergency codes.
*/
/*---------------------------------------------------------------------------*/
#define EMCY_CODE_MAX   CO_EMCY_N

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*---------------------------------------------------------------------------*/
/*! \brief CLEAR EMCY TABLE
*
* \details Clear the configuration table which mapps the supported emergency
*          codes of the test environment to the error register code.
*/
/*---------------------------------------------------------------------------*/
void EmcyResetTable(void);

/*---------------------------------------------------------------------------*/
/*! \brief GET EMCY TABLE
*
* \details Retrieve the pointer to the emergency map table.
*
* \return  Pointer to the emergency map table
*/
/*---------------------------------------------------------------------------*/
CO_EMCY_TBL *EmcyGetTable(void);

/*---------------------------------------------------------------------------*/
/*! \brief ADD EMCY CODE TO EMCY TABLE
*
* \details Add a new mapping from an emergency error code to the
*          corresponding error register information.
*
* \param   code
*          The emergency error code
*          (e.g. 'CO_EMCY_CODE_VOL_ERR + x' for a specific voltage error)
*
* \param   reg
*          The error register information for this emergency error
*          (e.g. 'CO_EMCY_REG_VOLTAGE' maps to the voltage bit in register)
*
* \return  Index of new entry in table, or EMCY_CODE_MAX if table is full.
*/
/*---------------------------------------------------------------------------*/
uint32_t EmcyAddCode(int16_t code, uint8_t reg);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif
