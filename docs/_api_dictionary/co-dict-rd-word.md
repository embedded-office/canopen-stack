---
layout: article
title: CODictRdWord()
sidebar:
  nav: docs
---

This function reads a 16bit value from the object dictionary.

<!--more-->

## Description

The object entry is addressed with the given key and the value will be written to the given destination pointer.

### Prototype

```c
CO_ERR CODictRdWord(CO_DICT *cod, uint32_t key, uint16_t *value);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| cod | pointer to the object dictionary |
| key | object entry key; should be generated with `CO_DEV` |
| value | pointer to value destination |

#### Returned Value

- `== CO_ERR_NONE` : successful operation
- `!= CO_ERR_NONE` : an error is detected

## Example

The following example reads the current value of the hypothetical application-specific object entry "[1234:56]" within the object dictionary of the CANopen node AppNode.

```c
    CO_ERR    err;
    uint16_t  value;
    :
    err = CODictRdWord (&(Appnode.Dict), CO_DEV(0x1234, 0x56), &value);
    if (err != CO_ERR_NONE) {
        /* object [1234:56] is missing or error during reading */
    } else {
        /* value holds the content of object [1234:56] */
    }
    :
```

Note: This function uses CODictFind() on each function call. To improve access performance for multiple accesses to a single object entry, the application may use CODictFind() once and COObjRdValue() multiple times.
