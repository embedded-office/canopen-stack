---
layout: article
title: Quickstart
sidebar:
  nav: docs
aside:
  toc: true
---

This quickstart example describes in detail the steps to build a CANopen node. The source files are included in the directory `/example/quickstart/app` and should be followed during reading this article.

<!--more-->

*The example provides a **CANopen Clock** (nothing serious, but nice to see in action), and demonstrates the principal way of development to this CANopen device.*

### Functional Specification

The CANopen clock shall run if the device is switched in *operational* mode. The object dictionary will be updated every second with the new time.

We shall send transmit process data object (TPDO) if the object entry *second* is changed and if the device is running in *operational* mode.

The service data object server (SDOS) shall allow access to all device information within the object dictionary.

### Object Dictionary

#### Static Object Dictionary

To keep the software as simple as possible, we decide to use a static object dictionary. Therefore, the object dictionary is an array of object entries, declared as constant array of object entries of type `CO_OBJ`.

See in file [clock_spec.c](https://github.com/embedded-office/canopen-stack/blob/master/example/quickstart/app/clock_spec.c#L50):

```c
  :
/* define the static object dictionary */
const CO_OBJ ClockOD[] = {
        :
    /* here is your object dictionary */
        :
    CO_OBJ_DIR_ENDMARK  /* mark end of used objects */
};
/* set number of object entries */
const uint16_t ClockODLen = sizeof(ClockOD)/sizeof(CO_OBJ);
  :
```

#### Mandatory Object Entries

The object dictionary must hold at least the following mandatory object entries:

| Index | Subindex | Type       | Access     | Value | Description        |
| ----- | -------- | ---------- | ---------- | ----- | ------------------ |
| 1000h | 0        | UNSIGNED32 | Const      | 0     | Device Type        |
| 1001h | 0        | UNSIGNED8  | Read Only  | 0     | Error Register     |
| 1005h | 0        | UNSIGNED32 | Const      | 0x80  | COB-ID SYNC        |
| 1017h | 0        | UNSIGNED16 | Const      | 0     | Heartbeat Producer |
| 1018h | 0        | UNSIGNED8  | Const      | 4     | *Identity Object*  |
| 1018h | 1        | UNSIGNED32 | Const      | 0     | - Vendor ID        |
| 1018h | 2        | UNSIGNED32 | Const      | 0     | - Product code     |
| 1018h | 3        | UNSIGNED32 | Const      | 0     | - Revision number  |
| 1018h | 4        | UNSIGNED32 | Const      | 0     | - Serial number    |

*Note: For complex object entries (like the 1018h), the subindex 0 holds the highest subindex in this object. The shown values are only dummy values. The correct value for vendor ID is managed by CiA, because each registered company will get a worldwide unique value.*

See the following lines in the object dictionary:

```c
  :
    {CO_KEY(0x1000, 0, CO_UNSIGNED32|CO_OBJ_D__R_), 0, (uintptr_t)0},
    {CO_KEY(0x1001, 0, CO_UNSIGNED8 |CO_OBJ____R_), 0, (uintptr_t)&Obj1001_00_08},
    {CO_KEY(0x1005, 0, CO_UNSIGNED32|CO_OBJ_D__R_), 0, (uintptr_t)0x80},
    {CO_KEY(0x1017, 0, CO_UNSIGNED16|CO_OBJ_D__R_), 0, (uintptr_t)0},

    {CO_KEY(0x1018, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, (uintptr_t)4},
    {CO_KEY(0x1018, 1, CO_UNSIGNED32|CO_OBJ_D__R_), 0, (uintptr_t)0},
    {CO_KEY(0x1018, 2, CO_UNSIGNED32|CO_OBJ_D__R_), 0, (uintptr_t)0},
    {CO_KEY(0x1018, 3, CO_UNSIGNED32|CO_OBJ_D__R_), 0, (uintptr_t)0},
    {CO_KEY(0x1018, 4, CO_UNSIGNED32|CO_OBJ_D__R_), 0, (uintptr_t)0},
  :
```

and the corresponding global variable for the runtime value of the error register object (1001h,00h):

```c
uint8_t Obj1001_00_08 = 0;
```

***Attention: You must keep the index/subindex of all entries in the object dictionary sorted in ascending order. The reason is: the CANopen Stack uses a binary search to get to the right object entry as fast as possible.***

#### SDO Server

The settings for the SDO server are defined in CiA301 and must contain the following object entries:

| Index | Subindex | Type       | Access     | Value | Description                       |
| ----- | -------- | ---------- | ---------- | ----- | --------------------------------- |
| 1200h | 0        | UNSIGNED8  | Const      | 2     | *Communication Object SDO Server* |
| 1200h | 1        | UNSIGNED32 | Const      | 600h  | - SDO Server Request COBID        |
| 1200h | 2        | UNSIGNED32 | Const      | 580h  | - SDO Server Response COBID       |

*Note: Since all COBIDs are settings which are dependant on the actual node ID. For this reason, the CANopen stack allows to specify such entries to consider the current node ID during runtime.*

See the following lines in the object dictionary:

```c
  :
    {CO_KEY(0x1200, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, (uintptr_t)2},
    {CO_KEY(0x1200, 1, CO_UNSIGNED32|CO_OBJ_DN_R_), 0, CO_COBID_SDO_REQUEST()},
    {CO_KEY(0x1200, 2, CO_UNSIGNED32|CO_OBJ_DN_R_), 0, CO_COBID_SDO_RESPONSE()},
  :
```

#### Application Object Entries

The quickstart specific object dictionary entries for the clock shall include:

| Index | Subindex | Type       | Access     | Value | Description    |
| ----- | -------- | ---------- | ---------- | ----- | -------------- |
| 2100h | 0        | UNSIGNED8  | Const      | 3     | *Clock Object* |
| 2100h | 1        | UNSIGNED32 | Read Only  | 0     | - Hour         |
| 2100h | 2        | UNSIGNED8  | Read Only  | 0     | - Minute       |
| 2100h | 3        | UNSIGNED8  | Read Only  | 0     | - Second       |

*Note: These entries are a free selection. Some CiA standards and profiles specify a bunch of entries, so keep your data in the manufacturer specific area (2000h up to 5FFFh), defined in CiA301.*

See the following lines in the object dictionary:

```c
  :
    {CO_KEY(0x2100, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, (uintptr_t)3},
    {CO_KEY(0x2100, 1, CO_UNSIGNED32|CO_OBJ___PR_), 0, (uintptr_t)&Obj2100_01_20},
    {CO_KEY(0x2100, 2, CO_UNSIGNED8 |CO_OBJ___PR_), 0, (uintptr_t)&Obj2100_02_08},
    {CO_KEY(0x2100, 3, CO_UNSIGNED8 |CO_OBJ___PR_), 0, (uintptr_t)&Obj2100_03_08},
  :
```

and the corresponding global variables for the runtime values of the clock object as global variables:

```c
uint32_t Obj2100_01_20 = 0;
uint8_t  Obj2100_02_08 = 0;
uint8_t  Obj2100_03_08 = 0;
```

#### TPDO Communication

The communication settings for the TPDO must contain the following object entries:

| Index | Subindex | Type       | Access     | Value     | Description                       |
| ----- | -------- | ---------- | ---------- | --------- | --------------------------------- |
| 1800h | 0        | UNSIGNED8  | Const      | 2         | *Communication Object TPDO #0*    |
| 1800h | 1        | UNSIGNED32 | Const      | 40000180h | - PDO transmission COBID (no RTR) |
| 1800h | 2        | UNSIGNED8  | Const      | 254       | - PDO transmission type           |

*Note: The CANopen stack didn't support CAN remote frames, because they are not recommended for new devices since many years. Bit30 in 1800h:1 defines, that remote transfers are not allowed for this PDO. The CAN identifier 180h is the recommended value out of the pre-defined connection set.*

See the following lines in the object dictionary:

```c
  :
    {CO_KEY(0x1800, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, (uintptr_t)2},
    {CO_KEY(0x1800, 1, CO_UNSIGNED32|CO_OBJ_D__R_), 0, CO_COBID_TPDO_DEFAULT(0)},
    {CO_KEY(0x1800, 2, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, (uintptr_t)254},
  :
```

#### TPDO Data Mapping

The mapping settings for the TPDO must contain the following object entries:

| Index | Subindex | Type       | Access     | Value     | Description                |
| ----- | -------- | ---------- | ---------- | --------- | -------------------------- |
| 1A00h | 0        | UNSIGNED8  | Const      | 3         | *Mapping Object TPDO #0*   |
| 1A00h | 1        | UNSIGNED32 | Const      | 21000120h | - map: 32-bit clock hour   |
| 1A00h | 2        | UNSIGNED32 | Const      | 21000208h | - map:  8-bit clock minute |
| 1A00h | 3        | UNSIGNED32 | Const      | 21000308h | - map:  8-bit clock second |

How we get these values is explained in section [configuration of PDO mapping](/docs/usecase/configuration#pdo-mapping-value). This way of defining the payload for PDOs is part of the CiA301 standard and leads us to the following lines in the object dictionary:

```c
  :
    {CO_KEY(0x1A00, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, (uintptr_t)3},
    {CO_KEY(0x1A00, 1, CO_UNSIGNED32|CO_OBJ_D__R_), 0, CO_LINK(0x2100, 0x01, 32)},
    {CO_KEY(0x1A00, 2, CO_UNSIGNED32|CO_OBJ_D__R_), 0, CO_LINK(0x2100, 0x02,  8)},
    {CO_KEY(0x1A00, 3, CO_UNSIGNED32|CO_OBJ_D__R_), 0, CO_LINK(0x2100, 0x03,  8)},
  :
```

### Node Specification

The CANopen node needs some memory for dynamic operations. Now we need some variables to allocate the right amount of memory and provide the memory portions to the CANopen Stack during initialization.

See in file [clock_spec.c](https://github.com/embedded-office/canopen-stack/blob/master/example/quickstart/app/clock_spec.c#L21):

```c
#include "co_core.h"
    :
/* Define some default values for our CANopen node: */
#define APP_NODE_ID       1u                  /* CANopen node ID             */
#define APP_BAUDRATE      250000u             /* CAN baudrate                */
#define APP_TMR_N         16u                 /* Number of software timers   */
#define APP_TICKS_PER_SEC 1000u               /* Timer clock frequency in Hz */
#define APP_OBJ_N         128u                /* Object dictionary max size  */
    :
/* Each software timer needs some memory for managing
 * the lists and states of the timed action events.
 */
CO_TMR_MEM TmrMem[APP_TMR_N];

/* Each SDO server needs memory for the segmented or
 * block transfer requests.
 */
uint8_t SdoSrvMem[CO_SDOS_N * CO_SDO_BUF_BYTE];
    :
/* Collect all node specification settings in a single
 * structure for initializing the node easily.
 */
CO_NODE_SPEC AppSpec = {
    APP_NODE_ID,             /* default Node-Id                */
    APP_BAUDRATE,            /* default Baudrate               */
    (CO_OBJ *)&ClockOD[0],   /* pointer to object dictionary   */
    APP_OBJ_N,               /* object dictionary max length   */
    NULL,                    /* no EMCY info fields (unused)   */
    &TmrMem[0],              /* pointer to timer memory blocks */
    APP_TMR_N,               /* number of timer memory blocks  */
    APP_TICKS_PER_SEC,       /* timer clock frequency in Hz    */
    (CO_IF_DRV *)&AppDriver, /* select drivers for application */
    &SdoSrvMem[0]            /* SDO Transfer Buffer Memory     */
};
```

***We should never use these variables directly. This will cause trouble.***

### Application Start

For the application code see the file [clock_app.c](https://github.com/embedded-office/canopen-stack/blob/master/example/quickstart/app/clock_app.c#L21). This file will include the CANopen Stack startup and the application specific clock function.

Let's start with the CANopen Stack [startup](https://github.com/embedded-office/canopen-stack/blob/master/example/quickstart/app/clock_app.c#L48):

```c
#include "co_core.h"

/* get external node specification */
extern const CO_NODE_SPEC AppSpec;

/* Allocate a global CANopen node object */
CO_NODE Clk;

/* main entry function */
void main(int argc, char *argv[])
{
    uint32_t ticks;

    /* Initialize your hardware layer and the CANopen stack.
     * Stop execution if an error is detected.
     */
    /* BSPInit(); */
    CONodeInit(&Clk, (CO_NODE_SPEC *)&AppSpec);
    if (CONodeGetErr(&Clk) != CO_ERR_NONE) {
        while(1);
    }

    /* Use CANopen software timer to create a cyclic function
     * call to the callback function 'AppClock()' with a period
     * of 1s (equal: 1000ms).
     */
    ticks = COTmrGetTicks(&Clk.Tmr, 1000, CO_TMR_UNIT_1MS);
    COTmrCreate(&Clk.Tmr, 0, ticks, AppClock, &Clk);

    /* Start the CANopen node and set it automatically to
     * NMT mode: 'OPERATIONAL'.
     */
    CONodeStart(&Clk);
    CONmtSetMode(&Clk.Nmt, CO_OPERATIONAL);

    /* In the background loop we process received CAN frames
     * and executes elapsed action callback functions.
     */
    while (1) {
        CONodeProcess(&Clk);
        COTmrProcess(&Clk.Tmr);
    }
}
```

### Application Callback

The timer callback function [AppClock()](https://github.com/embedded-office/canopen-stack/blob/master/example/quickstart/app/clock_app.c#L84) includes the main functionality of the clock node:

```c
/* timer callback function */
static void AppClock(void *p_arg)
{
    CO_NODE  *node;
    CO_OBJ   *od_sec;
    CO_OBJ   *od_min;
    CO_OBJ   *od_hr;
    uint8_t   second;
    uint8_t   minute;
    uint32_t  hour;

    /* For flexible usage (not needed, but nice to show), we use the argument
     * as reference to the CANopen node object. If no node is given, we ignore
     * the function call by returning immediatelly.
     */
    node = (CO_NODE *)p_arg;
    if (node == 0) {
        return;
    }

    /* Main functionality: when we are in operational mode, we get the current
     * clock values out of the object dictionary, increment the seconds and
     * update all clock values in the object dictionary.
     */
    if (CONmtGetMode(&node->Nmt) == CO_OPERATIONAL) {

        od_sec = CODictFind(&node->Dict, CO_DEV(0x2100, 3));
        od_min = CODictFind(&node->Dict, CO_DEV(0x2100, 2));
        od_hr  = CODictFind(&node->Dict, CO_DEV(0x2100, 1));

        COObjRdValue(od_sec, node, (void *)&second, sizeof(second), 0);
        COObjRdValue(od_min, node, (void *)&minute, sizeof(minute), 0);
        COObjRdValue(od_hr , node, (void *)&hour  , sizeof(hour),   0);

        second++;
        if (second >= 60) {
            second = 0;
            minute++;
        }
        if (minute >= 60) {
            minute = 0;
            hour++;
        }

        COObjWrValue(od_sec, node, (void *)&second, sizeof(second), 0);
        COObjWrValue(od_min, node, (void *)&minute, sizeof(minute), 0);
        COObjWrValue(od_hr , node, (void *)&hour  , sizeof(hour),   0);
    }
}
```

### Hardware Connection

For the hardware connection, you need to select or add some drivers to your project:

See in file [clock_spec.c](https://github.com/embedded-office/canopen-stack/blob/master/example/quickstart/app/clock_spec.c#L94):

```c
   :
                                              /* select application drivers: */
#include "co_can_dummy.h"                     /* CAN driver                  */
#include "co_timer_dummy.h"                   /* Timer driver                */
#include "co_nvm_dummy.h"                     /* NVM driver                  */
   :
/* Select the drivers for your application. For possible
 * selections, see the directory /drivers. In this example
 * we select the driver templates. You may fill them with
 * your specific hardware functionality.
 */
const CO_IF_DRV AppDriver = {
    &DummyCanDriver,
    &DummyTimerDriver,
    &DummyNvmDriver
};
   :
```

When you write your device driver, you will setup a hardware timer interrupt with your low-level layer, I call it Board Support Package (BSP), and configure a cyclic interrupt source with a frequency of `APP_TICKS_PER_SEC`. Somewhere in your BSP, the timer interrupt service handler should look like:

```c
#include "co_core.h"
   :
/* get external CANopen node */
extern CO_NODE Clk;
   :
void App_TmrIsrHandler(void)
{
    /* collect elapsed timed actions */
    COTmrService(&Clk.Tmr);
}
```
