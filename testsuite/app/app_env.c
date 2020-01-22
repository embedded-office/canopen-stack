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

#define TS_OD_MAX     64         /* Maximal size of dynamic object directory */
#define TS_TMR_N      16         /* Maximal number of timers in tests        */
#define TS_CAN_BUSID   0         /* Select simulated CAN bus identifier      */

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

static uint8_t      SdoBuf[CO_SDOS_N][CO_SDO_BUF_BYTE];
static CO_NODE     *LastNode = 0;
static CO_TMR_MEM   TmrMem[TS_TMR_N];

static OD_DYN       TS_ODDyn;                         /* Dynamic Object Directory Object          */
static CO_OBJ       TS_ODList[TS_OD_MAX];             /* Dynamic Object Directory Memory          */

static uint8_t      TS_ErrReg  = 0;                   /* Object Entry: Error Register             */
static int16_t      TS_HBTime  = 0;                   /* Object Entry: Heartbeat Producer         */

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

uint32_t  TS_TmrCallCnt;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

void TS_CanIsr(void)
{
    CONodeProcess(LastNode);
}

void TS_CreateSpec(CO_NODE *node, CO_NODE_SPEC *spec)
{
    LastNode       = node;

    spec->NodeId   = 1u;                              /*!< specify default Node-Id                */
    spec->Baudrate = 250000u;                         /*!< specify default Baudrate               */
    spec->Dir      = ODGetDir(&TS_ODDyn);             /*!< specify object directory               */
    spec->DirLen   = TS_OD_MAX;                       /*!< specify object directory (max) length  */
    spec->EmcyCode = EmcyGetTable();                  /*!< specify application EMCY info fields   */
    spec->TmrMem   = &TmrMem[0];                      /*!< specify timer memory blocks            */
    spec->TmrNum   = sizeof(TmrMem)/sizeof(CO_TMR_MEM);/*!< specify number of timer memory blocks  */
    spec->CanDrv   = TS_CAN_BUSID;                    /*!< specify linked CAN bus configuration   */
    spec->SdoBuf   = &SdoBuf[0][0];                   /*!< specify SDO Transfer Buffer Memory     */

    SetCanIsr(TS_CAN_BUSID, TS_CanIsr);
}

void TS_CreateNode(CO_NODE *node)
{
    CO_NODE_SPEC spec;

    TS_CreateEmcyTable();
    TS_CreateSpec(node, &spec);

    CONodeInit(node, &spec);
    CONodeStart(node);

    SimCanFlush(TS_CAN_BUSID);          /* remove all simulated CAN messages */
}

void TS_CreateNodeAutoStart(CO_NODE *node)
{
    TS_CreateNode(node);
    CONmtSetMode(&node->Nmt, CO_OPERATIONAL);
}

void TS_ODAdd(uint32_t key, CO_OBJ_TYPE *type, uintptr_t data)
{
    ODAdd(&TS_ODDyn, key, type, data);
}

void TS_CreateMandatoryDir(void)
{
    uint32_t srv;

    TS_ErrReg  = 0;
    TS_HBTime  = 0;

    DomInit();

    ODInit(&TS_ODDyn, &TS_ODList[0], TS_OD_MAX);
    TS_ODAdd(CO_KEY(0x1000, 0, CO_UNSIGNED32|CO_OBJ_D__R_), 0, 0x00000000);
    TS_ODAdd(CO_KEY(0x1001, 0, CO_UNSIGNED8 |CO_OBJ___PR_), 0, (uintptr_t)&TS_ErrReg);
    TS_ODAdd(CO_KEY(0x1005, 0, CO_UNSIGNED32|CO_OBJ_D__R_), 0, 0x00000080);
    TS_ODAdd(CO_KEY(0x1017, 0, CO_UNSIGNED16|CO_OBJ____RW), 0, (uintptr_t)&COTNmtHbProd);
    TS_ODAdd(CO_KEY(0x1018, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, 0x04);
    TS_ODAdd(CO_KEY(0x1018, 1, CO_UNSIGNED32|CO_OBJ_D__R_), 0, 0x00000000);
    TS_ODAdd(CO_KEY(0x1018, 2, CO_UNSIGNED32|CO_OBJ_D__R_), 0, 0x00000000);
    TS_ODAdd(CO_KEY(0x1018, 3, CO_UNSIGNED32|CO_OBJ_D__R_), 0, 0x00000000);
    TS_ODAdd(CO_KEY(0x1018, 4, CO_UNSIGNED32|CO_OBJ_D__R_), 0, 0x00000000);
    for (srv = 0; srv < CO_SDOS_N; srv++) {
        TS_ODAdd(CO_KEY(0x1200+srv, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, 0x02);
        TS_ODAdd(CO_KEY(0x1200+srv, 1, CO_UNSIGNED32|CO_OBJ_DN_R_), 0, 0x00000600+(srv*0x10));
        TS_ODAdd(CO_KEY(0x1200+srv, 2, CO_UNSIGNED32|CO_OBJ_DN_R_), 0, 0x00000580+(srv*0x10));
    }
    EmcyObjDir(EMCY_HIST_MAX);
}

void TS_CreateTPdoCom(uint8_t num, uint32_t *id, uint8_t *type, uint16_t *inhibit, uint16_t *evtimer)
{
    TS_ODAdd(CO_KEY(0x1800 + num, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0,         (uintptr_t)0x05);
    TS_ODAdd(CO_KEY(0x1800 + num, 1, CO_UNSIGNED32|CO_OBJ__N_R_), 0,         (uintptr_t)id);
    TS_ODAdd(CO_KEY(0x1800 + num, 2, CO_UNSIGNED8 |CO_OBJ____R_), 0,         (uintptr_t)type);
    TS_ODAdd(CO_KEY(0x1800 + num, 3, CO_UNSIGNED16|CO_OBJ____R_), 0,         (uintptr_t)inhibit);
    TS_ODAdd(CO_KEY(0x1800 + num, 5, CO_UNSIGNED16|CO_OBJ____RW), CO_TEVENT, (uintptr_t)evtimer);
}

void TS_CreateTPdoMap(uint8_t num, uint32_t *map, uint8_t *len)
{
    uint8_t n;

    TS_ODAdd(CO_KEY(0x1A00 + num, 0, CO_UNSIGNED8 |CO_OBJ____R_), 0, (uintptr_t)len);
    if (*len != 0) {
        for (n = 0; n < *len; n++) {
            TS_ODAdd(CO_KEY(0x1A00 + num, 1 + n, CO_UNSIGNED32|CO_OBJ____R_), 0, (uintptr_t)&map[n]);
        }
    }
}

void TS_CreateRPdoCom(uint8_t num, uint32_t *id, uint8_t *type)
{
    TS_ODAdd(CO_KEY(0x1400 + num, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, (uintptr_t)0x02);
    TS_ODAdd(CO_KEY(0x1400 + num, 1, CO_UNSIGNED32|CO_OBJ__N_R_), 0, (uintptr_t)id);
    TS_ODAdd(CO_KEY(0x1400 + num, 2, CO_UNSIGNED8 |CO_OBJ____R_), 0, (uintptr_t)type);
}

void TS_CreateRPdoMap(uint8_t num, uint32_t *map, uint8_t *len)
{
    uint8_t n;

    TS_ODAdd(CO_KEY(0x1600 + num, 0, CO_UNSIGNED8 |CO_OBJ____R_), 0, (uintptr_t)len);
    if (*len != 0) {
        for (n = 0; n < *len; n++) {
            TS_ODAdd(CO_KEY(0x1600 + num, 1 + n, CO_UNSIGNED32|CO_OBJ____R_), 0, (uintptr_t)&map[n]);
        }
    }
}

void TS_CreateDynTPdoCom(uint8_t num, uint32_t *id, uint8_t *type, uint16_t *inhibit, uint16_t *evtimer)
{
    TS_ODAdd(CO_KEY(0x1800 + num, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0,           (uintptr_t)0x02);
    TS_ODAdd(CO_KEY(0x1800 + num, 1, CO_UNSIGNED32|CO_OBJ__N_RW), CO_TPDOID,   (uintptr_t)id);
    TS_ODAdd(CO_KEY(0x1800 + num, 2, CO_UNSIGNED8 |CO_OBJ____RW), CO_TPDOTYPE, (uintptr_t)type);
    TS_ODAdd(CO_KEY(0x1800 + num, 3, CO_UNSIGNED16|CO_OBJ____RW), 0,           (uintptr_t)inhibit);
    TS_ODAdd(CO_KEY(0x1800 + num, 5, CO_UNSIGNED16|CO_OBJ____RW), CO_TEVENT,   (uintptr_t)evtimer);
}

void TS_CreateDynTPdoMap(uint8_t num, uint32_t *map, uint8_t *len)
{
    uint8_t n;

    TS_ODAdd(CO_KEY(0x1A00 + num, 0, CO_UNSIGNED8 |CO_OBJ____RW), CO_TPDONUM, (uintptr_t)len);
    if (*len != 0) {
        for (n = 0; n < *len; n++) {
            TS_ODAdd(CO_KEY(0x1A00 + num, 1 + n, CO_UNSIGNED32|CO_OBJ____RW), CO_TPDOMAP, (uintptr_t)&map[n]);
        }
    }
}

void TS_CreateDynRPdoCom(uint8_t num, uint32_t *id, uint8_t *type)
{
    TS_ODAdd(CO_KEY(0x1400 + num, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0,           (uintptr_t)0x02);
    TS_ODAdd(CO_KEY(0x1400 + num, 1, CO_UNSIGNED32|CO_OBJ__N_RW), CO_TPDOID,   (uintptr_t)id);
    TS_ODAdd(CO_KEY(0x1400 + num, 2, CO_UNSIGNED8 |CO_OBJ____RW), CO_TPDOTYPE, (uintptr_t)type);
}

void TS_CreateDynRPdoMap(uint8_t num, uint32_t *map, uint8_t *len)
{
    uint8_t n;

    TS_ODAdd(CO_KEY(0x1600 + num, 0, CO_UNSIGNED8 |CO_OBJ____RW), CO_TPDONUM, (uintptr_t)len);
    if (*len != 0) {
        for (n = 0; n < *len; n++) {
            TS_ODAdd(CO_KEY(0x1600 + num, 1 + n, CO_UNSIGNED32|CO_OBJ____RW), CO_TPDOMAP, (uintptr_t)&map[n]);
        }
    }
}

void TS_CreateEmcyTable(void)
{
    /* setup emergency table */
    EmcyResetTable();

    EmcyAddCode(CO_EMCY_CODE_GEN_ERR,  CO_EMCY_REG_GENERAL);
    EmcyAddCode(CO_EMCY_CODE_CUR_ERR,  CO_EMCY_REG_CURRENT);
    EmcyAddCode(CO_EMCY_CODE_VOL_ERR,  CO_EMCY_REG_VOLTAGE);
    EmcyAddCode(CO_EMCY_CODE_TEMP_ERR, CO_EMCY_REG_TEMP   );
}

void TS_Wait(CO_NODE *node, uint32_t millisec)
{
    uint32_t time = 0;                              /* Local: time counter limit                */
    int16_t elabsed;

    while (millisec > time) {                         /* wait until given amount of time is past  */
        elabsed = COTmrService(&node->Tmr);           /* handle high speed timer event            */
        if (elabsed > 0) {
            COTmrProcess(&node->Tmr);                 /* process the elapsed timer actions        */
        }
        time += (1000 / CO_TMR_TICKS_PER_SEC);
    }
}
 
void TS_TmrFunc(void *arg)
{
    (void)arg;                                        /* unused; prevent compiler warning         */

    TS_TmrCallCnt++;                                  /* increment timer callback calling counter */
}

void TS_DomainReset(CO_OBJ_DOM *dom)
{
    DomClear(dom);
}

void TS_DomainFill(CO_OBJ_DOM *dom, uint8_t start)
{
    uint32_t size = dom->Size;
    uint8_t *ptr  = dom->Start;
    uint8_t data  = start;

    while (size > 0) {
        *ptr = data;
        ptr++;
        data++;
        size--;
    }
}

int16_t TS_DomainCheck(CO_OBJ_DOM *dom, uint8_t start, uint8_t missing)
{
    int16_t result = 0;
    uint32_t size   = dom->Size;
    uint8_t data   = 0;
    uint8_t value  = start;
    uint8_t *ptr   = dom->Start;

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

void TS_SendBlk(uint32_t start, uint8_t segnum, uint8_t last, uint8_t seglost)
{
    uint8_t seg;                                   /* Local: segment counter                   */
    uint32_t idx;                                   /* Local: free running data index           */


    seg = 1;                                          /* initialize sequence counter              */
    idx = start;
    while ((seg <= segnum) && (seg > 0)) {
        if ( (seg >= segnum) &&
             (last != 0     ) ) {
            seg = seg | 0x80;
        }
        if (seg != seglost) {
            TS_SEG_SEND(seg, idx);                    /* send SDO request                         */
        } else {
            TS_SDO_SEND(0, 0, 0, 0);                  /* send SDO request                         */
        }
        RunSimCan(0, 0);                              /* run simulated CAN                        */
        idx += 7;
        seg =  (seg + 1) & 0x7F;                      /* calculate next segment counter           */
    }
}
