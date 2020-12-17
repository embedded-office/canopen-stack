---
layout: article
title: CONodeStart()
sidebar:
  nav: docs
---

This function will finish the initialization phase.

<!--more-->

## Description

The node will change into the PRE-OPERATIONAL state and is ready for communication.

### Prototype

```c
void CONodeStart(CO_NODE *node);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| node | pointer to node object |

#### Returned Value

- none

## Example

The following example shows the typical startup of a CANopen node AppNode with the specification AppSpec:

```c
    CO_ERR err;
    :
    CONodeInit (&AppNode, (CO_NODE_SPEC *)&AppSpec);

    err = CONodeGetErr (&AppNode);
    if (err == CO_ERR_NONE) {
        CONodeStart (&AppNode);
    } else {
        /* error handling and diagnostics */
    }
    :
```
