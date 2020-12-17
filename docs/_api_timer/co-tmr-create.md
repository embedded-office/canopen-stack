---
layout: article
title: COTmrCreate()
sidebar:
  nav: docs
---

This function creates the defined action and links this action into the event timer list at the correct timing.

<!--more-->

## Description

This function is used within the CANopen stack for timed actions. It is possible to use timed actions in the application, too.

### Prototype

```c
int16_t COTmrCreate(CO_TMR      *tmr,
                    uint32_t     start,
                    uint32_t     cycle,
                    CO_TMR_FUNC  function,
                    void        *para);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| tmr | pointer to timer object |
| start | delta time in ticks for the first timer-event |
| cycle | if != 0, the delta time in ticks for the cyclic timer events |
| function | pointer to the action callback function |
| para | pointer to the callback function parameter |

#### Returned Value

- `>=0` : action identifier (successful operation)
- `<0` : an error is detected

## Example

The following example installs a cyclic called function `AppCyclicFunc()` to the CANopen node AppNode. The timed function will be called the first time after 10ms and then every 25ms with the parameter: 0xcafe.

```c
    CO_TMR  *tmr;
    int16_t  aid;
    uint32_t start;
    uint32_t cycle;
    :
    tmr   = &AppNode.Tmr;
    start = COTmrGetTicks(tmr, 10, CO_TMR_UNIT_1MS);
    cycle = COTmrGetTicks(tmr, 25, CO_TMR_UNIT_1MS);
    aid = COTmrCreate(tmr, start, cycle, AppCyclicFunc, 0xcafe);
    if (aid >= 0) {
        /* tid holds the timer identifier */
    } else {
        /* error during creation of timed action */
    }
    :
```

*Note: The example assumes, that the timer callback function `AppCyclicFunc()` is implemented with the correct prototype. The hardware timer is configured with sufficient timer clock frequency to achieve the 5ms resolution.*
