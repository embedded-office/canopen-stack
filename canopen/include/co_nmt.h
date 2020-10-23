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

#ifndef CO_NMT_H_
#define CO_NMT_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_obj.h"
#include "co_if.h"
#include "co_err.h"

/******************************************************************************
* PuBLIC INCLUDES
******************************************************************************/

#define CO_NMT_ALLOWED   0x01    /*!< indication of NMT transfers allowed    */
#define CO_BOOT_ALLOWED  0x02    /*!< indication of BOOT transfers allowed   */
#define CO_EMCY_ALLOWED  0x04    /*!< indication of EMCY transfers allowed   */
#define CO_TIME_ALLOWED  0x08    /*!< indication of TIME transfers allowed   */
#define CO_SYNC_ALLOWED  0x10    /*!< indication of SYNC transfers allowed   */
#define CO_SDO_ALLOWED   0x20    /*!< indication of SDO transfers allowed    */
#define CO_PDO_ALLOWED   0x40    /*!< indication of PDO transfers allowed    */

#define CO_THB_PROD  ((CO_OBJ_TYPE *)&COTNmtHbProd)  /*!< Heartbeat Producer */
#define CO_THB_CONS  ((CO_OBJ_TYPE *)&COTNmtHbCons)  /*!< Heartbeat Consumer */

/******************************************************************************
* PUBLIC CONSTANTS
******************************************************************************/

/*! \brief OBJECT TYPE HEARTBEAT CONSUMER
*
*    This object type specializes the general handling of objects for the
*    object dictionary entry 0x1016. This entries is designed to provide
*    the heartbeat consumer monitor times.
*/
extern const CO_OBJ_TYPE COTNmtHbCons;

/*! \brief OBJECT TYPE HEARTBEAT PRODUCER
*
*    This object type specializes the general handling of objects for the
*    object dictionary entry 0x1017. This entries is designed to provide
*    the heartbeat producer cycle time.
*/
extern const CO_OBJ_TYPE COTNmtHbProd;

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

struct CO_NODE_T;             /* Declaration of canopen node structure       */
struct CO_HBCONS_T;           /* Declaration of heartbeat consumer structure */

/*! \brief RESET TYPE
*
*    This enumeration holds all possible reset types.
*/
typedef enum CO_NMT_RESET_T {
    CO_RESET_NODE = 0,           /*!< reset application (and communication)  */
    CO_RESET_COM,                /*!< reset communication                    */
    CO_RESET_NUM                 /*!< number of reset types                  */

} CO_NMT_RESET;

/*! \brief DEVICE MODE
*
*    This enumeration holds all possible CANopen device modes.
*/
typedef enum CO_MODE_T {
    CO_INVALID = 0,              /*!< device in INVALID mode                 */
    CO_INIT,                     /*!< device in INIT mode                    */
    CO_PREOP,                    /*!< device in PRE-OPERATIONAL mode         */
    CO_OPERATIONAL,              /*!< device in OPERATIONAL mode             */
    CO_STOP,                     /*!< device in STOP mode                    */
    CO_MODE_NUM                  /*!< number of device modes                 */

} CO_MODE;

/*! \brief NMT MANAGEMENT
*
*    This structure holds all data, which are needed for the NMT state
*    machine management.
*/
typedef struct CO_NMT_T {
    struct CO_NODE_T   *Node;    /*!< ptr to parent CANopen node info        */
    struct CO_HBCONS_T *HbCons;  /*!< The used heartbeat consumer chain      */
    enum CO_MODE_T      Mode;    /*!< NMT mode of this node                  */
    int16_t             Tmr;     /*!< heartbeat producer timer identifier    */
    uint8_t             Allowed; /*!< encoding of allowed CAN objects        */

} CO_NMT;

/*! \brief HEARTBEAT CONSUMER STRUCTURE
*
*    This structure holds all data, which are needed for the heartbeat
*    consumer handling within the object dictionary.
*/
typedef struct CO_HBCONS_T {
    struct CO_NODE_T   *Node;    /*!< Link to parent node                    */
    struct CO_HBCONS_T *Next;    /*!< Link to next consumer in active chain  */
    CO_MODE             State;   /*!< Received Node-State                    */
    int16_t             Tmr;     /*!< Timer Identifier                       */
    uint16_t            Time;    /*!< Time   (Bit00-15 when read object)     */
    uint8_t             NodeId;  /*!< NodeId (Bit16-23 when read object)     */
    uint8_t             Event;   /*!< Event Counter                          */

} CO_HBCONS;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*! \brief  RESET DEVICE
*
*    This function resets the CANopen device with the given type.
*
* \param nmt
*    reference to NMT structure
*
* \param type
*    the requested NMT reset type.
*/
void CONmtReset(CO_NMT *nmt, CO_NMT_RESET type);

/*! \brief  SET CURRENT MODE
*
*    This function sets the requested CANopen NMT state machine mode.
*
* \param nmt
*    reference to NMT structure
*
* \param mode
*    the requested NMT mode.
*/
void CONmtSetMode(CO_NMT *nmt, CO_MODE mode);

/*! \brief  GET CURRENT MODE
*
*    This function returns the current CANopen NMT state machine mode.
*
* \param nmt
*    reference to NMT structure
*
* \retval  >0    The current NMT mode
* \retval  =0    An error is detected
*/
CO_MODE CONmtGetMode(CO_NMT *nmt);

/*! \brief  SET NODE-ID
*
*    This function sets the requested CANopen Node-ID within the NMT module.
*
* \note
*    The following error are detected within this function:
*    - CO_ERR_NMT_MODE: the CANopen device is not in INIT mode
*    - CO_ERR_BAD_ARG: the given nodeId is invalid (e.g. zero)
*    
* \note
*    If one of these errors is detected, this function call will change 
*    nothing.
*
* \param nmt
*    reference to NMT structure
*
* \param nodeId
*    the requested NMT node ID
*/
void CONmtSetNodeId(CO_NMT *nmt, uint8_t nodeId);

/*! \brief  GET NODE-ID
*
*    This function returns the current CANopen Node-ID of the NMT module.
*
* \param nmt
*    reference to NMT structure
*
* \retval  =0    An error is detected
* \retval  >0    The current NMT node ID
*/
uint8_t CONmtGetNodeId(CO_NMT *nmt);

/*! \brief  DECODE NODE-STATE
*
*    This function returns the CANopen mode to the given heartbeat state
*    encoding.
*
* \param code
*    heartbeat state 
*
* \retval  =CO_INVALID    An error is detected
* \retval  >0             The corresponding NMT heartbeat state
*/
CO_MODE CONmtModeDecode(uint8_t code);

/*! \brief  ENCODE NODE-STATE
*
*    This function returns the heartbeat state code for the given CANopen
*    mode.
*
* \param mode
*    CANopen mode
*
* \return
*     The corresponding NMT heartbeat state code.
*/
uint8_t CONmtModeEncode(CO_MODE mode);

/*! \brief  GET NUMBER OF HEARTBEAT EVENTS
*
*    This function retuns the number of heartbeat (miss-)events, which are
*    detected since the last call of this function or initializing the node.
*
* \param nmt
*    reference to NMT structure
*
* \param nodeId
*    node ID of monitored node (or 0 for master node)
*
* \retval  >=0    number of detected heartbeat events for given node ID
* \retval   <0    error detected (e.g. node ID is not monitored)
*/
int16_t CONmtGetHbEvents(CO_NMT *nmt, uint8_t nodeId);

/*! \brief  GET LAST RECEIVED HEARTBEAT STATE
*
*    This function returns the last received heartbeat state of a given node.
*
* \param nmt
*    reference to NMT structure
*
* \param nodeId
*    node ID of monitored node
*
* \retval  !=CO_INVALID    last detected heartbeat state for given node ID
* \retval   =CO_INVALID    error detected (e.g. node ID is not monitored)
*/
CO_MODE CONmtLastHbState(CO_NMT *nmt, uint8_t nodeId);

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/*! \brief  NMT SLAVE INITIALIZATION
*
*    This function initializes the CANopen NMT environment.
*
* \param nmt
*    reference to NMT structure
*
* \param node
*    pointer to parent node structure
*/
void CONmtInit(CO_NMT *nmt, struct CO_NODE_T *node);

/*! \brief  BOOTUP EVENT
*
*    This function performs the bootup protocol to the configured CAN bus.
*    This protocol is used to signal that a NMT slave has entered the node
*    state PRE-OPERATIONAL after the state INITIALISING.
*
* \param nmt
*    reference to NMT structure
*/
void CONmtBootup(CO_NMT *nmt);

/*! \brief  NMT MESSAGE CHECK
*
*    This functions checks a received frame to be a NMT message.
*
* \param nmt
*    reference to NMT structure
*
* \param frm
*    received CAN frame
*
* \retval  =0    check function successful
* \retval  <0    mesage is not an NMT message
*/
int16_t CONmtCheck(CO_NMT *nmt, CO_IF_FRM *frm);

/*! \brief  HEARTBEAT PRODUCER INITIALIZATION
*
*    This function initializes the CANopen heartbeat producer.
*
* \param nmt
*    reference to NMT structure
*/
void CONmtHbProdInit(CO_NMT *nmt);

/*! \brief  HEARTBEAT PROTOCOL
*
*    This function is a (Timerevent-)callback function. The heartbeat
*    message will be generated and sent to the configured CAN bus.
*
* \param parg
*    reference to NMT structure
*/
void CONmtHbProdSend(void *parg);

/*! \brief  HEARTBEAT CONSUMER INITIALIZATION
*
*    This function initializes the CANopen heartbeat consumer.
*
* \param nmt
*    reference to NMT structure
*/
void CONmtHbConsInit(CO_NMT *nmt);

/*! \brief  HEARTBEAT CONSUMER ACTIVATION
*
*    This function activates a single heartbeat consumer.
*
* \param nmt
*    reference to NMT structure
*
* \param hbc
*    reference to heartbeat consumer structure
*
* \retval   =CO_ERR_NONE    successfull activated, or consumer is deactivated
*                           by command
* \retval  !=CO_ERR_NONE    error detected (double activation, timer delete
*                           problem)
*/
CO_ERR CONmtHbConsActivate(CO_NMT    *nmt, 
                           CO_HBCONS *hbc,
                           uint16_t   time,
                           uint8_t    nodeid);

/*! \brief  HEARTBEAT CONSUMER CHECK
*
*    This function checks a received CAN frame against all heartbeat
*    consumers and increments the receive counter in the specific
*    consumer monitor.
*
* \note
*    We allow the nodeID 0 for heartbeat consuming, because CANopen master
*    heartbeat may come with this nodeID.
*
* \param frm
*    reference to CAN frame structure
*
* \param nmt
*    pointer to network management structure
*
* \retval   <0    CAN message is not a matching heartbeat message
* \retval  >=0    CAN message is a matching (and consumed) heartbeat message.
*                 The return value is equal to the consumer node-ID
*/
int16_t CONmtHbConsCheck(CO_NMT *nmt, CO_IF_FRM *frm);

/*! \brief  HEARTBEAT CONSUMER TIMEOUT
*
*    This timer callback function checks that at least one received heartbeat 
*    is detected for this heartbeat consumer.
*
* \param parg
*    heartbeat consumer structure
*/
void CONmtHbConsMonitor(void *parg);

/*! \brief  WRITE HEARTBEAT CONSUMER CONFIG
*
*    This function is a typed object write function, special for object
*    entry 1016. This entry defines the expected rate of the heartbeat
*    message of a specific node.
*
* \param obj
*    ptr to accessed object entry
*
* \param node
*    reference to parent node
*
* \param buf
*    ptr to write value
*
* \param size
*    size of write value
*
* \retval  CO_ERR_NONE       heartbeat consumer config is written
* \retval  CO_ERR_TYPE_WR    an error is detected and function aborted
*/
CO_ERR COTypeNmtHbConsWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t size);

/*! \brief READ HEARTBEAT CONSUMER CONFIG
*
*    This function is a typed object read function, special for object
*    entry 1016. This entry defines the expected rate of the heartbeat
*    message of a specific node.
*
* \param obj
*    ptr to accessed object entry
*
* \param node
*    reference to parent node
*
* \param buf
*    ptr to write value
*
* \param len
*    size of write value
*
* \retval   =CO_ERR_NONE    Successfully operation
* \retval  !=CO_ERR_NONE    An error is detected
*/
CO_ERR COTypeNmtHbConsRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t len);

/*! \brief  WRITE HEARTBEAT PRODUCER TIME
*
*    This function is a typed object write function, special for object
*    entry 1017. This entry defines the refresh rate of the heartbeat message.
*
* \param obj
*    ptr to accessed object entry
*
* \param node
*    reference to parent node
*
* \param buf
*    ptr to write value
*
* \param size
*    size of write value
*
* \retval   >0    heartbeat cycle time write successful
* \retval  <=0    an error is detected and function aborted
*/
CO_ERR COTypeNmtHbProdWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t size);

/******************************************************************************
* CALLBACK FUNCTIONS
******************************************************************************/

/*! \brief NMT MODE CHANGE CALLBACK
*
*    This function is called when the NMT mode is changed.
*
* \note
*    The nmt object pointer is checked to be valid before calling this
*    function.
*
* \param nmt
*    reference to NMT structure
*
* \param mode
*    the new mode
*/
extern void CONmtModeChange(CO_NMT *nmt, CO_MODE mode);

/*! \brief HEARTBEAT CONSUMER EVENT CALLBACK
*
*    This function is called when a heartbeat consumer monitor timer
*    elapses, before receiving the corresponding heartbeat message.
*
* \note
*    The node pointer is checked to be valid before calling this function.
*
* \param nmt
*    reference to NMT structure
*
* \param nodeId
*    The nodeId of the missed heartbeat message
*/
extern void CONmtHbConsEvent(CO_NMT *nmt, uint8_t nodeId);

/*! \brief HEARTBEAT CONSUMER STATE CHANGE CALLBACK
*
*    This function is called when a heartbeat consumer monitor detects a
*    state change, of a monitored node.
*
* \note
*    The node pointer is checked to be valid before calling this function.
*
* \param nmt
*    reference to NMT structure
*
* \param nodeId
*    The nodeId of the monitored node
*
* \param mode
*    The new received node state of the monitored node
*/
extern void CONmtHbConsChange(CO_NMT *nmt, uint8_t nodeId, CO_MODE mode);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_NMT_H_ */
