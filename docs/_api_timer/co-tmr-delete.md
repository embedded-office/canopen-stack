---
layout: article
title: COTmrDelete()
sidebar:
  nav: docs
---

This function deletes the defined action and removes the timer, if this was the last action for this timer event.

<!--more-->

### Description

This function are used within the CANopen stack for timed actions. It is possible to use timed actions in the application, too.

#### Prototype

```c
int16_t COTmrDelete(CO_TMR  *tmr,
                    int16_t  actId);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| tmr | pointer to timer object |
| actId | action identifier, returned by [COTmrCreate()](co-tmr-create) |

#### Returned Value

- `>=0` : action identifier (successful operation)
- `<0` : an error is detected

### Example

The following calls a critical function `AppCritFunc()` and installs a function `AppEmcyFunc()` to the CANopen node AppNode to guarantee a maximum runtime of 150ms. The timed function will be called after 150ms and perform emergency handling with the parameter 0xdead. If the `AppCritFunc()` is finished before 150ms, the timed emergency function call will be removed.

```c
    CPU_INT16S  aid;
    :
    aid = COTmrCreate(&(AppNode.Tmr), 150, 0, AppEmcyFunc, 0xdead);
    if (aid >= 0) {
        AppCritFunc();
        err = COTmrDelete(&(AppNode.Tmr), aid);
        if (err < 0) {
            /* error during deleting the timed action */
        }
    } else {
        /* error during creation of timed action */
    }
    :
```

Note: The example assumes, that the timer callback function `AppEmcyFunc()` and the application function `AppCritFunc()` are implemented with the correct prototype. The hardware timer is configured with 1 tick equal to 1ms.
