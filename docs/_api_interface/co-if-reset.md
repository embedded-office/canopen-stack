---
layout: article
title: COIfCanReset()
sidebar:
  nav: docs
---

This function resets the CAN interface and flushes all already buffered CAN frames.

<!--more-->

## Description

This function resets the interface ONLY. Be careful in calling this function for an active CANopen node on this interface. This can result in unpredictable behavior of the overall system.

### Prototype

```c
void COIfCanReset(CO_IF *cif);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| cif | pointer to interface object |

#### Returned Value

- none

## Example

The following example shows how to reset the interface of the CANopen node AppNode:

```c
    :
    COIfCanReset (&(AppNode.If));
    :
```
