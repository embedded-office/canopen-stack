---
layout: article
title: COIfCanClose()
sidebar:
  nav: docs
---

This function closes the CAN interface.

<!--more-->

## Description

This function closes the interface ONLY. Be careful in calling this function for an active CANopen node on this interface. This can result in unpredictable behavior of the overall system.

### Prototype

```c
void COIfCanClose(CO_IF *cif);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| cif | pointer to interface object |

#### Returned Value

- none

## Example

The following example shows how to close the interface of the CANopen node AppNode:

```c
    :
    COIfCanClose (&(AppNode.If));
    :
```
