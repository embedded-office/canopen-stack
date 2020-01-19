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
