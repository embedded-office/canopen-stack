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

/*! \brief DEFAULT SDO SERVER
*
*    This configuration define specifies how many SDO servers the library
*    will support.
*/
#ifndef CO_SDOS_N
#define CO_SDOS_N               1
#endif

/*! \brief DEFAULT EMERGENCY CODES
*
*    This configuration define specifies how many emergency codes the library
*    will support.
*/
#ifndef CO_EMCY_N
#define CO_EMCY_N              32
#endif

/*! \brief DEFAULT RECEIVE PDO
*
*    This configuration define specifies how many receive PDOs the library
*    will support.
*/
#ifndef CO_RPDO_N
#define CO_RPDO_N               4
#endif

/*! \brief DEFAULT TRANSMIT PDO
*
*    This configuration define specifies how many transmit PDOs the library
*    will support.
*/
#ifndef CO_TPDO_N
#define CO_TPDO_N               4
#endif


#endif  /* #ifndef CO_CFG_H_ */
