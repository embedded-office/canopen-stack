---
layout: article
title: CONmtGetNodeId()
sidebar:
  nav: docs
---

This function returns the current node ID of the CANopen device.

<!--more-->

## Description

There should be always a valid node ID within the CANopen Stack. The default node ID is set in the node specification.

### Prototype

```c
uint8_t CONmtGetNodeId(CO_NMT *nmt);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| nmt | pointer to NMT object |
| nodeId | requested NMT node ID |

#### Returned Value

- `>0` : current NMT node ID
- `=0` : an error is detected

## Example

The following example shows how to get the node ID of the CANopen node AppNode:

```c
    uint8_t  id;
    :
    id = CONmtGetNodeId (&(AppNode.Nmt));
    if (id == 0) {
        /* error during reading the node-ID */
    } else {
        /* id holds the node ID */
    }
    :
```
