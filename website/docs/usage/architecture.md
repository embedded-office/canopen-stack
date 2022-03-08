# Device Architecture

## Standard Architecture

The architecture of a CANopen device is standardized and shown in the following figure:

<figure markdown>
  ![CANopen architecture](/assets/images/illustrations/standard-architecture.svg "Standardized CANopen Architecture"){ width=640px }
</figure>

### Object Dictionary

The central component in a CANopen device is the *Object Directory (OD)*. The OD is organized as a table with object entries. Each *Entry* is addressed with a unique combination of *Index* and *SubIdx*. The index is a 16 bit value (from 0...0xFFFF), and the sub-index is a 8 bit value (from 0...0xFF). Within the CANopen standard, the layout and usage of many object entries is specified and marked as *mandatory*, *optional* or *manufacturer-specific*. The following table shows an overview of the OD areas:

| Index           | Content                             |
| --------------- | ----------------------------------- |
| 0x0000          | Reserved                            |
| 0x0001 – 0x025F | Data types                          |
| 0x0260 – 0x0FFF | Reserved                            |
| 0x1000 – 0x1FFF | Communication profile area          |
| 0x2000 – 0x5FFF | Manufacturer specific profile area  |
| 0x6000 – 0x9FFF | Standardized device profile area    |
| 0xA000 – 0xBFFF | Standardized interface profile area |
| 0xC000 – 0xFFFF | Reserved                            |

### Communication Unit

The *Communication Unit (CU)* uses the OD to set and get configuration settings, inform the device application about new received process data and transmit the device process data to the CAN network. For the different requirements of these activities, different CANopen protocol elements are specified. The CU takes care of all necessary operations to be compliant with the CANopen standard. The following table shows an overview of the CU modules and their responsibility:

| CU Module | Responsibility               |
| --------- | ---------------------------- |
| NMT       | Network Management           |
| SDO       | Service Data Object Transfer |
| PDO       | Process Data Object Transfer |
| SYNC      | Synchronization Module       |
| EMCY      | Emergency Transfer           |
| TIME      | Time Management              |

### Network Management

The *Network Management State Machine (NMT)* of all CANopen devices within a CANopen network is controlled by the CANopen network master. The following state machine is specified:

<figure markdown>
  ![NMT States](/assets/images/illustrations/nmt-statemachine.svg "NMT States"){ width=480px }
</figure>

Within this state machine, the state transitions with a number (e.g. [129]) will be activated with a network management command equal to this number. If no transition criteria is stated, the transition will be activated after finished state operations automatically. The following table summarizes the network management commands:

| NMT Command | Description                   |
|:-----------:| ----------------------------- |
| 1           | Start remote node             |
| 2           | Stop remote node              |
| 128         | Enter "Pre-Operational" state |
| 129         | Reset node                    |
| 130         | Reset communication           |

These NMT commands used by the CANopen network boot-up master to control the network. During the different states, the listed communication services are allowed:

|                   | Init | Pre-Op. | Operational | Stop |
| ----------------- |:----:|:-------:|:-----------:|:----:|
| Boot-Up Object    | X    |         |             |      |
| NMT Object        |      | X       | X           | X    |
| SDO Transfer      |      | X       | X           |      |
| SYNC Object       |      | X       | X           |      |
| Time Stamp Object |      | X       | X           |      |
| Emergency Object  |      | X       | X           |      |
| PDO Transfer      |      |         | X           |      |

Note: The Boot-Up message is sent only once during the transition from *Initialization* to *Pre-Operational*.
