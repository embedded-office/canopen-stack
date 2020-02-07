---
layout: articles
title: Parameter Storage
sidebar:
  nav: docs
articles:
  data_source: site.api_parameter
  type: brief
  show_excerpt: true
---

<div class="article__content" markdown="1">

  ---

  This area describes the API functions of the CANopen parameter module. These functions are implemented within the source file: `co_para.c/h`

  <!--more-->

  The class `CO_PARA` is defined within `co_para.h` and is responsible for the CANopen parameter group handling. The following data members are in this class:

  | Data Member | Type | Description |
  | --- | --- | --- |
  | Size | `uint32_t` | size of parameter memory block |
  | Start | `uint8_t*` | pointer to parameter memory block |
  | Default | `uint8_t*` | pointer to default parameter block |
  | Type | `CO_NMT_RESET` | parameter group reset type |
  | Ident | `void*` | pointer to user identification code |
  | Value | `uint32_t` | value, when reading this parameter group |
  
  Note: The data within this structure must never be manipulated without the corresponding class member functions. This can lead to unpredictable behavior of the node.

</div>
