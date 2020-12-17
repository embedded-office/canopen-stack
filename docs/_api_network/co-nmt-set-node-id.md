---
layout: article
title: CONmtSetNodeId()
sidebar:
  nav: docs
---

This function sets the requested CANopen Node-ID within the NMT module.

<!--more-->

## Description

The following errors are detected within this function:
- **CO_ERR_NMT_MODE** - the CANopen device is not in INIT mode
- **CO_ERR_BAD_ARG** - the given nodeId is invalid (e.g. zero)

If one of these errors is detected, this function call will change nothing.

***Important: After the successful operation, the function CONmtReset() must be called to re-initialize the internal SDO and PDO tables.***

### Prototype

```c
void CONmtSetNodeId(CO_NMT *nmt, uint8_t nodeId);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| nmt | pointer to NMT object |
| nodeId | requested NMT node ID |

#### Returned Value

- none

## Example

The following example shows how to set a dynamically determined node-ID for the CANopen node AppNode:

```c
    CO_ERR   err;
    uint8_t  id;
    :
    id = AppCalculateNodeId();
    CONmtSetNodeId (&(AppNode.Nmt), id);
    err = CONodeGetErr(&AppNode);
    if (err == CO_ERR_NONE) {
        CONmtReset (&(AppNode.Nmt), CO_RESET_NODE);
        /* calculated node-ID is set */
    } else {
        /* error during setting the node-ID */
    }
    :
```
