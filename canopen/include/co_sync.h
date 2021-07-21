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

#ifndef CO_SYNC_H_
#define CO_SYNC_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"
#include "co_cfg.h"
#include "co_if.h"
#include "co_pdo.h"

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

#define CO_SYNC_COBID_OFF    ((uint32_t)1 << 30)    /*!< generator flag      */
#define CO_SYNC_COBID_EXT    ((uint32_t)1 << 29)    /*!< extended format     */
#define CO_SYNC_COBID_MASK   ((uint32_t)0x1FFFFFFF) /*!< identifier mask     */

#define CO_SYNC_FLG_TX    0x01    /*!< message type indication  TPDO         */
#define CO_SYNC_FLG_RX    0x02    /*!< message type indication: RPDO         */

#define CO_TSYNCID ((const CO_OBJ_TYPE *)&COTSyncId)  /*!< Dynamic COB-ID    */

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

/*! \brief SYNCHRONOUS PDO TABLE
*
*    This structure contains all needed data to handle synchronous PDOs.
*/
typedef struct CO_SYNC_T {
    struct CO_NODE_T *Node;             /*!< link to parent node             */
    uint32_t          CobId;            /*!< SYNC message identifier         */
    uint32_t          Time;             /*!< SYNC time (num of SYNCs)        */
    CO_IF_FRM         RFrm[CO_RPDO_N];  /*!< synchronous RPDO CAN frame      */
    struct CO_RPDO_T *RPdo[CO_RPDO_N];  /*!< Pointer to synchronous RPDO     */
    struct CO_TPDO_T *TPdo[CO_TPDO_N];  /*!< Pointer to synchronous TPDO     */
    uint8_t           TNum[CO_TPDO_N];  /*!< SYNCs until PDO shall be sent   */
    uint8_t           TSync[CO_TPDO_N]; /*!< SYNC time when tx must occur    */

} CO_SYNC;

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/*! \brief SYNC INITIALIZATION
*
*    This function reads and caches the synchronisation object.
*
* \param sync
*    Pointer to sync object
*
* \param node
*    Pointer to parent node
*/
void COSyncInit(CO_SYNC *sync, struct CO_NODE_T *node);

/*! \brief SYNC MESSAGE HANDLER
*
*    This function is called on receiving a SYNC message and handles the
*    synchronous PDOs as defined in the object dictionary.
*
* \param sync
*    Pointer to SYNC object
*/
void COSyncHandler(CO_SYNC *sync);

/*! \brief ADD SYNCHRONOUS PDO
*
*    This function adds a PDO to the synchronous PDO tables.
*
* \param sync
*    Pointer to SYNC object
*
* \param num
*    Number of PDO
*
* \param msgType
*    CAN message type
*
* \param txtype
*    PDO transmission type
*/
void COSyncAdd(CO_SYNC *sync, uint16_t num, uint8_t msgType, uint8_t txtype);

/*! \brief REMOVE SYNCHRONOUS PDO
*
*    This function removes a SYNC PDO from the SYNC PDO table.
*
* \param sync
*    Pointer to SYNC object
*
* \param num
*    Number of PDO
*
* \param msgType
*    CAN message type
*/
void COSyncRemove(CO_SYNC *sync, uint16_t num, uint8_t msgType);

/*! \brief RECEIVE SYNCHRONOUS PDO
*
*    This function handles a received synchronous RPDO
*
* \param sync
*    Pointer to SYNC object
*
* \param frm
*    CAN Frame, received from CAN bus
*/
void COSyncRx(CO_SYNC *sync, CO_IF_FRM *frm);

/*! \brief UPDATE SYNC MANAGEMENT TABLES
*
*    This function checks the given frame to be a SYNC message.
*
* \param sync
*    Pointer to sync object
*
* \param frm
*    CAN Frame, received from CAN bus
*
* \retval  =0    CAN message frame is a SYNC message
* \retval  <0    the given CAN message is no SYNC message
*/
int16_t COSyncUpdate(CO_SYNC *sync, CO_IF_FRM *frm);

/*! \brief RESTART SYNC TIMING
*
*    This function is used to restart SYNC. It's called on NMT Start
*    Operational and resets the SYNC time counter.
*
* \param sync
*    Pointer to SYNC object
*/
void COSyncRestart(CO_SYNC *sync);

/*! \brief SYNC COB-ID WRITE ACCESS
*
*    This function is responsible for the correct write access for the
*    SYNC COB-ID object entry (1005h). The access to bit 0 to 29 is only
*    allowed when bit 30 is set to 0.
*
* \param obj
*    SYNC COB-ID object entry reference
*
* \param node
*    reference to parent node
*
* \param buf
*    Pointer to buffer memory
*
* \param len
*    Length of buffer memory
*
* \retval  CO_ERR_NONE        SYNC COB-ID object entry is written
* \retval  CO_ERR_OBJ_RANGE   an error is detected and function aborted
*/
CO_ERR COTypeSyncIdWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t len);

/******************************************************************************
* CALLBACK FUNCTIONS
******************************************************************************/

/*! \brief  SYNC UPDATE
*
*    This function is called just after the synchronized RPDO is written to
*    the object dictionary.
*
* \param pdo
*    Pointer to received RPDO
*/
extern void COPdoSyncUpdate(CO_RPDO *pdo);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_SYNC_H_ */
