---
layout: article
title: COParaStore()
sidebar:
  nav: docs
---

This function is responsible for the storing activities of the given parameter group.

<!--more-->

## Description

The whole parameter group will be stored in NVM by calling the NVM driver function for writing data.

### Prototype

```c
void COParaStore(CO_PARA *pg, CO_NODE *node);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| pg | pointer to parameter group |
| node | pointer to parent node |

#### Returned Value

- none
