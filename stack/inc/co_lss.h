/******************************************************************************
* (c) by Embedded Office GmbH & Co. KG, http://www.embedded-office.com
*------------------------------------------------------------------------------
* This file is part of CANopenStack, an open source CANopen Stack.
* Project home page is <https://github.com/MichaelHillmann/CANopenStack.git>.
* For more information on CANopen see < http ://www.can-cia.org/>.
*
* CANopenStack is free and open source software: you can redistribute
* it and / or modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation, either version 2 of the
* License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
******************************************************************************/

#ifndef CO_LSS_H_
#define CO_LSS_H_

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_if.h"

#ifdef __cplusplus
extern "C" {
#endif

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

struct CO_NODE_T;

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
void CO_LssInit(CO_LSS *lss, struct CO_NODE_T *node);

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
int16_t CO_LssCheck(CO_LSS *lss, CO_IF_FRM *frm);

int16_t CO_LssSwitchStateGlobal(CO_LSS *lss, CO_IF_FRM *frm);
int16_t CO_LssSwitchStateSelective_Vendor(CO_LSS *lss, CO_IF_FRM *frm);
int16_t CO_LssSwitchStateSelective_Product(CO_LSS *lss, CO_IF_FRM *frm);
int16_t CO_LssSwitchStateSelective_Revision(CO_LSS *lss, CO_IF_FRM *frm);
int16_t CO_LssSwitchStateSelective_Serial(CO_LSS *lss, CO_IF_FRM *frm);
int16_t CO_LssActivateBitTiming(CO_LSS *lss, CO_IF_FRM *frm);
int16_t CO_LssConfigureBitTiming(CO_LSS *lss, CO_IF_FRM *frm);
int16_t CO_LssConfigureNodeId(CO_LSS *lss, CO_IF_FRM *frm);
int16_t CO_LssStoreConfiguration(CO_LSS *lss, CO_IF_FRM *frm);
int16_t CO_LssInquireAddress_Vendor(CO_LSS *lss, CO_IF_FRM *frm);
int16_t CO_LssInquireAddress_Product(CO_LSS *lss, CO_IF_FRM *frm);
int16_t CO_LssInquireAddress_Revision(CO_LSS *lss, CO_IF_FRM *frm);
int16_t CO_LssInquireAddress_Serial(CO_LSS *lss, CO_IF_FRM *frm);
int16_t CO_LssInquireNodeId(CO_LSS *lss, CO_IF_FRM *frm);
int16_t CO_LssIdentifyRemoteSlave_Vendor(CO_LSS *lss, CO_IF_FRM *frm);
int16_t CO_LssIdentifyRemoteSlave_Product(CO_LSS *lss, CO_IF_FRM *frm);
int16_t CO_LssIdentifyRemoteSlave_RevMin(CO_LSS *lss, CO_IF_FRM *frm);
int16_t CO_LssIdentifyRemoteSlave_RevMax(CO_LSS *lss, CO_IF_FRM *frm);
int16_t CO_LssIdentifyRemoteSlave_SerMin(CO_LSS *lss, CO_IF_FRM *frm);
int16_t CO_LssIdentifyRemoteSlave_SerMax(CO_LSS *lss, CO_IF_FRM *frm);
int16_t CO_LssNonConfiguredRemoteSlave(CO_LSS *lss, CO_IF_FRM *frm);

/******************************************************************************
* CALLBACK FUNCTIONS
******************************************************************************/


/*! \brief LSS CONFIGURATION STORE CALLBACK
*
*    This function is called with new configuration data, which is set by
*    the LSS service.
*
* \note
*    This implementation is an example implementation, which will do nothing.
*    This function is optional and application specific. The function can be
*    implemented somewhere in the in the application code. The activation of
*    the application callback function is done with \ref CO_CB_LSS_STORE_EN.
*
* \note
*    When disabling the application callback function, this example
*    implementation is enabled, but not called. In fact: disabling the
*    application function will remove the callback function call in the
*    interface receive processing.
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
*/
int16_t CO_LssStore(uint32_t baudrate, uint8_t nodeId);

/*! \brief LSS CONFIGURATION LOAD CALLBACK
*
*    This function is called during reset communication to load the stored
*    LSS configuration.
*
* \note
*    This implementation is an example implementation, which will do nothing.
*    This function is optional and application specific. The function can be
*    implemented somewhere in the in the application code. The activation of
*    the application callback function is done with \ref CO_CB_LSS_STORE_EN.
*
* \note
*    When disabling the application callback function, this example
*    implementation is enabled, but not called. In fact: disabling the
*    application function will remove the callback function call in the
*    interface receive processing.
*
* \note
*    The CAN frame pointer is checked to be valid before calling this
*    function.
*
* \param baudrate
*    The configured baudrate from storage
*    
* \param nodeId
*    The configured node id from storage
*/
int16_t CO_LssLoad(uint32_t *baudrate, uint8_t *nodeId);

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_LSS_H_ */
