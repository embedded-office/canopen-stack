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

#ifndef CO_EMCY_H_
#define CO_EMCY_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"
#include "co_cfg.h"
#include "co_err.h"

#include "co_obj.h"

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

#define CO_EMCY_COBID_OFF    ((uint32_t)1 << 31)    /*!< marked as unused    */
#define CO_EMCY_COBID_EXT    ((uint32_t)1 << 29)    /*!< extended format     */
#define CO_EMCY_COBID_MASK   ((uint32_t)0x1FFFFFFF) /*!< identifier mask     */

#define CO_TEMCY   ((const CO_OBJ_TYPE *)&COTEmcy)    /*!< EMCY History      */
#define CO_TEMCYID ((const CO_OBJ_TYPE *)&COTEmcyId)  /*!< Dynamic COB-ID    */

#define CO_EMCY_STORAGE  (1+((CO_EMCY_N-1)/8))  /*!< bytes for CO_EMCY_N bit */

/*! \brief EMCY CODE
*
*    The following defines holds the base values for the pre-defined standard
*    error groups. The values can be used to simplify the definition of
*    standard conform EMCY error codes.
*/
#define CO_EMCY_CODE_NO_ERR                0x0000     /*!< error reset or no error                */
#define CO_EMCY_CODE_GEN_ERR               0x1000     /*!< generic error                          */
#define CO_EMCY_CODE_CUR_ERR               0x2000     /*!< current                                */
#define CO_EMCY_CODE_CUR_INPUT_ERR         0x2100     /*!< current, device input side             */
#define CO_EMCY_CODE_CUR_INTERN_ERR        0x2200     /*!< current inside the device              */
#define CO_EMCY_CODE_CUR_OUTPUT_ERR        0x2300     /*!< current, device output side            */
#define CO_EMCY_CODE_VOL_ERR               0x3000     /*!< voltage                                */
#define CO_EMCY_CODE_VOL_INPUT_ERR         0x3100     /*!< mains voltage                          */
#define CO_EMCY_CODE_VOL_INTERN_ERR        0x3200     /*!< voltage inside the device              */
#define CO_EMCY_CODE_VOL_OUTPUT_ERR        0x3300     /*!< output voltage                         */
#define CO_EMCY_CODE_TEMP_ERR              0x4000     /*!< temperature                            */
#define CO_EMCY_CODE_TEMP_AMBIENT_ERR      0x4100     /*!< ambient temperature                    */
#define CO_EMCY_CODE_TEMP_DEVICE_ERR       0x4200     /*!< device temperature                     */
#define CO_EMCY_CODE_HW_ERR                0x5000     /*!< device hardware                        */
#define CO_EMCY_CODE_SW_ERR                0x6000     /*!< device software                        */
#define CO_EMCY_CODE_SW_INTERNAL_ERR       0x6100     /*!< internal software                      */
#define CO_EMCY_CODE_SW_USER_ERR           0x6200     /*!< user software                          */
#define CO_EMCY_CODE_SW_DATASET_ERR        0x6300     /*!< data set                               */
#define CO_EMCY_CODE_ADD_MODULES_ERR       0x7000     /*!< additional modules                     */
#define CO_EMCY_CODE_MON_ERR               0x8000     /*!< monitoring                             */
#define CO_EMCY_CODE_MON_COM_ERR           0x8100     /*!< communication                          */
#define CO_EMCY_CODE_MON_COM_OVERRUN_ERR   0x8110     /*!< CAN overrun (objects lost)             */
#define CO_EMCY_CODE_MON_COM_PASSIVE_ERR   0x8120     /*!< CAN in error passive mode              */
#define CO_EMCY_CODE_MON_COM_HEARTBEAT_ERR 0x8130     /*!< life guard error or heartbeat error    */
#define CO_EMCY_CODE_MON_COM_RECOVER_ERR   0x8140     /*!< recovered from bus off                 */
#define CO_EMCY_CODE_MON_COM_COLLISION_ERR 0x8150     /*!< transmit COB-ID collision              */
#define CO_EMCY_CODE_MON_PROT_ERR          0x8200     /*!< protocol error                         */
#define CO_EMCY_CODE_MON_PROT_PDO_IGN_ERR  0x8210     /*!< PDO not processed due to length error  */
#define CO_EMCY_CODE_MON_PROT_PDO_LEN_ERR  0x8220     /*!< PDO length exceeded                    */
#define CO_EMCY_CODE_EXT_ERR               0x9000     /*!< external error                         */
#define CO_EMCY_CODE_ADD_FUNC_ERR          0xF000     /*!< additional functions                   */
#define CO_EMCY_CODE_DEV_ERR               0xFF00     /*!< device specific                        */

/*! \brief EMCY REGISTER BITS
*
*    The following defines holds the bit-number within the error register
*    object (1001h) in the object dictionary. These values can be used to
*    specify a standard conform EMCY error code relation table.
*/
#define CO_EMCY_REG_GENERAL        0   /*!< general (includes all classes)   */
#define CO_EMCY_REG_CURRENT        1   /*!< class: current                   */
#define CO_EMCY_REG_VOLTAGE        2   /*!< class: voltage                   */
#define CO_EMCY_REG_TEMP           3   /*!< class: temperature               */
#define CO_EMCY_REG_COM            4   /*!< class: communication             */
#define CO_EMCY_REG_PROFILE        5   /*!< class: profile specific error    */
#define CO_EMCY_REG_MANUFACTURER   7   /*!< class: manufacturer specific     */
#define CO_EMCY_REG_NUM            8   /*!< number of supported classes      */

/******************************************************************************
* PUBLIC CONSTANTS
******************************************************************************/

/*! \brief OBJECT TYPE EMCY HISTORY
*
*    This type is responsible for the access to the EMCY history.
*/
extern const CO_OBJ_TYPE COTEmcy;      /* Link to EMCY Object Type Structure */

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

struct CO_OBJ_T;               /* Declaration of object entry structure      */

/*! \brief EMCY HISTORY
*
*    This structure holds all data, which are needed for the EMCY history
*    management within the object dictionary.
*/
typedef struct CO_EMCY_HIST_T {
    uint8_t Max;                 /*!< Total length of EMCY history           */
    uint8_t Num;                 /*!< Number of EMCY in history              */
    uint8_t Off;                 /*!< Subindex-Offset to newest EMCY entry   */

} CO_EMCY_HIST;

/*! \brief EMCY MANUFACTURER SPECIFIC FIELDS
*
*    This structure holds the optional manufacturer specific fields for the
*    EMCY message and for the EMCY history.
*/
typedef struct CO_EMCY_USR_T {
    uint16_t  Hist;              /*!< manufacturer specific field in History */
    uint8_t   Emcy[5];           /*!< manufacturer specific field in EMCY    */

} CO_EMCY_USR;

/*! \brief EMCY CODE DEFINITION TABLE
*
*    This structure holds the EMCY code and the relation to the error
*    register bit.
*/
typedef struct CO_EMCY_TBL_T {
    uint8_t   Reg;               /*!< bit number (0..7) in error register    */
    uint16_t  Code;              /*!< error code (category see CO_EMCY_CODE) */

} CO_EMCY_TBL;

/*! \brief EMCY MANAGEMENT
*
*    This structure holds the EMCY defintion table and informations to
*    manage the change detection on all individual EMCY codes.
*/
typedef struct CO_EMCY_T {
    struct CO_NODE_T      *Node;                  /*!< parent node           */
    struct CO_EMCY_TBL_T  *Root;                  /*!< root to EMCY table    */
    struct CO_EMCY_HIST_T  Hist;                  /*!< EMCY history          */
    uint8_t                Cnt[CO_EMCY_REG_NUM];  /*!< count register bits   */
    uint8_t                Err[CO_EMCY_STORAGE];  /*!< error status storage  */

} CO_EMCY;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*! \brief SET EMCY ERROR
*
*    This function checks the current error state and updates the object
*    dictionary. The EMCY message is transmitted, if the error is detected
*    for the first time. The given manufacturer specific fields are optional,
*    e.g. the ptr may be 0 to set all manufacturer specific values to 0.
*
* \param emcy
*    pointer to the EMCY object
*
* \param err
*    EMCY error identifier in User EMCY table
*
* \param usr
*    manufacturer specific fields in EMCY history and/or EMCY message
*/
void COEmcySet(CO_EMCY *emcy, uint8_t err, CO_EMCY_USR *usr);

/*! \brief CLEAR EMCY ERROR
*
*    This function checks the current error state and updates the object
*    dictionary. The EMCY message is transmitted, if the error was previously
*    detected.
*
* \param emcy
*    pointer to the EMCY object
*
* \param err
*    EMCY error identifier in User EMCY table
*/
void COEmcyClr(CO_EMCY *emcy, uint8_t err);

/*! \brief GET EMCY ERROR STATUS
*
*    This function returns the current EMCY error status.
*
* \param emcy
*    pointer to the EMCY object
*
* \param err
*    EMCY error identifier in User EMCY table
*
* \retval   =0    the error is not detected
* \retval   =1    the error was detected before
* \retval   <0    an error is detected inside of this function
*/
int16_t COEmcyGet(CO_EMCY *emcy, uint8_t err);

/*! \brief COUNT DETECTED EMCY ERRORS
*
*    This function returns the number of currently detected EMCY errors.
*
* \param emcy
*    pointer to the EMCY object
*
* \retval  >=0    the number of detected EMCY errors
* \retval  <0     an error is detected inside of this function
*/
int16_t COEmcyCnt(CO_EMCY *emcy);

/*! \brief RESET ALL EMCY ERRORS
*
*    This function clears all EMCY errors. The EMCY message transmission
*    can be suppressed by setting the parameter silent to 1.
*
* \param emcy
*    pointer to the EMCY object
*
* \param silent
*    Disables the EMCY message transmission for the state changes, made
*    by this function
*/
void COEmcyReset(CO_EMCY *emcy, uint8_t silent);

/*! \brief RESET EMCY HISTORY
*
*    This function clears the EMCY history in the object dictionary.
*
* \param emcy
*    pointer to the EMCY object
*/
void COEmcyHistReset(struct CO_EMCY_T *emcy);

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/*! \brief INIT EMCY MANAGEMENT
*
*    This function initializes the EMCY object and the internal error
*    storage memory. Additionally, the links to the given user EMCY error
*    codes to the given node are set.
*
* \param emcy
*    pointer to the EMCY object
*
* \param node
*    pointer to the CANopen device node information structure
*
* \param root
*    pointer to the start of the User EMCY table
*/
void COEmcyInit(CO_EMCY *emcy, struct CO_NODE_T *node, CO_EMCY_TBL *root);

/*! \brief CHECK EMCY PARAMETER STRUCTURE
*
*    This function helps to centralize the parameter checking of the
*    emcy structure.
*
* \param emcy
*    pointer to the EMCY object
*
* \retval  =0    the given parameter emcy is ok
* \retval  <0    an error is detected in parameter emcy
*/
int16_t COEmcyCheck(CO_EMCY *emcy);

/*! \brief GET INTERNAL ERROR STATE
*
*    This function gets the error state of the given error identifier out
*    of the internal error storage memory.
*
* \note
*    The parameter emcy shall be checked by the calling function.
*
* \param emcy
*    pointer to the EMCY object
*
* \param err
*    user error identifier
*
* \retval  =0    this error is not present
* \retval  =1    the error is already detected
*/
int16_t COEmcyGetErr(CO_EMCY *emcy, uint8_t err);

/*! \brief  SET INTERNAL ERROR STATE
*
*    This function sets the error state of the given error identifier into
*    the internal error storage memory.
*
* \note
*    The parameter emcy shall be checked by the calling function.
*
* \param emcy
*    pointer to the EMCY object
*
* \param err
*    user error identifier
*
* \param state
*    error state (=0: no error, !=0: error)
*
* \retval  =0    error state is unchanged
* \retval  =1    the error state has changed
*/
int16_t COEmcySetErr(CO_EMCY *emcy, uint8_t err, uint8_t state);

/*! \brief  SEND EMCY MESSAGE
*
*    This function transmits the EMCY message to the configured CANbus.
*
* \note
*    The parameter emcy shall be checked by the calling function.
*
* \param emcy
*    pointer to the EMCY object
*
* \param err
*    user error identifier
*
* \param usr
*    manufacturer specific fields in EMCY message and/or EMCY message
*
* \param state
*    error state (=0: no error, !=0: error)
*/
void COEmcySend(CO_EMCY     *emcy,
                uint8_t      err,
                CO_EMCY_USR *usr,
                uint8_t      state);

/*! \brief  UPDATE EMCY OBJECT ENTRIES
*
*    This function updates the EMCY object dictionary entries with the
*    given error code change.
*
* \note
*    The parameter emcy shall be checked by the calling function.
*
* \param emcy
*    pointer to the EMCY object
*
* \param err
*    user error identifier
*
* \param usr
*    manufacturer specific fields in EMCY history and/or EMCY message
*
* \param state
*    error state (=0: no error, !=0: error)
*/
void COEmcyUpdate(CO_EMCY     *emcy,
                  uint8_t      err,
                  CO_EMCY_USR *usr,
                  uint8_t      state);

/*! \brief  INIT EMCY HISTORY
*
*    This function initializes the EMCY history management. Additionally,
*    the object entries, mandatory for EMCY history are checked for existance.
*
* \note
*    The parameter emcy shall be checked by the calling function.
*
* \param emcy
*    pointer to the EMCY object
*/
void COEmcyHistInit(CO_EMCY *emcy);

/*! \brief  ADD EMCY IN HISTORY
*
*    This function appends a new EMCY entry into the history. To avoid moving
*    the object entries with each new entry, the following algorithm is
*    implemented:
*
*    -# This function will use the EMCY Entries as ringbuffer. Each
*       Add-function call will use the next higher subindex for storage of
*       the EMCY History informations.
*    -# The read/write access functions will remap the different subindex
*       entry values to the required order (latest EMCY add must appear at
*       subindex 1).
*
* \note
*    The parameter emcy shall be checked by the calling function.
*
* \param emcy
*    pointer to the EMCY object
*
* \param err
*    user error identifier
*
* \param usr
*    manufacturer specific fields in EMCY message and/or EMCY message
*/
void COEmcyHistAdd(CO_EMCY *emcy, uint8_t err, CO_EMCY_USR *usr);

/*! \brief EMCY OBJECT READ ACCESS
*
*    This function is responsible for the correct read access within the
*    EMCY History object entry (1003h).
*
* \note
*    When accessing the EMCY history entries, the subidx is remapped to
*    the correct value, because the ringbuffer must be transfered into
*    a LIFO list.
*
* \param obj
*    EMCY history object entry reference
*
* \param node
*    Reference to parent node
*
* \param buf
*    Pointer to buffer memory
*
* \param len
*    Length of buffer memory
*
* \retval   =CO_ERR_NONE    emergency entry is read
* \retval  !=CO_ERR_NONE    an error is detected and function aborted
*/
CO_ERR COTypeEmcyRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t len);

/*! \brief EMCY OBJECT WRITE ACCESS
*
*    This function is responsible for the correct write access within the
*    EMCY History object entry (1003h). The access is only allowed in
*    subidx 0; the only writeable value is 0; the result should be a complete
*    reset of the EMCY history.
*
* \param obj
*    EMCY history object entry reference
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
* \retval  CO_ERR_NONE        emergency history is cleared
* \retval  CO_ERR_TYPE_WR     an error is detected and function aborted
*/
CO_ERR COTypeEmcyWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t len);

/*! \brief EMCY COB-ID WRITE ACCESS
*
*    This function is responsible for the correct write access for the
*    EMCY COB-ID object entry (1014h). The access to bit 0 to 29 is only
*    allowed when bit 31 is set to 1.
*
* \param obj
*    EMCY COB-ID object entry reference
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
* \retval  CO_ERR_NONE        EMCY COB-ID object entry is written
* \retval  CO_ERR_OBJ_RANGE   an error is detected and function aborted
*/
CO_ERR COTypeEmcyIdWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t len);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_EMCY_H_ */
