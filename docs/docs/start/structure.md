---
layout: article
title: Structure
sidebar:
  nav: docs
aside:
  toc: true
---

## Layout of Repository

The following description explains the directories within this repository.

```
  root
  +- canopen        : --- CANopen Stack source code ---
  |  +- config      : configuration files
  |  +- include     : include files
  |  +- source      : C source files
  +- docs           : --- dcoumentation ---
  |  +- _api_xxx    : collection of API function descriptions
  |  +- _examples   : collection of example descriptions
  |  +- docs        : static documentation pages
  |  +- ...         : jekyll structure for generating project page
  +- testsuite      : --- testsuite ---
  |  +- app         : test application
  |  +- driver      : CAN simulation driver
  |  +- testfrm     : test framework
  |  +- tests       : tests for CANopen Stack
```

When using the CANopen Stack, you just need the `canopen` directory tree. All other directories are for managing the development, testing and documentation of the CANopen Stack.

### Directory: canopen

This directory tree contains the embedded source code of the CANopen Stack. The intended purpose is to create a library with your cross-compiler with all source files in the directory `source` and a specific configuration, defined in the file `co_cfg.h`:

```
  - CO_SDOS_N   : maximum number of possible SDO servers (default: 1)
  - CO_TPDO_N   : maximum number of possible TPDOs (default: 4)
  - CO_RPDO_N   : maximum number of possible RPDOs (default: 4)
  - CO_EMCY_N   : maximum number of possible emergency codes (default: 32)
```

### Directory: docs

This directory tree contains the complete documentation, which is presented as a GitHub project page. The project page is automatically generated and deployed, when there is a push into the `docs` directory tree in the master branch.

The README within the docs directory explains how to setup a local environment for writing the documentation with immediate feedback on saving a change. Thanks Jekyll for this nice feature!

### Directory: testsuite

This directory tree contains a test application running on host and checking the behavior of the CANOpen Stack with a simulated CAN interface.

The README within the repository root explains how to setup a local development environment, which is able to run all tests after enhancements.
