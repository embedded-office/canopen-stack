---
layout: article
title: COIfCanRead()
sidebar:
  nav: docs
---

This function waits for the next CAN frame on the interface without a timeout.

<!--more-->

## Description

If a CAN frame is received, the given receive frame buffer will be filled with the received data. The read function is rarely used from within the application because the CANopen will receive all messages by default and allows to work on all non-CANopen messages with a callback function.

*For evaluation, demonstration, or testing purposes this CAN read function may poll for a new CAN frame. In this special case, the additional return value with no received CAN frame is possible. Don't use the polling mode in production; you should use interrupt-driven CAN communication. The CAN polling is not suitable for CANopen.*

### Prototype

```c
int16_t COIfCanRead(CO_IF *cif, CO_IF_FRM *frm);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| cif | pointer to interface object |
| frm | pointer to destination frame buffer |

#### Returned Value

- `>0` : size of `CO_IF_FRM` on success
- `=0` : special: nothing received during polling (timeout)
- `<0` : an error is detected

## Example

If necessary, the following example show how to call the blocking receive function for the interface of the CANopen node AppNode:

```c
    CO_IF_FRM  frame;
    int16_t    err;
    :
    err = COIfCanRead (&(AppNode.If), &frame);
    if (err <= 0) {
        /* error in interface layer */
    } else {
        /* frame contains received data */
    }
    :
```
