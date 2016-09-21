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
    CPU_INT32U Change;                                /*!< link to version control system         */
    CPU_INT08U Build;                                 /*!< version build number                   */
    CPU_INT08U Minor;                                 /*!< minor version number                   */
    CPU_INT08U Major;                                 /*!< major version number                   */
} CO_VER;

/*
****************************************************************************************************
*                                          Local CONSTANTS
****************************************************************************************************
*/

static const CPU_CHAR COChange[] = "$Change: 78313 $";

/*
****************************************************************************************************
*                                          LOCAL VARIABLES
****************************************************************************************************
*/

static CO_VER COVer = { 0L, 0, 0, 0 };

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
CPU_INT08U COVerMajor(void)
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
CPU_INT08U COVerMinor(void)
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
CPU_INT08U COVerBuild(void)
{
    if (COVer.Major == 0L) {                          /* see, if class CO_VER is not initialized  */
        CO_VerInit();                                 /* yes: initialize static instance now      */
    }
                                                      /*------------------------------------------*/
    return COVer.Build;                               /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  GET CHANGE VALUE
*
* \ingroup  API
*
*           This function returns the change value of the encoded version information:
*
*           Version <major>.<minor>.<build>-<change>
*
* \return   The change value.
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT32U COVerChange(void)
{
    if (COVer.Major == 0L) {                          /* see, if class CO_VER is not initialized  */
        CO_VerInit();                                 /* yes: initialize static instance now      */
    }
                                                      /*------------------------------------------*/
    return COVer.Change;                              /* return function result                   */
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
CPU_INT32U COVersion(void)
{
    CPU_INT32U version;                               /* Local: function result                   */
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
    const CPU_CHAR *str;                              /* Local: ptr in constant string            */
    CPU_INT32U  change;                               /* Local: converted change identification   */
    CPU_INT08U  numeric;                              /* Local: numeric character indication      */
    CPU_INT08U  safety_cnt = 32;                      /* Local: check max. N characters in change */
                                                      /*------------------------------------------*/
    str    = &COChange[0];                            /* set start of version change string       */
    change = (CPU_INT32U)0;                           /* clear change number                      */
    do {                                              /*------------------------------------------*/
        if ((*str >= '0') && (*str <= '9')) {         /* see, if character is a numeric           */
            change  = change * 10;                    /* shift number by a decimal digit to left  */
            change += (CPU_INT32U)(*str - '0');       /* convert number in value                  */
            numeric = 1;                              /* indicate numeric digit is processed      */
        } else {                                      /* otherwise: no numeric digit in string    */
            numeric = 0;                              /* indicate no numeric digit                */
        }
        str++;                                        /* switch to next character                 */
                                                      /*------------------------------------------*/
        safety_cnt--;                                 /* update number of processing characters   */
        if (safety_cnt == 0) {                        /* see, if limit is reached                 */
            change = 0xFFFFFFFF;                      /* set change to invalid value              */
            break;                                    /* abort further checkings                  */
        }
    } while ((numeric==1) || (change==0));            /* repeat until end of number string        */
                                                      /*------------------------------------------*/
    COVer.Change    = change;                         /* set calculated change number             */
    COVer.Major     = CO_VER_MAJOR;                   /* set given major version number           */
    COVer.Minor     = CO_VER_MINOR;                   /* set given minor version number           */
    COVer.Build     = CO_VER_BUILD;                   /* set given build information              */
}
