---
layout: article
title: CONmtLastHbState()
sidebar:
  nav: docs
---

This function returns the NMT state, received by the heartbeat consumer for the given node ID.

<!--more-->

## Description

The possible values of the device mode CO_MODE are:

| Value | Description |
| --- | --- |
| CO_INVALID | device in INVALID mode, e.g. not initialized |
| CO_INIT | device in INIT mode |
| CO_PREOP | device in PRE-OPERATIONAL mode |
| CO_OPERATIONAL | device in OPERATIONAL mode |
| CO_STOP | device in STOP mode |

### Prototype

```c
CO_MODE CONmtLastHbState (CO_NMT *nmt, uint8_t nodeId);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| nmt | pointer to NMT object |
| nodeId | address heartbeat consumer for given node |

#### Returned Value

- `==CO_INVALID` : an error is detected; e.g. the given node ID is not monitored by a heartbeat consumer
- `!=CO_INVALID` : the heartbeat consumer received state from the given node ID

## Example

The following example shows how to check the heartbeat consumer received mode of the CANopen node AppNode for the CANopen Node with node ID = 10:

```c
    CO_MODE  state;
    :
    state = CONmtLastHbState (&(AppNode.Nmt), 10);
    if (state == CO_INVALID) {
        /* no heartbeat consumer running for node 10 */
    } else {
        /* you may react on specific NMT states here... */
    }
    :
```
