---
layout: article
title: COTmrService()
sidebar:
  nav: docs
---

This function updates the timer states.

<!--more-->

## Description

The function is used to generate a periodic time base and must be called cyclic.

### Prototype

```c
int16_t COTmrService(CO_TMR *tmr);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| tmr | pointer to timer object |

#### Returned Value

- `=0` : no timer is elapsed
- `>0` : at least one timer is elapsed
- `<0` : an error is detected

## Example

The following interrupt service function generates the time base for the CANopen timed actions with periodic calls of the function `COTmrService()`. When using an RTOS, the return value may be used to decide of triggering a separate timer task to process the elapsed events.

```c
void HardwareTimerISR (void)
{
    int16_t num;

    num = COTmrService(&(AppNode.Tmr));
    if (num > 0) {
        /* post an event to the timer processing task */
    }
}
```
