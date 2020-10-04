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
int16_t COIfCanRead (CO_IF *cif, CO_IF_FRM *frm)
{
    int16_t err;
    const CO_IF_CAN_DRV *can = cif->Drv.Can;

    err = can->Read(frm);
    if (err < 0u) {
        cif->Node->Error = CO_ERR_IF_READ;
    }
    return (err);
}

/*
* see function definition
*/
int16_t COIfCanSend(CO_IF *cif, CO_IF_FRM *frm)
{
    int16_t err;
    const CO_IF_CAN_DRV *can = cif->Drv.Can;

    err = can->Send(frm);
    if (err < 0u) {
        cif->Node->Error = CO_ERR_IF_SEND;
    }
    return (err);
}

/*
* see function definition
*/
void COIfCanReset(CO_IF *cif)
{
    int16_t err;
    const CO_IF_CAN_DRV *can = cif->Drv.Can;

    err = can->Reset();    
    if (err < 0) {
        cif->Node->Error = CO_ERR_IF_RESET;
    }
}

/*
* see function definition
*/
void COIfCanClose(CO_IF *cif)
{
    int16_t err;
    const CO_IF_CAN_DRV *can = cif->Drv.Can;

    err = can->Close(); 
    if (err < 0) {
        cif->Node->Error = CO_ERR_IF_CLOSE;
    }
}

/*
* see function definition
*/
void COIfCanInit(CO_IF *cif, struct CO_NODE_T *node)
{
    int16_t err;
    const CO_IF_CAN_DRV *can = cif->Drv.Can;

    err = can->Init(); 
    if (err < 0) {
        node->Error = CO_ERR_IF_INIT;
    } else {
        cif->Node = node;
    }
}

/*
* see function definition
*/
void COIfCanEnable(CO_IF *cif, uint32_t baudrate)
{
    int16_t err;
    const CO_IF_CAN_DRV *can = cif->Drv.Can;

    if (baudrate == 0) {
    	baudrate = cif->Node->Baudrate;
    }

    err = can->Enable(baudrate);
    if (err < 0) {
        cif->Node->Error = CO_ERR_IF_ENABLE;
    } else {
    	cif->Node->Baudrate = baudrate;
    }
}
