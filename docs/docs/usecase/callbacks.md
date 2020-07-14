---
layout: article
title: Callback Interface
sidebar:
  nav: docs
aside:
  toc: true
---

## Application Callback Functions

This chapter gives an overview of the component Callback Interface. The listed functions are available for programming the application specific reactions on several events of the CANopen device.


### Fatal Error

This function is called, after detecting a fatal error within the stack, and no way out of the situation (*panic*). The function is intended to allow the implementation of a pre-defined shutdown sequence and setting the device in a safe state.

```c
  void CONodeFatalError(void);
```


### Interface Receive

This function is called for each CAN frame, which is not consumed (processed) by the CANopen stack. The CAN frame pointer is checked to be valid before the CANopen stack calls this function.

```c
  void COIfReceive(CO_IF_FRM *frm);
```

**Arguments**

| Parameter | Description |
| --- | --- |
| frm | The received CAN frame |


### LSS Configuration Store

This function is called with new configuration data, which is set by the LSS service. If a configuration data should stay unchanged, the argument is 0. The CAN frame pointer is checked to be valid before the CANopen stack calls this function.

```c
  int16_t COLssStore(uint32_t baudrate, uint8_t nodeId);
```

**Arguments**

| Parameter | Description |
| --- | --- |
| baudrate | The configured baudrate for storage |
| nodeId | The configured node id for storage |

**Returned Value**

- `==CO_ERR_NONE` : configuration stored
- `!=CO_ERR_NONE` : error is detected


### LSS Configuration Load

This function is called during reset communication to load the stored LSS configuration. If no storage for a configuration data is found, the referenced argument should stay unchanged. The CAN frame pointer is checked to be valid before the CANopen stack calls this function.

```c
  int16_t COLssLoad(uint32_t *baudrate, uint8_t *nodeId);
```

**Arguments**

| Parameter | Description |
| --- | --- |
| baudrate | Reference to the baudrate, which should be set to storage value |
| nodeId | Reference to the baudrate, which should be set to storage value |

**Returned Value**

- `==CO_ERR_NONE` : configuration stored
- `!=CO_ERR_NONE` : error is detected


### NMT Mode Change

This function is called when the NMT mode is changed. The nmt object pointer is checked to be valid before the CANopen stack calls this function.

```c
  void CONmtModeChange(CO_NMT *nmt, CO_MODE mode);
```

**Arguments**

| Parameter | Description |
| --- | --- |
| nmt | reference to NMT structure |
| mode | the new mode |


### Heartbeat Consumer Error Event

This function is called when a heartbeat consumer monitor timer elapses, before receiving the corresponding heartbeat message. The node pointer is checked to be valid before the CANopen stack calls this function.

```c
  void CONmtHbConsEvent(CO_NMT *nmt, uint8_t nodeId);
```

**Arguments**

| Parameter | Description |
| --- | --- |
| nmt | reference to NMT structure |
| nodeId | The nodeId of the missed heartbeat message |


### Heartbeat Consumer State Change

This function is called when a heartbeat consumer monitor detects a state change, of a monitored node. The node pointer is checked to be valid before the CANopen stack calls this function.

```c
  void CONmtHbConsChange(CO_NMT *nmt, uint8_t nodeId, CO_MODE mode);
```

**Arguments**

| Parameter | Description |
| --- | --- |
| nmt | reference to NMT structure |
| nodeId | The nodeId of the monitored node |
| mode | The new received node state of the monitored node |


### Parameter Load Values

This callback function will be called during reset and powerup events. The function is responsible for loading the parameter from NVM into the parameter group memory. The parameter group info pointer is checked to be valid before the CANopen stack calls this function.

```c
  int16_t COParaLoad(CO_PARA *pg);
```

**Arguments**

| Parameter | Description |
| --- | --- |
| pg | Ptr to parameter group info |

**Returned Value**

- `=0` : parameter loading successful
- `<0` : error is detected and function aborted


### Parameter Save Values

This callback function will be called during storing a parameter group. The function is responsible for saving the current parameter group memory into NVM. The parameter group info pointer is checked to be valid before the CANopen stack calls this function.

```c
  int16_t COParaSave(CO_PARA *pg);
```

**Arguments**

| Parameter | Description |
| --- | --- |
| pg | Ptr to parameter group info |

**Returned Value**

- `=0` : parameter loading successful
- `<0` : error is detected and function aborted


### Parameter Default Values

This callback function will be called during restoring the default values of a parameter group. The function is responsible for setting the factory defaults in the current parameter group memory. The parameter group info pointer is checked to be valid before the CANopen stack calls this function.

```c
  int16_t COParaDefault(CO_PARA *pg);
```

**Arguments**

| Parameter | Description |
| --- | --- |
| pg | Ptr to parameter group info |

**Returned Value**

- `=0` : parameter default values successful set
- `<0` : error is detected and function aborted


### PDO Transmit

This function is called just before the PDO transmission will send the PDO message frame to the CANopen network.

```c
  void COPdoTransmit(CO_IF_FRM *frm);
```

**Arguments**

| Parameter | Description |
| --- | --- |
| frm | Pointer to PDO message frame |


### PDO Receiption

This function is called just before the PDO receiption will distribute the PDO message frame into the object dictionary. This callback function is able to *consume* the PDO message frame, e.g. the distribution into the object dictionary will be skipped. Furthermore without *consuming* the PDO message frame, this function could modify the received data before distribution takes place.

```c
  int16_t COPdoReceive(CO_IF_FRM *frm);
```

**Arguments**

| Parameter | Description |
| --- | --- |
| frm | Pointer to received PDO message frame |

**Returned Value**

- `=0` : CAN message frame is not consumed
- `>0` : CAN message frame is consumed


### PDO Synchron Update

This function is called after a synchronized PDO is received and the data is distributed into the object dictionary.

```c
  void COPdoSyncUpdate(CO_RPDO *pdo);
```

**Arguments**

| Parameter | Description |
| --- | --- |
| pdo | Pointer to received and synchron processed RPDO |


### Timer Locks

These function pair is called during timer management and is the right place to add a locking mechanisms for the timer management structure. The simplest implementation is disabling and re-enabling the timer hardware interrupt. In more advanced scenarios there could be a timer mutex or spin-lock, depending on the RTOS and number of active cores.

```c
  void COTmrLock(void);
  void COTmrUnlock(void);
```
