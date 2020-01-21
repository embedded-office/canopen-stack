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

#include "drv_can.h"
#include "ts_printf.h"

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

static SIM_CAN_BUS SimCan[SIM_CAN_BUS_N] = { 0 };
static uint32_t    SimCanDiag = 0;

/******************************************************************************
* PRIVATE MACROS
******************************************************************************/

#define ASSERT_VALID_BUSID(node,id,err)  do { \
    if ((id < 0) || (id >= SIM_CAN_BUS_N)) {  \
        node->Error = err;                    \
        return;                               \
    } } while(0)


#define ASSERT_VALID_BUSID_N(node,id,err,val)  do { \
    if ((id < 0) || (id >= SIM_CAN_BUS_N)) {        \
        node->Error = err;                          \
        return(val);                                \
    } } while(0)

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

void COIfInit(CO_IF *cif, struct CO_NODE_T *node)
{
    SIM_CAN_BUS *bus;
    CO_IF_DRV    busId = cif->Drv;
    
    ASSERT_VALID_BUSID(node, busId, CO_ERR_IF_INIT);

    bus = &SimCan[busId];
    if (bus->Addr == bus) {
        /* (re-)set init state */
        bus->Status = SIM_CAN_STAT_INIT; 
    } else {
        /* indicate CAN bus to initialized state */
        bus->Addr   = bus;
        bus->Status = SIM_CAN_STAT_INIT; 
    }
    bus->Baudrate = 0;
    bus->TxOvr    = 0;
    bus->RxOvr    = 0;
    bus->RxWr     = &bus->RxQ[0];
    bus->RxRd     = &bus->RxQ[0];
    bus->TxWr     = &bus->TxQ[0];
    bus->TxRd     = &bus->TxQ[0];
    cif->Node     = node;
}

void COIfEnable(CO_IF *cif, uint32_t baudrate)
{
    SIM_CAN_BUS *bus;
    CO_IF_DRV    busId = cif->Drv;
    
    ASSERT_VALID_BUSID(cif->Node, busId, CO_ERR_IF_ENABLE);

    if (baudrate == 0) {
    	baudrate = cif->Node->Baudrate;
    }

    bus = &SimCan[busId];
    bus->Status |= SIM_CAN_STAT_ACTIVE;
    bus->Baudrate = baudrate;
}

int16_t COIfSend(CO_IF *cif, CO_IF_FRM *frm)
{
    SIM_CAN_BUS  *bus;
    CO_IF_FRM    *tx;
    int16_t       result = 0;
    uint8_t       byte;
    CO_IF_DRV     busId = cif->Drv;
    
    ASSERT_VALID_BUSID_N(cif->Node, busId, CO_ERR_IF_SEND, -1);

    bus = &SimCan[busId];
    if ((bus->Status & SIM_CAN_STAT_ACTIVE) == 0) {
        /* CAN bus is passive */
        return (-1);
    }

    tx = bus->TxWr;
    bus->TxWr++;
    if (bus->TxWr >= &bus->TxQ[SIM_CAN_Q_LEN]) {
        bus->TxWr = &bus->TxQ[0];
    }
    if (bus->TxWr == bus->TxRd) {
        bus->TxOvr++;
        bus->TxWr = tx;
    } else {
        tx->Identifier = frm->Identifier;
        tx->DLC        = frm->DLC;
        for (byte = 0; byte < 8; byte++) {
            if (frm->DLC > byte) {
                tx->Data[byte] = frm->Data[byte] & 0xFF;
            } else {
                tx->Data[byte] = 0;
            }
        }
        if ((SimCanDiag & SIM_CAN_STAT_DIAGNOSTIC) != 0) {
            /* CAN bus diagnostic is ON */
            TS_Printf("Tx%d: %08x (%02x) -> [", busId, tx->Identifier, tx->DLC);
            for (byte = 0; byte < 8; byte++) {
                TS_Printf("%02x ", tx->Data[byte] & 0xFF);
            }
            TS_Printf("]\n");
        }
        result = sizeof(CO_IF_FRM);
    }
    return (result);
}

int16_t COIfRead (CO_IF *cif, CO_IF_FRM *frm)
{
    SIM_CAN_BUS  *bus;
    CO_IF_FRM    *rx;
    int16_t       result = 0;
    uint8_t       byte;
    CO_IF_DRV     busId = cif->Drv;
    
    ASSERT_VALID_BUSID_N(cif->Node, busId, CO_ERR_IF_SEND, -1);

    bus = &SimCan[busId];
    if ((bus->Status & SIM_CAN_STAT_ACTIVE) == 0) {
        /* CAN bus is passive */
        return (-1);
    }

    if (bus->RxRd != bus->RxWr) {
        /* CAN frame is available */
        rx = bus->RxRd;
        bus->RxRd++;
        if (bus->RxRd >= &bus->RxQ[SIM_CAN_Q_LEN]) {
            bus->RxRd = &bus->RxQ[0];
        }

        frm->Identifier = rx->Identifier;
        frm->DLC        = rx->DLC;
        for (byte = 0; byte < 8; byte++) {
            if (frm->DLC > byte) {
                frm->Data[byte] = rx->Data[byte] & 0xFF;
            } else {
                frm->Data[byte] = 0;
            }
        }
        if ((SimCanDiag & SIM_CAN_STAT_DIAGNOSTIC) != 0) {
            /* CAN bus diagnostic is ON */
            TS_Printf("Rx%d: %08x (%02x) <- [ ", busId, frm->Identifier, frm->DLC);
            for (byte = 0; byte < 8; byte++) {
                TS_Printf("%02x ", frm->Data[byte] & 0xFF);
            }
            TS_Printf("]\n");
        }
        result = sizeof(CO_IF_FRM);
    }
    return (result);
}

void COIfReset(CO_IF *cif)
{
    ASSERT_VALID_BUSID(cif->Node, cif->Drv, CO_ERR_IF_RESET);

    COIfInit(cif, cif->Node);
    COIfEnable(cif, cif->Node->Baudrate);
}

void COIfClose(CO_IF *cif)
{
    SIM_CAN_BUS *bus;
    CO_IF_DRV    busId = cif->Drv;
    
    ASSERT_VALID_BUSID(cif->Node, busId, CO_ERR_IF_CLOSE);

    bus = &SimCan[busId];
    bus->Status &= ~SIM_CAN_STAT_ACTIVE;
}


int16_t GetFrm(int16_t busId, uint8_t *buf, uint16_t size)
{
    SIM_CAN_BUS    *bus;
    CO_IF_FRM      *tx;
    CO_IF_FRM      *frm;
    int16_t         result = 0;

    if ((busId < 0) ||
        (busId >= (int16_t)SIM_CAN_BUS_N)) {
        return (-1);
    }
    if (size != sizeof(CO_IF_FRM)) {
        return (-1);
    }
    if (buf == 0) {
        return (-1);
    }

    bus = &SimCan[busId];
    if (bus->TxRd != bus->TxWr) {
        tx = bus->TxRd;
        bus->TxRd++;
        if (bus->TxRd >= &bus->TxQ[SIM_CAN_Q_LEN]) {
            bus->TxRd = &bus->TxQ[0];
        }

        frm             = (CO_IF_FRM*)buf;
        frm->Identifier = tx->Identifier;
        frm->DLC        = tx->DLC;
        frm->Data[0]    = tx->Data[0] & 0xFF;
        frm->Data[1]    = tx->Data[1] & 0xFF;
        frm->Data[2]    = tx->Data[2] & 0xFF;
        frm->Data[3]    = tx->Data[3] & 0xFF;
        frm->Data[4]    = tx->Data[4] & 0xFF;
        frm->Data[5]    = tx->Data[5] & 0xFF;
        frm->Data[6]    = tx->Data[6] & 0xFF;
        frm->Data[7]    = tx->Data[7] & 0xFF;

        result = 1;
    }

    return (result);
}


int16_t SetRxFrm (int16_t busId, uint16_t Delay,
                  uint32_t Identifier, uint8_t DLC,
                  uint8_t Byte0, uint8_t Byte1, uint8_t Byte2, uint8_t Byte3,
                  uint8_t Byte4, uint8_t Byte5, uint8_t Byte6, uint8_t Byte7)
{
    SIM_CAN_BUS  *bus;
    CO_IF_FRM    *rx;
    int16_t       result = 0;

    (void)Delay;

    if ((busId < 0) ||
        (busId >= (int16_t)SIM_CAN_BUS_N)) {
        return (-1);
    }

    bus = &SimCan[busId];

    rx = bus->RxWr;
    bus->RxWr++;
    if (bus->RxWr >= &bus->RxQ[SIM_CAN_Q_LEN]) {
        bus->RxWr = &bus->RxQ[0];
    }
    if (bus->RxWr == bus->RxRd) {
        bus->RxOvr++;
        bus->RxWr = rx;
    } else {
        rx->Identifier = Identifier;
        rx->DLC        = DLC;
        rx->Data[0]    = Byte0 & 0xFF;
        rx->Data[1]    = Byte1 & 0xFF;
        rx->Data[2]    = Byte2 & 0xFF;
        rx->Data[3]    = Byte3 & 0xFF;
        rx->Data[4]    = Byte4 & 0xFF;
        rx->Data[5]    = Byte5 & 0xFF;
        rx->Data[6]    = Byte6 & 0xFF;
        rx->Data[7]    = Byte7 & 0xFF;
        result         = sizeof(CO_IF_FRM);
    }

    return (result);
}


SIM_CAN_IRQ SetCanIsr(int16_t busId, SIM_CAN_IRQ handler)
{
    SIM_CAN_IRQ     old = 0;
    SIM_CAN_BUS    *bus;                              /* Local: Pointer to CAN device data        */

    if ((busId < 0) ||                                /* devId out of range?                      */
        (busId >= (int16_t)SIM_CAN_BUS_N)) {
        return (0);
    }

    bus = &SimCan[busId];                             /* Set pointer to CAN device data           */

    old          = bus->Handler;
    bus->Handler = handler;

    return (old);
}


uint32_t CanDiagnosticOff(int16_t busId)
{
    uint32_t diag;
                                                      /*------------------------------------------*/

    if ((busId < 0) ||                                /* busId out of range?                      */
        (busId >= (int16_t)SIM_CAN_BUS_N)) {
        return (0);
    }

    diag = SimCanDiag & SIM_CAN_STAT_DIAGNOSTIC;
    SimCanDiag &= ~SIM_CAN_STAT_DIAGNOSTIC;

    if (diag != (SimCanDiag & SIM_CAN_STAT_DIAGNOSTIC)) {  /* see, if status changes              */
        TS_Printf("----- CAN Diagnostic: Disabled -----\n\n");
    }

    return (diag);
}


uint32_t CanDiagnosticOn(int16_t busId)
{
    uint32_t diag;
                                                      /*------------------------------------------*/

    if ((busId < 0) ||                                /* busId out of range?                      */
        (busId >= (int16_t)SIM_CAN_BUS_N)) {
        return (0);
    }

    diag = SimCanDiag & SIM_CAN_STAT_DIAGNOSTIC;
    SimCanDiag |= SIM_CAN_STAT_DIAGNOSTIC;

    if (diag != (SimCanDiag & SIM_CAN_STAT_DIAGNOSTIC)) { /* see, if status changes              */
        TS_Printf("\n----- CAN Diagnostic: Enabled -----\n");
    }

    return (diag);
}


void CanDiagnosticSet(int16_t busId, uint32_t diag)
{
    if ((busId < 0) ||                                /* busId out of range?                      */
        (busId >= (int16_t)SIM_CAN_BUS_N)) {
        return;
    }

    if ((diag & SIM_CAN_STAT_DIAGNOSTIC) != 0) {
        SimCanDiag |= SIM_CAN_STAT_DIAGNOSTIC;
    } else {
        SimCanDiag &= ~SIM_CAN_STAT_DIAGNOSTIC;
    }
}


int16_t CanDiagnosticGet(int16_t busId)
{
    uint32_t diag;

    if ((busId < 0) ||                                /* busId out of range?                      */
        (busId >= (int16_t)SIM_CAN_BUS_N)) {
        return (-1);
    }

    diag = SimCanDiag & SIM_CAN_STAT_DIAGNOSTIC;

    return (diag);
}


/*------------------------------------------------------------------------------------------------*/
/*! \brief  SIMULATE RX INTERRUPT
*
*           This function simulates the receiption of the given amount of CAN frames. If the
*           given number of CAN frames is 0 (zero), all CAN frames in the buffer are delivered.
*
* \param[in]       busId        simulated bus identifier
*
* \param[in]       max          max. number of delivered CAN frames
*/
/*------------------------------------------------------------------------------------------------*/
int16_t RunSimCan(int16_t busId, uint16_t max)
{
    uint16_t   unlimited = 0;                       /* Local: mark unlimited CAN receiption     */
    SIM_CAN_BUS *bus;                                 /* Local: Pointer to CAN bus                */
                                                      /*------------------------------------------*/

    if ((busId < 0) ||                                /* busId out of range?                      */
        (busId >= (int16_t)SIM_CAN_BUS_N)) {
        return (-1);
    }

    bus = &SimCan[busId];                             /* get addressed CAN bus object             */

    if (max == 0) {                                   /* see, if unlimited delivery is wanted     */
        unlimited = 1;                                /* mark unlimited CAN delivery              */
    }
    while (bus->RxRd != bus->RxWr) {                  /* see, if CAN messages are ready to rx     */
        if ((max > 0) || (unlimited == 1)) {          /* see, if CAN message can be delivered     */
            bus->Handler();                           /* handle CAN receive event                 */
            max--;                                    /* reduce maximal delivery counter          */
        } else {                                      /* otherwise: max number is reached         */
            break;                                    /* stop CAN frame delivery                  */
        }
    }
    return (max);                                     /* return remaining deliverable frames      */
}


int16_t SimCanFlush (int16_t busId)
{
    SIM_CAN_BUS *bus;                                 /* Local: Pointer to CAN device data        */
                                                      /*------------------------------------------*/
    if ((busId < 0) ||                                /* devId out of range?                      */
        (busId >= (int16_t)SIM_CAN_BUS_N)) {
        return (-1);
    }

    bus        = &SimCan[busId];                      /* Set pointer to CAN device data           */
    bus->RxWr  = &bus->RxQ[0];
    bus->RxRd  = &bus->RxQ[0];
    bus->TxWr  = &bus->TxQ[0];
    bus->TxRd  = &bus->TxQ[0];

    return (0);
}
