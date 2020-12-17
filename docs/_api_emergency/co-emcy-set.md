---
layout: article
title: COEmcySet()
sidebar:
  nav: docs
---

This function checks the current error state and updates the object dictionary.

<!--more-->

## Description

The EMCY message is transmitted if the error is detected for the first time. The given manufacturer-specific fields are optional, e.g. the pointer may be 0 to set all manufacturer-specific values to 0.

### Prototype

```c
void COEmcySet(CO_EMCY *emcy, uint8_t err, CO_EMCY_USR *user);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| emcy | pointer to EMCY object |
| err | error code identifier in application EMCY table |
| user | manufacturer-specific fields in EMCY history and/or EMCY message |

#### Returned Value

- none

## Example

The following example demonstrates the detection of an emergency event APP_EMCY_2 within the application of the CANopen node AppNode. The specific user information data for this event is: 0x1234567890.

```c
    CO_EMCY_USR usr;
    :
    usr.Emcy[0] = 0x12;
    usr.Emcy[1] = 0x34;
    usr.Emcy[2] = 0x56;
    usr.Emcy[3] = 0x78;
    usr.Emcy[4] = 0x90;
    COEmcySet (&(AppNode.Emcy), APP_EMCY_2, &usr);
    :
```

Note: The specific user information data field is optional. If not used, the last parameter during calling COEmcySet() can be 0 (zero). The emergency identifier APP_EMCY_2 must be defined within the application-specific configuration.
