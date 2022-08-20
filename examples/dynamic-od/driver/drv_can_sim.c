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

#include "drv_can_sim.h"

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

/* queue length is 128 messages per CAN bus and direction(send/receive) */
#define SIM_CAN_Q_LEN               128u

#define SIM_CAN_STAT_PASSIVE        (uint32_t)0x00000000
#define SIM_CAN_STAT_INIT           (uint32_t)0x00000001
#define SIM_CAN_STAT_ACTIVE         (uint32_t)0x00000002

/******************************************************************************
* PRIVATE TYPES
******************************************************************************/

typedef struct SIM_CAN_BUS_T {
    struct SIM_CAN_BUS_T *Addr;
    uint32_t              Status;
    uint32_t              Baudrate;
    uint32_t              TxOvr;
    uint32_t              RxOvr;
    CO_IF_FRM            *RxRd;
    CO_IF_FRM            *RxWr;
    CO_IF_FRM            *TxRd;
    CO_IF_FRM            *TxWr;
    CO_IF_FRM             RxQ[SIM_CAN_Q_LEN];
    CO_IF_FRM             TxQ[SIM_CAN_Q_LEN];
    SIM_CAN_IRQ           Handler;
} SIM_CAN_BUS;

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

static SIM_CAN_BUS CanBus = { 0u };

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void    DrvCanInit   (void);
static void    DrvCanEnable (uint32_t baudrate);
static int16_t DrvCanSend   (CO_IF_FRM *frm);
static int16_t DrvCanRead   (CO_IF_FRM *frm);
static void    DrvCanReset  (void);
static void    DrvCanClose  (void);

/******************************************************************************
* PUBLIC VARIABLE
******************************************************************************/

const CO_IF_CAN_DRV SimCanDriver = {
    DrvCanInit,
    DrvCanEnable,
    DrvCanRead,
    DrvCanSend,
    DrvCanReset,
    DrvCanClose
};

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void DrvCanInit(void)
{
    SIM_CAN_BUS *bus = &CanBus;

    if (bus->Addr == bus) {                           /* reset init state    */
        bus->Status = SIM_CAN_STAT_INIT; 
    } else {                                          /* initialize bus      */
        bus->Addr   = bus;
        bus->Status = SIM_CAN_STAT_INIT; 
    }
    bus->Baudrate = 0u;
    bus->TxOvr    = 0u;
    bus->RxOvr    = 0u;
    bus->RxWr     = &bus->RxQ[0u];
    bus->RxRd     = &bus->RxQ[0u];
    bus->TxWr     = &bus->TxQ[0u];
    bus->TxRd     = &bus->TxQ[0u];
}

static void DrvCanEnable(uint32_t baudrate)
{
    SIM_CAN_BUS *bus = &CanBus;

    bus->Status   |= SIM_CAN_STAT_ACTIVE;
    bus->Baudrate  = baudrate;
}

static int16_t DrvCanSend(CO_IF_FRM *frm)
{
    int16_t       result = 0u;
    SIM_CAN_BUS  *bus    = &CanBus;;
    CO_IF_FRM    *tx;
    uint8_t       byte;
    
    if ((bus->Status & SIM_CAN_STAT_ACTIVE) == 0u) {   /* CAN bus is passive */
        return ((int16_t)-1u);
    }

    tx = bus->TxWr;
    bus->TxWr++;
    if (bus->TxWr >= &bus->TxQ[SIM_CAN_Q_LEN]) {
        bus->TxWr = &bus->TxQ[0u];
    }
    if (bus->TxWr == bus->TxRd) {
        bus->TxOvr++;
        bus->TxWr = tx;
    } else {
        tx->Identifier = frm->Identifier;
        tx->DLC        = frm->DLC;
        for (byte = 0u; byte < 8u; byte++) {
            if (frm->DLC > byte) {
                tx->Data[byte] = frm->Data[byte] & 0xFFu;
            } else {
                tx->Data[byte] = 0u;
            }
        }
        result = sizeof(CO_IF_FRM);
    }
    return (result);
}

static int16_t DrvCanRead (CO_IF_FRM *frm)
{
    int16_t       result = 0u;
    SIM_CAN_BUS  *bus    = &CanBus;
    CO_IF_FRM    *rx;
    uint8_t       byte;

    if ((bus->Status & SIM_CAN_STAT_ACTIVE) == 0u) {   /* CAN bus is passive */
        return ((int16_t)-1u);
    }

    if (bus->RxRd != bus->RxWr) {                  /* CAN frame is available */
        rx = bus->RxRd;
        bus->RxRd++;
        if (bus->RxRd >= &bus->RxQ[SIM_CAN_Q_LEN]) {
            bus->RxRd = &bus->RxQ[0u];
        }

        frm->Identifier = rx->Identifier;
        frm->DLC        = rx->DLC;
        for (byte = 0u; byte < 8u; byte++) {
            if (frm->DLC > byte) {
                frm->Data[byte] = rx->Data[byte] & 0xFFu;
            } else {
                frm->Data[byte] = 0u;
            }
        }
        result = sizeof(CO_IF_FRM);
    }
    return (result);
}

static void DrvCanReset(void)
{
    SIM_CAN_BUS *bus      = &CanBus;
    uint32_t     baudrate = bus->Baudrate;

    DrvCanInit();
    DrvCanEnable(baudrate);
}

static void DrvCanClose(void)
{
    SIM_CAN_BUS *bus = &CanBus;

    bus->Status &= ~SIM_CAN_STAT_ACTIVE;
}

/******************************************************************************
* SPECIAL PUBLIC FUNCTIONS
******************************************************************************/

int16_t SimCanGetFrm(uint8_t *buf, uint16_t size)
{
    int16_t         result = 0u;
    SIM_CAN_BUS    *bus    = &CanBus;
    CO_IF_FRM      *tx;
    CO_IF_FRM      *frm;

    if (bus->TxRd != bus->TxWr) {
        tx = bus->TxRd;
        bus->TxRd++;
        if (bus->TxRd >= &bus->TxQ[SIM_CAN_Q_LEN]) {
            bus->TxRd = &bus->TxQ[0u];
        }

        if ((size >= sizeof(CO_IF_FRM)) &&
            (buf  != NULL             )) {
            frm             = (CO_IF_FRM*)buf;
            frm->Identifier = tx->Identifier;
            frm->DLC        = tx->DLC;
            frm->Data[0u]   = tx->Data[0u] & 0xFFu;
            frm->Data[1u]   = tx->Data[1u] & 0xFFu;
            frm->Data[2u]   = tx->Data[2u] & 0xFFu;
            frm->Data[3u]   = tx->Data[3u] & 0xFFu;
            frm->Data[4u]   = tx->Data[4u] & 0xFFu;
            frm->Data[5u]   = tx->Data[5u] & 0xFFu;
            frm->Data[6u]   = tx->Data[6u] & 0xFFu;
            frm->Data[7u]   = tx->Data[7u] & 0xFFu;
        }

        result = 1u;
    }

    return (result);
}

int16_t SimCanSetFrm (uint32_t Identifier, uint8_t DLC,
                  uint8_t Byte0, uint8_t Byte1, uint8_t Byte2, uint8_t Byte3,
                  uint8_t Byte4, uint8_t Byte5, uint8_t Byte6, uint8_t Byte7)
{
    int16_t       result = 0u;
    SIM_CAN_BUS  *bus    = &CanBus;
    CO_IF_FRM    *rx;

    rx = bus->RxWr;
    bus->RxWr++;
    if (bus->RxWr >= &bus->RxQ[SIM_CAN_Q_LEN]) {
        bus->RxWr = &bus->RxQ[0u];
    }
    if (bus->RxWr == bus->RxRd) {
        bus->RxOvr++;
        bus->RxWr = rx;
    } else {
        rx->Identifier = Identifier;
        rx->DLC        = DLC;
        rx->Data[0u]   = Byte0 & 0xFFu;
        rx->Data[1u]   = Byte1 & 0xFFu;
        rx->Data[2u]   = Byte2 & 0xFFu;
        rx->Data[3u]   = Byte3 & 0xFFu;
        rx->Data[4u]   = Byte4 & 0xFFu;
        rx->Data[5u]   = Byte5 & 0xFFu;
        rx->Data[6u]   = Byte6 & 0xFFu;
        rx->Data[7u]   = Byte7 & 0xFFu;
        result         = sizeof(CO_IF_FRM);
    }

    return (result);
}

void SimCanSetIsr(SIM_CAN_IRQ handler)
{
    SIM_CAN_BUS *bus = &CanBus;

    bus->Handler = handler;
}

void SimCanRun(void)
{
    SIM_CAN_BUS *bus = &CanBus;

    while (bus->RxRd != bus->RxWr) {
        bus->Handler();
    }
}

void SimCanFlush (void)
{
    SIM_CAN_BUS *bus = &CanBus;

    bus->RxWr  = &bus->RxQ[0u];
    bus->RxRd  = &bus->RxQ[0u];
    bus->TxWr  = &bus->TxQ[0u];
    bus->TxRd  = &bus->TxQ[0u];
}
