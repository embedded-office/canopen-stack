---
layout: article
title: CONodeStop()
sidebar:
  nav: docs
---

This function will stop all communication activities.

<!--more-->

## Description

This function removes the CANopen node from the CAN bus interface.

To reactivate a stopped CANopen node, the functions `CONodeInit()` and `CONodeStart()` must be called again.

### Prototype

```c
void CONodeStop(CO_NODE *node);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| node | pointer to node object |

#### Returned Value

- none

## Example

The following example shows the sequence for restarting a CANopen node AppNode. Assuming the CANopen stack is initialized and started before, we just need to stop and re-initialize the stack to reset the node.

```c
    :
    CONodeStop (&AppNode);
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

Attention: The shown example is NOT identical to the standard node reset which can be requested from the CANopen master or via the API function `CONmtReset()`. The difference is: the example will transmit the boot-up message.
