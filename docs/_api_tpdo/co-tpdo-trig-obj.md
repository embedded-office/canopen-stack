---
layout: article
title: COTPdoTrigObj()
sidebar:
  nav: docs
---

This function allows the application to trigger a TPDO event for the given object entry.

<!--more-->

## Description

The event will be distributed to all TPDOs, which holds an active mapping entry to this object entry.

### Prototype

```c
void COTPdoTrigObj(CO_TPDO *pdo, CO_OBJ *obj);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| pdo | pointer to start of TPDO array |
| obj | pointer to object entry |

#### Returned Value

- none

## Example

The following example triggers the transmission of all TPDOs of the CANopen node AppNode, which contains the hypothetical object directory entry "[1234:56]".

```c
    CO_OBJ  *entry;
    :
    entry = CODirFind(&(AppNode.Dir), CO_DEV(0x1234, 0x56));
    COTPdoTrigObj(&(AppNode.TPdo[0]), entry);
    :
```
