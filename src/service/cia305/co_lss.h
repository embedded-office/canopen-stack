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

#ifndef CO_LSS_H_
#define CO_LSS_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_if.h"

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

#define CO_LSS_MAX_SID           21       /*!< number of LSS services        */
#define CO_LSS_MAX_BAUD          10       /*!< number of standard baudrates  */

#define CO_LSS_RX_ID           2021       /*!< LSS request identifier        */
#define CO_LSS_TX_ID           2020       /*!< LSS response identifier       */

#define CO_LSS_WAIT            0x01       /*!< waiting mode                  */
#define CO_LSS_CONF            0x02       /*!< configuration mode            */
#define CO_LSS_EXIT            0x04       /*!< exit LSS slave state machine  */

#define CO_LSS_STORED          0x10       /*!< flag: configuration is stored */

#define CO_LSS_CMD_WAIT           0
#define CO_LSS_CMD_CONF           1

#define CO_LSS_RES_SEL_OK        68
#define CO_LSS_RES_SLAVE         79
#define CO_LSS_RES_UNCONF        80

#define CO_LSS_SEL_VENDOR         0
#define CO_LSS_SEL_PRODUCT        1
#define CO_LSS_SEL_REVISION       2
#define CO_LSS_SEL_SERIAL         3
#define CO_LSS_SEL_ACTIVE         4

#define CO_LSS_REM_VENDOR        10
#define CO_LSS_REM_PRODUCT       11
#define CO_LSS_REM_REVISION_MIN  12
#define CO_LSS_REM_REVISION_MAX  13
#define CO_LSS_REM_SERIAL_MIN    14
#define CO_LSS_REM_SERIAL_MAX    15

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

struct CO_NODE_T;              /* Declaration of canopen node structure      */

/*! \brief LSS SLAVE HANDLING
*
*    This structure contains all needed data to handle the CANOpen LSS slave.
*/
typedef struct CO_LSS_T {
    struct CO_NODE_T *Node;          /* link to parent CANopen node          */
    uint32_t          CfgBaudrate;   /* buffered baudrate config for storage */
    int16_t           Tmr;           /* timer for baudrate activation        */
    uint8_t           CfgNodeId;     /* buffered node ID config for storage  */
    uint8_t           Mode;          /* mode of layer setting service slave  */
    uint8_t           Step;          /* LSS address selection step           */
    uint8_t           Flags;         /* event flags                          */

} CO_LSS;

typedef int16_t(*CO_LSS_SERVICE)(CO_LSS *, CO_IF_FRM *);

typedef struct CO_LSS_MAP_T {
    uint8_t        Request;        /* received LSS command specifier         */
    uint8_t        Allowed;        /* allowed service in bitmasked LSS modes */
    CO_LSS_SERVICE Service;        /* service function                       */

} CO_LSS_MAP;

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/*! \brief LSS INITIALIZATION
*
*    This function initializes the LSS object.
*/
void COLssInit(CO_LSS *lss, struct CO_NODE_T *node);

/*! \brief LSS MESSAGE CHECK
*
*    This functions checks a received frame to be a LSS message. If this is
*    a valid LSS frame, the frame will be processed and replaced by the
*    corresponding response frame.
*
* \param frm
*    The CAN frame
*
* \retval  =0    no LSS request
* \retval  <0    ignored LSS request
* \retval  >0    successful processed LSS request
*/
int16_t COLssCheck(CO_LSS *lss, CO_IF_FRM *frm);

int16_t COLssSwitchStateGlobal(CO_LSS *lss, CO_IF_FRM *frm);
int16_t COLssSwitchStateSelective_Vendor(CO_LSS *lss, CO_IF_FRM *frm);
int16_t COLssSwitchStateSelective_Product(CO_LSS *lss, CO_IF_FRM *frm);
int16_t COLssSwitchStateSelective_Revision(CO_LSS *lss, CO_IF_FRM *frm);
int16_t COLssSwitchStateSelective_Serial(CO_LSS *lss, CO_IF_FRM *frm);
int16_t COLssActivateBitTiming(CO_LSS *lss, CO_IF_FRM *frm);
int16_t COLssConfigureBitTiming(CO_LSS *lss, CO_IF_FRM *frm);
int16_t COLssConfigureNodeId(CO_LSS *lss, CO_IF_FRM *frm);
int16_t COLssStoreConfiguration(CO_LSS *lss, CO_IF_FRM *frm);
int16_t COLssInquireAddress_Vendor(CO_LSS *lss, CO_IF_FRM *frm);
int16_t COLssInquireAddress_Product(CO_LSS *lss, CO_IF_FRM *frm);
int16_t COLssInquireAddress_Revision(CO_LSS *lss, CO_IF_FRM *frm);
int16_t COLssInquireAddress_Serial(CO_LSS *lss, CO_IF_FRM *frm);
int16_t COLssInquireNodeId(CO_LSS *lss, CO_IF_FRM *frm);
int16_t COLssIdentifyRemoteSlave_Vendor(CO_LSS *lss, CO_IF_FRM *frm);
int16_t COLssIdentifyRemoteSlave_Product(CO_LSS *lss, CO_IF_FRM *frm);
int16_t COLssIdentifyRemoteSlave_RevMin(CO_LSS *lss, CO_IF_FRM *frm);
int16_t COLssIdentifyRemoteSlave_RevMax(CO_LSS *lss, CO_IF_FRM *frm);
int16_t COLssIdentifyRemoteSlave_SerMin(CO_LSS *lss, CO_IF_FRM *frm);
int16_t COLssIdentifyRemoteSlave_SerMax(CO_LSS *lss, CO_IF_FRM *frm);
int16_t COLssNonConfiguredRemoteSlave(CO_LSS *lss, CO_IF_FRM *frm);

/******************************************************************************
* CALLBACK FUNCTIONS
******************************************************************************/

/*! \brief LSS CONFIGURATION STORE CALLBACK
*
*    This function is called with new configuration data, which is set by
*    the LSS service. If a configuration data should stay unchanged, the
*    argument is 0.
*
* \note
*    The CAN frame pointer is checked to be valid before calling this
*    function.
*
* \param baudrate
*    The configured baudrate for storage
*
* \param nodeId
*    The configured node id for storage
*
* \retval  =CO_ERR_NONE   configuration stored
* \retval !=CO_ERR_NONE   error is detected
*/
extern CO_ERR COLssStore(uint32_t baudrate, uint8_t nodeId);

/*! \brief LSS CONFIGURATION LOAD CALLBACK
*
*    This function is called during reset communication to load the stored
*    LSS configuration. If no storage for a configuration data is found,
*    the referenced argument should stay unchanged.
*
* \note
*    The CAN frame pointer is checked to be valid before calling this
*    function.
*
* \param baudrate
*    Reference to the baudrate, which should be set to storage value
*
* \param nodeId
*    Reference to the baudrate, which should be set to storage value
*
* \retval  =CO_ERR_NONE   configuration stored
* \retval !=CO_ERR_NONE   error is detected
*/
extern CO_ERR COLssLoad(uint32_t *baudrate, uint8_t *nodeId);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_LSS_H_ */
