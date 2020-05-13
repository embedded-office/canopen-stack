---
layout: article
title: Write serial number
---

In the production line, the devices should be programmed with the identical firmware image. The object entry 1018 holds optionally a serial number which sould be unique for each device. We need a one-time programmable object entry.

<!--more-->

## Example Goal

The main idea is a manufacturer specific data type, which allows writing into a FLASH memory area if the current FLASH memory area is empty.

# Object Type Idea

The main idea is an object entry, which is directly mapped to FLASH cells. The type checks on a write access for empty FLASH cells and allows writing with a FLASH write algorithm if FLASH cells are empty.


### Object Entry Definitions

We define some manufacturer specific entries in the object dictionary:

| Index | Subindex | Type       | Access     | Value    | Description        |
| ----- | -------- | ---------- | ---------- | -------- | ------------------ |
| 1018h | 4        | UNSIGNED32 | Const      | FFFFFFFF | Serial No.         |

The key to get the wanted functionality is the object type for the entry at subindex 4. This object type accepts a single permanent write access if the current value is `0xFFFFFFFF`.

## Implement Object Type

Lets implement the one time programmable (OTP) user type as shown in the [CANopen Usage: User Object](/docs/usecase/dictionary#user-objects):

```c
const uint32_t serialNo = 0xFFFFFFFFu;  /* locate this variable to FLASH and initialize with empty cell value */

const CO_OBJ_TYPE COTOtp = { 0, 0, 0, OtpWrite };

#define CO_TOTP ((CO_OBJ_TYPE*)&COTOtp)
```

The write function is called, when the CAN network writes to the related object entry via a SDO request.
Now we want to allow the write algorithm for the constant FLASH value after a serial-no is written to the object entry at subindex 4.

```c
int16_t OtpWrite(CO_OBJ *obj, struct CO_NODE_T *node, void *buf, uint32_t size)
{
  uint32_t value  = *((uint32_t *)buf);
  uint32_t serial = *((uinter_t *)obj->Data);
  CO_ERR   result = CO_ERR_TYPE_WR;

  if (serial == 0xFFFFFFFF) {
    /* call your hardware specific FLASH algorithm with
     * - start address in FLASH 
     * - new serial value
     * - length of constant
     */
    HwFlashWriteAlgorithm(obj->Data, value, 4);
  }

  return result;
}
```

## Implement Object Entries


We use our user type to define the calibration object entry:

```c
const CO_OBJ ExampleObjDir[] = {
    :
  { CO_KEY(0x1018, 4, CO_UNSIGNED32|CO_OBJ_D__R_), CO_TOTP, (uintptr_t)&serialNo },
    :
};
```

Now we have a one time programmable object entry for the serial number. This concept shows how to integrate application specific logic to specific object entry access.
