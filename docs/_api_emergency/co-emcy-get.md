---
layout: article
title: COEmcyGet()
sidebar:
  nav: docs
---

This function returns the current EMCY error status.

<!--more-->

## Description

When calling this function, the EMCY error status will stay unchanged.

### Prototype

```c
int16_t COEmcyGet(CO_EMCY *emcy, uint8_t error);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| emcy | pointer to EMCY object |
| error | error code identifier in application EMCY table |

#### Returned Value

- `=0` : success with error is not detected
- `=1` : success with an error was detected before
- `<0` : an error is detected inside of this function

## Example

The following example reads the current status of a possible detected emergency event APP_EMCY_2 within the application of the CANopen node AppNode.

```c
    int16_t warning;
    :
    warning = COEmcyGet (&(AppNode.Emcy), APP_EMCY_2);
    if (warning < 0) {
        /* error handling */
    } else {
        /* warning holds current emergency status */
    }
    :
```

Note: The emergency identifier APP_EMCY_2 must be defined within the application-specific configuration.
