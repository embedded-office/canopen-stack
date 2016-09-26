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

/*
****************************************************************************************************
*                                          TYPE DEFINITIONS
****************************************************************************************************
*/

typedef struct CO_VER_T {
    uint8_t Build;                                 /*!< version build number                   */
    uint8_t Minor;                                 /*!< minor version number                   */
    uint8_t Major;                                 /*!< major version number                   */
} CO_VER;

/*
****************************************************************************************************
*                                          LOCAL VARIABLES
****************************************************************************************************
*/

static CO_VER COVer = { 0L, 0, 0 };

/*
****************************************************************************************************
*                                     LOCAL FUNCTION PROTOTYPES
****************************************************************************************************
*/

static void CO_VerInit(void);

/*
****************************************************************************************************
*                                            FUNCTIONS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief  GET MAJOR VERSION VALUE
*
* \ingroup  API
*
*           This function returns the major version value of the encoded version information:
*
*           Version <major>.<minor>.<build>-<change>
*
* \return   The major version value.
*/
/*------------------------------------------------------------------------------------------------*/
uint8_t COVerMajor(void)
{
    if (COVer.Major == 0L) {                          /* see, if class CO_VER is not initialized  */
        CO_VerInit();                                 /* yes: initialize static instance now      */
    }
                                                      /*------------------------------------------*/
    return COVer.Major;                               /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  GET MINOR VERSION VALUE
*
* \ingroup  API
*
*           This function returns the minor version value of the encoded version information:
*
*           Version <major>.<minor>.<build>-<change>
*
* \return   The minor version value.
*/
/*------------------------------------------------------------------------------------------------*/
uint8_t COVerMinor(void)
{
    if (COVer.Major == 0L) {                          /* see, if class CO_VER is not initialized  */
        CO_VerInit();                                 /* yes: initialize static instance now      */
    }
                                                      /*------------------------------------------*/
    return COVer.Minor;                               /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  GET BUILD VERSION VALUE
*
* \ingroup  API
*
*           This function returns the build version value of the encoded version information:
*
*           Version <major>.<minor>.<build>-<change>
*
* \return   The build version value.
*/
/*------------------------------------------------------------------------------------------------*/
uint8_t COVerBuild(void)
{
    if (COVer.Major == 0L) {                          /* see, if class CO_VER is not initialized  */
        CO_VerInit();                                 /* yes: initialize static instance now      */
    }
                                                      /*------------------------------------------*/
    return COVer.Build;                               /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  GET VERSION INFORMATION
*
* \ingroup  API
*
*           This function returns the compact version information of the encoded version
*           information:
*
*           Version <major>.<minor>.<build>-<change>
*
*           in the following way:
*
*           Version Information =  <major> * 10000 + <minor> * 100 + <build>
*
*           For example the version 3.1.0 is encoded as decimal value: 30100.
*
* \return   The calculated version information value.
*/
/*------------------------------------------------------------------------------------------------*/
uint32_t COVersion(void)
{
    uint32_t version;                               /* Local: function result                   */
                                                      /*------------------------------------------*/
    version = COVerMajor() * 10000 +                  /* calculate version identification         */
              COVerMinor() * 100 +
              COVerBuild();
                                                      /*------------------------------------------*/
    return version;                                   /* return function result                   */
}

/*
****************************************************************************************************
*                                       INTERNAL FUNCTIONS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief  INITIALIZE VERSION INFORMATIONS
*
* \ingroup  INTERNAL
*
*           This function initializes the version information values in the local version object.
*/
/*------------------------------------------------------------------------------------------------*/
static void CO_VerInit(void)
{
    COVer.Major     = CO_VER_MAJOR;                   /* set given major version number           */
    COVer.Minor     = CO_VER_MINOR;                   /* set given minor version number           */
    COVer.Build     = CO_VER_BUILD;                   /* set given build information              */
}
