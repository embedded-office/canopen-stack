---
layout: article
title: COTmrDelete()
sidebar:
  nav: docs
---

This function deletes the defined action and removes the timer if this was the last action for this timer event.

<!--more-->

## Description

This function is used within the CANopen stack for timed actions. It is possible to use timed actions in the application, too.

### Prototype

```c
int16_t COTmrDelete(CO_TMR  *tmr,
                    int16_t  action);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| tmr | pointer to timer object |
| action | action identifier, returned by [COTmrCreate()](co-tmr-create) |

#### Returned Value

- `>=0` : action identifier (successful operation)
- `<0` : an error is detected

## Example

The following calls a critical function `AppCritFunc()` and installs a function `AppEmcyFunc()` to the CANopen node AppNode to guarantee a maximum runtime of 150ms. The timed function will be called after 150ms and perform emergency handling with the parameter 0xdead. If the `AppCritFunc()` is finished before 150ms, the timed emergency function call will be removed.

```c
    CO_TMR     *tmr;
    CPU_INT16S  aid;
    uint32_t    max;
    :
    tmr = &AppNode.Tmr;
    max = COTmrGetTicks(tmr, 150, CO_TMR_UNIT_1MS);
    aid = COTmrCreate(tmr, max, 0, AppEmcyFunc, 0xdead);
    if (aid >= 0) {
        AppCritFunc();
        err = COTmrDelete(tmr, aid);
        if (err < 0) {
            /* error during deleting the timed action */
        }
    } else {
        /* error during creation of timed action */
    }
    :
```

*Note: The example assumes, that the timer callback function `AppEmcyFunc()` and the application function `AppCritFunc()` are implemented with the correct prototype. The hardware timer is configured with sufficient timer clock frequency to achieve the 50ms resolution.*
