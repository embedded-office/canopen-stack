# Structure

## Layout of Repository

The following description explains the directories within this repository.

```
  root
  +- canopen        : --- CANopen Stack source code ---
  |  +- config      : configuration files
  |  +- include     : include files
  |  +- source      : C source files
  +- driver         : --- driver templates ---
  |  +- include     : include files
  |  +- source      : C source files
  +- example        : --- example projects ---
  |  +- quickstart  : quickstart files
  +- testsuite      : --- testsuite ---
  |  +- app         : test application
  |  +- testfrm     : test framework
  |  +- tests       : tests for CANopen Stack
```

When using the CANopen Stack, you need the `canopen` directory tree only. All other directories are for managing the development and testing of the CANopen Stack.

### Directory: canopen

This directory tree contains the embedded source code of the CANopen Stack. The intended purpose is to create a library with your cross-compiler with all source files in the directory `source` and a specific configuration, defined in the file `co_cfg.h`:

```
mandatory settings
- CO_SSDO_N     : maximum number of possible SDO servers (default: 1)
- CO_CSDO_N     : maximum number of possible SDO clients (default: 1)
- CO_TPDO_N     : maximum number of possible TPDOs (default: 4)
- CO_RPDO_N     : maximum number of possible RPDOs (default: 4)
- CO_EMCY_N     : maximum number of possible emergency codes (default: 32)
```

```
settings for optimizing resource usage
- USE_LSS       : enable/disable support for LSS functionality (default: 1)
- USE_PARAMS    : enable/disable support for NVM parameters (default: 1)
- USE_CSDO      : enable/disable support for SDO clients (default: 1)
```

### Directory: driver

This directory tree contains the template files for your driver development (the `dummy` devices). For the testsuite, there are `sim` drivers included which simulates the hardware behavior for testing purpose.

### Directory: example

This directory tree contains example projects which you can take for inspiration and learning the stack. You may copy and adjust these projects to your needs.

### Directory: testsuite

This directory tree contains a test application running on the host and checking the behavior of the CANOpen Stack with a simulated CAN interface.

The README within the repository root explains how to setup a local development environment, which is able to run all tests after enhancements.
