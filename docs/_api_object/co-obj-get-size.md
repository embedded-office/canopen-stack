---
layout: article
title: COObjGetSize()
sidebar:
  nav: docs
---

This function returns the size of the given object entry.

<!--more-->

## Description

The argument width is most likely given '0' (=unknown) when getting the size of an object entry. The intended use of this argument is for write access of dynamic sizes of user type objects (e.g. firmware loading with a maximum size).

### Prototype

```c
uint32_t COObjGetSize(CO_OBJ *obj, uint32_t width);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| obj | pointer to object entry |
| width | expected object size in byte (or 0 if unknown) |

#### Returned Value

- `>0` : object entry size in bytes
- `=0` : an error is detected

## Example

The following example gets the size of the hypothetical application-specific object entry "[1234:56]" within the object dictionary of the CANopen node AppNode.

```c
    uint32_t  size;
    CO_OBJ   *entry;
    :
    entry = CODirFind    (&(AppNode.Dir), CO_DEV(0x1234,0x56));
    size  = COObjGetSize (entry, 0);
    :
```
