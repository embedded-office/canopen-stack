---
layout: article
title: Quickstart
sidebar:
  nav: docs
aside:
  toc: true
---

## CANopen Clock

This quickstart tutorial describes in detail the steps to build a CANopen Clock.

### Functional Specification

The internal clock shall be running if the device is switched in *operational* mode. The object dictionary will be updated every second with the new time.

A transmit process data object (TPDO) shall be sent if the object entry *second* is changed and if the device is running in *operational* mode.

The service data object server (SDOS) allows access to all device information within the object dictionary.

### Object Dictionary

#### Static Object Dictionary

To keep the software as simple as possible, we decide to use a static object dictionary. Therefore, the object dictionary is an array of object entries, declared as constant array of object entries of type `CO_OBJ`.

Create a file `clock_obj.c` and add the following lines as a starting point into this file:

```c
#include "co_core.h"

/* define the static object dictionary */
const CO_OBJ ClockOD[] = {
    /* place here the coming object entry lines */
};
/* set number of object entries */
const uint16_t ClockODLen = sizeof(ClockOD)/sizeof(CO_OBJ);
```

#### Mandatory Object Entries

The object dictionary must hold at least the following mandatory object entries:

| Index | Subindex | Type       | Access     | Value | Description        |
| ----- | -------- | ---------- | ---------- | ----- | ------------------ |
| 1000h | 0        | UNSIGNED32 | Const      | 0     | Device Type        |
| 1001h | 0        | UNSIGNED8  | Read Only  | 0     | Error Register     |
| 1017h | 0        | UNSIGNED16 | Const      | 0     | Heartbeat Producer |
| 1018h | 0        | UNSIGNED8  | Const      | 1     | *Identity Object*  |
| 1018h | 1        | UNSIGNED32 | Const      | 0     | - Vendor ID        |

*Note: For complex object entries (like the 1018h), the subindex 0 holds the highest subindex in this object. The shown vendor ID is only a dummy value. The correct value is managed by CiA, because each registered company will get a worldwide unique value.*

Add the following lines into the object dictionary:

```c
  :
    {CO_KEY(0x1000, 0, CO_UNSIGNED32|CO_OBJ_D__R_), 0, 0},
    {CO_KEY(0x1001, 0, CO_UNSIGNED8 |CO_OBJ____R_), 0, &Obj1001_00_08},
    {CO_KEY(0x1017, 0, CO_UNSIGNED16|CO_OBJ_D__R_), 0, 0},
    {CO_KEY(0x1018, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, 1},
    {CO_KEY(0x1018, 1, CO_UNSIGNED32|CO_OBJ_D__R_), 0, 0},
  :
```

#### SDO Server

The settings for the SDO server are defined in CiA301 and must contain the following object entries:

| Index | Subindex | Type       | Access     | Value | Description                       |
| ----- | -------- | ---------- | ---------- | ----- | --------------------------------- |
| 1200h | 0        | UNSIGNED8  | Const      | 2     | *Communication Object SDO Server* |
| 1200h | 1        | UNSIGNED32 | Const      | 600h  | - SDO Server Request COBID        |
| 1200h | 2        | UNSIGNED32 | Const      | 580h  | - SDO Server Response COBID       |

*Note: Since all COBIDs are settings which are dependant on the actual node ID. For this reason, the CANopen stack allows to specify such entries to consider the current node ID during runtime.*

Add the following lines into the object dictionary:

```c
  :
    {CO_KEY(0x1200, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, 2},
    {CO_KEY(0x1200, 1, CO_UNSIGNED32|CO_OBJ_DN_R_), 0, 0x600},
    {CO_KEY(0x1200, 2, CO_UNSIGNED32|CO_OBJ_DN_R_), 0, 0x580},
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

Add the following lines into the object dictionary:

```c
  :
    {CO_KEY(0x2100, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, 3},
    {CO_KEY(0x2100, 1, CO_UNSIGNED32|CO_OBJ___PR_), 0, &Obj2100_01_20},
    {CO_KEY(0x2100, 2, CO_UNSIGNED8 |CO_OBJ___PR_), 0, &Obj2100_02_08},
    {CO_KEY(0x2100, 3, CO_UNSIGNED8 |CO_OBJ___PR_), 0, &Obj2100_03_08},
  :
```

#### TPDO Communication

The communication settings for the TPDO must contain the following object entries:

| Index | Subindex | Type       | Access     | Value     | Description                    |
| ----- | -------- | ---------- | ---------- | --------- | ------------------------------ |
| 1800h | 0        | UNSIGNED8  | Const      | 2         | *Communication Object TPDO #0* |
| 1800h | 1        | UNSIGNED32 | Const      | 40000180h | - PDO transmission COBID       |
| 1800h | 2        | UNSIGNED8  | Const      | 254       | - PDO transmission type        |

*Note: The CANopen stack didn't support CAN remote frames, because they are not recommended for new devices since many years. Bit30 in 1800h:1 defines, that remote transfers are not allowed for this PDO. The CAN identifier 180h is the recommended value out of the pre-defined connection set.*

Add the following lines into the object dictionary:

```c
  :
    {CO_KEY(0x1800, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, 2},
    {CO_KEY(0x1800, 1, CO_UNSIGNED32|CO_OBJ_D__R_), 0, 0x40000180},
    {CO_KEY(0x1800, 2, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, 254},
  :
```

***Attention: You must add the entries into the object dictionary and ensure that index/subindex is sorted in ascending order. The reason is: the CANopen Stack uses a binary search to get to the right object entry as fast as possible.***

#### TPDO Data Mapping

The mapping settings for the TPDO must contain the following object entries:

| Index | Subindex | Type       | Access     | Value     | Description                |
| ----- | -------- | ---------- | ---------- | --------- | -------------------------- |
| 1A00h | 0        | UNSIGNED8  | Const      | 3         | *Mapping Object TPDO #0*   |
| 1A00h | 1        | UNSIGNED32 | Const      | 21000120h | - map: 32-bit clock hour   |
| 1A00h | 2        | UNSIGNED32 | Const      | 21000208h | - map:  8-bit clock minute |
| 1A00h | 3        | UNSIGNED32 | Const      | 21000308h | - map:  8-bit clock second |

Add the following lines into the object dictionary:

```c
  :
    {CO_KEY(0x1A00, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, 3},
    {CO_KEY(0x1A00, 1, CO_UNSIGNED32|CO_OBJ_D__R_), 0, 0x21000120},
    {CO_KEY(0x1A00, 2, CO_UNSIGNED32|CO_OBJ_D__R_), 0, 0x21000208},
    {CO_KEY(0x1A00, 3, CO_UNSIGNED32|CO_OBJ_D__R_), 0, 0x21000308},
  :
```

***Again: keep the index/subindex sorted in ascending order!***

### Node Specification

The CANopen node needs some memory for dynamic operations. Now we create some variables to allocate the right amount of memory and provide the memory portions to the CANopen Stack during initialization for internal use.

Create the file `clock_spec.c` and insert the following code:

```c
#include "co_core.h"

/* Define some default values for our CANopen node: */
#define APP_NODE_ID     1u          /* CANopen node ID           */
#define APP_BAUDRATE    250000u     /* CAN baudrate              */
#define APP_CAN_BUS_ID  0u          /* Bus ID (driver specific)  */
#define APP_TMR_N       16u         /* Number of software timers */

/* get external object dictionary */
extern const CO_OBJ   *ClockOD;
extern const uint16_t  ClockODLen;

/* Each software timer needs some memory for managing
 * the lists and states of the timed action events.
 */
CO_TMR_MEM TmrMem[APP_TMR_N];

/* Each SDO server needs memory for the segmented or
 * block transfer requests.
 */
uint8_t SdoSrvMem[CO_SDOS_N][CO_SDO_BUF_BYTE];

/* Collect all node specification settings in a single
 * structure for initializing the node easily.
 */
const CO_NODE_SPEC AppSpec = {
    APP_NODE_ID,         /* default Node-Id                */
    ABB_BAUDRATE,        /* default Baudrate               */
    &ClockOD[0],         /* pointer to object dictionary   */
    ClockODLen,          /* object dictionary length       */
    NULL,                /* no EMCY info fields (unused)   */
    &TmrMem[0],          /* pointer to timer memory blocks */
    APP_TMR_N,           /* number of timer memory blocks  */
    APP_CAN_BUS_ID,      /* linked CAN bus driver          */
    &SdoSrvMem[0]        /* SDO Transfer Buffer Memory     */
};
```

***We should never use these variables directly. This will cause trouble.***

### Application Start

For the application code we create a file `clock_app.c`. This file will include the CANopen Stack startup and the application specific clock function.

Ok, let's start with the CANopen Stack startup:

```c
#include "co_core.h"

/* get external node specification */
extern const CO_NODE_SPEC AppSpec;

/* Allocate a global CANopen node object */
CO_NODE Clk;

/* main entry function */
void main(int argc, char *argv[])
{
    /* Initialize your hardware layer and the CANopen stack.
     * Stop execution if an error is detected.
     */
    BSPInit();
    CONodeInit(&Clk, (CO_NODE_SPEC *)&AppSpec);
    if (CONodeGetErr(&Clk) != CO_ERR_NONE) {
        while(1);
    }
     
    /* Use CANopen software timer to create a cyclic function
     * call to the callback function 'AppClock()'.
     */
    COTmrCreate(&Clk.Tmr, 0, CO_TMR_TICKS_PER_SEC, AppClock, &Clk);

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

The timer callback function `AppClock()` includes the main functionality of the clock node:

```c
void AppClock(void *p_arg)
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
    node = (CO_NODE *)arg;
    if (node == 0) {
        return;
    }

    /* Main functionality: when we are in operational mode, we get the current
     * clock values out of the object dictionary, increment the seconds and
     * update all clock values in the object dictionary.
     */
    if (CONmtGetMode(&node->Nmt) == CO_OPERATIONAL) {

        od_sec = CODirFind(&node->Dir, CO_DEV(0x2100, 3));
        od_min = CODirFind(&node->Dir, CO_DEV(0x2100, 2));
        od_hr  = CODirFind(&node->Dir, CO_DEV(0x2100, 1));

        COObjRdValue(od_sec, (void *)&second, sizeof(second), 0);
        COObjRdValue(od_min, (void *)&minute, sizeof(minute), 0);
        COObjRdValue(od_hr , (void *)&hour  , sizeof(hour),   0);

        second++;
        if (second >= 60) {
            second = 0;
            minute++;
        }
        if (minute >= 60) {
            minute = 0;
            hour++;
        }

        COObjWrValue(od_sec, (void *)&second, sizeof(second), 0);
        COObjWrValue(od_min, (void *)&minute, sizeof(minute), 0);
        COObjWrValue(od_hr , (void *)&hour  , sizeof(hour),   0);
    }
}
```

### Hardware Connection

For the hardware connection, you need to add a CAN driver which implements the following interface functions:

```c
int16_t COIfRead  (CO_IF *cif, CO_IF_FRM *frm);
int16_t COIfSend  (CO_IF *cif, CO_IF_FRM *frm);
void    COIfReset (CO_IF *cif);
void    COIfClose (CO_IF *cif);
void    COIfInit  (CO_IF *cif, struct CO_NODE_T *node);
void    COIfEnable(CO_IF *cif, uint32_t baudrate);
```

Furthermore, setup a hardware timer interrupt with your low-level layer (I call it Board Support Package) and configure a cyclic interrupt with a frequency of `CO_TMR_TICK_PER_SEC`. The timer interrupt service handler should look like:

```c
#include "co_core.h"

/* get external CANopen node */
extern CO_NODE Clk;

void App_TmrIsrHandler(void)
{
    /* update all timers and collect elapsed timed actions */				           
    COTmrService(&Clk.Tmr);
}
```
