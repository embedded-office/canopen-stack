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

#ifndef CO_VER_H_
#define CO_VER_H_

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"

/******************************************************************************
* DEFINES
******************************************************************************/

#define CO_VER_MAJOR          4
#define CO_VER_MINOR          3
#define CO_VER_BUILD          1

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/*! \brief  GET MAJOR VERSION VALUE
*
*    This function returns the major version value of the encoded version
*    information:
*
*    Version <major>.<minor>.<build>
*
* \return
*    The major version value.
*/
uint8_t COVerMajor(void);

/*! \brief  GET MINOR VERSION VALUE
*
*    This function returns the minor version value of the encoded version
*    information:
*
*    Version <major>.<minor>.<build>
*
* \return
*    The minor version value.
*/
uint8_t COVerMinor(void);

/*! \brief  GET BUILD VERSION VALUE
*
*    This function returns the build version value of the encoded version
*    information:
*
*    Version <major>.<minor>.<build>
*
* \return
*    The build version value.
*/
uint8_t COVerBuild(void);

/*! \brief  GET VERSION INFORMATION
*
*    This function returns the compact version information of the encoded
*    version information:
*
*    Version <major>.<minor>.<build>
*
*    in the following way:
*
*    Version Information =  <major> * 10000 + <minor> * 100 + <build>
*
*    For example the version 3.1.0 is encoded as decimal value: 30100.
*
* \return
*    The calculated version information value.
*/
uint32_t COVersion(void);

/*! \brief  INITIALIZE VERSION INFORMATIONS
*
*    This function initializes the version information values in the local
*    version object.
*/
void CO_VerInit(void);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif /* ifndef CO_VER_H_ */
