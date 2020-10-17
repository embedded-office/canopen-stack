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

#ifndef APP_H_
#define APP_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_core.h"
#include "co_cfg.h"

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*---------------------------------------------------------------------------*/
/*! \brief TEST APPLICATION ENTRY
*
* \details The main-function is responsible for the test organization and
*          (optional) the pre-selection of a subset of tests for execution.
*          Per default are all existing test selected for execution. You may
*          limit the amount of running tests during the test development.
*
* \return
*          number of failed tests (matching the typical errorcode convention)
*/
/*---------------------------------------------------------------------------*/
int main(void);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif
