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
* \file     co_ver.h
*
* \brief    CANOPEN IDENTIFICATION DEFINITION
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_ver.h#2 $
*
*           This include file defines the symbolic constants and data types for the CANopen
*           version identifivation.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_TYPES_H_
#define CO_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************************************
 *                                     ANSI-C STANDARD TYPES
 **************************************************************************************************/

#if defined(__STDC__) && (__STDC_VERSION__ >= 199901L)           /* ANSI C99 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#else                                                            /* ANSI C90 */

/*-- stddef.h --*/
#define NULL             0u

/*-- stdbool.h --*/
typedef int              bool;
#define false            0u
#define true             1u

/*-- stdint.h --*/
typedef unsigned char    uint8_t;
typedef unsigned char    int8_t;
typedef unsigned short   uint16_t;
typedef unsigned short   int16_t;
typedef unsigned long    uint32_t;
typedef unsigned long    int32_t;

/*-- crtdefs.h --*/
typedef unsigned long    intptr_t;

#endif


#ifdef __cplusplus
}
#endif

#endif /* ifndef CO_TYPES_H_ */
