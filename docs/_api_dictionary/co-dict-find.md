---
layout: article
title: CODictFind()
sidebar:
  nav: docs
---

This function searches the given key within the given object dictionary.

<!--more-->

## Description

The function is often called from the application to get an object entry. If an object entry is not found, this generates no stack error.

### Prototype

```c
CO_OBJ *CODictFind(CO_DICT *cod, uint32_t key);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| cod | pointer to the object dictionary |
| key | object entry key; should be generated with `CO_DEV` |

#### Returned Value

- `>0` : pointer to identified object entry
- `=0` : object entry is not found

## Example

The following example checks the existence of the mandatory object entry "[1017:00] - Heartbeat" within the object dictionary of the CANopen node AppNode.

```c
    CO_OBJ *obj;
    :
    obj = CODictFind (&(Appnode.Dict), CO_DEV(0x1017, 0x00));
    if (obj == 0) {
        /* object [1017:00] is missing */
    } else {
        /* object [1017:00] is available */
    }
    :
```

Note: When using a static object dictionary, you can store and reuse the pointer to the identified object entry with the [Object Entry](/docs/api/object) access functions. The pointer to the object entry will NOT change during runtime in a static object dictionary.
