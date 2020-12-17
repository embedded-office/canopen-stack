---
layout: article
title: CONodeInit()
sidebar:
  nav: docs
---

This function initializes the internals of the CANopen stack.

<!--more-->

## Description

The specification of the CANopen node, and the CANopen node object itself is given as a parameter.

### Prototype

```c
void CONodeInit(CO_NODE *node, CO_NODE_SPEC *spec);
```

#### Arguments

| Parameter | Description |
| --- | --- |
| node | pointer to node object |
| spec | pointer to node specification object |

#### Returned Value

- none

## Example

The following example shows the external reference to the node specification AppSpec which is typically allocated within a configuration file and the allocation of the CANopen node memory AppNode.

```c
    extern const CO_NODE_SPEC AppSpec;
                 CO_NODE      AppNode;
```

With these objects, the CANopen node can be initialized and started:

```c
    :
    CONodeInit (&AppNode, (CO_NODE_SPEC *)&AppSpec);
    :
```
