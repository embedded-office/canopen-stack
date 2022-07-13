
# Free CANopen Stack

This project is a free implementation of the CANopen protocol according to the free specification **CiA 301**. You need to register at [Can in Automation (CiA)](https://www.can-cia.org/) to retrieve your copy of the specification.

The source code is compliant to the C99 standard and you must cross-compile the source files as part of your project with the cross-compiler of your choice.

*Note: The source code of this project is independent from the CAN controller and microcontroller hardware. The hardware specific parts are called drivers. For a full featured CANopen stack, we need the drivers for hardware timer, CAN controller and a non-volatile storage media.*

## Features

**General**

- Usable with or without a real-time operating system (RTOS)
- Software timer management

**CiA 301 - CANopen application layer and communication profile**

- Unlimited number of SDO servers which supports:
  - Expedited transfers
  - Segmented transfers
  - Block transfers
- Unlimited number of SDO clients which supports:
  - Expedited transfers
  - Segmented transfers
- Unlimited number of TPDO and RPDOs which supports:
  - Synchronized operation
  - Asynchronous operation
  - Manufacturer specific operation
- Unlimited number of entries in object dictionary
  - Static or dynamic object dictionary
  - Data types: signed/unsigned 8/16/32-Bit integer, string, domain and user-types
- Unlimited number of parameter groups for parameter storage
- Emergency producer which supports:
  - Manufacturer specific extensions
  - Unlimited error history
- Unlimited Emergency consumers
- SYNC producer
- Unlimited number of SYNC consumers
- Heartbeat producer
- Unlimited number of Heartbeat consumers
- Network Management consumer

**CiA 305 - Layer Setting Services (LSS)**

- Baudrate configuration
- NodeId configuration

*Note: the term 'unlimited' means, that the implementation introduces no additional limit. There are technical limits, described in the specification (for example: up to 511 possible TPDOs)*

## Usage

### Integrate sourcecode to your project

Get the project repository and add to your include search path:
- `canopen/config`
- `canopen/include`

and to your source path:
- `canopen/source`

Select the driver templates for CAN, NVM and Timer you want as starting point for your hardware interface and add them to your project:
- `driver/include`
- `driver/source`

*Note: in future versions, we want to remove the pre-compiler configuration file in `canopen/config`. The corresponding configuration will be possible in a more flexible way. The main goal is to get a CANopen library for a specific microcontroller, with no application specific configuration included and therefore usable in all applications.*

### Hardware interfaces

The project contains a driver layer for required hardware interfaces. In this repository no real hardware specific drivers are included. The repository contains a template device (`dummy`) as a starting point for your own drivers and a driver for simulated device (`sim`) for CAN and NVM (used by the test suite) and a device for software based cyclic timer (`swcycle`) for the test suite.

| Interface | Component                                   |
| --------- | ------------------------------------------- |
| can       | CAN contoller communication channel         |
| timer     | Hardware timer or cyclic interrupt source   |
| nvm       | Non-volatile memory storage media           |

#### Connect hardware drivers

For selecting the hardware interfaces you need to include the header file of one specific driver module and use the matching global interface variable. The naming convention for the include file is `co_<interface>_<device>.c/h` and for the matching interface variable `<Device><Interface>Driver`. Therefore, as an example for connecting the STM32F429 microcontroller (`stm32f429`) for all interfaces leads us to:

```c
#include "co_can_stm32f429.h"
#include "co_timer_stm32f429.h"
#include "co_nvm_stm32f429.h"

const CO_IF_DRV MyDriver = {
  Stm32f429CanDriver,
  Stm32f429TimerDriver,
  Stm32f429NvmDriver
};
```

#### Integrate timer service

How to integrate the timer management depends on the timer driver and is explained in the corresponding documentation. For the software cycle timer, we need to call the timer service function periodically. The service function of the timer management checks if a timer event is elapsed and coordinates the CANopen software timers.
- call the function `COTmrService()` periodically
- ensure the calling frequency you define in member `TmrFreq` of the node specification

The processing of all actions related to a timer event is performed with the process function of the timer management. This execution can take place within the timer interrupt service handler, or within a task of your RTOS.
- call the function `COTmrProcess()` where you want to process the timed actions

#### Integrate storage media

Most likely some parameter should be stored in non-volatile memory. For the factory default values the CANopen stack needs a callback function.
- provide `COParaDefault()` to set the factory default values for the object dictionary parameters

### Setup your CANopen node

The CANopen node is configured with global data structures:

#### Define CANopen Dictionary

The CANopen dictionary is an array of object entries, which we can allocate statically:

```c
const CO_OBJ MyDict[MY_DICT_LEN] = {
    /* setup application specific dictionary, example entry: */
    { CO_KEY(0x1000, 0, CO_UNSIGNED32|CO_OBJ_D__R_), 0, (CO_DATA)(0u) },
    /* : */
};
```

#### Define Emergency Table

The emergency code table maps the application emergency codes to the corresponding bits in the mandatory error register.

```c
const CO_EMCY MyEmcyTbl[MY_EMCY_TBL_LEN] = {
    /* setup application specific error codes, example entry: */
    { MY_OVER_VOLTAGE_ERROR_CODE, CO_EMCY_REG_VOLTAGE },
    /* : */
}
```

#### Initialize CANopen Node

Fill the specification structure with your configuration constants, memory areas and default values and call the CANopen initialization function for setting up all the internal references and structures.

```c
CO_NODE myNode;

void foo(void)
{
    CO_NODE_SPEC spec;

    spec.NodeId   = 1u;
    spec.Baudrate = 250000u;
    spec.Dict     = &MyDict;
    spec.DictLen  = MY_DICT_LEN;
    spec.EmcyCode = &MyEmcyTbl;
    spec.TmrMem   = &MyTmrMem[0];
    spec.TmrNum   = 16u;
    spec.TmrFreq  = 1000u;
    spec.Drv      = &MyDriver;
    spec.SdoBuf   = &MySdoMem[0][0];

    CONodeInit (&myNode, &spec);
}
```

# History

The first release of this CANopen stack is back in 2005. It is still used in many CANopen nodes from small startup companies up to big players in the automation market. Since Embedded Office sells an OEM license to Micrium to provide the CANopen stack as a part of the uC/ product line, we maintain the CANopen stack for the Flexible Safety RTOS and bare metal usage in parallel.

Some years later, now in 2020, we think it is time for a new way of software development of components where no product specific know-how is neccessary. This project is the try with the hope, that this way of software development is good for existing customers, for Embedded Offic and for all potential new users.

To avoid confusion, it is the best to continue with the release version numbering. The first stable release of the open-source variant of the CANopen Stack is: **V4.0.0**

# License

The Apache 2.0 license is suitable for commercial usage, so we think this is the best for this free component. If you have problems or concerns with this license, just contact us at Embedded Ofice. We will help you to get the legal approvals within your company.

# Code of Conduct

As everywhere in the world (especially in the internet) and at every time, we think a respectful and open minded communication is essential for peaceful and innovative developments. Please have a look in our [Code of Conduct](.github/CODE_OF_CONDUCT.md) and think about your writing before submitting.

# Contribution

## Issues and Questions

Feel free to write bug reports, questions or and feedback as issue within this github repository.

## Development Environment

The development environment for the CANopen stack takes place on our local windows machine with the free MSVC compiler and the hardware independent test framework.

*Remember: For usage in embedded systems, the source code needs to get recompiled with the cross-compiler of your choice. Good practise is the generation, testing and release of a static library with your cross-compiler for usage in your project.*

### Required Tools

Download and install these free tools:

- [Visual Studio 2019 Build Tools](https://visualstudio.microsoft.com/de/downloads) - the free C compiler for Windows, which includes the used build tools `CMake`, `Ninja` and `Make`

- [Visual Studio Code](https://code.visualstudio.com/download) - this is my editor for coding. You can use your prefered coding editor without trouble.

Perform the configuration and setup for the environment by following the nice [Tutorial](https://code.visualstudio.com/docs/cpp/config-msvc).
