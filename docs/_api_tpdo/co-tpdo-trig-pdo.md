---
layout: article
title: COTPdoTrigPdo()
sidebar:
  nav: docs
---

This function allows the application to trigger a TPDO event for the given TPDO number.

<!--more-->

## Description

The function call will immediately initiate the TPDO transmission, independent on the TPDO communication type settings in the object dictionary.

### Prototype

```c
void COTPdoTrigPdo(CO_TPDO *pdo, uint16_t num);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| pdo | pointer to start of TPDO array |
| num | number of TPDO (0..511) |

#### Returned Value

- none

## Example

The following example triggers the transmission of TPDO #3 of the CANopen node AppNode.

```c
    :
    COTPdoTrigPdo(&(AppNode.TPdo[0]), 3);
    :
```
