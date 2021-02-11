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

#include "app_env.h"
#include <stdio.h>

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

/* Maximal size of dynamic object dictionary */
#define TS_OD_MAX   64

/* Maximal depth of emergency history */
#define TS_EMCY_HIST_MAX   4

/* Maximal number of timers in tests */
#define TS_TMR_N   16

/* Timer clock frequency */
#define TS_TMR_FREQ   100

/* Maximal number of SDO servers */
#define TS_SDOS_N   1

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

/* reference to the CANopen node for testing */
static CO_NODE *TS_TestNode;
/* allocate memory for SDO server buffer */
static uint8_t SdoBuf[TS_SDOS_N][CO_SDO_BUF_BYTE];
/* allocate memory for highspeed timer */
static CO_TMR_MEM TmrMem[TS_TMR_N];
/* management structure for dynamic object dictionary */
static OD_DYN TS_ODDyn;
/* list of object entries for dynamic object dictionary */
static CO_OBJ TS_ODList[TS_OD_MAX];
/* object entry variable for 0x1001:0 (error register) */
static uint8_t TS_Obj1001_0;
/* object entry variable for 0x1003:0 (number of emergency errors) */
static uint8_t TS_Obj1003_0;
/* object entry variables for 0x1003:1..x (the emergency history) */
static uint32_t TS_Obj1003[TS_EMCY_HIST_MAX];
/* object entry variables for 0x12xx:1 (the SDOS request COB-ID) */
static uint32_t TS_Obj12xx_1[TS_SDOS_N];
/* object entry variables for 0x12xx:2 (the SDOS response COB-ID) */
static uint32_t TS_Obj12xx_2[TS_SDOS_N];
/* object entry variable for 0x1014:0 (COB-ID of EMCY message) */
static uint32_t TS_Obj1014_0;
/* object entry variable for 0x1017:0 (producer heartbeat time) */
static uint16_t TS_Obj1017_0;
/* select test drivers for simulated hardware modules */
static CO_IF_DRV TS_Driver = {
    &SimCanDriver,
    &SwCycleTimerDriver,
    &SimNvmDriver
};

/******************************************************************************
* PUBLIC VARIABLES
******************************************************************************/

uint32_t  TS_TmrCallCnt;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*---------------------------------------------------------------------------*/
/*! \brief REQ-ENV-0100
*
* \details When using no RTOS, the connection from CAN communication to the
*          CANopen stack is done with a direct function call within the
*          CAN interrupt handler.
*/
/*---------------------------------------------------------------------------*/
void TS_CanIsr(void)
{
    CONodeProcess(TS_TestNode);
}

/*
* \details Set the CANopen specification for the test specific environment:
*
*          **constant settings:**
*          - emergency: module 'app_emcy' with 4 pre-defined errors:
*            + error #0: CO_EMCY_CODE_GEN_ERR / CO_EMCY_REG_GENERAL
*            + error #1: CO_EMCY_CODE_CUR_ERR / CO_EMCY_REG_CURRENT
*            + error #2: CO_EMCY_CODE_VOL_ERR / CO_EMCY_REG_VOLTAGE
*            + error #3: CO_EMCY_CODE_TEMP_ERR / CO_EMCY_REG_TEMP
*
*          **object dictionary settings:**
*          - 1003:0 - Emergency number of errors (local variable)
*          - 1003:X - TS_EMCY_HIST_MAX error history fields (local variables)
*          - 1014:0 - COB-ID EMCY message (local variable)
*/
void TS_CreateEmcy(void)
{
    uint8_t n;

    EmcyAddCode(CO_EMCY_CODE_GEN_ERR,  CO_EMCY_REG_GENERAL);
    EmcyAddCode(CO_EMCY_CODE_CUR_ERR,  CO_EMCY_REG_CURRENT);
    EmcyAddCode(CO_EMCY_CODE_VOL_ERR,  CO_EMCY_REG_VOLTAGE);
    EmcyAddCode(CO_EMCY_CODE_TEMP_ERR, CO_EMCY_REG_TEMP   );

    TS_ODAdd(OBJ1003_0(&TS_Obj1003_0));
    TS_Obj1003_0 = 0;
    for (n = 1; n < TS_EMCY_HIST_MAX; n++) {
        TS_ODAdd(OBJ1003_X(n, &TS_Obj1003[n]));
        TS_Obj1003[n] = 0;
    }

    TS_ODAdd(OBJ1014_0(&TS_Obj1014_0));
    TS_Obj1014_0 = 0x80;
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-ENV-0110
*
* \details Set the CANopen specification for the test specific environment:
*
*          **constant settings:**
*          - node-id: 1
*          - baudrate: 250k
*          - can-driver: link the node to simulated CAN bus
*
*          **managed in dynamic application modules:**
*          - object dictionary: module 'app_dir' with size TS_OD_MAX
*
*          **local memory allocations:**
*          - timer: TS_TMR_N timers of type CO_TMR_MEM
*          - SDO buffer: TS_SDOS_N servers with CO_SDO_BUF_BYTE bytes
*/
/*---------------------------------------------------------------------------*/
void TS_CreateSpec(CO_NODE *node, CO_NODE_SPEC *spec, uint32_t freq)
{
    TS_TestNode    = node;                               /* link parent node */

    spec->NodeId   = 1u;                         /* setup node specification */
    spec->Baudrate = 250000u;
    spec->Drv      = &TS_Driver;

    spec->Dict      = ODGetDict(&TS_ODDyn);
    spec->DictLen   = TS_OD_MAX;

    spec->EmcyCode = EmcyGetTable();

    spec->TmrMem   = &TmrMem[0];
    spec->TmrNum   = TS_TMR_N;
    if (freq > 0) {
        spec->TmrFreq = freq;
    } else {
        spec->TmrFreq = TS_TMR_FREQ;
    }
    spec->SdoBuf   = &SdoBuf[0][0];

    SimCanSetIsr(TS_CanIsr);                /* connect to test can interface */
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-ENV-0120
*
* \details Create a CANopen node, using a local CANopen node specification.
*          Use this specification to create and start a CANopen node for
*          testing. Remove all existing CAN frames from the used CAN bus.
*
*          Starting mode is: PRE-OPERATIONAL
*/
/*---------------------------------------------------------------------------*/
void TS_CreateNode(CO_NODE *node, uint32_t freq)
{
    CO_NODE_SPEC spec;

    TS_CreateSpec(node, &spec, freq);

    CONodeInit(node, &spec);
    CONodeStart(node);

    SimCanFlush();
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-ENV-0130
*
* \details Create and start the CANopen testing node in PRE-OPERATIONAL mode
*          and switch the mode automatically to OPERATIONAL.
*/
/*---------------------------------------------------------------------------*/
void TS_CreateNodeAutoStart(CO_NODE *node)
{
    TS_CreateNode(node, 0);
    CONmtSetMode(&node->Nmt, CO_OPERATIONAL);
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-ENV-0140
*
* \details Append an object entry to the local object dictionary 'TS_ODDyn',
*          which is referenced in the specification, too.
*/
/*---------------------------------------------------------------------------*/
void TS_ODAdd(uint32_t key, const CO_OBJ_TYPE *type, uintptr_t data)
{
    ODAdd(&TS_ODDyn, key, (CO_OBJ_TYPE *)type, data);
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-ENV-0150
*
* \details Clear object dictionary and append the following object entries:
*          - 1000:0 - Device type (constant 0)
*          - 1001:0 - Error register (local variable)
*          - 1005:0 - COB-ID SYNC message (constant 0x80)
*          - 1017:0 - Producer heartbeat time (loval variable)
*          - 1018:0 - Identity object (constant 4)
*          - 1018:X - 1..4: Identity object values (constant 0)
*          - 12xx:0 - SDO server communication settings (constant 2)
*          - 12xx:1 - SDO request COB-ID (local variable)
*          - 12xx:2 - SDO response COB-ID (local variable)
*
*          Reset all local variables referenced in object entries.
*/
/*---------------------------------------------------------------------------*/
void TS_CreateMandatoryDir(void)
{
    uint32_t n;

    ODInit(&TS_ODDyn, &TS_ODList[0], TS_OD_MAX);
    TS_ODAdd(OBJ1000_0(0));
    TS_ODAdd(OBJ1001_0(&TS_Obj1001_0));
    TS_Obj1001_0 = 0;
    TS_ODAdd(OBJ1005_0(0x80));
    TS_ODAdd(OBJ1017_0(&TS_Obj1017_0));
    TS_Obj1017_0 = 0;
    TS_ODAdd(OBJ1018_0(4));
    TS_ODAdd(OBJ1018_1(0));
    TS_ODAdd(OBJ1018_2(0));
    TS_ODAdd(OBJ1018_3(0));
    TS_ODAdd(OBJ1018_4(0));
    for (n = 0; n < CO_SDOS_N; n++) {
        TS_ODAdd(OBJ12XX_0(n, 2));
        TS_ODAdd(OBJ12XX_1(n, &TS_Obj12xx_1[n]));
        TS_Obj12xx_1[n] = 0x600 + (n * 0x10);
        TS_ODAdd(OBJ12XX_2(n, &TS_Obj12xx_2[n]));
        TS_Obj12xx_2[n] = 0x580 + (n * 0x10);
    }
    
    EmcyResetTable();
    DomInit();
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-ENV-0160
*
* \details Clear object dictionary and append the following object entries:
*          - 14xx:0 - RPDO communication settings (constant 2)
*          - 14xx:1 - RPDO message COB-ID (parameter id)
*          - 14xx:2 - RPDO type (parameter type)
*
* \note    Parameter, used as object entries must be initialized by
*          calling function.
*/
/*---------------------------------------------------------------------------*/
void TS_CreateRPdoCom(uint8_t num, uint32_t *id, uint8_t *type)
{
    TS_ODAdd(OBJ14XX_0(num, 2));
    TS_ODAdd(OBJ14XX_1(num, id));
    TS_ODAdd(OBJ14XX_2(num, type));
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-ENV-0170
*
* \details Clear object dictionary and append the following object entries:
*          - 16xx:0 - RPDO mapping number (parameter len)
*          and for each entry N < len:
*          - 16xx:N - RPDO application object mapping (parameter map[N])
*
* \note    Parameter, used as object entries must be initialized by
*          calling function.
*/
/*---------------------------------------------------------------------------*/
void TS_CreateRPdoMap(uint8_t num, uint32_t *map, uint8_t *len)
{
    uint8_t n;

    TS_ODAdd(OBJ16XX_0(num, len));
    if (*len != 0) {
        for (n = 0; n < *len; n++) {
            TS_ODAdd(OBJ16XX_N(num, 1 + n, &map[n]));
        }
    }
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-ENV-0180
*
* \details Clear object dictionary and append the following object entries:
*          - 18xx:0 - TPDO communication settings (constant 2)
*          - 18xx:1 - TPDO message COB-ID (parameter id)
*          - 18xx:2 - TPDO type (parameter type)
*          - 18xx:3 - TPDO inhibit time (parameter inhibit)
*          - 18xx:5 - TPDO event timer (parameter evtimer)
*
* \note    Parameter, used as object entries must be initialized by
*          calling function.
*/
/*---------------------------------------------------------------------------*/
void TS_CreateTPdoCom(uint8_t   num,
                      uint32_t *id,
                      uint8_t  *type,
                      uint16_t *inhibit,
                      uint16_t *evtimer)
{
    TS_ODAdd(OBJ18XX_0(num, 5));
    TS_ODAdd(OBJ18XX_1(num, id));
    TS_ODAdd(OBJ18XX_2(num, type));
    TS_ODAdd(OBJ18XX_3(num, inhibit));
    TS_ODAdd(OBJ18XX_5(num, evtimer));
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-ENV-0190
*
* \details Clear object dictionary and append the following object entries:
*          - 1Axx:0 - TPDO mapping number (parameter len)
*          and for each entry N < len:
*          - 1Axx:N - TPDO application object mapping (parameter map[N])
*
* \note    Parameter, used as object entries must be initialized by
*          calling function.
*/
/*---------------------------------------------------------------------------*/
void TS_CreateTPdoMap(uint8_t num, uint32_t *map, uint8_t *len)
{
    uint8_t n;

    TS_ODAdd(OBJ1AXX_0(num, len));
    if (*len != 0) {
        for (n = 0; n < *len; n++) {
            TS_ODAdd(OBJ1AXX_N(num, 1 + n, &map[n]));
        }
    }
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-ENV-0200
*
* \details Loop the given amount of milliseconds and manage the highspeed
*          timer service and processing functions.
*/
/*---------------------------------------------------------------------------*/
void TS_Wait(CO_NODE *node, uint32_t millisec)
{
    uint32_t time  = 0;
    uint32_t frac  = 0;
    uint32_t ticks = COTmrGetTicks(&node->Tmr, 1000, CO_TMR_UNIT_1MS);
    int16_t  elabsed;

    while (millisec > time) {               /* wait for given amount of time */
        elabsed = COTmrService(&node->Tmr); /* handle high speed timer event */
        if (elabsed > 0) {
            COTmrProcess(&node->Tmr);       /* process elapsed timer actions */
        }
        if (ticks <= 1000) {
            time += (1000 / ticks);
        } else {
            if (frac == 0) {
                frac = ticks / 1000;
            }
            if (frac > 0) {
                frac--;
                if (frac == 0) {
                    time++;
                }
            }
        }
    }
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-ENV-0210
*
* \details Implement the timer callback interface with a local calling
*          counter incremented.
*/
/*---------------------------------------------------------------------------*/
void TS_TmrFunc(void *arg)
{
    (void)arg;
    TS_TmrCallCnt++;
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-ENV-0220
*
* \details Check for upcounting byte values in the given domain.
*/
/*---------------------------------------------------------------------------*/
int16_t TS_DomainCheck(CO_OBJ_DOM *dom, uint8_t start, uint8_t missing)
{
    int16_t  result = 0;
    uint32_t size   = dom->Size;
    uint8_t  data   = 0;
    uint8_t  value  = start;
    uint8_t *ptr    = dom->Start;

    while (size > 0) {
        if (size > 1) {
            data = *ptr;
            ptr++;
            if (data != value) {
                result = value;
            }
        } else {
            data = *ptr;
            ptr++;
        }
        size--;
        if (size <= missing) {
            value = 0xFF;
        } else {
            value++;
            value &= 0xFF;
        }
    }

    return result;
}

/*---------------------------------------------------------------------------*/
/*! \brief REQ-ENV-0230
*
* \details Send SDO segments for transfering a upcounting byte counter.
*/
/*---------------------------------------------------------------------------*/
void TS_SendBlk(uint32_t start, uint8_t segnum, uint8_t last, uint8_t seglost)
{
    uint8_t  seg;                                         /* segment counter */
    uint32_t idx;                                 /* free running data index */


    seg = 1;                                  /* initialize sequence counter */
    idx = start;
    while ((seg <= segnum) && (seg > 0)) {
        SimCanGetFrm(NULL, 0);
        if ( (seg >= segnum) &&
             (last != 0     ) ) {
            seg = seg | 0x80;
        }
        if (seg != seglost) {
            TS_SEG_SEND(seg, idx);                       /* send SDO request */
        } else {
            TS_SDO_SEND(0, 0, 0, 0);                     /* send SDO request */
        }
        idx += 7;
        seg  = (seg + 1) & 0x7F;           /* calculate next segment counter */
    }
}
