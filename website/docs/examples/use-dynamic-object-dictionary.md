# Dynamic Object Dictionary - Part II

In the [first part](/examples/setup-dynamic-object-dictionary) of this example, we discover the needed activities to create and manage a dynamic CANopen Object Dictionary. With this example, we will have a look at using the dynamic CANopen Dictionary functions. We will see, how powerful these little functions are and how we can use them to get a dynamic Quickstart application. For details of the object entries, please have a look to the example [Quickstart](/start/quickstart).

## Overview

We should have a short look into the Quickstart example as well. The following list summarizes the needed Object Directory entries.

Object Entries, mandatory in CiA DS301:

- Device Type (Index 1000h)
- Error Register (Index 1001h)
- Heartbeat Time (Index 1017h)
- Identity Object (Index 1018h)
- SDO Server (Index 1200h)

Object Entries, optional in CiA DS301 but needed by our demo:

- Transmit PDO Communication (Index 1800h)
- Transmit PDO Mapping (Index 1A00h)

Manufacturer specific object entry for this demo:

- Clock Data (Index 2100h)


## Create Object Entries

First, we will take a look at all CiA DS301 object entries one by one. After getting these building blocks, we will combine them into a function, which creates our complete Object Directory.


### Device Type

The device type is a 32bit value, which identifies the CANopen device. Due to the fact, that our demo is not a real CANopen device, we store a constant 32bit dummy value (0x00000000) in this entry.

```c
ODAddUpdate(self, CO_KEY(0x1000, 0, CO_UNSIGNED32|CO_OBJ_D__R_), 0, (uintptr_t)0x00000000);
```

We choose the additional flags `CO_OBJ_D__R_` to store the data directly (`D`) in the object entry and allow read access (`R`) from the communication interface.


### Error Register

The error register is an 8bit value, which holds the error register flags. The CANopen EMGY module manages the single error register flags.

```c
ODAddUpdate(self, CO_KEY(0x1001, 0, CO_UNSIGNED32|CO_OBJ___PR_), 0, (uintptr_t)&ErrReg);
```

For this entry, we choose the additional flags `CO_OBJ___PR_` to store a pointer to the data in the object entry (`_` instead of `D`), allow the PDO mapping (`P`) and allow read access (`R`) from the communication interface.


### Heartbeat Time

The heartbeat producer is a 16bit value, which holds the time in ms between two heartbeats. The entry is a system type, which injects the heartbeat callback functions into this entry. The callback functions are responsible for the system behavior when reading or writing this entry.

```c
ODAddUpdate(self, CO_KEY(0x1017, 0, CO_UNSIGNED32|CO_OBJ____RW), CO_THEARTBEAT, (uintptr_t)&HbTime);
```

For the system type `CO_THEARTBEAT` we must choose the storage of a data pointer in the object directory (`_` instead of `D`). To get a system, which conforms to CiA DS301, we disallow the PDO mapping (`_` instead of `P`) and allow read (`R`) and write (`W`) access from the communication interface. Therefore we set the additional flags: `CO_OBJ____RW`.


### Identity Object

The identity object is a structure of detailed node information. Due to the fact, that our demo is not a real CANopen device, we store a constant 32bit dummy value (0x00000000) in the mandatory entry at Sub-index 01h.

```c
ODAddUpdate(self, CO_KEY(0x1018, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, (uintptr_t)0x01);
ODAddUpdate(self, CO_KEY(0x1018, 1, CO_UNSIGNED32|CO_OBJ_D__R_), 0, (uintptr_t)0x00000000);
```

The subindex 00h holds the highest supported subindex in this index as a constant 8bit value.


### SDO Server Function

We encapsulate the SDO server communication object entry in a separate function. This function allows us to add multiple SDO servers with a smile. The subindex 01h is the COB-ID for the SDO request. The subindex 02h holds the COB-ID for the SDO response.
Note: The CiA DS301 specifies for SDO server #0 the fixed COB-IDs:

* SDO request : COB-ID = 600h + NodeID
* SDO response : COB-ID = 580h + NodeID

For all additional SDO servers (#1 .. #127) we can specify the COB-IDs. To get full flexibility and highest possible guidance by the function, we define the following function:

```c
static void ODCreateSDOServer(OD_DYN *self, uint8_t srv, uint32_t request, uint32_t response)
{
  if (srv == 0) {
    request  = (uint32_t)0x600;
    response = (uint32_t)0x580;
  }
  ODAddUpdate(self, CO_KEY(0x1200+srv, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, (uintptr_t)0x02);
  ODAddUpdate(self, CO_KEY(0x1200+srv, 1, CO_UNSIGNED32|CO_OBJ_DN_R_), 0, (uintptr_t)request);
  ODAddUpdate(self, CO_KEY(0x1200+srv, 2, CO_UNSIGNED32|CO_OBJ_DN_R_), 0, (uintptr_t)response);
}
```

We choose the additional flags for the request and response identifiers to allow the storage of the identifiers independent of the used CANopen node ID. The node ID may change over time, and we don't want to update all depending object entries. Well, we can realize this by enabling the node ID flag `N` in the flags to remove the current node ID while writing and adding the current node ID while reading the object entry.

### TPDO Communication Function

Now we want to setup the transmit PDO (TPDO) communication object entry with a similar service function. This static PDO communication index array contains powerful settings. For details on the influence on the transmission behavior of every single subindex, please have a look into the CiA DS301 specification.

```c
static void ODCreateTPDOCom(OD_DYN *self, uint8_t num, uint32_t id, uint8_t type, uint16_t inhibit, uint16_t evtimer)
{
  ODAddUpdate(self, CO_KEY(0x1800+num, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, (uintptr_t)0x05);
  ODAddUpdate(self, CO_KEY(0x1800+num, 1, CO_UNSIGNED32|CO_OBJ_DN_R_), 0, (uintptr_t)id);
  ODAddUpdate(self, CO_KEY(0x1800+num, 2, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, (uintptr_t)type);
  ODAddUpdate(self, CO_KEY(0x1800+num, 3, CO_UNSIGNED16|CO_OBJ_D__RW), 0, (uintptr_t)inhibit);
  ODAddUpdate(self, CO_KEY(0x1800+num, 5, CO_UNSIGNED16|CO_OBJ_D__RW), CO_TEVENT, (uintptr_t)evtimer);
}
```

Note, that the subindex 04h is missing in this array. This gab is specified in the CiA DS301 for backward compatibility reasons. Furthermore, the subindex 00h states the highest accessible subindex (05h) and *NOT* the number of entries. This definition is a small pitfall when building this object entry.

### TPDO Mapping Function

The PDO mapping for the TPDOs needs an array with the mapping information as an argument to our service function. The mapping information is specified in CiA DS301 and is very similar to the key information of an object entry:

* Index : 16bit value (0000h to FFFFh)
* Subindex : 8bit value (0 to 255)
* Width : 8bit value with the number of bits (e.g., 8, 16 or 32)

For building this mapping information, we can use the well known CO_KEY(..) macro where we use the 3rd argument with the number of bits.

```c
static void ODCreateTPdoMap(OD_DYN *self, uint8_t num, uint32_t *map, uint8_t len)
{
  uint8_t n;

  ODAddUpdate(self, CO_KEY(0x1A00+num, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, (uintptr_t)len);
  for (n = 0; n < len; n++) {
    ODAddUpdate(self, CO_KEY(0x1A00+num, 1+n, CO_UNSIGNED32|CO_OBJ_D__R_), 0, (uintptr_t)map[n]);
  }
}
```

This function will work correctly, but you can improve it with some guidance checks. The CANopen stack supports the number of bits 8, 16, and 32. Other values are not supported. Furthermore, the sum of all bits could be checked to be lower or equal to 64 bits.

## Create Directory Function

Now we are ready for building our dynamic object directory for the quickstart project smartly and efficiently:

```c
static uint8_t  Obj1001_00_08;
static uint32_t Obj2100_01_20;
static uint8_t  Obj2100_02_08;
static uint8_t  Obj2100_03_08;
  :
static void ODCreateDict(OD_DYN *self)
{
  uint32_t map[3];

  Obj1001_00_08 = 0;

  ODAddUpdate(self, CO_KEY(0x1000, 0, CO_UNSIGNED32|CO_OBJ_D__R_), 0, (uintptr_t)0x00000000);
  ODAddUpdate(self, CO_KEY(0x1001, 0, CO_UNSIGNED8 |CO_OBJ___PR_), 0, (uintptr_t)&Obj1001_00_08);
  ODAddUpdate(self, CO_KEY(0x1005, 0, CO_UNSIGNED32|CO_OBJ_D__R_), 0, (uintptr_t)0x80);
  ODAddUpdate(self, CO_KEY(0x1017, 0, CO_UNSIGNED16|CO_OBJ_D__R_), 0, (uintptr_t)0);
  ODAddUpdate(self, CO_KEY(0x1018, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, (uintptr_t)4);
  ODAddUpdate(self, CO_KEY(0x1018, 1, CO_UNSIGNED32|CO_OBJ_D__R_), 0, (uintptr_t)0);
  ODAddUpdate(self, CO_KEY(0x1018, 2, CO_UNSIGNED32|CO_OBJ_D__R_), 0, (uintptr_t)0);
  ODAddUpdate(self, CO_KEY(0x1018, 3, CO_UNSIGNED32|CO_OBJ_D__R_), 0, (uintptr_t)0);
  ODAddUpdate(self, CO_KEY(0x1018, 4, CO_UNSIGNED32|CO_OBJ_D__R_), 0, (uintptr_t)0);

  ODCreateSDOServer(self, 0, CO_COBID_SDO_REQUEST(), CO_COBID_SDO_RESPONSE());

  ODCreateTPDOCom(self, 0, CO_COBID_TPDO_DEFAULT(0), 254, 0, 0);

  map[0] = CO_LINK(0x2100, 1, 32);
  map[1] = CO_LINK(0x2100, 2,  8);
  map[2] = CO_LINK(0x2100, 3,  8);
  ODCreateTPdoMap(self, 0, map, 3);

  ODAddUpdate(self, CO_KEY(0x2100, 0, CO_UNSIGNED8 |CO_OBJ_D__R_), 0, (uintptr_t)3);
  ODAddUpdate(self, CO_KEY(0x2100, 1, CO_UNSIGNED32|CO_OBJ___PR_), 0, (uintptr_t)&Obj2100_01_20);
  ODAddUpdate(self, CO_KEY(0x2100, 2, CO_UNSIGNED8 |CO_OBJ___PR_), 0, (uintptr_t)&Obj2100_02_08);
  ODAddUpdate(self, CO_KEY(0x2100, 3, CO_UNSIGNED8 |CO_OBJ___PR_), CO_TASYNC, (uintptr_t)&Obj2100_03_08);
}
```

Note, in the marked line we use the system type `CO_TASYNC` to specify the manufacturer specific trigger event "on change" for this object entry. This setting results in the transmission of any TPDO which holds a communication type 254 and a mapping entry for this signal, if the node is in operational mode. For our demo, this results in the transmission of the TPDO with the clock values every second.

## Starting the Node

For setup and starting a CANopen node, we can now integrate the functions to a startup sequence:

```c
#define APP_OBJ_N   128
    :
static  OD_DYN      DynDict;
static  CO_OBJ      ClockOD[APP_OBJ_N];
    :
struct CO_NODE_SPEC_T AppSpec = {
  APP_NODE_ID,             /* default Node-Id                */
  APP_BAUDRATE,            /* default Baudrate               */
  &ClockOD[0],             /* pointer to object dictionary   */
  APP_OBJ_N,               /* object dictionary max length   */
  &AppEmcyTbl[0],          /* EMCY code & register bit table */
  &TmrMem[0],              /* pointer to timer memory blocks */
  APP_TMR_N,               /* number of timer memory blocks  */
  APP_TICKS_PER_SEC,       /* timer clock frequency in Hz    */
  &AppDriver,              /* select drivers for application */
  &SdoSrvMem[0]            /* SDO Transfer Buffer Memory     */
};
    :
void ClkStartNode(CO_NODE *node)
{
  /* Clear all entries in object dictionary */
  ODInit(&DynDict, &ClockOD[0], APP_OBJ_N);

  /* Setup the object dictionary during runtime */
  ODCreateDict(&DynDict);

  /* Create a node with generated object dictionary */
  CONodeInit(node, &AppSpec);
}
```

This function will generate a (temporary) node specification, initializes our new dynamic object directory and initializes the CANopen node with this specification. To integrate this in the delivered quick start demo, the only line we must change is the initialization of the specified CANopen node:

```c
void main(int argc, char *argv[])
{
    :
  /* CONodeInit(&Clk, (CO_NODE_SPEC *)&AppSpec);  OLD */
  ClkStartNode(&Clk);                          /* NEW */
    :
}
```

Well, that's it.

Enjoy the new flexibility with our dynamic generated object directories.
