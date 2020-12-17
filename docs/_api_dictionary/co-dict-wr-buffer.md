---
layout: article
title: CODictWrBuffer()
sidebar:
  nav: docs
---

This function writes a buffer byte stream to the given object dictionary.

<!--more-->

## Description

The object entry is addressed with the given key and the bytes will be read from to the given source buffer of the given length.

### Prototype

```c
CO_ERR CODictWrBuffer(CO_DICT  *cod,
                      uint32_t  key,
                      uint8_t  *buffer,
                      uint32_t  length);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| cod | pointer to the object dictionary |
| key | object entry key; should be generated with `CO_DEV` |
| buffer | pointer to the source bytes |
| length | length of source buffer |

#### Returned Value

- `== CO_ERR_NONE` : successful operation
- `!= CO_ERR_NONE` : an error is detected

## Example

The following example writes the value to the hypothetical application-specific object entry "[1234:56]" within the object dictionary of the CANopen node AppNode.

```c
    CO_ERR   err;
    uint8_t  buffer[10] = { 'a','b','c','d','e','f','g','h','i', 0 };
    :
    err = CODictWrBuffer (&(Appnode.Dict), CO_DEV(0x1234, 0x56), buffer, 10);
    if (err != CO_ERR_NONE) {
        /* object [1234:56] is missing or error during writing */
    }
    :
```
