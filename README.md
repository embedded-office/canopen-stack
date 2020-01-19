
# CANopen Stack

This is the development environmnet for the CANopen stack library. The development takes place on a windows machine with MSVC compiler and a hardware independent coding environment.
For usage in embedded systems, the source code needs to get recompiled with the cross-compiler of your choice. We recommend to generate a static library for the CANopen stack and use the library in your embedded project.


## Recommended Tools to Setup CANopen Stack Development Environment

- [Visual Studio 2019 Build Tools](https://visualstudio.microsoft.com/de/downloads) - this is the free C compiler for Windows, which includes the build tools like `CMake`, `Ninja` and `Make`

- [Visual Studio Code](https://code.visualstudio.com/download) - this is a highly flexible and Open Source editor for coding. You can use your prefered coding editor, too.

- [Git for Windows](https://gitforwindows.org) - the popular version control management is required when you want to contribute.


## Setting up the CANopen Stack Development Environment

- [Tutorial](https://code.visualstudio.com/docs/cpp/config-msvc) for the setup of Visual Studio Code with MSVC. 

- Recommended: A few Visual Studio Code Extensions can be installed using the extension marketplace of VS Code. Follow the recommended steps during the installation process of the tools.
  - [CMake Tools](https://github.com/microsoft/vscode-cmake-tools): In order to give VS Code knowlegde about the language CMake, it's also recommended to install the "CMake language support for Visual Studio Code".  
  - [GitLens](https://marketplace.visualstudio.com/items?itemName=eamodio.gitlens) simplifies the handling of git-branches in Visual Studio Code.

- Starting VS Code: To start VS Code with the correct environment settings simply navigate to your project folder in the "Developer Command Prompt for VS" and type `code .`. Note: Users of VS Code insiders need to type `code-insiders .`.
