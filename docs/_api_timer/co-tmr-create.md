---
layout: article
title: COTmrCreate()
sidebar:
  nav: docs
---

This function creates the defined action and links this action into the event timer list at the correct timing.

<!--more-->

### Description

This function are used within the CANopen stack for timed actions. It is possible to use timed actions in the application, too.

#### Prototype

```c
int16_t COTmrCreate(CO_TMR      *tmr,
                    uint32_t     startTime,
                    uint32_t     cycleTime,
                    CO_TMR_FUNC  func,
                    void        *para);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| tmr | pointer to timer object |
| startTime | delta time in ticks for the first timer event |
| cycleTime | if != 0, the delta time in ticks for the cyclic timer events |
| func | pointer to the action callback function |
| para | pointer to the callback function parameter |

#### Returned Value

- `>=0` : action identifier (successful operation)
- `<0` : an error is detected

### Example

The following example installs a cyclic called function `AppCyclicFunc()` to the CANopen node AppNode. The timed function will be called the first time after 10ms and then every 25ms with the parameter: 0xcafe.

```c
    CPU_INT16S  aid;
    :
    aid = COTmrCreate(&(AppNode.Tmr), 10, 25, AppCyclicFunc, 0xcafe);
    if (aid >= 0) {
        /* tid holds the timer identifier */
    } else {
        /* error during creation of timed action */
    }
    :
```

Note: The example assumes, that the timer callback function `AppCyclicFunc()` is implemented with the correct prototype. The hardware timer is configured with 1 tick equal to 1ms.
