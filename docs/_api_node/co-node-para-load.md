---
layout: article
title: CONodeParaLoad()
sidebar:
  nav: docs
---

This function is responsible for the loading of all parameter groups with the given type.

<!--more-->

## Description

A single parameter group will be loaded from NVM by calling the nvm driver function for reading data.

This function considers all parameter groups, which are linked to the parameter store index (1010h) within the object dictionary. Every not linked parameter group is not the scope of this function and must be handled within the application.

### Prototype

```c
int16_t CONodeParaLoad(CO_NODE *node, CO_NMT_RESET type);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| node | pointer to node object |
| type | reset type, e.g. CO_RESET_COM or CO_RESET_NODE |

#### Returned Value

- `=0` : loading successful
- `<0` : an error is detected and function aborted

## Example

The function `CONodeParaLoad()` is rarely called from within the applications. Typically the CANopen master requests the parameter loading via NMT command. In some special cases it is useful to initiate a reset from the slave application. The following example shows the sequence to load all parameters, which are linked to the object directory entries related to the communication profile (index 1000h to 1FFFh).

```c
    :
    success = CONodeParaLoad (&AppNode, CO_RESET_COM);
    if (success < 0) {
        err = CONodeGetErr (&AppNode);
    }
    :
```
