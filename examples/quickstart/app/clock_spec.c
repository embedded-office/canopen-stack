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
#include "drv_can_sim.h"                      /* CAN driver                  */
#include "drv_timer_swcycle.h"                /* Timer driver                */
#include "drv_nvm_sim.h"                      /* NVM driver                  */

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

/* allocate variables for dynamic runtime value in RAM */
static uint8_t  Obj1001_00_08 = 0;

static uint16_t Obj1017_00_10 = 0;

static uint32_t Obj2100_01_20 = 0;
static uint8_t  Obj2100_02_08 = 0;
static uint8_t  Obj2100_03_08 = 0;

/* allocate variables for constant values in FLASH */
const  uint32_t Obj1000_00_20 = 0x00000000L;

const  uint32_t Obj1014_00_20 = 0x00000080L;

const  uint32_t Obj1018_01_20 = 0x00000000L;
const  uint32_t Obj1018_02_20 = 0x00000000L;
const  uint32_t Obj1018_03_20 = 0x00000000L;
const  uint32_t Obj1018_04_20 = 0x00000000L;

const  uint32_t Obj1200_01_20 = CO_COBID_SDO_REQUEST();
const  uint32_t Obj1200_02_20 = CO_COBID_SDO_RESPONSE();

const  uint32_t Obj1800_01_20 = CO_COBID_TPDO_DEFAULT(0);

const  uint32_t Obj1A00_01_20 = CO_LINK(0x2100, 0x01, 32);
const  uint32_t Obj1A00_02_20 = CO_LINK(0x2100, 0x02,  8);
const  uint32_t Obj1A00_03_20 = CO_LINK(0x2100, 0x03,  8);

/* define the static object dictionary */
static struct CO_OBJ_T ClockOD[APP_OBJ_N] = {
    {CO_KEY(0x1000, 0, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1000_00_20)},
    {CO_KEY(0x1001, 0, CO_OBJ_____R_), CO_TUNSIGNED8 , (CO_DATA)(&Obj1001_00_08)},
    {CO_KEY(0x1014, 0, CO_OBJ_____R_), CO_TEMCY_ID,    (CO_DATA)(&Obj1014_00_20)},
    {CO_KEY(0x1017, 0, CO_OBJ_____RW), CO_THB_PROD,    (CO_DATA)(&Obj1017_00_10)},

    {CO_KEY(0x1018, 0, CO_OBJ_D___R_), CO_TUNSIGNED8 , (CO_DATA)(4)             },
    {CO_KEY(0x1018, 1, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1018_01_20)},
    {CO_KEY(0x1018, 2, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1018_02_20)},
    {CO_KEY(0x1018, 3, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1018_03_20)},
    {CO_KEY(0x1018, 4, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1018_04_20)},

    {CO_KEY(0x1200, 0, CO_OBJ_D___R_), CO_TUNSIGNED8 , (CO_DATA)(2)             },
    {CO_KEY(0x1200, 1, CO_OBJ__N__R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1200_01_20)},
    {CO_KEY(0x1200, 2, CO_OBJ__N__R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1200_02_20)},

    {CO_KEY(0x1800, 0, CO_OBJ_D___R_), CO_TUNSIGNED8 , (CO_DATA)(2)             },
    {CO_KEY(0x1800, 1, CO_OBJ__N__R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1800_01_20)},
    {CO_KEY(0x1800, 2, CO_OBJ_D___R_), CO_TUNSIGNED8 , (CO_DATA)(254)           },

    {CO_KEY(0x1A00, 0, CO_OBJ_D___R_), CO_TUNSIGNED8 , (CO_DATA)(3)             },
    {CO_KEY(0x1A00, 1, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1A00_01_20)},
    {CO_KEY(0x1A00, 2, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1A00_02_20)},
    {CO_KEY(0x1A00, 3, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1A00_03_20)},

    {CO_KEY(0x2100, 0, CO_OBJ_D___R_), CO_TUNSIGNED8 , (CO_DATA)(3)             },
    {CO_KEY(0x2100, 1, CO_OBJ____PR_), CO_TUNSIGNED32, (CO_DATA)(&Obj2100_01_20)},
    {CO_KEY(0x2100, 2, CO_OBJ____PR_), CO_TUNSIGNED8 , (CO_DATA)(&Obj2100_02_08)},
    {CO_KEY(0x2100, 3, CO_OBJ___APR_), CO_TUNSIGNED8 , (CO_DATA)(&Obj2100_03_08)},

    CO_OBJ_DICT_ENDMARK  /* mark end of used objects */
};

/* Each software timer needs some memory for managing
 * the lists and states of the timed action events.
 */
static CO_TMR_MEM TmrMem[APP_TMR_N];

/* Each SDO server needs memory for the segmented or
 * block transfer requests.
 */
static uint8_t SdoSrvMem[CO_SSDO_N * CO_SDO_BUF_BYTE];

/* Select the drivers for your application. For possible
 * selections, see the directory /drivers. In this example
 * we select the driver templates. You may fill them with
 * your specific hardware functionality.
 */
static struct CO_IF_DRV_T AppDriver = {
    &SimCanDriver,
    &SwCycleTimerDriver,
    &SimNvmDriver
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
