# CAN Driver

## CAN Driver Functions

The CAN driver contains a set of functions, which are called at specific locations within the CANopen stack.

| driver function | calling location                                   |
| --------------- | -------------------------------------------------- |
| `Init()`        | during the start of node and bit timing switching  |
| `Enable()`      | during the start of node and bit timing switching  |
| `Send()`        | when the node needs to send a CAN message          |
| `Read()`        | when the node processing is started                |
| `Reset()`       | when NMT resets the communication                  |
| `Close()`       | when the node is stopped and bit timing switching  |

### CAN Init

This driver function is called during the initialization phase and the bit timing switching of the CANopen stack. The function is intended to prepare the CAN controller for operation.

```c
void DrvCanInit(void);
```

The function initializes the internal driver variables for managing the CAN communication and prepares the CAN controller for operation.

!!! note

    The CAN controller must not active on the network because at this calling time the bit timing is not known.

### CAN Enable

This driver function is called during the initialization phase and the bit timing switching of the CANopen stack. The function is intended to setup the bit timing and activate the CAN controller on the network.

```c
void DrvCanEnable(uint32_t baudrate);
```

The function initializes the bit timing according to the given `baudrate` and enables the CAN communication.

!!! note

    Check the specified sample points from the CiA specifications and from your own specification. Wrong settings may result in instable networks.

### CAN Send

This driver function is called when the CANopen node needs to send a message to the CAN network. The function returns the number processed bytes `sizeof(CO_IF_FRM)` on success, `(int16_t)0` in case transmission is not possible, or `(int16_t)-1` when an error is detected.

```c
int16_t DrvCanSend(CO_IF_FRM *frm);
```

!!! note

    You must ensure that the messages on the CAN bus keeps the order. Check the behavior of your CAN controller when you want to use multiple message buffers for a queued transmission.

### CAN Read

This driver function is called when the CANopen node processing is started. This function is intended to receive all messages from the CAN network. The function returns the number processed bytes `sizeof(CO_IF_FRM)` on success, `(int16_t)0` in case of no reception, or `(int16_t)-1` when an error is detected.

```c
int16_t DrvCanRead(CO_IF_FRM *frm);
```

!!! note

    You must ensure that the messages are processed in the same order as they arrived from the CAN bus. Check the behavior of your CAN controller when you want to use multiple message buffers for a queued reception.

!!! attention

    For evaluation, demonstration, or testing purpose this CAN read function may poll for a new CAN frame. In this special case, the additional return value with no received CAN frame is possible. Don't use the polling mode in production; you should use interrupt-driven CAN communication (ideally with some kind of CAN frame queueing). The CAN polling is not suitable for robust CANopen communication.

### CAN Reset

This driver function is called when the CANopen node needs to perform a communication reset. This function is intended to clear all errors from the CAN controller bus statistics and (re-)establishes the CAN communication.

```c
void DrvCanReset(void);
```

### CAN Close

This driver function is called when the CANopen node is stopped (e.g. with the API function `CONodeStop()`). This function is intended to remove the node from the CAN bus.

```c
void DrvCanClose(void);
```

## CAN Driver Integration

During the design of the driver interface for usage with the CANopen stack, we want to decouple the CANopen library from the driver implementation. At the same time, we want to keep the overall usage as easy as possible.

The solution for this requirement is the implementation of the CAN driver function as static functions within a single file and an allocated interface structure of type `CO_IF_CAN_DRV`:

```c
#include "co_if.h"
   :
static void    DrvCanInit   (void);
static void    DrvCanEnable (uint32_t baudrate);
static int16_t DrvCanSend   (CO_IF_FRM *frm);
static int16_t DrvCanRead   (CO_IF_FRM *frm);
static void    DrvCanReset  (void);
static void    DrvCanClose  (void);
   :
const CO_IF_CAN_DRV <MyDeviceDriverName>CanDriver = {
    DrvCanInit,
    DrvCanEnable,
    DrvCanRead,
    DrvCanSend,
    DrvCanReset,
    DrvCanClose
};
```

With this kind of implementation, the usage is simply the import of the interface structure as an external symbol, and we are ready to go:

```c
  :
extern const CO_IF_CAN_DRV <MyDeviceDriverName>CanDriver;
  :
```

To keep the drivers organized, we put the implementation in a file which follows the naming convention: `co_can_<my-device-driver-name>.c`, and the declaration of the external symbol in the corresponding header file `co_can_<my-device-driver-name>.h`.

### CAN Driver Template

The files `/driver/source/co_can_dummy.c` and `/driver/include/co_can_dummy.h` are templates for CAN drivers.
