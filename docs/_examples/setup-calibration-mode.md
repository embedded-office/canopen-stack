---
layout: article
title: Setup calibration mode
---

When dealing with analog values, there is often a calibration of the hardware required to achieve the best accuracy. This example situation is a good reason for implementing a manufacturer specific factory area.

<!--more-->

## Example Goal

The main idea is a collection of object entries, which are visible or at least only writable after a simple identification mechanism.

To achieve a simple calibration we assume a raw ADC value, which is transformed in the application with a formular:

```c
adcValue = (adcRaw * calFactor) / calDivisor + calOffset;
```

### Object Entry Definitions

We define the manufacturer specific entries in the object dictionary:

| Index | Subindex | Type       | Access     | Value | Description        |
| ----- | -------- | ---------- | ---------- | ----- | ------------------ |
| 2F00h | 0        | UNSIGNED32 | Const      | 4     | Max. Subindex      |
| 2F00h | 1        | UNSIGNED32 | Write Only | 0     | Calibration Key    |
| 2F00h | 2        | SIGNED32   | Read Write | 1     | Cal. Factor        |
| 2F09h | 3        | SIGNED32   | Read Write | 1     | Cal. Divisor       |
| 2F00h | 4        | SIGNED32   | Read Write | 0     | Cal. Offset        |

To achieve the permanent calibration after setting the values, the subindizes 1 to max must be storable (e.g. parameters).

The Number of Entries is a constant, which is read only for the CAN network and conforms to the standard way of defining subindex 0 of an array.

The key to get the wanted functionality is the entry at subindex 1, which accepts a write access of a calibration key value. With the correct key, this operation enables the write access to the other calibration object entries from subindex 2 to 4.

## Implement Object Type

Lets implement the calibration user type as shown in the [CANopen Usage: User Object](usecase/dictionary#user-objects):

```c
uint8_t calWriteAllowed = 0u;

const CO_OBJ_TYPE COTCal = { 0, 0, 0, CalWrite };

#define CO_TCAL ((CO_OBJ_TYPE*)&COTCal)
```

The write function is called, when the CAN network tries to write to the object entry (e.g. via a SDO request). So we typically send an error, because the calibration data is read only per default.

```c
int16_t CalWrite(CO_OBJ *obj, struct CO_NODE_T *node, void *buf, uint32_t size)
{
  return -1;
}
```

Now we want to enhance this function to enable writing for calibration values, if a *secret* key-value is written to the object entry at subindex 1:

```c
#define CAL_KEY 12345678  /* choose a random value for enabling write access */

int16_t CalWrite(CO_OBJ *obj, struct CO_NODE_T *node, void *buf, uint32_t size)
{
  uint32_t value  = *((uint32_t *)buf);
  int16_t  result = -1;
  uint8_t  subidx = CO_GET_SUB(obj->Key);

  if (subidx == 1u) {
    if (value == CAL_KEY) {
      calWriteAllowed = 1u;
      result = 0u;
    } else {
      calWriteAllowed = 0u;
    }
  } else if (subidx > 1u) {
    if (calWriteAllowed == 1u) {
      *(int32_t *)obj->Data = value;
      result = 0u;
    }
  }
  return result;
}
```

## Implement Object Entries

### Parameter Handling

First, we need to get a memory area for our calibration data. We get this area by creating a structure with the calibration data values:

```c
struct CAL_MEM_T {
  int32_t Factor;
  int32_t Divisor;
  int32_t  Offset;
};

struct CAL_MEM_T CalDefaults = {
  (int32_t)1u,  /* factor  */
  (int32_t)1u,  /* divisor */
  (int32_t)0u   /* offset  */
};
struct CAL_MEM_T CalValue;
```

For store and restore operations, the parameter group control structure specifies the memory area and the reset type of these parameters:

```c
const CO_PARA CalParaObj = {
  sizeof(struct CAL_MEM_T),      /* size of parameter memory        */
  (uint8_t*)&CalValue,           /* start address of parameter mem  */
  (uint8_t*)&CalDefaults,        /* start address of default para.  */
  CO_RESET_NODE,                 /* reset type for reload parameter */
  (void*)"Calibration",          /* user parameter identification   */
  CO_PARA___E                    /* enable parameter storage on cmd */
};
```

Finaly, in the standard paramter store/restore entries, we use a separate subindex for the calibration values (for example: subindex #2):

```c
const CO_OBJ ExampleObjDir[] = {
    :
  { CO_KEY(0x1010, 2, CO_UNSIGNED32|CO_OBJ____RW), CO_TPARA, (uintptr_t)&CalParaObj },
    :
};
```

We use our user type to define the calibration object entry:

```c
const CO_OBJ ExampleObjDir[] = {
    :
  { CO_KEY(0x2F00, 0, CO_UNSIGNED8 |CO_OBJ_D__RW), 0, 4 },
  { CO_KEY(0x2F00, 1, CO_UNSIGNED32|CO_OBJ_D___W), 0, 0 },
  { CO_KEY(0x2F00, 2, CO_UNSIGNED32|CO_OBJ____RW), CO_TCAL, (uintptr_t)&CalValue.Factor },
  { CO_KEY(0x2F00, 3, CO_UNSIGNED32|CO_OBJ____RW), CO_TCAL, (uintptr_t)&CalValue.Divisor },
  { CO_KEY(0x2F00, 4, CO_SIGNED32  |CO_OBJ____RW), CO_TCAL, (uintptr_t)&CalValue.Offset },
    :
};
```
