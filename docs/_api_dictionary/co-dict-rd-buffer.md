---
layout: article
title: CODictRdBuffer()
sidebar:
  nav: docs
---

This function reads a buffer byte stream from the given object dictionary.

<!--more-->

## Description

The object entry is addressed with the given key and the bytes will be read from the dictionary and copied to the given destination buffer with the given length.

### Prototype

```c
CO_ERR CODictRdBuffer(CO_DICT  *cod,
                      uint32_t  key,
                      uint8_t  *buffer,
                      uint32_t  length);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| cod | pointer to the object dictionary |
| key | object entry key; should be generated with `CO_DEV` |
| buffer | pointer to the destination bytes |
| length | length of destination buffer in number of bytes |

#### Returned Value

- `== CO_ERR_NONE` : successful operation
- `!= CO_ERR_NONE` : an error is detected

## Example

The following example reads the value to the hypothetical application-specific object entry "[1234:56]" within the object dictionary of the CANopen node AppNode.

```c
    CO_ERR   err;
    uint8_t  buffer[10];
    :
    err = CODictRdBuffer (&(Appnode.Dict), CO_DEV(0x1234, 0x56), buffer, 10);
    if (err != CO_ERR_NONE) {
        /* object [1234:56] is missing or error during writing */
    }
    :
```
