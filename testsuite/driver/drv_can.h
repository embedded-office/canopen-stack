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

#ifndef _DRV_CAN_H_
#define _DRV_CAN_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_core.h"

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

/* simulation supports 2 CAN busses with BusId = 0 and BusId = 1 */
#define SIM_CAN_BUS_N               2

/* queue length is 128 messages per CAN bus and direction(send/receive) */
#define SIM_CAN_Q_LEN               128

#define SIM_CAN_STAT_PASSIVE        (uint32_t)0x00000000
#define SIM_CAN_STAT_INIT           (uint32_t)0x00000001
#define SIM_CAN_STAT_ACTIVE         (uint32_t)0x00000002
#define SIM_CAN_STAT_DIAGNOSTIC     (uint32_t)0x80000000

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

typedef void (*SIM_CAN_IRQ)(void);

typedef struct SIM_CAN_BUS_T {
    struct SIM_CAN_BUS_T *Addr;
    uint32_t    Status;
    uint32_t    Baudrate;
    uint32_t    TxOvr;
    uint32_t    RxOvr;
    CO_IF_FRM  *RxRd;
    CO_IF_FRM  *RxWr;
    CO_IF_FRM  *TxRd;
    CO_IF_FRM  *TxWr;
    CO_IF_FRM   RxQ[SIM_CAN_Q_LEN];
    CO_IF_FRM   TxQ[SIM_CAN_Q_LEN];
    SIM_CAN_IRQ Handler;
} SIM_CAN_BUS;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/* Device Driver Functions: 
*  - Interface functions as defined in co_if.h
*/

/* CAN Bus Simulation Interface (for interfacing with automated tests only) */
int16_t     GetFrm          (int16_t busId, uint8_t *buf, uint16_t size);
int16_t     SetRxFrm        (int16_t busId, uint16_t Delay,
                             uint32_t Identifier, uint8_t DLC,
                             uint8_t Byte0, uint8_t Byte1, uint8_t Byte2,
                             uint8_t Byte3, uint8_t Byte4, uint8_t Byte5,
                             uint8_t Byte6, uint8_t Byte7);
SIM_CAN_IRQ SetCanIsr       (int16_t busId, SIM_CAN_IRQ handler);
int16_t     RunSimCan       (int16_t busId, uint16_t max);
int16_t     SimCanFlush     (int16_t busId);

uint32_t    CanDiagnosticOff(int16_t busId);
uint32_t    CanDiagnosticOn (int16_t busId);
void        CanDiagnosticSet(int16_t busId, uint32_t diag);
uint32_t    CanDiagnosticGet(int16_t busId);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif
