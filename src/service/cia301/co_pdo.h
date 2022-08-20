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

#ifndef CO_PDO_H_
#define CO_PDO_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_obj.h"
#include "co_if.h"
#include "co_tmr.h"

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

#define CO_TPDO_COBID_OFF    ((uint32_t)1 << 31)    /*!< marked as unused    */
#define CO_TPDO_COBID_REMOTE ((uint32_t)1 << 30)    /*!< RTR is not allowed  */
#define CO_TPDO_COBID_EXT    ((uint32_t)1 << 29)    /*!< extended format     */

#define CO_RPDO_COBID_OFF    ((uint32_t)1 << 31)    /*!< marked as unused    */
#define CO_RPDO_COBID_EXT    ((uint32_t)1 << 29)    /*!< extended format     */

#define CO_TPDO_FLG___E     0x01   /*!< PDO event occured                    */
#define CO_TPDO_FLG__I_     0x02   /*!< PDO TX inhibited                     */
#define CO_TPDO_FLG__IE     0x03   /*!< PDO event occured + TX inhibited     */
#define CO_TPDO_FLG_S__     0x04   /*!< PDO synced                           */
#define CO_TPDO_FLG_S_E     0x05   /*!< PDO synced + event occured           */
#define CO_TPDO_FLG_SI_     0x06   /*!< PDO synced + TX inhibited            */
#define CO_TPDO_FLG_SIE     0x07   /*!< PDO synved + event occured + TX inh. */

#define CO_RPDO_FLG__E      0x01                    /*!< enabled RPDO        */
#define CO_RPDO_FLG_S_      0x02                    /*!< synchronized RPDO   */


/*! \brief RPDO COB-ID parameter
*
*    These macros constructs the COB-ID for usage in object entries
*    index 1400..14ff: COB-ID for RPDO communication. The standard
*    defines the following encoding:
*    - bit31: valid: PDO exists (0) or not (1)
*    - bit30: reserved: don't care
*    - bit29: frame = 11bit CAN-ID (0) or 29bit CAN-ID (1)
*    - bit28-0: CAN-ID
*
* \param valid
*    RPDO exists (1), or not (0)
*
* \param id
*    the CAN-ID (standard or extended format)
*
* \note: the macro inverts the given value for valid in comparison to the
*        standard definition to avoid negated logic
* \{
*/
#define CO_COBID_RPDO_STD(valid, id)                         \
    (uint32_t)(((uint32_t)(id) & 0x7ffuL)                  | \
               ((uint32_t)(1uL - ((valid) & 0x1u)) << 31u))

#define CO_COBID_RPDO_EXT(valid, id)                         \
    (uint32_t)(((uint32_t)(id) & 0x1fffffffuL)             | \
               ((uint32_t)0x1u << 29u)                     | \
               ((uint32_t)(1uL - ((valid) & 0x1u)) << 31u))
/*! \} */

#define CO_COBID_RPDO_BASE   (uint32_t)0x200
#define CO_COBID_RPDO_INC    (uint32_t)0x100

/*! \brief RPDO Default Connectionset
*
*    The CANopen standard recommends a default connection set. This set
*    includes 4 RPDOs.
*
* \param rpdo
*    the RPDO number (0..3)
*/
#define CO_COBID_RPDO_DEFAULT(rpdo)                                       \
    CO_COBID_RPDO_STD(1u, CO_COBID_RPDO_BASE + ((rpdo) * CO_COBID_RPDO_INC))

/*! \brief TPDO COB-ID parameter
*
*    These macros constructs the COB-ID for usage in object entries
*    index 1800..18ff: COB-ID for TPDO communication. The standard
*    defines the following encoding:
*    - bit31: valid: PDO exists (0) or not (1)
*    - bit30: RTR: RTR allowed on PDO (0) or not (1)
*    - bit29: frame = 11bit CAN-ID (0) or 29bit CAN-ID (1)
*    - bit28-0: CAN-ID
*
* \param valid
*    TPDO exists (1), or not (0)
*
* \param id
*    the CAN-ID (standard or extended format)
*
* \note: the macro inverts the given value for valid in comparison to the
*        standard definition to avoid negated logic
* \{
*/
#define CO_COBID_TPDO_STD(valid, id)                         \
    (uint32_t)(((uint32_t)(id) & 0x7ffuL)                  | \
               ((uint32_t)0x1u << 30u)                     | \
               ((uint32_t)(1uL - ((valid) & 0x1u)) << 31u))

#define CO_COBID_TPDO_EXT(valid, id)                         \
    (uint32_t)(((uint32_t)(id) & 0x1fffffffuL)             | \
               ((uint32_t)0x1u << 29u)                     | \
               ((uint32_t)0x1u << 30u)                     | \
               ((uint32_t)(1uL - ((valid) & 0x1u)) << 31u))
/*! \} */

#define CO_COBID_TPDO_BASE   (uint32_t)0x180
#define CO_COBID_TPDO_INC    (uint32_t)0x100

/*! \brief TPDO Default Connectionset
*
*    The CANopen standard recommends a default connection set. This set
*    includes 4 TPDOs.
*
* \param tpdo
*    the TPDO number (0..3)
*/
#define CO_COBID_TPDO_DEFAULT(tpdo)                                       \
    CO_COBID_TPDO_STD(1u, CO_COBID_TPDO_BASE + ((tpdo) * CO_COBID_TPDO_INC))

/*! \brief OBJECT MAPPING LINK
*
*    This macro helps to build the CANopen object entry for a PDO mapping.
*    This entry is used to specify the linked signal within a PDO.
*
* \param idx
*    CANopen object index [0x0000..0xFFFF]
*
* \param sub
*    CANopen object subindex [0x00..0xFF]
*
* \param bit
*    Length of mapped signal in bits [8,16,24 or 32]
*/
#define CO_LINK(idx,sub,bit)    \
    (uint32_t)( (((uint32_t)(idx) & 0xFFFFL) << 16) | \
                (((uint32_t)(sub) &   0xFFL) << 8 ) | \
                 ((uint32_t)(bit) &   0xFFL) )

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

struct CO_OBJ_T;

/*! \brief TPDO SIGNAL LINK TABLE
*
*    This structure holds all data, which are needed for managing the links
*    from a signal to all TPDOs, which has active mapping entries to this
*    signal.
*/
typedef struct CO_TPDO_LINK_T {
    struct CO_OBJ_T *Obj;        /*!< pointer to object                      */
    uint16_t         Num;        /*!< currently mapped to TPDO-num (0..511)  */

} CO_TPDO_LINK;

/*! \brief TPDO DATA
*
*    This structure holds all data, which are needed for managing a
*    single TPDO.
*/
typedef struct CO_TPDO_T {
    struct CO_NODE_T *Node;        /*!< link to parent CANopen node          */
    uint32_t          Identifier;  /*!< message identifier                   */
    struct CO_OBJ_T  *Map[8];      /*!< pointer list with mapped objects     */
    uint8_t           Size[8];     /*!< size of mapped object value in bytes */
    int16_t           EvTmr;       /*!< event timer id                       */
    uint32_t          Event;       /*!< event time in timer ticks            */
    int16_t           InTmr;       /*!< inhibit timer id                     */
    uint32_t          Inhibit;     /*!< inhibit time in timer ticks          */
    uint8_t           Flags;       /*!< info flags                           */
    uint8_t           ObjNum;      /*!< Number of linked objects             */

} CO_TPDO;

/*! \brief RPDO DATA
*
*    This structure holds all data, which are needed for managing a
*    single RPDO.
*/
typedef struct CO_RPDO_T {
    struct CO_NODE_T *Node;        /*!< link to parent CANopen node          */
    uint32_t          Identifier;  /*!< message identifier                   */
    struct CO_OBJ_T  *Map[8];      /*!< pointer list with mapped objects     */
    uint8_t           Size[8];     /*!< size of mapped object value in bytes */
    uint8_t           ObjNum;      /*!< Number of linked objects             */
    uint8_t           Flag;        /*!< Flags attributed of PDO              */

} CO_RPDO;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*! \brief TPDO EVENT TRIGGER VIA OBJECT
*
*    This function allows the application to trigger a TPDO event for the
*    given object entry. The event will be distributed to all TPDOs, which
*    holds an active mapping entry to this object entry.
*
* \param tpdo
*    Pointer to start of TPDO array
*
* \param obj
*    Pointer to object entry
*/
void COTPdoTrigObj(CO_TPDO *tpdo, struct CO_OBJ_T *obj);

/*! \brief TPDO EVENT TRIGGER VIA TPDO-NUM
*
*    This function allows the application to trigger a TPDO event for the
*    given TPDO number.
*
* \param tpdo
*    Pointer to start of TPDO array
*
* \param num
*    Number of TPDO (0..511)
*/
void COTPdoTrigPdo(CO_TPDO *tpdo, uint16_t num);

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/*! \brief TPDO CLEAR
*
*    This function sets the PDO related configuration data to a known
*    state (inactive).
*
* \param pdo
*    Pointer to start of TPDO array
*
* \param node
*    Pointer to parent node object
*/
void COTPdoClear(CO_TPDO *pdo, struct CO_NODE_T *node);

/*! \brief TPDO INITIALIZATION
*
*    This function checks the object dictionary and creates the PDO related
*    configuration data from the object dictionary settings.
*
* \param pdo
*    Pointer to start of TPDO array
*
* \param node
*    Pointer to parent node object
*/
void COTPdoInit(CO_TPDO *pdo, struct CO_NODE_T *node);

/*! \brief RESET TPDO COMMUNICATION PROFILE
*
*    This function scans the object dictionary for the TPDO communication and
*    mapping profile. The function will fill the TPDO info structure with the
*    corresponding informations.
*
*    The following list shows the considered communication profile entries:
*    -# 0x1800+[num] : 0x01 = TPDO CAN identifier
*    -# 0x1800+[num] : 0x02 = Transmission type
*    -# 0x1800+[num] : 0x03 = Inhibit time
*    -# 0x1800+[num] : 0x05 = Event timer (for transmission type 254 and 255)
*
* \note
*    The object entry 0x1800+[num] : 0x04 is reserved and should be not
*    existing. This is not checked with this function.
*
* \param pdo
*    Pointer to start of TPDO array
*
* \param num
*    Number of TPDO (0..511)
*/
void COTPdoReset(CO_TPDO *pdo, uint16_t num);

/*! \brief GET PDO MAPPING DATA
*
*    This function gets the PDO mapping datas out of the object dictionary and
*    puts the pre-calculated values in the CAN message configuration.
*
*    The following list shows the considered mapping profile entries:
*    -# 0x1A00+[num] : 0x00 = Number of mapped signals (0..8)
*    -# 0x1A00+[num] : 0x01..0x08 = Mapped signal
*
* \param pdo
*    Pointer to start of TPDO array
*
* \param num
*    Number of TPDO (0..511)
*
* \retval  ==CO_ERR_NONE      Mapping successful parsed out of object dictionary
* \retval  !=CO_ERR_NONE      At least one error is detected within the mapping
*                             configuration
*/
CO_ERR COTPdoGetMap(CO_TPDO *pdo, uint16_t num);

/*! \brief TPDO TRIGGER EVENT TIMER CALLBACK
*
*    This function is used to trigger a PDO with the event timer object
*    (see subindex 5 of communication parameters).
*
* \param parg
*    Pointer to TPDO element
*/
void COTPdoTmrEvent(void *parg);

/*! \brief TPDO INHIBIT TIMER CALLBACK
*
*    This function is used to release the transmission stop after the PDO
*    inhibit time.
*
* \param parg
*    Pointer to TPDO element
*/
void COTPdoTmrInhibit(void *parg);

/*! \brief TPDO TRANSMIT
*
*    This function is responsible for the transmission of a TPDO.
*
*    If the inhibit time is enabled, further transmission will be disabled -
*    and a one-shot timer 'end of inhibit time' callback function is created.
*
* \param pdo
*    Pointer to TPDO element
*/
void COTPdoTx(CO_TPDO *pdo);

/*! \brief TPDO LINK MAP ADD
*
*    This function is used to add an entry into the signal to TPDO link
*    mapping table.
*
* \param map
*    Pointer to start of link mapping table
*
* \param obj
*    Pointer to object entry
*
* \param num
*    Linked TPDO number
*/
void COTPdoMapAdd(CO_TPDO_LINK *map, struct CO_OBJ_T *obj, uint16_t num);

/*! \brief TPDO LINK MAP DEL VIA TPDO-NUM
*
*    This function is used to delete all entries, which contains the given
*    TPDO number.
*
* \param map
*    Pointer to start of link mapping table
*
* \param num
*    Linked TPDO number
*/
void COTPdoMapDelNum(CO_TPDO_LINK *map, uint16_t num);

/*! \brief TPDO LINK MAP DEL VIA SIGNAL-ID
*
*    This function is used to delete all entries, which contains the given
*    signal identifier.
*
* \param map
*    Pointer to start of link mapping table
*
* \param obj
*    Pointer to object entry
*/
void COTPdoMapDelSig(CO_TPDO_LINK *map, struct CO_OBJ_T *obj);

/*! \brief RPDO CLEAR
*
*    This function sets the PDO related configuration data to a known
*    state (inactive).
*
* \param pdo
*    Pointer to start of RPDO array
*
* \param node
*    Pointer to parent node object
*/
void CORPdoClear(CO_RPDO *pdo, struct CO_NODE_T *node);

/*! \brief RPDO INITIALIZATION
*
*    This function checks the object dictionary and creates the PDO related
*    configuration data from the object dictionary settings.
*
* \param pdo
*    Pointer to start of RPDO array
*
* \param node
*    Pointer to parent node object
*/
void CORPdoInit(CO_RPDO *pdo, struct CO_NODE_T *node);

/*! \brief RESET RPDO COMMUNICATION PROFILE
*
*    This function scans the object dictionary for the RPDO communication and
*    mapping profile. The function will fill the RPDO info structure with the
*    corresponding informations.
*
*    The following list shows the considered communication profile entries:
*    -# 0x1400+[num] : 0x01 = RPDO CAN identifier
*    -# 0x1400+[num] : 0x02 = Transmission type
*
* \note
*    The object entry 0x1400+[num] : 0x03..0x05 are reserved and should be
*    not existing. This is not checked with this function.
*
* \param pdo
*    Pointer to start of RPDO array
*
* \param num
*    Number of RPDO (0..511)
*
* \retval  ==CO_ERR_NONE    The communication profile is checked, and data are considered
* \retval  !=CO_ERR_NONE    At least one communication profile entry is invalid, further
*                           checks aborted
*/
CO_ERR CORPdoReset(CO_RPDO *pdo, uint16_t num);

/*! \brief GET PDO MAPPING DATA
*
*    This function gets the PDO mapping datas out of the object dictionary
*    and puts the pre-calculated values in the CAN message configuration.
*
*    The following list shows the considered mapping profile entries:
*    -# 0x1600+[num] : 0x00 = Number of mapped signals (0..8)
*    -# 0x1600+[num] : 0x01..0x08 = Mapped signal
*
* \param pdo
*    Pointer to start of RPDO array
*
* \param num
*    Number of RPDO (0..511)
*
* \retval  ==CO_ERR_NONE    Mapping successful parsed out of object dictionary
* \retval  !=CO_ERR_NONE    At least one error is detected within the mapping
*                           configuration
*/
CO_ERR CORPdoGetMap(CO_RPDO *pdo, uint16_t num);

/*! \brief RPDO CHECK
*
*    This function is used to check the received CAN message frame to be a
*    RPDO message.
*
* \param pdo
*    Pointer to start of RPDO array
*
* \param frm
*    Received CAN message frame
*
* \retval  !=NULL    Pointer to the receive PDO
* \retval  ==NULL    Not a valid RPDO CAN message
*/
CO_RPDO *CORPdoCheck(CO_RPDO *pdo, CO_IF_FRM *frm);

/*! \brief RPDO RECEIVE
*
*    This function is responsible for the distribution of a RPDO into the
*    corresponding signals within the object dictionary.
*
* \param pdo
*    Pointer to RPDO element
*
* \param frm
*    Pointer to received CAN frame
*/
void CORPdoRx(CO_RPDO *pdo, CO_IF_FRM *frm);

/*! \brief RPDO WRITE
*
*    This function is used to write the received CAN message data to the
*    object dictionary.
*
* \param pdo
*    Pointer to start of RPDO array
*
* \param frm
*    Received CAN message frame
*/
void CORPdoWrite(CO_RPDO *pdo, CO_IF_FRM *frm);

/******************************************************************************
* CALLBACK FUNCTIONS
******************************************************************************/

/*! \brief  PDO TRANSMIT CALLBACK
*
*    This function is called just before the PDO transmission will send
*    the PDO message frame to the CANopen network.
*
* \param frm
*    Pointer to PDO message frame
*/
extern void COPdoTransmit(CO_IF_FRM *frm);

/*! \brief  PDO RECEIVE CALLBACK
*
*    This function is called just before the PDO receiption will distribute
*    the PDO message frame into the object dictionary. This callback function
*    is able to <i>consume</i> the PDO message frame, e.g. the distribution
*    into the object dictionary will be skipped. Furthermore without
*    <i>consuming</i> the PDO message frame, this function could modify the
*    recieved data before distribution takes place.
*
* \param frm
*    Pointer to PDO message frame
*
* \retval   =0    CAN message frame is not consumed
* \retval   >0    CAN message frame is consumed
*/
extern int16_t COPdoReceive(CO_IF_FRM *frm);

/*! \brief  PDO WRITE DATA CALLBACK
*
*    This function is called during PDO distribution of the PDO message
*    frame into the object dictionary when mapped data value consumes
*    more than 4 bytes.
*
* \param frm
*    Pointer to PDO message frame
*
* \param pos
*    The start index in the CAN message frame payload
*
* \param size
*    The mapped value size in bytes
*
* \param obj
*    Pointer to the target object entry
*/
extern void CORpdoWriteData(CO_IF_FRM *frm, uint8_t pos, uint8_t size, CO_OBJ *obj);

/*! \brief  PDO READ DATA CALLBACK
*
*    This function is called during PDO transmission when a mapped
*    data value consumes more than 4 bytes.
*
* \param frm
*    Pointer to PDO message frame
*
* \param pos
*    The start index in the CAN message frame payload
*
* \param size
*    The mapped value size in bytes
*
* \param obj
*    Pointer to the source object entry
*/
extern void COTpdoReadData(CO_IF_FRM *frm, uint8_t pos, uint8_t size, CO_OBJ *obj);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_TPDO_H_ */
