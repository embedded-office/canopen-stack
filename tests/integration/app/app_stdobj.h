/******************************************************************************
   Copyright 2020 Embedded Office GmbH & Co. KG

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
******************************************************************************/

#ifndef APP_STDOBJ_H_
#define APP_STDOBJ_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_core.h"

/******************************************************************************
* PUBLIC MACROS
******************************************************************************/

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1000h:0 - DEVICE TYPE
*
* \param   val
*          Constant device type with device profile number (bit0 to 15) and
*          additional information (bit16 to 31).
*/
/*---------------------------------------------------------------------------*/
#define OBJ1000_0(val)  \
    CO_KEY(0x1000, 0, CO_OBJ_D___R_), CO_TUNSIGNED32, (CO_DATA)(val)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1001h:0 - ERROR REGISTER
*
* \param   ref
*          Reference to 8bit error register variable.
*/
/*---------------------------------------------------------------------------*/
#define OBJ1001_0(ref)  \
    CO_KEY(0x1001, 0, CO_OBJ____PR_), CO_TUNSIGNED8, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1002h:0 - MANUFACTURER STATUS REGISTER
*
* \param   ref
*          Reference to 32bit manufacturer specific status register variable.
*/
/*---------------------------------------------------------------------------*/
#define OBJ1002_0(ref)  \
    CO_KEY(0x1002, 0, CO_OBJ____PR_), CO_TUNSIGNED32, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1003h:0 - NUMBER OF ERRORS
*
* \param   refNum
*          Reference to 8bit variable with number of actual errors.
*/
/*---------------------------------------------------------------------------*/
#define OBJ1003_0(ref)  \
    CO_KEY(0x1003, 0, CO_OBJ_____RW), CO_TEMCY_HIST, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1003h:n - STANDARD ERROR FIELD
*
* \param   sub
*          Subindex for this standard error field (01h to FEh)
*
* \param   ref
*          Reference to standard error field with error code (bit0 to 15)
*          and additional information (bit16 to 31).
*/
/*---------------------------------------------------------------------------*/
#define OBJ1003_X(sub,ref)  \
    CO_KEY(0x1003, (sub), CO_OBJ_____R_), CO_TEMCY_HIST, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1005h:0 - COB-ID SYNC MESSAGE
*
* \param   ref
*          Reference to value of 32bit COB-ID with CAN-ID (bit0 to 10).
*/
/*---------------------------------------------------------------------------*/
#define OBJ1005_0(ref)  \
    CO_KEY(0x1005, 0, CO_OBJ_____RW), CO_TSYNC_ID, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1006h:0 - COMMUNICATION CYCLE PERIOD
*
* \param   ref
*          Reference to period value defines the SYNC interval in us.
*/
/*---------------------------------------------------------------------------*/
#define OBJ1006_0(ref)  \
    CO_KEY(0x1006, 0, CO_OBJ_____RW), CO_TSYNC_CYCLE, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1014h:0 - COB-ID EMCY MESSAGE
*
* \param   ref
*          Reference to value of 32bit COB-ID with CAN-ID (bit0 to 10).
*
* \note    The node-id is considered when using this object entry
*/
/*---------------------------------------------------------------------------*/
#define OBJ1014_0(ref)  \
    CO_KEY(0x1014, 0, CO_OBJ__N__RW), CO_TEMCY_ID, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1017h:0 - PRODUCER HEARTBEAT TIME
*
* \param   ref
*          Reference to value of 16bit heartbeat time in 1 ms.
*/
/*---------------------------------------------------------------------------*/
#define OBJ1017_0(ref)  \
    CO_KEY(0x1017, 0, CO_OBJ_____RW), CO_THB_PROD, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1018h:0 - IDENTITY OBJECT
*
* \param   val
*          Constant value for highest sub-index (1h to 4h).
*/
/*---------------------------------------------------------------------------*/
#define OBJ1018_0(val)  \
    CO_KEY(0x1018, 0, CO_OBJ_D___R_), CO_TUNSIGNED8, (CO_DATA)(val)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1018h:1 - IDENTITY OBJECT VENDOR-ID
*
* \param   val
*          Constant vendor-id (assigned uniquely to manufacturers by CiA)
*/
/*---------------------------------------------------------------------------*/
#define OBJ1018_1(val)  \
    CO_KEY(0x1018, 1, CO_OBJ_D___R_), CO_TUNSIGNED32, (CO_DATA)(val)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1018h:2 - IDENTITY OBJECT PRODUCT CODE
*
* \param   val
*          Constant product code
*/
/*---------------------------------------------------------------------------*/
#define OBJ1018_2(val)  \
    CO_KEY(0x1018, 2, CO_OBJ_D___R_), CO_TUNSIGNED32, (CO_DATA)(val)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1018h:3 - IDENTITY OBJECT REVISION NUMBER
*
* \param   val
*          Constant revision number
*/
/*---------------------------------------------------------------------------*/
#define OBJ1018_3(val)  \
    CO_KEY(0x1018, 3, CO_OBJ_D___R_), CO_TUNSIGNED32, (CO_DATA)(val)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1018h:3 - IDENTITY OBJECT SERIAL NUMBER
*
* \param   val
*          Constant serial number
*/
/*---------------------------------------------------------------------------*/
#define OBJ1018_4(val)  \
    CO_KEY(0x1018, 4, CO_OBJ_D___R_), CO_TUNSIGNED32, (CO_DATA)(val)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1200h:0 - SDO SERVER PARAMETER
*
* \param   srv
*          Constant value for SDO server (0 to 127)
*
* \param   val
*          Constant value for highest sub-index (2h to 3h)
*/
/*---------------------------------------------------------------------------*/
#define OBJ120X_0(srv,val)  \
    CO_KEY(0x1200 + (srv), 0, CO_OBJ_D___R_), CO_TUNSIGNED8, (CO_DATA)(val)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1200h:1 - SDO SERVER PARAMETER COB-ID CLIENT TO SERVER (RX)
*
* \param   srv
*          Constant value for SDO server (0 to 127)
*
* \param   ref
*          Reference to value of 32bit COB-ID with CAN-ID (bit0 to 10)
*
* \note    The node-id is considered when using this object entry
*/
/*---------------------------------------------------------------------------*/
#define OBJ120X_1(srv,ref)  \
    CO_KEY(0x1200 + (srv), 1, CO_OBJ__N__RW), CO_TSDO_ID, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1200h:2 - SDO SERVER PARAMETER COB-ID SERVER TO CLIENT (TX)
*
* \param   srv
*          Constant value for SDO server (0 to 127)
*
* \param   ref
*          Reference to value of 32bit COB-ID with CAN-ID (bit0 to 10)
*
* \note    The node-id is considered when using this object entry
*/
/*---------------------------------------------------------------------------*/
#define OBJ120X_2(srv,ref)  \
    CO_KEY(0x1200 + (srv), 2, CO_OBJ__N__RW), CO_TSDO_ID, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1280h:0 - SDO CLIENT PARAMETER
*
* \param   srv
*          Constant value for SDO client (0 to 127)
*
* \param   val
*          Constant value for highest sub-index (2h to 3h)
*/
/*---------------------------------------------------------------------------*/
#define OBJ128X_0(cli,val)  \
    CO_KEY(0x1200 + (cli), 0, CO_OBJ_D___R_), CO_TUNSIGNED8, (CO_DATA)(val)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1280h:1 - SDO CLIENT PARAMETER COB-ID CLIENT TO SERVER (TX)
*
* \param   cli
*          Constant value for SDO client (0 to 127)
*
* \param   ref
*          Reference to value of 32bit COB-ID with CAN-ID (bit0 to 10)
*
* \note    The subindex 3 (node-id) is considered when using this object entry
*/
/*---------------------------------------------------------------------------*/
#define OBJ128X_1(cli,ref)  \
    CO_KEY(0x1280 + (cli), 1, CO_OBJ_____RW), CO_TSDO_ID, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1280h:2 - SDO CLIENT PARAMETER COB-ID SERVER TO CLIENT (RX)
*
* \param   cli
*          Constant value for SDO client (0 to 127)
*
* \param   ref
*          Reference to value of 32bit COB-ID with CAN-ID (bit0 to 10)
*
* \note    The subindex 3 (node-id) is considered when using this object entry
*/
/*---------------------------------------------------------------------------*/
#define OBJ128X_2(cli,ref)  \
    CO_KEY(0x1280 + (cli), 2, CO_OBJ_____RW), CO_TSDO_ID, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1280h:3 - SDO CLIENT PARAMETER NODE-ID OF THE SDO SERVER
*
* \param   cli
*          Constant value for SDO client (0 to 127)
*
* \param   ref
*          Reference to value of 8bit SDO server Node-ID (1 to 127)
*/
/*---------------------------------------------------------------------------*/
#define OBJ128X_3(cli,ref)  \
    CO_KEY(0x1280 + (cli), 3, CO_OBJ_____RW), CO_TUNSIGNED8, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 14XXh:0 - RPDO COMMUNICATION PARAMETER
*
* \param   num
*          Constant value for RPDO number (0 to 511)
*
* \param   val
*          Constant value for highest sub-index (2h to 5h)
*/
/*---------------------------------------------------------------------------*/
#define OBJ14XX_0(num,val)  \
    CO_KEY(0x1400 + (num), 0, CO_OBJ_D___R_), CO_TUNSIGNED8, (CO_DATA)(val)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 14XXh:1 - RPDO COMMUNICATION PARAMETER COB-ID USED BY RPDO
*
* \param   num
*          Constant value for RPDO number (0 to 511)
*
* \param   ref
*          Reference to value of 32bit COB-ID with CAN-ID (bit0 to 10)
*
* \note    The node-id is considered when using this object entry
*/
/*---------------------------------------------------------------------------*/
#define OBJ14XX_1(num,ref)  \
    CO_KEY(0x1400 + (num), 1, CO_OBJ__N__RW), CO_TPDO_ID, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 14XXh:2 - RPDO COMMUNICATION PARAMETER TRANSMISSION TYPE
*
* \param   num
*          Constant value for RPDO number (0 to 511)
*
* \param   ref
*          Reference to value of 8bit with encoded transmission type
*/
/*---------------------------------------------------------------------------*/
#define OBJ14XX_2(num,ref)  \
    CO_KEY(0x1400 + (num), 2, CO_OBJ_____RW), CO_TPDO_TYPE, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 16XXh:0 - RPDO MAPPING PARAMETER
*
* \param   num
*          Constant value for RPDO number (0 to 511)
*
* \param   ref
*          Reference to 8bit value with number of mapped application objects
*          in RPDO
*/
/*---------------------------------------------------------------------------*/
#define OBJ16XX_0(num,ref)  \
    CO_KEY(0x1600 + (num), 0, CO_OBJ_____RW), CO_TPDO_NUM, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 16XXh:N - RPDO MAPPING PARAMETER APPLICATION OBJECT #N
*
* \param   num
*          Constant value for RPDO number (0 to 511)
*
* \param   map
*          Constant value for number of application object (1 to 8)
*
* \param   ref
*          Reference to mapping value with an object index (bit31 to 16),
*          subindex (bit15 to 8) and length in number of bits (bit7 to 0)
*/
/*---------------------------------------------------------------------------*/
#define OBJ16XX_N(num,map,ref)  \
    CO_KEY(0x1600 + (num), (map), CO_OBJ_____RW), CO_TPDO_MAP, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 18XXh:0 - TPDO COMMUNICATION PARAMETER
*
* \param   num
*          Constant value for TPDO number (0 to 511)
*
* \param   val
*          Constant value for highest sub-index (2h to 5h)
*/
/*---------------------------------------------------------------------------*/
#define OBJ18XX_0(num,val)  \
    CO_KEY(0x1800 + (num), 0, CO_OBJ_D___R_), CO_TUNSIGNED8, (CO_DATA)(val)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 18XXh:1 - TPDO COMMUNICATION PARAMETER COB-ID USED BY TPDO
*
* \param   num
*          Constant value for TPDO number (0 to 511)
*
* \param   ref
*          Reference to value of 32bit COB-ID with CAN-ID (bit0 to 10)
*
* \note    The node-id is considered when using this object entry
*/
/*---------------------------------------------------------------------------*/
#define OBJ18XX_1(num,ref)  \
    CO_KEY(0x1800 + (num), 1, CO_OBJ__N__RW), CO_TPDO_ID, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 18XXh:2 - TPDO COMMUNICATION PARAMETER TRANSMISSION TYPE
*
* \param   num
*          Constant value for TPDO number (0 to 511)
*
* \param   ref
*          Reference to 8bit value with encoded transmission type
*/
/*---------------------------------------------------------------------------*/
#define OBJ18XX_2(num,ref)  \
    CO_KEY(0x1800 + (num), 2, CO_OBJ_____RW), CO_TPDO_TYPE, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 18XXh:3 - TPDO COMMUNICATION PARAMETER INHIBIT TIME
*
* \param   num
*          Constant value for TPDO number (0 to 511)
*
* \param   ref
*          Reference to 16bit value with inhibit time in 100us
*/
/*---------------------------------------------------------------------------*/
#define OBJ18XX_3(num,ref)  \
    CO_KEY(0x1800 + (num), 3, CO_OBJ_____RW), CO_TUNSIGNED16, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 18XXh:5 - TPDO COMMUNICATION PARAMETER EVENT TIMER
*
* \param   num
*          Constant value for TPDO number (0 to 511)
*
* \param   ref
*          Reference to 16bit value with event time in 1ms
*/
/*---------------------------------------------------------------------------*/
#define OBJ18XX_5(num,ref)  \
    CO_KEY(0x1800 + (num), 5, CO_OBJ_____RW), CO_TPDO_EVENT, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1AXXh:0 - TPDO MAPPING PARAMETER
*
* \param   num
*          Constant value for TPDO number (0 to 511)
*
* \param   ref
*          Reference to 8bit value with number of mapped application objects
*          in TPDO
*/
/*---------------------------------------------------------------------------*/
#define OBJ1AXX_0(num,ref)  \
    CO_KEY(0x1A00 + (num), 0, CO_OBJ_____RW), CO_TPDO_NUM, (CO_DATA)(ref)

/*---------------------------------------------------------------------------*/
/*! \brief OBJECT 1AXXh:N - TPDO MAPPING PARAMETER APPLICATION OBJECT #N
*
* \param   num
*          Constant value for TPDO number (0 to 511)
*
* \param   map
*          Constant value for number of application object (1 to 8)
*
* \param   ref
*          Reference to mapping value with an object index (bit31 to 16),
*          subindex (bit15 to 8) and length in number of bits (bit7 to 0)
*/
/*---------------------------------------------------------------------------*/
#define OBJ1AXX_N(num,map,ref)  \
    CO_KEY(0x1A00 + (num), (map), CO_OBJ_____RW), CO_TPDO_MAP, (CO_DATA)(ref)

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif
