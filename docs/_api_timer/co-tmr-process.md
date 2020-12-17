---
layout: article
title: COTmrProcess()
sidebar:
  nav: docs
---

This function processes a single elapsed timer action.

<!--more-->

## Description

The function is used decouple the generation of the periodic time base and the timed action processing.

### Prototype

```c
void COTmrProcess(CO_TMR *tmr);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| tmr | pointer to timer object |

#### Returned Value

- none

## Example

The timed action processing must be called cyclic. Due to the timer management, the calling sequence is irrelevant. Therefore the function may be called in the background loop of the application.

```c
    :
    while (1) {
        COTmrProcess(&(AppNode.Tmr));
        :
    }
```
