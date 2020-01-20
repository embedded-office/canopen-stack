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
