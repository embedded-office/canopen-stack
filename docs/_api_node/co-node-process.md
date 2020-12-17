---
layout: article
title: CONodeProcess()
sidebar:
  nav: docs
---

This function is the main communication processing.

<!--more-->

## Description

This function is responsible for performing the necessary response to a received CAN frame.

### Prototype

```c
void CONodeProcess(CO_NODE *node);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| node | pointer to node object |

#### Returned Value

- none

## Example

The function `CONodeProcess()` is most likely called in the background function loop. When using an RTOS, the CANopen node processing from within a separate task is possible, too. In this case, the blocking mode is suitable for most applications:

```c
void CanRxTask(void *arg_p)
{
    CO_NODE *node = (CO_NODE *)arg_p;   /* task parameter is a ptr to   */
                                        /* CANopen node object          */

    while(1) {                          /* endless RTOS task loop       */
        CONodeProcess (node);           /* wait and process CAN frames  */
    }
}
```
