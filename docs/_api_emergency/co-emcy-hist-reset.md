---
layout: article
title: COEmcyHistReset()
sidebar:
  nav: docs
---

This function clears the EMCY history in the object dictionary.

<!--more-->

## Description

There is no EMCY message transmission due to this function call.

### Prototype

```c
void COEmcyHistReset(CO_EMCY *emcy);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| emcy | pointer to EMCY object |

#### Returned Value

- none

## Example

The following example clears all emergency history entries within the object dictionary of the CANopen node AppNode.

```c
    :
    COEmcyHistReset (&(AppNode.Emcy));
    :
```
