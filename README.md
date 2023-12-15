
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
- Fastscan slave service

*Note: the term 'unlimited' means, that the implementation introduces no additional limit. There are technical limits, described in the specification (for example: up to 511 possible TPDOs)*

## Usage

With version 4.4, the CANopen Stack project introduces an ecosystem which supports you in managing target specific applications. This support uses multiple repositories for different aspects of the embedded software project setup:

- **[cmake-scripts](https://github.com/embedded-office/cmake-scripts)** - this repository is responsible for the embedded toolchains and the component package management.
- **[canopen-stack](https://github.com/embedded-office/canopen-stack)** - this repository represents the platform independent CANopen stack component.

### Embedded Target: STM32F7 series

- **[canopen-stm32f7xx](https://github.com/embedded-office/canopen-stm32f7xx)** - this repository contains a complete Quickstart example setup for the device `STM32F769`.
- **[STM32CubeF7](https://github.com/embedded-office/STM32CubeF7)** - this fork of the ST Microelectronics HAL package is integrated into the CMake build system and packaged with minimal required source files to get the ST HAL/LL drivers working (No middleware and documentation).

### Embedded Target: STM32F4 series

- **[canopen-stm32f4xx](https://github.com/embedded-office/canopen-stm32f4xx)** - this repository contains a complete Quickstart example setup for the device `STM32F446`.
- **[STM32CubeF4](https://github.com/embedded-office/STM32CubeF4)** - this fork of the ST Microelectronics HAL package is integrated into the CMake build system and packaged with minimal required source files to get the ST HAL/LL drivers working (No middleware and documentation).

## Add Component in CMake (recommended)

The build system is realized with CMake and the CPM.cmake package management. See [cmake-scripts](https://github.com/embedded-office/cmake-scripts) for details. Adding the CANopen Stack into your project is done during the configuration phase of the build environment. During this phase, the CANopen Stack is fetched in the defined version and is available for usage.

```cmake
set(CO_TARGET   "canopen-stack")
set(CO_PROJECT  "embedded-office/canopen-stack")
set(CO_VERSION  "4.4.0")
CPMAddPackage(
  NAME    ${CO_TARGET}
  URL     https://github.com/${CO_PROJECT}/releases/download/v${CO_VERSION}/${CO_TARGET}-src.zip
  VERSION ${CO_VERSION}
)
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

The development environment for the CANopen stack takes place on the host machine. In release 4.4.0, we have decided to switch from the Windows-only MSVC compiler to the LLVM compiler. The main reason for this decision is that the same compiler is available for Windows, Linux, and Mac OS users.

### Development Tools

Download and install these free tools for your system:

- Install the build tools [Cmake](https://cmake.org/)
- Install the build system [Ninja](https://ninja-build.org/)
- Install the static checker [cppcheck](http://cppcheck.net/)

#### Linux and Mac

- Install the compiler [LLVM](https://clang.llvm.org/)

#### Windows

- Install the [Build Tools for Visual Studio 2019](https://my.visualstudio.com/Downloads?q=Build%20Tools%20for%20Visual%20Studio%202019) with the folowing components selected:

  - MSVC v142 - VS 2019 C++

  - Windows 10 SDK

  - C++-CLang-Tools for Windows

  *Note: Add the clang.exe binary path to the path environment variable so cmake can find it (On my machine: `%ProgramFiles(x86)%\Microsoft Visual Studio\2019\BuildTools\VC\Tools\Llvm\bin`)*

## Run the Test Applications

The application is build with CMake using the provided presets:

```bash
# configure the project build environment for your host system
$ cmake --preset llvm-host
# build the library for your host and all test applications
$ cmake --build --preset debug

# execute all test applications
$ ctest --preset all
```
