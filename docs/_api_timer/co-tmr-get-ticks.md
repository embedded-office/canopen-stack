---
layout: article
title: COTmrGetTicks()
sidebar:
  nav: docs
---

This function calculates the number of required timer ticks for a given time interval.

<!--more-->

## Description

This function is used within the CANopen stack for calculating timeouts, event-times and other needed time intervals.

### Prototype

```c
int32_t COTmrGetTicks(CO_TMR   *tmr,
                      uint16_t  time,
                      uint32_t  unit);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| tmr | pointer to timer object |
| time | time interval as a value of given units |
| unit | unit of given time (`CO_TMR_UNIT_1MS` or `CO_TMR_UNIT_100US`) |

#### Returned Value

- `>0` : timer ticks for the given time interval
- `=0` : given time interval is not possible with the used timer

## Example

The following example calculates the number of ticks for 10.7ms and for 2.5s.

```c
    CO_TMR  *tmr;
    uint32_t tick_10_7ms;
    uint32_t tick_2500ms;
    :
    tmr = &AppNode.Tmr;
    :
    tick_10_7ms = COTmrGetTicks(tmr, 107, CO_TMR_UNIT_100US);
    if (tick_10_7ms == 0) {
        /* time interval 10.7ms is not possible */
    } else {
        /* you can use the time interval 10.7ms */
    }
    :
    tick_2500ms = COTmrGetTicks(tmr, 2500, CO_TMR_UNIT_1MS);
    if (tick_2500ms == 0) {
        /* time interval 2.5s is not possible */
    } else {
        /* you can use the time interval 2.5s */
    }
    :
```

*Note: The example assumes that the CANopen node is already initialized with `CONodeInit()` because the specified frequency in the specification structure is the foundation for these calculations.*
