---
layout: article
title: COObjRdValue()
sidebar:
  nav: docs
---

This function reads the value of the given object entry.

<!--more-->

## Description

The access with this function to an object entry will be done with the casting of the object entry values to the requested value width.

### Prototype

```c
int16_t COObjRdValue(CO_OBJ  *obj ,
                     void    *value,
                     uint8_t  width,
                     uint8_t  nodeId);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| obj | pointer to object entry |
| value | pointer to destination memory |
| width | width of read value (must be 1, 2 or 4 and reflecting the width of the referenced variable, given by parameter value) |
| nodeId | device node ID (only relevant in case of node ID dependent value) |

#### Returned Value

- `==CO_ERR_NONE` : successful operation
- `!=CO_ERR_NONE` : an error is detected (see [CONodeGetErr()](/api_node/co-node-get-err))

## Example

The following example gets the value of the hypothetical application-specific object entry "[1234:56]" within the object dictionary of the CANopen node AppNode.

```c
    uint32_t  value;
    CO_OBJ   *entry;
    :
    entry = CODirFind     (&(AppNode.Dir), CO_DEV(0x1234,0x56));
    err   = COObjRdValue  (entry, &value, 4, 0);
    :
```

Note: The example shows the read access with the knowledge, that the addressed object entry is independent of the node ID. To be independent of this knowledge, the API function `CONmtGetNodeId()` may be used to get the current node ID.
