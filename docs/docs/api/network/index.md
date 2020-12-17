---
layout: articles
title: Network Management
sidebar:
  nav: docs
articles:
  data_source: site.api_network
  type: table
  show_excerpt: true
---

<div class="article__content" markdown="1">

The network component provides an interface to the CANopen mode control, the node management, and the heartbeat handling.

## Structure

```mermaid
  classDiagram
      CO_NODE o-- CO_NMT : Node
      CO_MODE -- CO_NMT
      CO_NMT_RESET -- CO_NMT
      class CO_NMT {
        -int16_t Tmr
        -uint8_t Allowed
        +CONmtGetHbEvents(nodeId) int16_t
        +CONmtGetMode() CO_MODE
        +CONmtGetNodeId() uint8_t
        +CONmtLastHbState(nodeId) CO_MODE
        +CONmtReset(type)
        +CONmtSetMode(mode)
        +CONmtSetNodeId(nodeId)
      }
      class CO_NMT_RESET {
        <<enumeration>>
      }
```

### Data

The class `CO_NMT` is defined within `co_nmt.h` and is responsible for the CANopen network slave management. The following data members are in this class:

| Data Member | Type | Description |
| --- | --- | --- |
| Node | `CO_NODE*` | pointer to parent node |
| Mode | `CO_MODE` | current NMT mode |
| Tmr | `int16_t` | heartpeat producer timer identifier |
| Allowed | `uint8_t` | encoding of allowed CANopen services |

**Note:** The data within this structure must never be manipulated without the corresponding class member functions. This can lead to unpredictable behavior of the node.
{:.info}

## Functions

The following table describes the API functions of the CANopen network management module. Furthermore, this module includes the heartbeat producer and consumer. These functions are implemented within the source file: `co_nmt.c/h`, `co_hbprod.c/h` and `co_hbcons.c/h`

</div>
