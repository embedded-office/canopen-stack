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

#ifndef CO_HB_CONS_H_
#define CO_HB_CONS_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"
#include "co_err.h"
#include "co_obj.h"
#include "co_nmt.h"

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

#define CO_THB_CONS  ((const CO_OBJ_TYPE *)&COTNmtHbCons)

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

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
* PUBLIC CONSTANTS
******************************************************************************/

/*! \brief OBJECT TYPE HEARTBEAT CONSUMER
*
*    This object type specializes the general handling of objects for the
*    object dictionary entry 0x1016. This entries is designed to provide
*    the heartbeat consumer monitor times.
*/
extern const CO_OBJ_TYPE COTNmtHbCons;

/******************************************************************************
* PROTECTED API FUNCTION
******************************************************************************/

int16_t CONmtHbConsCheck(CO_NMT *nmt, CO_IF_FRM *frm);

/******************************************************************************
* PUBLIC API FUNCTION
******************************************************************************/

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
* \retval   =CO_ERR_NONE    successfull activated, or consumer is deactivated by command
* \retval  !=CO_ERR_NONE    error detected (double activation, timer delete problem)
*/
CO_ERR CONmtHbConsActivate(CO_HBCONS *hbc, uint16_t time, uint8_t nodeid);

/******************************************************************************
* CALLBACK FUNCTIONS
******************************************************************************/

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

#endif  /* #ifndef CO_HB_CONS_H_ */
