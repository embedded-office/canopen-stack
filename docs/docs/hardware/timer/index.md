---
layout: articles
title: Hardware Timer
sidebar:
  nav: docs
articles:
  data_source: site.api_timer
  type: brief
  show_excerpt: true
---

<div class="article__content" markdown="1">

  ---

  This area describes the API functions of the CANopen timer module. These functions are implemented within the source file: `co_tmr.c/h`

  <!--more-->

  The class `CO_TMR` is defined within `co_tmr.h` and is responsible for the CANopen highspeed timer management. The following data members are in this class:

  | Data Member | Type | Description |
  | --- | --- | --- |
  | Node | `CO_NODE*` | pointer to parent node object |
  | Max | `uint32_t` | maximum nunber of timed actions |
  | APool[] | `CO_TMR_ACTION` | timer action pool array |
  | TPool[] | `CO_TMR_TIME` | timer event pool array |
  | Acts | `CO_TMR_ACTION*` | pointer to root of free actions linked list |
  | Free | `CO_TMR_TIME*` | pointer to root of free events linked list |
  | Use | `CO_TMR_TIME*` | pointer to root of used events linked list |
  | Time | `uint32_t` | time ticks of next event since creation |
  | Delay | `uint32_t` | time ticks of next event from now |
  
  Note: The data within this structure must never be manipulated without the corresponding class member functions. This can lead to unpredictable behavior of the node.

</div>
