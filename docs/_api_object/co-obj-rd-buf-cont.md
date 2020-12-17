---
layout: article
title: COObjRdBufCont()
sidebar:
  nav: docs
---

This function continues the read operation at the current offset of the byte stream from the given object entry into the given destination buffer.

<!--more-->

## Description

The function is used together with `COObjRdBufStart()` to read an object entry with a size greater than 4.

### Prototype

```c
int16_t COObjRdBufCont(CO_OBJ  *obj,
                       void    *buffer,
                       uint8_t  length);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| obj | pointer to object entry |
| buffer | pointer to destination memory |
| length | length of destination buffer |

#### Returned Value

- `==CO_ERR_NONE` : successful operation
- `!=CO_ERR_NONE` : an error is detected (see [CONodeGetErr()](/api_node/co-node-get-err))

## Example

The following example reads the byte-stream of the hypothetical application-specific object entry "[1234:56]" within the object directory of the CANopen node AppNode.

```c
    uint8_t  buffer[10];
    int16_t  err;
    CO_OBJ  *entry;
    :
    entry = CODirFind      (&(AppNode.Dir), CO_DEV(0x1234,0x56));
    err   = COObjRdBufStart(entry, buffer, 10);
    if (err == CO_ERR_NONE) {
        do {
            /* read all packages from stream */
            err = COObjRdBufCont(entry, buffer, 10);
        } while (err == CO_ERR_NONE);
    } else {
        /* error during reading */
    }
    :
```
