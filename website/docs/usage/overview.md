# CANopen Stack Overview

## CANopen stack actions

In the architecture we see, that the CANopen stack is the connection between the CAN network and the application. The central information store is the object dictionary which is used to control the behavior of the device, transmit and receive process data or configure runtime parameters.

In principle, all of these activities can initiated by external devices in the CAN network, or the internal CANopen application. The following chapters gives an overall of the possible scenarios.

### Autonomous external request

An *autonomous external request* is initiated by an external device and is handled by the CANopen stack according to the standard. No interaction with the application is necessary.

```mermaid
sequenceDiagram
  participant C as CAN Network
  participant N as CANopen Stack
  participant A as Application
  C->>+N: external request
  N-->>-C: response
```

*An example for this type of request is the SDO access to an object entry.*

Therefore, there is nothing to do in the application. It is important to know the CANopen standard CiA-301 to know how to configure the CANopen stack responses within the object dictionary.

### External request

An *external request* is similar to the autonomous external request with the difference that the application needs to provide a callback function to realize the application-specific behavior.

```mermaid
sequenceDiagram
    participant C as CAN Network
    participant N as CANopen Stack
    participant A as Application
    C->>+N: external request
    N->>+A: call callback function
    A-->>-N: return
    N-->>-C: response
```

*An example for this type of request is the parameter store request which uses the parameter write callback from the application.*

The Callback functions are documented in the CANopen usage category Callback:

| Category                | Content                               |
| ----------------------- | ------------------------------------- |
| [Callback Interface][1] | description of all callback functions |

### Internal request

An *internal request* is an API function call initiated by the application.

```mermaid
sequenceDiagram
  participant C as CAN Network
  participant N as CANopen Stack
  participant A as Application
  A->>+N: API function call
  N-->>-A: return
```

*An example for this type of request is the update of a value in an object entry.*

The API functions are documented in some categories within the chapter API Functions:

| Category                | Content                                                    |
| ----------------------- | ---------------------------------------------------------- |
| [CANopen Node][2]       | controlling the node (init, start, stop, etc. )            |
| [Object Dictionary][3]  | basic reading and writing in the object dictionary         |
| [EMCY Handling][4]      | handle and communicate emergency errors (set, clear, etc.) |
| [Network Management][5] | local network management (set/get modes, node-ID, etc.)    |
| [Object Entry][6]       | read and write object entries of any type and size         |
| [TPDO Event][7]         | triggering the transmission of TPDO                        |


[1]: ../callbacks
[2]: ../../api/node
[3]: ../../api/dictionary
[4]: ../../api/emergency
[5]: ../../api/network
[6]: ../../api/object
[7]: ../../api/tpdo
