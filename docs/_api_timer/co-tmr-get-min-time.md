---
layout: article
title: COTmrGetMinTime()
sidebar:
  nav: docs
---

This function calculates the smallest possible time interval with the configured timer.

<!--more-->

## Description

This function returns the time interval in the given unit, which represents a single configured timer tick.

### Prototype

```c
uint16_t COTmrGetMinTime(CO_TMR   *tmr,
                         uint32_t  unit);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| tmr | pointer to timer object |
| unit | unit of given time (`CO_TMR_UNIT_1MS` or `CO_TMR_UNIT_100US`) |

#### Returned Value

- `>0` : time value in the given time interval for a single timer tick
- `=0` : a single given time interval unit is not possible with the used timer

## Example

The following example calculates the smallest possible time interval in 100us.

```c
    CO_TMR  *tmr;
    uint32_t min_100us;
    :
    tmr = &AppNode.Tmr;
    :
    min_100us = COTmrGetMinTime(tmr, CO_TMR_UNIT_100US);
    if (min_100us == 0) {
        /* timer is not configured */
    } else {
        /* you can use time intervals with multiple of min_100us */
    }
    :
```
