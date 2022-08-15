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

#ifndef CO_CORE_H_
#define CO_CORE_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"
#include "co_cfg.h"

/* basic types */
#include "co_domain.h"
#include "co_string.h"
#include "co_integer8.h"
#include "co_integer16.h"
#include "co_integer32.h"

/* cia301 types */
#include "co_emcy_hist.h"
#include "co_emcy_id.h"
#include "co_hb_cons.h"
#include "co_hb_prod.h"
#include "co_para.h"
#include "co_para_store.h"
#include "co_para_restore.h"
#include "co_pdo_event.h"
#include "co_pdo_id.h"
#include "co_pdo_map.h"
#include "co_pdo_num.h"
#include "co_pdo_type.h"
#include "co_sdo_id.h"
#include "co_sync_cycle.h"
#include "co_sync_id.h"

#include "co_dict.h"
#include "co_if.h"
#include "co_emcy.h"
#include "co_nmt.h"
#include "co_tmr.h"
#include "co_sdo.h"
#include "co_ssdo.h"
#include "co_csdo.h"
#include "co_pdo.h"
#include "co_sync.h"
#if USE_LSS
#include "co_lss.h"
#endif //USE_LSS
#include "co_err.h"
#include "co_obj.h"


/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

/*! \brief CANOPEN NODE
*
*    This data structure holds all informations, which represents a complete
*    CANopen node.
*/
typedef struct CO_NODE_T {
    struct CO_DICT_T       Dict;                 /*!< Object dictionary      */
    struct CO_IF_T         If;                   /*!< Can driver interface   */
    struct CO_EMCY_T       Emcy;                 /*!< Node error status      */
    struct CO_NMT_T        Nmt;                  /*!< Network management     */
    struct CO_TMR_T        Tmr;                  /*!< Timer manager          */
    struct CO_SDO_T        Sdo[CO_SSDO_N];       /*!< SDO Server Array       */
    uint8_t               *SdoBuf;               /*!< SDO Transfer Buffer    */
#if USE_CSDO
    struct CO_CSDO_T       CSdo[CO_CSDO_N];      /*!< SDO client array       */
#endif
    struct CO_RPDO_T       RPdo[CO_RPDO_N];      /*!< RPDO Array             */
    struct CO_TPDO_T       TPdo[CO_TPDO_N];      /*!< TPDO Array             */
    struct CO_TPDO_LINK_T  TMap[CO_TPDO_N * 8];  /*!< TPDO mapping links     */
    struct CO_SYNC_T       Sync;                 /*!< SYNC management        */
#if USE_LSS
    struct CO_LSS_T        Lss;                  /*!< LSS slave handling     */
#endif //USE_LSS
    enum   CO_ERR_T        Error;                /*!< detected error code    */
    uint32_t               Baudrate;             /*!< default CAN baudrate   */
    uint8_t                NodeId;               /*!< default Node-ID        */

} CO_NODE;

/*! \brief NODE SPECIFICATION
*
*    This data structure holds all configurable components of a complete
*    CANopen node.
*/
typedef struct CO_NODE_SPEC_T {
    uint8_t                NodeId;       /*!< default Node-Id                */
    uint32_t               Baudrate;     /*!< default Baudrate               */
    struct CO_OBJ_T       *Dict;         /*!< object dictionary              */
    uint16_t               DictLen;      /*!< object dictionary (max) length */
    struct CO_EMCY_TBL_T  *EmcyCode;     /*!< application EMCY info fields   */
    struct CO_TMR_MEM_T   *TmrMem;       /*!< timer memory blocks            */
    uint16_t               TmrNum;       /*!< number of timer memory blocks  */
    uint32_t               TmrFreq;      /*!< timer clock frequency in Hz    */
    CO_IF_DRV             *Drv;          /*!< linked interface drivers       */
    uint8_t               *SdoBuf;       /*!< SDO Transfer Buffer Memory     */

} CO_NODE_SPEC;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*! \brief  CANOPEN STACK INITIALIZATION
*
*    This function initializes the internals of the CANopen stack. The
*    specification of the CANopen node, and the CANopen node object itself
*    is given as parameter.
*
* \note
*    The node is still in INIT mode after this function call. To finalize
*    the initialization phase (e.g. profile specific or application actions,
*    etc..), see \ref CONodeStart().
*
* \param node
*    pointer to the CANopen node object
*
* \param spec
*    pointer to the configuration collection of config aspects (specification)
*/
void CONodeInit(CO_NODE *node, CO_NODE_SPEC *spec);

/*! \brief  START NODE
*
*    This function will finish the initialization phase. The node will change
*    into PRE-OPERATIONAL state and is ready for communication.
*
* \param node
*    pointer to the CANopen node object
*/
void CONodeStart(CO_NODE *node);

/*! \brief  STOP NODE
*
*    This function will stop all communication activities and removes the
*    CANopen node from the CAN bus interface.
*
* \note
*    To reactivate a stopped CANopen node, the functions \ref CONodeInit()
*    and \ref CONodeStart() must be called again.
*
* \param node
*    pointer to the CANopen node object
*/
void CONodeStop(CO_NODE *node);

/*! \brief  GET NODE ERROR
*
*    This function returns the current error status of the given node. If an
*    error was detected, the error is cleared with this function call.
*
* \param node
*    pointer to the CANopen node object
*
* \return
*    One of the error codes in \ref CO_ERR
*/
CO_ERR CONodeGetErr(CO_NODE *node);

/*! \brief  CAN RECEIVE PROCESSING
*
*    This function processes one received CAN frame from the given CAN node
*    and initiates the specific protocol activity. If the CAN frame is not
*    handled by the stack, the user will get this CAN frame into the
*    (optional) callback function \see CO_IfReceive()
*
* \param node
*    Ptr to node info
*/
void CONodeProcess(CO_NODE *node);

/******************************************************************************
* CALLBACK FUNCTIONS
******************************************************************************/

/*! \brief  FATAL ERROR CALLBACK
*
*    This function is called, after detecting a fatal error within the
*    stack, and no way out of the situation ('panic'). The function is
*    intended to allow the implementation of a pre-defined shutdown sequence
*    and setting the device in a safe state.
*/
extern void CONodeFatalError(void);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_CORE_H_ */
