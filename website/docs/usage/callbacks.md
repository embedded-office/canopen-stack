# Callback Interface

## Application Callback Functions

This chapter gives an overview of the component Callback Interface. The listed functions are available for programming the application-specific reactions on several events of the CANopen device.


### CONodeFatalError()

This **mandatory** function is called, after detecting a fatal error within the stack, and no way out of the situation (*panic*). The function is intended to allow the implementation of a pre-defined shutdown sequence and setting the device in a safe state.

```c
void CONodeFatalError(void)
{
  /* Place here your fatal error handling.
   * There is most likely a programming error.
   * !! Please don't ignore this errors. !!
   */
  for (;;);
}
```


### COTmrLock()

These **mandatory** function pair is called during timer management and is the right place to add a locking mechanism for the timer management structure. The simplest implementation is disabling and re-enabling the timer hardware interrupt. In more advanced scenarios there could be a timer mutex or spin-lock, depending on the RTOS and number of active cores.

```c
void COTmrLock(void)
{
  /* This function helps to guarantee the consistancy
   * of the internal timer management while interrupted
   * by the used timer interrupt. Most likely you need
   * at this point on of the following mechanisms:
   * - disable the used hardware timer interrupt
   * - get a 'timer-mutex' from your RTOS (ensure to
   *   call COTmrService() in a timer triggered task)
   */
}

void COTmrUnlock(void)
{
  /* This function helps to guarantee the consistancy
   * of the internal timer management while interrupted
   * by the used timer interrupt. Most likely you need
   * at this point on of the following mechanisms:
   * - (re)enable the used hardware timer interrupt
   * - release the 'timer-mutex' from your RTOS (ensure
   *   to call COTmrService() in a timer triggered task)
   */
}
```


### COIfCanReceive()

This **optional** function is called for each CAN frame, which is not consumed (processed) by the CANopen stack. The CAN frame pointer is checked to be valid before the CANopen stack calls this function.

```c
void COIfCanReceive(CO_IF_FRM *frm)
{
  /* Optional: place here some code, which is called
   * when you need to handle CAN messages, which are
   * not part of the CANopen protocol.
   */
}
```

**Arguments**

| Parameter | Description            |
| --------- | ---------------------- |
| frm       | The received CAN frame |


### COLssStore()

This **optional** function is called with new configuration data, which is set by the LSS service. If the configuration data should stay unchanged, the argument is 0. The CAN frame pointer is checked to be valid before the CANopen stack calls this function.

```c
int16_t COLssStore(uint32_t baudrate, uint8_t nodeId)
{
  /* Optional: place here some code, which is called
   * when LSS client is in use and the CANopen node
   * needs to store updated values.
   */
  return (0u);
}
```

**Arguments**

| Parameter | Description                         |
| --------- | ----------------------------------- |
| baudrate  | The configured baudrate for storage |
| nodeId    | The configured node id for storage  |

**Returned Value**

- `==0` : configuration stored
- `!=0` : error is detected


### COLssLoad()

This **optional** function is called during reset communication to load the stored LSS configuration. If no storage for the configuration data is found, the referenced argument should stay unchanged. The CAN frame pointer is checked to be valid before the CANopen stack calls this function.

```c
int16_t COLssLoad(uint32_t *baudrate, uint8_t *nodeId)
{
  /* Optional: place here some code, which is called
   * when LSS client is in use and the CANopen node
   * is initialized.
   */
  return (0u);
}
```

**Arguments**

| Parameter | Description                                                     |
| --------- | --------------------------------------------------------------- |
| baudrate  | Reference to the baudrate, which should be set to storage value |
| nodeId    | Reference to the node-id, which should be set to storage value  |

**Returned Value**

- `==0` : configuration loaded
- `!=0` : error is detected


### CONmtModeChange()

This **optional** function is called when the NMT mode is changed. The nmt object pointer is checked to be valid before the CANopen stack calls this function.

```c
void CONmtModeChange(CO_NMT *nmt, CO_MODE mode)
{
  /* Optional: place here some code, which is called
   * when a NMT mode change is initiated.
   */
}
```

**Arguments**

| Parameter | Description                |
| --------- | -------------------------- |
| nmt       | reference to NMT structure |
| mode      | the new mode               |


### CONmtResetRequest()

This **optional** function is called when the NMT reset is requested via CAN network. The nmt object pointer is checked to be valid before the CANopen stack calls this function.

```c
void CONmtResetRequest(CO_NMT *nmt, CO_NMT_RESET reset)
{
  /* Optional: place here some code, which is called
   * when a NMT reset is requested by the network.
   */
}
```

**Arguments**

| Parameter | Description                |
| --------- | -------------------------- |
| nmt       | reference to NMT structure |
| reset     | the requested reset type   |


### CONmtHbConsEvent()

This **optional** function is called when a heartbeat consumer monitor timer elapses, before receiving the corresponding heartbeat message. The node pointer is checked to be valid before the CANopen stack calls this function.

```c
void CONmtHbConsEvent(CO_NMT *nmt, uint8_t nodeId)
{
  /* Optional: place here some code, which is called
   * called when heartbeat consumer is in use and
   * detects an error on monitored node(s).
   */
}
```

**Arguments**

| Parameter | Description                                |
| --------- | ------------------------------------------ |
| nmt       | reference to NMT structure                 |
| nodeId    | The nodeId of the missed heartbeat message |


### CONmtHbConsChange()

This **optional** function is called when a heartbeat consumer monitor detects a state change, of a monitored node. The node pointer is checked to be valid before the CANopen stack calls this function.

```c
void CONmtHbConsChange(CO_NMT *nmt, uint8_t nodeId, CO_MODE mode)
{
  /* Optional: place here some code, which is called
   * when heartbeat consumer is in use and detects a
   * NMT state change on monitored node(s).
   */
}
```

**Arguments**

| Parameter | Description                                       |
| --------- | ------------------------------------------------- |
| nmt       | reference to NMT structure                        |
| nodeId    | The nodeId of the monitored node                  |
| mode      | The new received node state of the monitored node |


### COPdoTransmit()

This **optional** function is called just before the PDO transmission will send the PDO message frame to the CANopen network.

```c
void COPdoTransmit(CO_IF_FRM *frm)
{
  /* Optional: place here some code, which is called
   * just before a PDO is transmitted. You may adjust
   * the given CAN frame which is send afterwards.
   */
}
```

**Arguments**

| Parameter | Description                  |
| --------- | ---------------------------- |
| frm       | Pointer to PDO message frame |


### COPdoReceive()

This **optional** function is called just before the PDO reception will distribute the PDO message frame into the object dictionary. This callback function is able to *consume* the PDO message frame, e.g. the distribution into the object dictionary will be skipped. Furthermore without *consuming* the PDO message frame, this function could modify the received data before distribution takes place.

```c
int16_t COPdoReceive(CO_IF_FRM *frm)
{
  /* Optional: place here some code, which is called
   * right after receiving a PDO. You may adjust
   * the given CAN frame which is written into the
   * object dictionary afterwards or suppress the
   * write operation.
   */
  return(0u);
}
```

**Arguments**

| Parameter | Description                           |
| --------- | ------------------------------------- |
| frm       | Pointer to received PDO message frame |

**Returned Value**

- `=0` : CAN message frame is not consumed
- `>0` : CAN message frame is consumed


### COPdoSyncUpdate()

This **optional** function is called after a synchronized PDO is received and the data is distributed into the object dictionary.

```c
void COPdoSyncUpdate(CO_RPDO *pdo)
{
  /* Optional: place here some code, which is called
   * right after the object dictionary update due to
   * a synchronized PDO.
   */
}
```

**Arguments**

| Parameter | Description                                        |
| --------- | -------------------------------------------------- |
| pdo       | Pointer to received and synchronous processed RPDO |


### COParaDefault()

This **optional** function will be called during restoring the default values of a parameter group. The function is responsible for setting the factory defaults in the current parameter group memory. The parameter group info pointer is checked to be valid before the CANopen stack calls this function.

```c
int16_t COParaDefault(CO_PARA *pg)
{
  /* Optional: place here some code, which is called
   * when a parameter group is restored to factory
   * settings.
   */
}
```

**Arguments**

| Parameter | Description                 |
| --------- | --------------------------- |
| pg        | Ptr to parameter group info |

**Returned Value**

- `=0` : parameter default values successful set
- `<0` : error is detected and function aborted


## CORpdoWriteData()

This **optional** function is called during PDO distribution of the PDO message frame into the object dictionary when mapped data value consumes more than 4 bytes.

```c
void CORpdoWriteData(CO_IF_FRM *frm, uint8_t pos, uint8_t size, CO_OBJ *obj)
{
  /* Optional: place here some code, which is called
   * when a PDO is received with mapped values with
   * a size larger than 4 byte.
   */
}
```

**Arguments**

| Parameter | Description                                      |
| --------- | ------------------------------------------------ |
| frm       | Ptr to PDO message frame                         |
| pos       | The start index in the CAN message frame payload |
| size      | The mapped value size in bytes                   |
| obj       | Ptr to the target object entry                   |


## COTpdoReadData()

This **optional** function is called during PDO transmission when a mapped data value consumes more than 4 bytes.

```c
void COTpdoReadData(CO_IF_FRM *frm, uint8_t pos, uint8_t size, CO_OBJ *obj)
{
  /* Optional: place here some code, which is called
   * when a PDO is constructed for transmission which
   * needs a mapped values with a size larger than 4 byte.
   */
}
```

**Arguments**

| Parameter | Description                                      |
| --------- | ------------------------------------------------ |
| frm       | Ptr to PDO message frame                         |
| pos       | The start index in the CAN message frame payload |
| size      | The mapped value size in bytes                   |
| obj       | Ptr to the source object entry                   |
