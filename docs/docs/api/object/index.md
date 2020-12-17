---
layout: articles
title: Object Entry
sidebar:
  nav: docs
articles:
  data_source: site.api_object
  type: table
  show_excerpt: true
---

<div class="article__content" markdown="1">

The object component provides an interface to the individual object entries.

## Structure

```mermaid
  classDiagram
      CO_NODE o-- CO_OBJ : Node
      CO_OBJ_TYPE o-- CO_OBJ : Type
      class CO_OBJ {
        -uint32_t Key
        -uintptr_t Data
        +COObjGetSize(width) uint32_t
        +COObjRdBufCont(destination, length) int16_t
        +COObjRdBufStart(destination, length) int16_t
        +COObjRdValue(destination, width, nodeId) int16_t
        +COObjWrBufCont(source, length) int16_t
        +COObjWrBufStart(source, length) int16_t
        +COObjWrValue(source, width, nodeId) int16_t
      }
```

### Data

The class `CO_OBJ` is defined within `co_obj.h` and is responsible for the CANopen object entry data access handling. The following data members are in this class:

| Data Member | Type | Description |
| --- | --- | --- |
| Key | `uint32_t` | Encoded object entry properties: index, subindex, external access mode, PDO mappable, type of object entry, consideration of node ID and data storage information |
| Type | `CO_OBJ_TYPE*` | pointer to object type |
| Data | `uintptr_t` | data information of object entry |

**Note:** The data within this structure must never be manipulated without the corresponding class member functions. This can lead to unpredictable behavior of the node.
{:.info}

### Functions

The following table describes the API functions of the CANopen object entry module. These functions are implemented within the source file: `co_obj.c/h`

</div>
