---
layout: article
title: COEmcyClr()
sidebar:
  nav: docs
---

This function checks the current error state and updates the object dictionary. 

<!--more-->

## Description

The EMCY message is transmitted if the error was previously detected.

### Prototype

```c
void COEmcyClr(CO_EMCY *emcy, uint8_t error);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| emcy | pointer to EMCY object |
| error | error code identifier in application EMCY table |

#### Returned Value

- none

## Example

The following example demonstrates the clearing of a possible detected emergency event APP_EMCY_2 within the application of the CANopen node AppNode.

```c
    :
    COEmcyClr (&(AppNode.Emcy), APP_EMCY_2);
    :
```

Note: The emergency identifier APP_EMCY_2 must be defined within the application-specific configuration.
