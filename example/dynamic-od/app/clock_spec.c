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

#include "clock_spec.h"
                                              /* select application drivers: */
#include "co_can_dummy.h"                     /* CAN driver                  */
#include "co_timer_dummy.h"                   /* Timer driver                */
#include "co_nvm_dummy.h"                     /* NVM driver                  */

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

/* Define some default values for our CANopen node: */
#define APP_NODE_ID       1u                  /* CANopen node ID             */
#define APP_BAUDRATE      250000u             /* CAN baudrate                */
#define APP_TMR_N         16u                 /* Number of software timers   */
#define APP_TICKS_PER_SEC 1000u               /* Timer clock frequency in Hz */
#define APP_OBJ_N         128u                /* Object dictionary max size  */

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

/* allocate global variables for runtime value of objects */
static uint8_t  Obj1001_00_08 = 0;

static uint32_t Obj2100_01_20 = 0;
static uint8_t  Obj2100_02_08 = 0;
static uint8_t  Obj2100_03_08 = 0;

/* allocate the dynamic object dictionary */
static CO_OBJ ClockOD[APP_OBJ_N];

/* allocate the dynamic dictionary management structure */
static OD_DYN DynDict;

/* Each software timer needs some memory for managing
 * the lists and states of the timed action events.
 */
static CO_TMR_MEM TmrMem[APP_TMR_N];

/* Each SDO server needs memory for the segmented or
 * block transfer requests.
 */
static uint8_t SdoSrvMem[CO_SDOS_N * CO_SDO_BUF_BYTE];

/* Select the drivers for your application. For possible
 * selections, see the directory /drivers. In this example
 * we select the driver templates. You may fill them with
 * your specific hardware functionality.
 */
static struct CO_IF_DRV_T AppDriver = {
    &DummyCanDriver,
    &DummyTimerDriver,
    &DummyNvmDriver
};

/* Specify the EMCY error codes with the corresponding
 * error register bit. There is a collection of defines
 * for the predefined emergency codes CO_EMCY_CODE...
 * and for the error register bits CO_EMCY_REG... for
 * readability. You can use plain numbers, too.
 */
static CO_EMCY_TBL AppEmcyTbl[APP_ERR_ID_NUM] = {
    { CO_EMCY_REG_GENERAL, CO_EMCY_CODE_GEN_ERR          }, /* APP_ERR_CODE_SOMETHING */
    { CO_EMCY_REG_TEMP   , CO_EMCY_CODE_TEMP_AMBIENT_ERR }  /* APP_ERR_CODE_HAPPENS   */
};

/******************************************************************************
* PUBLIC VARIABLES
******************************************************************************/

/* Collect all node specification settings in a single
 * structure for initializing the node easily.
 */
struct CO_NODE_SPEC_T AppSpec = {
    APP_NODE_ID,             /* default Node-Id                */
    APP_BAUDRATE,            /* default Baudrate               */
    &ClockOD[0],             /* pointer to object dictionary   */
    APP_OBJ_N,               /* object dictionary max length   */
    &AppEmcyTbl[0],          /* EMCY code & register bit table */
    &TmrMem[0],              /* pointer to timer memory blocks */
    APP_TMR_N,               /* number of timer memory blocks  */
    APP_TICKS_PER_SEC,       /* timer clock frequency in Hz    */
    &AppDriver,              /* select drivers for application */
    &SdoSrvMem[0]            /* SDO Transfer Buffer Memory     */
};

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void ODCreateSDOServer(OD_DYN *self, uint8_t srv, uint32_t request, uint32_t response)
{
    if (srv == 0) {
        request  = (uint32_t)0x600;
        response = (uint32_t)0x580;
    }
    ODAddUpdate(self, CO_KEY(0x1200+srv, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, CO_DATA_DIRECT(0x02));
    ODAddUpdate(self, CO_KEY(0x1200+srv, 1, CO_UNSIGNED32|CO_OBJ_DN_R_), 0, CO_DATA_DIRECT(request));
    ODAddUpdate(self, CO_KEY(0x1200+srv, 2, CO_UNSIGNED32|CO_OBJ_DN_R_), 0, CO_DATA_DIRECT(response));
}

static void ODCreateTPDOCom(OD_DYN *self, uint8_t num, uint32_t id, uint8_t type, uint16_t inhibit, uint16_t evtimer)
{
    ODAddUpdate(self, CO_KEY(0x1800+num, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, CO_DATA_DIRECT(0x05));
    ODAddUpdate(self, CO_KEY(0x1800+num, 1, CO_UNSIGNED32|CO_OBJ_DN_R_), 0, CO_DATA_DIRECT(id));
    ODAddUpdate(self, CO_KEY(0x1800+num, 2, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, CO_DATA_DIRECT(type));
    ODAddUpdate(self, CO_KEY(0x1800+num, 3, CO_UNSIGNED16|CO_OBJ_D__RW), 0, CO_DATA_DIRECT(inhibit));
    ODAddUpdate(self, CO_KEY(0x1800+num, 5, CO_UNSIGNED16|CO_OBJ_D__RW), CO_TEVENT, CO_DATA_DIRECT(evtimer));
}

static void ODCreateTPdoMap(OD_DYN *self, uint8_t num, uint32_t *map, uint8_t len)
{
    uint8_t n;

    ODAddUpdate(self, CO_KEY(0x1A00+num, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, CO_DATA_DIRECT(len));
    for (n = 0; n < len; n++) {
        ODAddUpdate(self, CO_KEY(0x1A00+num, 1+n, CO_UNSIGNED32|CO_OBJ_D__R_), 0, CO_DATA_DIRECT(map[n]));
    }
}

/* function to setup the quickstart object dictionary */
static void ODCreateDict(OD_DYN *self)
{
    uint32_t map[3];

    Obj1001_00_08 = 0;

    ODAddUpdate(self, CO_KEY(0x1000, 0, CO_UNSIGNED32|CO_OBJ_D__R_), 0, CO_DATA_DIRECT(0x00000000));
    ODAddUpdate(self, CO_KEY(0x1001, 0, CO_UNSIGNED8 |CO_OBJ___PR_), 0, CO_DATA_INDIRECT(&Obj1001_00_08));
    ODAddUpdate(self, CO_KEY(0x1005, 0, CO_UNSIGNED32|CO_OBJ_D__R_), 0, CO_DATA_DIRECT(0x80));
    ODAddUpdate(self, CO_KEY(0x1017, 0, CO_UNSIGNED16|CO_OBJ_D__R_), 0, CO_DATA_DIRECT(0));
    ODAddUpdate(self, CO_KEY(0x1018, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, CO_DATA_DIRECT(4));
    ODAddUpdate(self, CO_KEY(0x1018, 1, CO_UNSIGNED32|CO_OBJ_D__R_), 0, CO_DATA_DIRECT(0));
    ODAddUpdate(self, CO_KEY(0x1018, 2, CO_UNSIGNED32|CO_OBJ_D__R_), 0, CO_DATA_DIRECT(0));
    ODAddUpdate(self, CO_KEY(0x1018, 3, CO_UNSIGNED32|CO_OBJ_D__R_), 0, CO_DATA_DIRECT(0));
    ODAddUpdate(self, CO_KEY(0x1018, 4, CO_UNSIGNED32|CO_OBJ_D__R_), 0, CO_DATA_DIRECT(0));

    ODCreateSDOServer(self, 0, CO_COBID_SDO_REQUEST(), CO_COBID_SDO_RESPONSE());

    ODCreateTPDOCom(self, 0, CO_COBID_TPDO_DEFAULT(0), 254, 0, 0);

    map[0] = CO_LINK(0x2100, 1, 32);
    map[1] = CO_LINK(0x2100, 2,  8);
    map[2] = CO_LINK(0x2100, 3,  8);
    ODCreateTPdoMap(self, 0, map, 3);

    ODAddUpdate(self, CO_KEY(0x2100, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, CO_DATA_DIRECT(3));
    ODAddUpdate(self, CO_KEY(0x2100, 1, CO_UNSIGNED32|CO_OBJ___PR_), 0, CO_DATA_INDIRECT(&Obj2100_01_20));
    ODAddUpdate(self, CO_KEY(0x2100, 2, CO_UNSIGNED8 |CO_OBJ___PR_), 0, CO_DATA_INDIRECT(&Obj2100_02_08));
    ODAddUpdate(self, CO_KEY(0x2100, 3, CO_UNSIGNED8 |CO_OBJ___PR_), CO_TASYNC, CO_DATA_INDIRECT(&Obj2100_03_08));
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

void ClkStartNode(CO_NODE *node)
{
    /* Clear all entries in object dictionary */
    ODInit(&DynDict, &ClockOD[0], APP_OBJ_N);

    /* Setup the object dictionary during runtime */
    ODCreateDict(&DynDict);

    /* Create a node with generated object dictionary */
    CONodeInit(node, &AppSpec);
}
