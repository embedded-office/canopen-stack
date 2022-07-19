# Quickstart

This quickstart example describes in detail the steps to build a CANopen node. The source files are included in the directory [example/quickstart/app][1] and should be followed during reading this article. In this example, we will create a *CANopen Clock*. While this clock node is not intended for serious applications, the example illustrates the key principles of development using the CANopen Stack.

## Functional Specification

The CANopen clock will only run if the device is switched to *operational* mode. The object dictionary will be updated every second with the new time. In *operational* mode, the node will transmit a process data object (`TPDO`) whenever the object dictionary entry *"second"* (`2100h:03`) is changed. The service data object server (`SDOS`) will allow access to the device information contained within the object dictionary.

## Object Dictionary

To keep the software as simple as possible, we will use a static object dictionary. In this case, the object dictionary is an array of object entries, declared as a constant array of object entries of type `CO_OBJ`. The object dictionary is declared in the file [clock_spec.c][2]:

```c
  :
/* define the static object dictionary */
const CO_OBJ ClockOD[] = {
    :
  /* here is your object dictionary */
    :
  CO_OBJ_DICT_ENDMARK  /* mark end of used objects */
};
/* set number of object entries */
const uint16_t ClockODLen = sizeof(ClockOD)/sizeof(CO_OBJ);
  :
```

### Mandatory Object Entries

The object dictionary must hold at least the following mandatory object entries:

| Index:sub | Type       | Access    | Value | Description        |
| --------- | ---------- | --------- | ----- | ------------------ |
| 1000h:00  | UNSIGNED32 | Const     | 0     | Device Type        |
| 1001h:00  | UNSIGNED8  | Read-only | 0     | Error Register     |
| 1005h:00  | UNSIGNED32 | Const     | 0x80  | COB-ID SYNC        |
| 1017h:00  | UNSIGNED16 | Const     | 0     | Heartbeat Producer |
| 1018h:00  | UNSIGNED8  | Const     | 4     | *Identity Object*  |
| 1018h:01  | UNSIGNED32 | Const     | 0     | - Vendor ID        |
| 1018h:02  | UNSIGNED32 | Const     | 0     | - Product code     |
| 1018h:03  | UNSIGNED32 | Const     | 0     | - Revision number  |
| 1018h:04  | UNSIGNED32 | Const     | 0     | - Serial number    |

!!! info "Remember"

    For complex object dictionary entries (e.g. `1018h`), subindex `0` holds the **highest subindex** of this object.

!!! info

    The values shown for `1018h` are only dummy values. Vendor IDs are managed by CiA, and each registered company is assigned to a globally unique value.

These mandatory entries are added with the following lines of code:

```c
  :
{CO_KEY(0x1000, 0, CO_OBJ_D___R_), CO_TUNSIGNED32, (CO_DATA)(0)},
{CO_KEY(0x1001, 0, CO_OBJ_____R_), CO_TUNSIGNED8,  (CO_DATA)(&Obj1001_00_08)},
{CO_KEY(0x1005, 0, CO_OBJ_D___R_), CO_TUNSIGNED32, (CO_DATA)(0x80)},
{CO_KEY(0x1017, 0, CO_OBJ_D___R_), CO_TUNSIGNED16, (CO_DATA)(0)},
{CO_KEY(0x1018, 0, CO_OBJ_D___R_), CO_TUNSIGNED8,  (CO_DATA)(4)},
{CO_KEY(0x1018, 1, CO_OBJ_D___R_), CO_TUNSIGNED32, (CO_DATA)(0)},
{CO_KEY(0x1018, 2, CO_OBJ_D___R_), CO_TUNSIGNED32, (CO_DATA)(0)},
{CO_KEY(0x1018, 3, CO_OBJ_D___R_), CO_TUNSIGNED32, (CO_DATA)(0)},
{CO_KEY(0x1018, 4, CO_OBJ_D___R_), CO_TUNSIGNED32, (CO_DATA)(0)},
  :
```

!!! warning "Important"

    The CANopen Stack relies on a binary search algorithm to ensure that object dictionary entries are found quickly. Because of this, you must keep the index / subindex of all entries in the object dictionary sorted in ascending order.

Most of these entries are constant, and their values can be stored directly inside the object dictionary. This is not the case for the error register object `1001h`. This entry is read-only, but can be changed by the node. We need to declare a global variable to contain the runtime value of the entry:

```c
uint8_t Obj1001_00_08 = 0;
```

A pointer to this variable is stored in the corresponding object dictionary entry. This entry must be marked as `CO_OBJ_____R_` instead of `CO_OBJ_D___R_` to let the stack know that this entry contains a pointer instead of a direct value.

!!! warning "Important"
    
    When using architectures with pointer types lower than 32bit (e.g. 16bit microcontrollers), you can store only values up to the pointer width directly in the object dictionary. For larger values declare a constant variable and place a pointer to this constant into the object dictionary:

    ```c
    const uint32_t Obj1000_00_32 = 0x00000000L;
    ```

    and

    ```c
      :
    {CO_KEY(0x1000, 0, CO_OBJ_____R_), CO_TUNSIGNED32, (CO_DATA)(&Obj1000_00_32)},
      :
    ```

### SDO Server

The settings for the SDO server are defined in CiA301 and must contain the following object dictionary entries:

| Index:sub | Type       | Access | Value          | Description                       |
| --------- | ---------- | ------ | -------------- | --------------------------------- |
| 1200h:00  | UNSIGNED8  | Const  | 2              | *Communication Object SDO Server* |
| 1200h:01  | UNSIGNED32 | Const  | 600h + node ID | - SDO Server Request COBID        |
| 1200h:02  | UNSIGNED32 | Const  | 580h + node ID | - SDO Server Response COBID       |

!!! info

    The predefined COBIDs are dependent on the actual node ID. For this reason, the CANopen stack allows you to specify entries whose value depends on the current node ID at runtime. This behavior is specified using the `CO_OBJ__N____` flag.

The following lines add the SDO server entries to the object dictionary:

```c
  :
{CO_KEY(0x1200, 0, CO_OBJ_D___R_), CO_TUNSIGNED8,  (CO_DATA)(2)},
{CO_KEY(0x1200, 1, CO_OBJ_DN__R_), CO_TUNSIGNED32, (CO_DATA)(CO_COBID_SDO_REQUEST())},
{CO_KEY(0x1200, 2, CO_OBJ_DN__R_), CO_TUNSIGNED32, (CO_DATA)(CO_COBID_SDO_RESPONSE())},
  :
```

### Application Object Entries

Next, we need to add some application-specific object entries to support the clock-functionality of the example:

| Index:sub | Type       | Access    | Value | Description    |
| --------- | ---------- | --------- | ----- | -------------- |
| 2100h:00  | UNSIGNED8  | Const     | 3     | *Clock Object* |
| 2100h:01  | UNSIGNED32 | Read Only | 0     | - Hour         |
| 2100h:02  | UNSIGNED8  | Read Only | 0     | - Minute       |
| 2100h:03  | UNSIGNED8  | Read Only | 0     | - Second       |

!!! info

    These entries are placed within the manufacturer-specific area (from `2000h` up to `5FFFh`) and can be chosen freely (see CiA301). Entries outside of this range cannot be chosen freely, and should conform to the various CiA standards and profiles (e.g. CiA301 for communication profile area, CiA401 for generic IO modules, etc).

These entries are created using the following lines of code:

```c
  :
{CO_KEY(0x2100, 0, CO_OBJ_D___R_), CO_UNSIGNED8,  (CO_DATA)(3)},
{CO_KEY(0x2100, 1, CO_OBJ____PR_), CO_UNSIGNED32, (CO_DATA)(&Obj2100_01_20)},
{CO_KEY(0x2100, 2, CO_OBJ____PR_), CO_UNSIGNED8,  (CO_DATA)(&Obj2100_02_08)},
{CO_KEY(0x2100, 3, CO_OBJ____PR_), CO_UNSIGNED8,  (CO_DATA)(&Obj2100_03_08)},
  :
```

!!! info

    The type `CO_TASYNC` for the object entry `2100h:03` indicates the transmission of all PDOs, which includes this object. We use this mechanism to achieve the PDO transmission on each write access to the second.

We also need to create three global variables to hold the runtime values of the clock object:

```c
uint32_t Obj2100_01_20 = 0;
uint8_t  Obj2100_02_08 = 0;
uint8_t  Obj2100_03_08 = 0;
```

### TPDO Communication

The communication settings for the TPDO must contain the following object entries:

| Index:sub | Type       | Access | Value     | Description                       |
| ----------| ---------- | ------ | --------- | --------------------------------- |
| 1800h:00  | UNSIGNED8  | Const  | 2         | *Communication Object TPDO #0*    |
| 1800h:01  | UNSIGNED32 | Const  | 40000180h | - PDO transmission COBID (no RTR) |
| 1800h:02  | UNSIGNED8  | Const  | 254       | - PDO transmission type           |

!!! info

    The CANopen stack does not support remote CAN frames as they are no longer recommended for new devices. The use of RTR frames in CANopen devices has been deprecated for many years now. Bit 30 in `1800h:01` indicates that remote transfers are not allowed for this PDO. The CAN identifier `180h + node-ID` is the recommended value from the pre-defined connection set.

See the following lines in the object dictionary:

```c
  :
{CO_KEY(0x1800, 0, CO_OBJ_D___R_), CO_UNSIGNED8,  (CO_DATA)(2)},
{CO_KEY(0x1800, 1, CO_OBJ_DN__R_), CO_UNSIGNED32, (CO_DATA)(CO_COBID_TPDO_DEFAULT(0))},
{CO_KEY(0x1800, 2, CO_OBJ_D___R_), CO_UNSIGNED8,  (CO_DATA)(254)},
  :
```

### TPDO Data Mapping

The mapping settings for the TPDO must contain the following object entries:

| Index:sub | Type       | Access | Value     | Description                |
| --------- | ---------- | ------ | --------- | -------------------------- |
| 1A00h:00  | UNSIGNED8  | Const  | 3         | *Mapping Object TPDO #0*   |
| 1A00h:01  | UNSIGNED32 | Const  | 21000120h | - map: 32-bit clock hour   |
| 1A00h:02  | UNSIGNED32 | Const  | 21000208h | - map:  8-bit clock minute |
| 1A00h:03  | UNSIGNED32 | Const  | 21000308h | - map:  8-bit clock second |

How we get these values is explained in section [configuration of PDO mapping][3]. This way of defining the payload for PDOs is part of the CiA301 standard and leads us to the following lines in the object dictionary:

```c
  :
{CO_KEY(0x1A00, 0, CO_OBJ_D___R_), CO_UNSIGNED8,  (CO_DATA)(3)},
{CO_KEY(0x1A00, 1, CO_OBJ_D___R_), CO_UNSIGNED32, (CO_DATA)(CO_LINK(0x2100, 0x01, 32))},
{CO_KEY(0x1A00, 2, CO_OBJ_D___R_), CO_UNSIGNED32, (CO_DATA)(CO_LINK(0x2100, 0x02,  8))},
{CO_KEY(0x1A00, 3, CO_OBJ_D___R_), CO_UNSIGNED32, (CO_DATA)(CO_LINK(0x2100, 0x03,  8))},
  :
```

## EMCY Error Specification

We use in the application abstract EMCY error identifiers (`0` to `Number of EMCY - 1`). To simplify maintenance, we should define these identifiers as enumerations. Refer to the file [clock_spec.h][9] for more detail:

```c
enum {
  APP_ERR_ID_SOMETHING = 0,
  APP_ERR_ID_HAPPENS,

  APP_ERR_ID_NUM            /* number of EMCY error identifiers in application */
};
```

!!! info

    With these enumerations in place, we can call the EMCY service functions in our application (e.g. in timer callback function like: `COEmcySet(&node->Emcy, APP_ERR_ID_HOT, 0);`).

### EMCY Codes

The CANopen stack behavior for each of these EMCY error identifier is defined in a EMCY table. Here we define the EMCY code and the error register bit correlation. We don't use them in the simple clock application, but an example definition is shown in [clock_spec.c][10]:

```c
static CO_EMCY_TBL AppEmcyTbl[APP_ERR_ID_NUM] = {
  { CO_EMCY_REG_GENERAL, CO_EMCY_CODE_GEN_ERR          }, /* APP_ERR_CODE_SOMETHING */
  { CO_EMCY_REG_TEMP   , CO_EMCY_CODE_TEMP_AMBIENT_ERR }  /* APP_ERR_CODE_HOT       */
};
```

The constants `CO_EMCY_REG_...` for the error register bits and `CO_EMCY_CODE_...` for the EMCY code base values are the specified values out of the CANopen specification.

!!! warning

    When using pure numbers, check the possible range for the error register bits (8bit) and the EMCY code (16bit).

## Node Specification

The main settings of the node are configured inside the `CO_NODE_SPEC` struct. This struct will not be modified at runtime, so it can be declared as a constant, reducing RAM usage. The CANopen node needs some memory buffers for dynamic operations. These two buffers are statically allocated, and a pointer is stored inside the `CO_NODE_SPEC` struct. Refer to the file [clock_spec.c][4] for more detail:

```c
#include "co_core.h"
  :
/* Define some default values for our CANopen node: */
#define APP_NODE_ID       1u                 /* CANopen node ID             */
#define APP_BAUDRATE      250000u            /* CAN baudrate                */
#define APP_TMR_N         16u                /* Number of software timers   */
#define APP_TICKS_PER_SEC 1000u              /* Timer clock frequency in Hz */
#define APP_OBJ_N         128u               /* Object dictionary max size  */
  :
/* Each software timer needs some memory for managing
 * the lists and states of the timed action events.
 */
CO_TMR_MEM TmrMem[APP_TMR_N];

/* Each SDO server needs memory for the segmented or
 * block transfer requests.
 */
uint8_t SdoSrvMem[CO_SSDO_N * CO_SDO_BUF_BYTE];
  :
/* Collect all node specification settings in a single
 * structure for initializing the node easily.
 */
CO_NODE_SPEC AppSpec = {
  APP_NODE_ID,                          /* default Node-Id                */
  APP_BAUDRATE,                         /* default Baudrate               */
  (CO_OBJ *)&ClockOD[0],                /* pointer to object dictionary   */
  APP_OBJ_N,                            /* object dictionary max length   */
  &AppEmcyTbl[0],                       /* EMCY code & register bit table */
  &TmrMem[0],                           /* pointer to timer memory blocks */
  APP_TMR_N,                            /* number of timer memory blocks  */
  APP_TICKS_PER_SEC,                    /* timer clock frequency in Hz    */
  (CO_IF_DRV *)&AppDriver,              /* select drivers for application */
  &SdoSrvMem[0]                         /* SDO Transfer Buffer Memory     */
};
```

!!! danger

    Never manipulate these variables directly. Doing so will cause problems.

## Application Start

The application code is implemented in the file [clock_app.c][5]. This file is responsible for the CANopen Stack startup as well as the application-specific clock function. Let's start with the CANopen Stack [startup][6]:

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

## Application Callback

The timer callback function [AppClock()][7] includes the main functionality of the clock node:

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

    COObjRdValue(od_sec, node, (void *)&second, sizeof(second));
    COObjRdValue(od_min, node, (void *)&minute, sizeof(minute));
    COObjRdValue(od_hr , node, (void *)&hour  , sizeof(hour));

    second++;
    if (second >= 60) {
      second = 0;
      minute++;
    }
    if (minute >= 60) {
      minute = 0;
      hour++;
    }

    COObjWrValue(od_hr , node, (void *)&hour  , sizeof(hour));
    COObjWrValue(od_min, node, (void *)&minute, sizeof(minute));
    COObjWrValue(od_sec, node, (void *)&second, sizeof(second));
  }
}
```

!!! info

    The last write access with `COObjWrValue()` triggers the transmission of the PDO, because the corresponding object entry is defined as type `CO_TASYNC`.

## Hardware Connection

Next, you will need to add drivers to your project to interface the stack with your hardware, as shown in [clock_spec.c][8]:


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

When you write your device driver, you will need to set up a hardware timer interrupt within your low-level layer - the Board Support Package (BSP) - and configure a periodic interrupt source with a frequency of `APP_TICKS_PER_SEC`. The timer interrupt service handler should look something like this:

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

[1]: https://github.com/embedded-office/canopen-stack/blob/master/example/quickstart/app/
[2]: https://github.com/embedded-office/canopen-stack/blob/master/example/quickstart/app/clock_spec.c#L50
[3]: ../usage/configuration#pdo-mapping-value
[4]: https://github.com/embedded-office/canopen-stack/blob/master/example/quickstart/app/clock_spec.c#L21
[5]: https://github.com/embedded-office/canopen-stack/blob/master/example/quickstart/app/clock_app.c
[6]: https://github.com/embedded-office/canopen-stack/blob/master/example/quickstart/app/clock_app.c#L90
[7]: https://github.com/embedded-office/canopen-stack/blob/master/example/quickstart/app/clock_app.c#L36
[8]: https://github.com/embedded-office/canopen-stack/blob/master/example/quickstart/app/clock_spec.c#L98
[9]: https://github.com/embedded-office/canopen-stack/blob/master/example/quickstart/app/clock_spec.h#L34
[10]: https://github.com/embedded-office/canopen-stack/blob/master/example/quickstart/app/clock_spec.c#L110
