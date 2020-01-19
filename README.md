
# CANopen Stack

This is an implementation of the CANopen Specification CiA 301 version 4.2.0. The source code is compliant to C99 and you must cross-compile the source files within your project with your cross compiler of your choice.

*Note: The hardware dependent part (the CAN driver) is not included. This part is most likely provided by the chip vendor of your device. We just need to integrate this drivers in small interface functions.*


## Features

The CANopen stack implementation supports all mandatory services and many of the optional services, too. Even very advanced features like multiple independent CANopen stacks on multiple CAN busses of a single device are possible.

- Unlimited number of SDO servers
  - Expedited transfers
  - Segmented transfers
  - Block transfers
- Unlimited number of TPDO and RPDOs
  - Synchronized operation
  - Asynchronous operation
  - Manufacturer specific operation
- Emergency producer and consumer
  - Manufacturer specific extensions
  - Unlimited error history
- Heartbeat producer and consumer
- Parameter storage
  - Parameter groups
  - Storage media read and write hooks
- Network Management consumer
- Unlimited number of entries in object dictionary
  - Static or dynamic object dictionary
  - Data types: signed/unsigned 8/16/32-Bit integer, string, domain and user-types
- Extension: Layer Setting Services (CiA 305)
- Usable with or without an real-time operating system (RTOS)
- Included high precision timer management

*Note: the term 'unlimited' means, that the implementation introduces no additional limit. There are technical limits, described in the specification (for example: up to 127 possible TPDOs)*


## Usage

### Integrate sourcecode in your project

Get the repository and add the directory `/stack/src` to your project sourcefiles and the directories `stack/inc` and `stack/cfg` to your include searchpath. You should be able to compile your project without an error.

### Connect with CAN controller

You need to write 5 interface functions to get access to your CAN controller. There are a couple of interface functions existing. Just ask for support of your Mikrocontroller.

*TODO: Show how to use CMSIS drivers*

### Connect with hardware timer

The high precision timer management needs a single hardware timer with a selected interrupt rate between 100us and 10ms. This is the accuracy of the measured and used timing. So keep in mind which interrupt rate makes sense in your project.

*TODO: Show how to set this up with CMSIS*

### Specify your CANopen device

The CANopen stack is configured with a specification structure and an object dictionary. Fill the specification structure with your configuration constants, memory areas and default values. Call the CANopen initialization function for setting up all the internal references and structures.


# Contribution

## CANopen Stack Development Environment

The development environmnet for the CANopen stack library takes place on a windows machine with MSVC compiler and a hardware independent CANopen test environment.

Remember: For usage in embedded systems, the source code needs to get recompiled with the cross-compiler of your choice. Good practise is the generation, testing and release of a static library with your cross-compiler for usage in your project.


### Tools for CANopen Stack Development Environment

- [Visual Studio 2019 Build Tools](https://visualstudio.microsoft.com/de/downloads) - this is the free C compiler for Windows, which includes the build tools like `CMake`, `Ninja` and `Make`

- [Visual Studio Code](https://code.visualstudio.com/download) - this is a highly flexible and Open Source editor for coding. You may use your prefered coding editor, too.

- [Git for Windows](https://gitforwindows.org) - the popular version control management is required when you want to contribute.


### Setting up the CANopen Stack Development Environment

- [Tutorial](https://code.visualstudio.com/docs/cpp/config-msvc) for the setup of Visual Studio Code with MSVC. 

- Recommended: A few Visual Studio Code Extensions can be installed using the extension marketplace of VS Code. Follow the recommended steps during the installation process of the tools.
  - [CMake Tools](https://github.com/microsoft/vscode-cmake-tools): In order to give VS Code knowlegde about the language CMake, it's also recommended to install the "CMake language support for Visual Studio Code".  
  - [GitLens](https://marketplace.visualstudio.com/items?itemName=eamodio.gitlens) simplifies the handling of git-branches in Visual Studio Code.

- Starting VS Code: To start VS Code with the correct environment settings simply navigate to your project folder in the "Developer Command Prompt for VS" and type `code .`. Note: Users of VS Code insiders need to type `code-insiders .`.
