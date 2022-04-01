# NVM Driver

## NVM Driver Functions

The NVM driver contains a set of functions, which are called at specific locations within the CANopen stack.

| driver function | calling location                                     |
| --------------- | ---------------------------------------------------- |
| `Init()`        | during the start of node                             |
| `Read()`        | during the start of node and parameter load requests |
| `Write()`       | when parameter storage is requested                  |

### NVM Init

This driver function is called during the startup phase of the CANopen stack. The function is intended to initialize the driver internal variables and setup the non-volatile memory communication.

```c
void DrvNvmInit(void);
```

### NVM Read

This driver function is called during the startup phase of the CANopen stack. The function is intended to read stored data out of the non-volatile memory from the address `start` with a given `size` and copy them into the given `buffer`. The function returns the number of successfully read bytes.

```c
uint32_t DrvNvmRead(uint32_t start, uint8_t *buffer, uint32_t size);
```

### NVM Write

This driver function is called during the storing data in the CANopen stack. The function is intended to write data given by the given `buffer` into the non-volatile memory starting at the address `start` with a given `size`. The function returns the number of successfully written bytes.

```c
uint32_t DrvNvmWrite(uint32_t start, uint8_t *buffer, uint32_t size);
```

## NVM Driver Reference

During the design of the driver interface for usage with the CANopen stack, we want to decouple the CANopen library from the driver implementation. At the same time, we want to keep the overall usage as easy as possible.

The solution for this requirement is the implementation of the NVM driver function as static functions within a single file and an allocated interface structure of type `CO_IF_NVM_DRV`:

```c
#include "co_if.h"
   :
static void     DrvNvmInit (void);
static uint32_t DrvNvmRead (uint32_t start, uint8_t *buffer, uint32_t size);
static uint32_t DrvNvmWrite(uint32_t start, uint8_t *buffer, uint32_t size);
   :
const CO_IF_NVM_DRV <MyDeviceDriverName>NvmDriver = {
  DrvNvmInit,
  DrvNvmRead,
  DrvNvmWrite
};
```

With this kind of implementation, the usage is simply the import of the interface structure as an external symbol, and we are ready to go:

```c
  :
extern const CO_IF_NVM_DRV <MyDeviceDriverName>NvmDriver;
  :
```

To keep the drivers organized, we put the implementation in a file which follows the naming convention: `co_nvm_<my-device-driver-name>.c`, and the declaration of the external symbol in the corresponding header file `co_nvm_<my-device-driver-name>.h`.

### NVM Driver Template

The files `/driver/source/co_nvm_dummy.c` and `/driver/include/co_nvm_dummy.h` are templates for NVM drivers.
