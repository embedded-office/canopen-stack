---
layout: article
title: CONmtGetMode()
sidebar:
  nav: docs
---

This function returns the current CANopen NMT state machine mode.

<!--more-->

## Description

The possible values of the reset type CO_MODE are:

| Value | Description |
| --- | --- |
| CO_INVALID | device in INVALID mode, e.g. not initialized |
| CO_INIT | device in INIT mode |
| CO_PREOP | device in PRE-OPERATIONAL mode |
| CO_OPERATIONAL | device in OPERATIONAL mode |
| CO_STOP | device in STOP mode |


### Prototype

```c
CO_MODE CONmtGetMode(CO_NMT *nmt);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| nmt | pointer to NMT object |

#### Returned Value

- `>0` : current NMT mode
- `=0` : an error is detected

## Example

The following example shows how to perform operations only in the operational mode of the CANopen node AppNode:

```c
    CO_MODE mode;
    :
    mode = CONmtGetMode (&(AppNode.Nmt));
    if (mode == CO_OPERATIONAL) {
        /* perform actions in operational mode */
    }
    :
```
