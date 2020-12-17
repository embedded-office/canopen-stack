---
layout: article
title: CONmtGetHbEvents()
sidebar:
  nav: docs
---

This function returns the number of detected heartbeat miss events of the given CANopen Node-ID out of the heartbeat consumer.

<!--more-->

## Description

If a second heartbeat is received within the heartbeat deadline, the heartbeat timing is marked as *ok*. If the second heartbeat is received after the heartbeat deadline, the heartbeat timing is marked as *missed*.

### Prototype

```c
int16_t CONmtGetHbEvents (CO_NMT *nmt, uint8_t nodeId);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| nmt | pointer to NMT object |
| nodeId | address heartbeat consumer for given node |

#### Returned Value

- `<0` : error, e.g. the given node ID is not monitored by a heartbeat consumer
- `=0` : the heartbeat consumer detects no miss event for the given node ID
- `>0` : the heartbeat consumer detects this number of missed heartbeat deadlines

## Example

The following example shows how to check the heartbeat consumer results of the CANopen node AppNode for the CANopen Node with node-ID = 10:

```c
    int16_t  events;
    :
    events = CONmtGetHbEvents (&(AppNode.Nmt), 10);
    if (events > 0) {
        /* number of missing heartbeats of the monitored node 10 */
    } else if (events < 0) {
        /* no heartbeat consumer running for node 10 */
    } else {
        /* the heartbeats of node 10 are received as expected */
    }
    :
```
