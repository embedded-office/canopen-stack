---
layout: article
title: COEmcyReset()
sidebar:
  nav: docs
---

This function clears all EMCY errors.

<!--more-->

## Description

The EMCY message transmission can be suppressed by setting the parameter silent to 1.

### Prototype

```c
void COEmcyReset(CO_EMCY *emcy, uint8_t silent);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| emcy | pointer to EMCY object |
| silent | disables the EMCY message transmission for the state changes, made by this function |

#### Returned Value

- none

## Example

The following example clears all detected emergency within the application of the CANopen node AppNode. The corresponding emergency state change messages will be sent.

```c
    :
    COEmcyReset (&(AppNode.Emcy), 0);
    :
```
