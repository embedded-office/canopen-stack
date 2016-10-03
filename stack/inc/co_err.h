/******************************************************************************
* (c) by Embedded Office GmbH & Co. KG, <http://www.embedded-office.com/>
*------------------------------------------------------------------------------
* This file is part of CANopenStack, an open source CANopen Stack.
* Project home page is <https://github.com/MichaelHillmann/CANopenStack.git>.
* For more information on CANopen see <http://www.can-cia.org/>.
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

#ifndef CO_ERR_H_
#define CO_ERR_H_

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/
    
#ifndef CO_ERR_BASE
#define CO_ERR_BASE    0x0100    /*!< First CANopen stack error code         */
#endif

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

/*! \brief INTERNAL ERROR CODES
*
*    This enumeration holds all internal used error codes. The errors are
*    node specific an can be accessed with the function \ref CONodeGetErr().
*/
typedef enum CO_ERR_T {
    CO_ERR_NONE = 0,             /*!< no error                               */

    CO_ERR_BAD_ARG = CO_ERR_BASE,/*!< invalid argument                       */

    CO_ERR_OBJ_NOT_FOUND,        /*!< searched object not found in directory */
    CO_ERR_OBJ_READ,             /*!< error during reading a object entry    */
    CO_ERR_OBJ_WRITE,            /*!< error during writing a object entry    */
    CO_ERR_OBJ_SIZE,             /*!< read/write with wrong size to object   */

    CO_ERR_OBJ_MAP_LEN,          /*!< invalid mapping length                 */
    CO_ERR_OBJ_MAP_TYPE,         /*!< invalid mapping type                   */
    CO_ERR_OBJ_ACC,              /*!< unsupported access                     */
    CO_ERR_OBJ_RANGE,            /*!< value range of parameter exceeded      */
    CO_ERR_OBJ_INCOMPATIBLE,     /*!< incompatible parameter value           */

    CO_ERR_PARA_IDX,             /*!< wrong index for parameter type         */
    CO_ERR_PARA_STORE,           /*!< error during storing parameter         */
    CO_ERR_PARA_RESTORE,         /*!< error during restoring parameter       */
    CO_ERR_PARA_LOAD,            /*!< error during loading parameter         */

    CO_ERR_LSS_STORE,            /*!< error during storing LSS configuration */
    CO_ERR_LSS_LOAD,             /*!< error during loading LSS configuration */

    CO_ERR_CFG_1001_0,           /*!< entry 1001:0 is bad/not existing       */
    CO_ERR_CFG_1003_0,           /*!< entry 1003:0 is bad/not existing       */
    CO_ERR_CFG_1003_1,           /*!< entry 1003:1 is bad/not existing       */
    CO_ERR_CFG_1005_0,           /*!< entry 1005:0 is bad/not existing       */
    CO_ERR_CFG_1010_0,           /*!< entry 1010:0 is bad/not existing       */
    CO_ERR_CFG_1011_0,           /*!< entry 1011:0 is bad/not existing       */
    CO_ERR_CFG_1014_0,           /*!< entry 1014:0 is bad/not existing       */
    CO_ERR_CFG_1017_0,           /*!< entry 1017:0 is bad/not existing       */
    CO_ERR_CFG_1016,             /*!< entry in 1016 is bad/not existing      */
    CO_ERR_CFG_1018,             /*!< entry in 1018 is bad/not existing      */

    CO_ERR_TMR_NO_ACT,           /*!< no action available while creating     */
    CO_ERR_TMR_INSERT,           /*!< error during insert action in tmr-list */
    CO_ERR_TMR_CREATE,           /*!< error during creating a timed action   */
    CO_ERR_TMR_DELETE,           /*!< error during deleting a timed action   */

    CO_ERR_NMT_INIT,             /*!< error during initializing NMT slave    */
    CO_ERR_NMT_APP_RESET,        /*!< error in resetting application         */
    CO_ERR_NMT_COM_RESET,        /*!< error in resetting communication       */
    CO_ERR_NMT_MODE,             /*!< action not allowed in current NMT mode */

    CO_ERR_EMCY_BAD_ROOT,        /*!< error in emcy structure, member: Root  */

    CO_ERR_TPDO_COM_OBJ,         /*!< config error in TPDO communication     */
    CO_ERR_TPDO_MAP_OBJ,         /*!< config error in TPDO mapping           */
    CO_ERR_TPDO_OBJ_TRIGGER,     /*!< error during trigger via an object     */
    CO_ERR_TPDO_NUM_TRIGGER,     /*!< error during trigger via an PDO number */
    CO_ERR_TPDO_INHIBIT,         /*!< error during inhibit timer creation    */
    CO_ERR_TPDO_EVENT,           /*!< error during event timer creation      */

    CO_ERR_RPDO_COM_OBJ,         /*!< config error in RPDO communication     */
    CO_ERR_RPDO_MAP_OBJ,         /*!< config error in RPDO mapping           */

    CO_ERR_SDO_READ,             /*!< error during in SDO block reading      */
    CO_ERR_SDO_WRITE,            /*!< error during in SDO block writing      */

    CO_ERR_SYNC_MSG,             /*!< error during receive synchronous PDO   */

    CO_ERR_IF_INIT,              /*!< error during initialization            */
    CO_ERR_IF_ENABLE,            /*!< error during enabling the interface    */
    CO_ERR_IF_FLUSH_RX,          /*!< error during flushing RX interface     */
    CO_ERR_IF_FLUSH_TX,          /*!< error during flushing TX interface     */
    CO_ERR_IF_RESET,             /*!< error during resetting interface       */
    CO_ERR_IF_CLOSE,             /*!< error during closing the interface     */
    CO_ERR_IF_READ,              /*!< error during reading from interface    */
    CO_ERR_IF_SEND,              /*!< error during sending to interface      */

    CO_ERR_SIG_INIT,             /*!< error during initializing CAN signals  */
    CO_ERR_SIG_CREATE,           /*!< error during creating a needed signal  */

    CO_ERR_MSG_INIT,             /*!< error during message initialization    */
    CO_ERR_MSG_CREATE,           /*!< error during creating a message        */
    CO_ERR_MSG_READ,             /*!< error during reading a message         */

    CO_ERR_TYPE_RD,              /*!< error during reading type              */
    CO_ERR_TYPE_CTRL,            /*!< error during type control              */
    CO_ERR_TYPE_WR               /*!< error during writing type              */

} CO_ERR;

#endif  /* #ifndef CO_ERR_H_ */
