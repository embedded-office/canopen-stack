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

#ifndef CO_CFG_H_
#define CO_CFG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef CO_SDO_N
#define CO_SDO_N                1
#endif
#ifndef CO_EMCY_N
#define CO_EMCY_N               4
#endif
#ifndef CO_TMR_N
#define CO_TMR_N                5
#endif
#ifndef CO_RPDO_N
#define CO_RPDO_N               4
#endif
#ifndef CO_TPDO_N
#define CO_TPDO_N               4
#endif

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_CFG_H_ */
