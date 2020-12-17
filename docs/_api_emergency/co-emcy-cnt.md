---
layout: article
title: COEmcyCnt()
sidebar:
  nav: docs
---

This function returns the number of currently detected EMCY errors.

<!--more-->

## Description

When calling this function, all EMCY error status will stay unchanged.

### Prototype

```c
int16_t COEmcyCnt(CO_EMCY *emcy);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| emcy | pointer to EMCY object |

#### Returned Value

- `>=0` : success with the number of currently detected EMCY errors
- `<0` : an error is detected inside of this function

## Example

The following example calculates the current number of detected emergency events within the application of the CANopen node AppNode.

```c
    int16_t emcy;
    :
    emcy = COEmcyCnt (&(AppNode.Emcy));
    if (emcy < 0) {
        /* error handling */
    } else {
        /* emcy holds number of detected emergencies */
    }
    :
```
