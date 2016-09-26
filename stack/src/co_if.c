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

#include "co_core.h"

/******************************************************************************
* FUNCTIONS
******************************************************************************/

/*
* see function definition
*/
int16_t COIfRead (CO_IF *cif, CO_IF_FRM *frm)
{
    int16_t err = -1;

    /* insert your code here */

    return (err);
}

/*
* see function definition
*/
int16_t COIfSend(CO_IF *cif, CO_IF_FRM *frm)
{
    int16_t err = -1;

    /* insert your code here */

    if (err < 0) {
        cif->Node->Error = CO_ERR_IF_SEND;
    }

    return (err);
}

/*
* see function definition
*/
void COIfReset(CO_IF *cif)
{
    int16_t err = -1;

    /* insert your code here */
    
    if (err < 0) {
        cif->Node->Error = CO_ERR_IF_RESET;
    }
}

/*
* see function definition
*/
void COIfClose(CO_IF *cif)
{
    int16_t err = -1;

    /* insert your code here */
    
    if (err < 0) {
        cif->Node->Error = CO_ERR_IF_CLOSE;
    }
}

/*
* see function definition
*/
void COIfInit(CO_IF *cif, struct CO_NODE_T *node)
{
    int16_t err = -1;

    /* insert your code here */
    
    if (err < 0) {
        node->Error = CO_ERR_IF_INIT;
    }
}

/*
* see function definition
*/
void COIfEnable(CO_IF *cif, uint32_t baudrate)
{
    int16_t err = -1;
    CO_IF_DRV  drv;

    if (baudrate == 0) {
    	baudrate = cif->Node->Baudrate;
    }

    /* insert your code here */

    if (err < 0) {
        cif->Node->Error = CO_ERR_IF_ENABLE;
    } else {
    	cif->Node->Baudrate = baudrate;
    }
}

/*
* see function definition
*/
void COIfReceive(CO_IF_FRM *frm)
{
    (void)frm;
}
