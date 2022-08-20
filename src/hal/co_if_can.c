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
* PUBLIC FUNCTIONS
******************************************************************************/

/*
* see function definition
*/
void COIfCanInit(CO_IF *cif, struct CO_NODE_T *node)
{
    const CO_IF_CAN_DRV *can = cif->Drv->Can;
    (void)node;
    can->Init();
}

/*
* see function definition
*/
int16_t COIfCanRead (CO_IF *cif, CO_IF_FRM *frm)
{
    int16_t err;
    const CO_IF_CAN_DRV *can = cif->Drv->Can;

    err = can->Read(frm);
    if (err < (int16_t)0) {
        cif->Node->Error = CO_ERR_IF_CAN_READ;
    }
    return (err);
}

/*
* see function definition
*/
int16_t COIfCanSend(CO_IF *cif, CO_IF_FRM *frm)
{
    int16_t err;
    const CO_IF_CAN_DRV *can = cif->Drv->Can;

    err = can->Send(frm);
    if (err < (int16_t)0) {
        cif->Node->Error = CO_ERR_IF_CAN_SEND;
    }
    return (err);
}

/*
* see function definition
*/
void COIfCanReset(CO_IF *cif)
{
    const CO_IF_CAN_DRV *can = cif->Drv->Can;
    can->Reset();    
}

/*
* see function definition
*/
void COIfCanClose(CO_IF *cif)
{
    const CO_IF_CAN_DRV *can = cif->Drv->Can;
    can->Close(); 
}

/*
* see function definition
*/
void COIfCanEnable(CO_IF *cif, uint32_t baudrate)
{
    const CO_IF_CAN_DRV *can = cif->Drv->Can;

    if (baudrate == (uint32_t)0) {
    	baudrate = cif->Node->Baudrate;
    } else {
      	cif->Node->Baudrate = baudrate;
    }

    can->Enable(baudrate);
}
