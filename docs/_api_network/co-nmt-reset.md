---
layout: article
title: CONmtReset()
sidebar:
  nav: docs
---

This function resets the CANopen device with the given type. 

<!--more-->

## Description

The possible values of the reset type CO_NMT_RESET are:

| Value | Description |
| --- | --- |
| CO_RESET_NODE | reset node (application and communication) |
| CO_RESET_COM | reset communication |

### Prototype

```c
void CONmtReset(CO_NMT *nmt, CO_NMT_RESET type);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| nmt | pointer to NMT object |
| type | requested NMT reset type |

#### Returned Value

- none

## Example

The following example shows how to request a communication reset of the CANopen node AppNode:

```c
    :
    CONmtReset (&(AppNode.Nmt), CO_RESET_COM);
    :
```
