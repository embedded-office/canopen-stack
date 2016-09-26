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

uint8_t  COVerMajor   (void);
uint8_t  COVerMinor   (void);
uint8_t  COVerBuild   (void);
uint32_t COVerChange  (void);
uint32_t COVersion    (void);

#ifdef __cplusplus
}
#endif

#endif /* ifndef CO_VER_H_ */
