/******************************************************************************
* (c) by Embedded Office GmbH & Co. KG, <http://www.embedded-office.com/>
*------------------------------------------------------------------------------
* This file is part of CANopenStack, an open source CANopen Stack.
* Project home page is <https://github.com/MichaelHillmann/CANopenStack.git>.
* For more information on CANopen see <http://www.can-cia.org/>.
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

#ifndef CO_TYPES_H_
#define CO_TYPES_H_

#if defined(__STDC__) && (__STDC_VERSION__ >= 199901L)           /* ANSI C99 */

/******************************************************************************
* INCLUDES
******************************************************************************/

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#else                                                            /* ANSI C90 */

/******************************************************************************
* ANSI-C STANDARD TYPES
******************************************************************************/

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

#endif /* ifndef CO_TYPES_H_ */
