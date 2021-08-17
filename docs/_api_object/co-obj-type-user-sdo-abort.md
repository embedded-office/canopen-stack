---
layout: article
title: COObjTypeUserSDOAbort()
sidebar:
  nav: docs
---

This function allows user defined SDO Abort codes when implementing user type object entries.

<!--more-->

## Description

This function allows the definition of user defined SDO abort codes within user type function implementations.

### Prototype

```c
void COObjTypeUserSDOAbort(CO_OBJ *obj,
                           CO_NODE *node,
                           uint32_t abort);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| obj | pointer to object entry |
| node | pointer to parent node |
| abort | user defined abort code |

## Example

The following write-function for the type `COTDemo` send the user defined SDO Abort code "0x01020304" on a write access to object entries with this user type.

```c
const CO_OBJ_TYPE COTDemo = { 0, 0, 0, DemoWrite };

#define CO_TDEMO ((CO_OBJ_TYPE*)&COTDemo)

int16_t DemoWrite(CO_OBJ *obj, struct CO_NODE_T *node, void *buf, uint32_t size)
{
  /* define the SDO Abort code */
  COObjTypeUserSDOAbort(obj, node, 0x01020304);

  /* indicate an write error */
  return CO_ERR_TYPE_WR;
}
```

Note: The CiA standard doesn't reserve or allow any manufacturer specific SDO Abort code regions. Therefore, use this function with care.
