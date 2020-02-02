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
