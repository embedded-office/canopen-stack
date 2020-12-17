---
layout: article
title: CONmtSetMode()
sidebar:
  nav: docs
---

This function sets the requested NMT state machine mode.

<!--more-->

## Description

The possible values of the device mode CO_MODE are:

| Value | Description |
| --- | --- |
| CO_INVALID | device in INVALID mode, e.g. not initialized |
| CO_INIT | device in INIT mode |
| CO_PREOP | device in PRE-OPERATIONAL mode |
| CO_OPERATIONAL | device in OPERATIONAL mode |
| CO_STOP | device in STOP mode |


### Prototype

```c
void CONmtSetMode(CO_NMT *nmt, CO_MODE mode);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| nmt | pointer to NMT object |
| mode | requested NMT mode |

#### Returned Value

- none

## Example

The following example shows how to switch the CANopen node AppNode in the operational mode without a CANopen master network command:

```c
    :
    CONmtSetMode (&(AppNode.Nmt), CO_OPERATIONAL);
    :
```
