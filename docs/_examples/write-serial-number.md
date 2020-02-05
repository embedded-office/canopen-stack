---
layout: article
title: Write serial number
---

In the production line, the devices should be programmed with the identical firmware image. The object entry 1018 holds optionally a serial number which sould be unique for each device. We need a one-time programmable object entry.

<!--more-->

The main idea is a manufacturer specific data type, which allows writing into a FLASH memory area if the current FLASH memory area is empty.
