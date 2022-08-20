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

#ifndef CO_CAN_SIM_H_
#define CO_CAN_SIM_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_if.h"

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

typedef void (*SIM_CAN_IRQ)(void);

/******************************************************************************
* PUBLIC SYMBOLS
******************************************************************************/

extern const CO_IF_CAN_DRV SimCanDriver;

/******************************************************************************
* SPECIAL PUBLIC DRIVER FUNCTIONS
******************************************************************************/

/* CAN Bus Simulation Interface (for interfacing with automated tests only) */
int16_t     SimCanGetFrm    (uint8_t *buf, uint16_t size);
int16_t     SimCanSetFrm    (uint32_t Identifier, uint8_t DLC,
                             uint8_t Byte0, uint8_t Byte1, uint8_t Byte2,
                             uint8_t Byte3, uint8_t Byte4, uint8_t Byte5,
                             uint8_t Byte6, uint8_t Byte7);
void        SimCanSetIsr    (SIM_CAN_IRQ handler);
void        SimCanRun       (void);
void        SimCanFlush     (void);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif
