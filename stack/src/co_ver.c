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

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_ver.h"

/******************************************************************************
* PRIVATE TYPES
******************************************************************************/

typedef struct CO_VER_T {
    uint8_t Build;               /*!< version build number                   */
    uint8_t Minor;               /*!< minor version number                   */
    uint8_t Major;               /*!< major version number                   */

} CO_VER;

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

static CO_VER COVer = { 0L, 0, 0 };

/******************************************************************************
* FUNCTIONS
******************************************************************************/

/*
* see function definition
*/
uint8_t COVerMajor(void)
{
    if (COVer.Major == 0L) {
        CO_VerInit();
    }

    return COVer.Major;
}

/*
* see function definition
*/
uint8_t COVerMinor(void)
{
    if (COVer.Major == 0L) {
        CO_VerInit();
    }

    return COVer.Minor;
}

/*
* see function definition
*/
uint8_t COVerBuild(void)
{
    if (COVer.Major == 0L) {
        CO_VerInit();
    }

    return COVer.Build;
}

/*
* see function definition
*/
uint32_t COVersion(void)
{
    uint32_t version;

    version = COVerMajor() * 10000 +
              COVerMinor() * 100 +
              COVerBuild();

    return version;
}

/*
* see function definition
*/
void CO_VerInit(void)
{
    COVer.Major     = CO_VER_MAJOR;
    COVer.Minor     = CO_VER_MINOR;
    COVer.Build     = CO_VER_BUILD;
}
