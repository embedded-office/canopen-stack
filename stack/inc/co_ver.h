/******************************************************************************
* (c) by Embedded Office GmbH & Co. KG, http://www.embedded-office.com
*------------------------------------------------------------------------------
* This file is part of CANopenStack, an open source CANopen Stack.
* Project home page is <https://github.com/MichaelHillmann/CANopenStack.git>.
* For more information on CANopen see < http ://www.can-cia.org/>.
*
* CANopenStack is free and open source software: you can redistribute
* it and / or modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation, either version 2 of the
* License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
******************************************************************************/

#ifndef CO_VER_H_
#define CO_VER_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"

/******************************************************************************
* DEFINES
******************************************************************************/

#define CO_VER_MAJOR          3
#define CO_VER_MINOR          3
#define CO_VER_BUILD          0

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

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

#ifdef __cplusplus
}
#endif

#endif /* ifndef CO_VER_H_ */
