/*
****************************************************************************************************
* (c) copyright by
*     Embedded Office GmbH & Co. KG       Tel : (07522) 97 00 08-0
*     Friedrich-Ebert-Str. 20/1           Fax : (07522) 97 00 08-99
*     D-88239 Wangen                      Mail: info@embedded-office.de
*                                         Web : http://www.embedded-office.de
*
* All rights reserved. Confidential and Proprietary. Protected by international copyright laws.
* Knowledge of the source code may not be used to write a similar product.
* This file may only be used in accordance with a license and should not be
* redistributed in any way.
****************************************************************************************************
*/
/*!
****************************************************************************************************
* \file     co_ver.c
*
* \brief    CANOPEN IDENTIFICATION
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_ver.c#5 $
*
*           This file implements the CANopen version identification
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

/*
****************************************************************************************************
*                                             INCLUDES
****************************************************************************************************
*/

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
