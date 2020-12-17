---
layout: article
title: COParaRestore()
sidebar:
  nav: docs
---

This function is responsible for removing the changes on the parameter values of the given parameter group.

<!--more-->

## Description

The changes in the NVM of the given parameter group will be replaced with the default values by calling the user callback function `CO_ParaDefault()`.

### Prototype

```c
void COParaRestore(CO_PARA *pg, CO_NODE *node);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| pg | pointer to parameter group |
| node | pointer to parent node |

#### Returned Value

- none
