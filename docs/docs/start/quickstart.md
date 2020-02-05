---
layout: article
title: Quickstart
sidebar:
  nav: docs
aside:
  toc: true
---

**Note: This project is not production ready!**

*I work hard (in my spare time) to transfer every aspect from internal management at Embedded Office to this open source repository. I expect the first public release in Q2/2020.*

## CANopen Clock

This quickstart tutorial describes in detail the steps to build a CANopen Clock.

### Functional Specification

The internal clock shall be running if the device is switched in *operational* mode. The object dictionary will be updated every second with the new time.

A transmit process data object (TPDO) shall be sent if the object entry *second* is changed and if the device is running in *operational* mode.

The service data object server (SSDO) allows access to all device information within the object dictionary.

### Object Dictionary

#### Static Object Dictionary

To keep the software as simple as possible, we decide to use a static object dictionary. Therefore, the object dictionary is an array of object entries, allocated as constant array.

Create a file `clock_obj.c` and add the following lines as a starting point into this file:

```c
#include "co_core.h"

/* define the static object dictionary */
const CO_OBJ ClockOD[] = {
    /* place here the coming object entry lines */
};
/* set number of object entries */
const ClockODLen = sizeof(ClockOD)/sizeof(CO_OBJ);
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

#### Clock Specific Object Entries

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

#### TPDO Communication Settings

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

#### TPDO Data Mapping Settings

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
