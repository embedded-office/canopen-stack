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
    CO_RESET_INVALID = 0,        /*!< invalid reset type (for testing)       */
    CO_RESET_NODE,               /*!< reset application (and communication)  */
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

/*! \brief NMT RESET REQUEST CALLBACK
*
*    This function is called when a NMT reset is requested.
*
* \note
*    The nmt object pointer is checked to be valid before calling this
*    function.
*
* \param nmt
*    reference to NMT structure
*
* \param reset
*    the reset request
*/
extern void CONmtResetRequest(CO_NMT *nmt, CO_NMT_RESET reset);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_NMT_H_ */
