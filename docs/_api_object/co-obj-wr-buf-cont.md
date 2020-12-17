---
layout: article
title: COObjWrBufCont()
sidebar:
  nav: docs
---

This function continues the write operation at the current offset of the byte stream from the given source buffer to the object entry.

<!--more-->

## Description

The function is used together with `COObjWrBufStart()` to write an object entry with a size greater than 4.

### Prototype

```c
int16_t COObjWrBufStart(CO_OBJ  *obj,
                        void    *buffer,
                        uint8_t  length);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| obj | pointer to object entry |
| buffer | pointer to source memory |
| length | length of source buffer |

#### Returned Value

- `==CO_ERR_NONE` : successful operation
- `!=CO_ERR_NONE` : an error is detected (see [CONodeGetErr()](/api_node/co-node-get-err))

## Example

The following example writes a byte-stream to the hypothetical application-specific object entry "[1234:56]" within the object directory of the CANopen node AppNode.

```c
    CPU_INT08U  buffer[10] = { 'H', 'e', 'l', 'l', 'o', 'W', 'o', 'r', 'l', 'd' };
    CPU_INT16S  err;
    CO_OBJ     *entry;
    :
    entry = CODirFind      (&(AppNode.Dir), CO_DEV(0x1234,0x56));
    err   = COObjWrBufStart(entry, buffer, 10);
    if (err == CO_ERR_NONE) {
        do {
            /* stream bytes to object */
            err = COObjWrBufCont(entry, buffer, 10);
        } while (err == CO_ERR_NONE);
    } else {
        /* error during writing */
    }
    :
```
