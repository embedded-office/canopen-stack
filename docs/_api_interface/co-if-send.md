---
layout: article
title: COIfCanSend()
sidebar:
  nav: docs
---

This function sends the given CAN frame to the interface without delay.

<!--more-->

## Description

The send function may be used within the application to transmit additional messages on the CANopen interface.

### Prototype

```c
int16_t COIfCanSend(CO_IF *cif, CO_IF_FRM *frm);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| cif | pointer to interface object |
| frm | pointer to destination frame buffer |

#### Returned Value

- `>0` : size of `CO_IF_FRM` on success
- `<0` : an error is detected

## Example

The following example shows how to call the transmit function for the interface of the CANopen node AppNode:

```c
    CO_IF_FRM  frame;
    int16_t    err;
    :
    frame.Identifier = 0x123;
    frame.DLC        = 8;
    frame.Data[0]    = 0x12;
    frame.Data[1]    = 0x23;
    frame.Data[2]    = 0x34;
    frame.Data[3]    = 0x45;
    frame.Data[4]    = 0x56;
    frame.Data[5]    = 0x67;
    frame.Data[6]    = 0x78;
    frame.Data[7]    = 0x89;
    err = COIfCanSend (&(AppNode.If), &frame);
    if (err < 0) {
        /* error in interface layer */
    }
    :
```
