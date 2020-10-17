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

#ifndef CO_TMR_H_
#define CO_TMR_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"
#include "co_cfg.h"

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

#define CO_TMR_UNIT_1MS          1000
#define CO_TMR_UNIT_100US        10000

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

/*! \brief TIMER CALLBACK
*
*    This type specifies the timer callback function prototype.
*/
typedef void (*CO_TMR_FUNC)(void *arg_p);


/*! \brief TIMER ACTION
*
*    This structure holds all data, which are needed for managing a
*    timed action.
*/
typedef struct CO_TMR_ACTION_T {
    uint16_t                Id;            /*!< unique action identifier     */
    struct CO_TMR_ACTION_T *Next;          /*!< link to next action          */
    CO_TMR_FUNC             Func;          /*!< pointer to callback function */
    void                   *Para;          /*!< callback function parameter  */
    uint32_t                CycleTicks;    /*!< action cycle time in ticks   */

} CO_TMR_ACTION;

/*! \brief TIMER EVENT
*
*    This structure holds all data, which are needed for managing a
*    timer event.
*/
typedef struct CO_TMR_TIME_T {
    struct CO_TMR_TIME_T   *Next;       /*!< link to next timer              */
    struct CO_TMR_ACTION_T *Action;     /*!< root of action list             */
    struct CO_TMR_ACTION_T *ActionEnd;  /*!< last element in action list     */
    uint32_t                Delta;      /*!< delta ticks from previous event */

} CO_TMR_TIME;

/*! \brief TIMER MEMORY
*
*    This structure is intended to simplify the memory allocation in the
*    application. The number of action and timer structures are always
*    the same, therefore we can reduce the configuration effort to the
*    memory array, and the length of this memory array.
*/
typedef struct CO_TMR_MEM_T {
    CO_TMR_ACTION  Act;                  /*!< memory portion for action info */
    CO_TMR_TIME    Tmr;                  /*!< memory portion for timer info  */

} CO_TMR_MEM;

/*! \brief TIMER MANAGEMENT
*
*    This structure holds all data, which are needed for managing the
*    highspeed timer events.
*/
typedef struct CO_TMR_T {
    struct CO_NODE_T       *Node;      /*!< Link to parent node              */
    uint32_t                Max;       /*!< Num. of elements in pools        */
    struct CO_TMR_ACTION_T *APool;     /*!< Timer action pool                */
    struct CO_TMR_TIME_T   *TPool;     /*!< Timer event pool                 */
    struct CO_TMR_ACTION_T *Acts;      /*!< Timer action free list           */
    struct CO_TMR_TIME_T   *Free;      /*!< Timer event free list            */
    struct CO_TMR_TIME_T   *Use;       /*!< Timer event used list            */
    struct CO_TMR_TIME_T   *Elapsed;   /*!< Timer event elapsed list         */
    uint32_t                Freq;      /*!< Timer ticks per second           */

} CO_TMR;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*! \brief  GET TICKS FOR MILLISECONDS
*
*    This function converts a given time into the corresponding number
*    of timer ticks. This depends on the specific timer driver and 
*    clock frequency of the timer.
*
* \param time
*    time in given unit
*
* \param unit
*    unit of given time (CO_TMR_UNIT_1MS or CO_TMR_UNIT_100US)
*
* \return
*    time ticks which represents at least the given time
*/
uint32_t COTmrGetTicks(CO_TMR *tmr, uint16_t time, uint32_t unit);

/*! \brief  GET MINIMAL POSSIBLE TIME
*
*    This function calculates the minimal possible time for timer
*    management. This depends on the specific timer driver and clock
*    frequency of the timer.
*
* \param unit
*    unit of returned time (CO_TMR_UNIT_1MS or CO_TMR_UNIT_100US)
*
* \return
*    minimal possible time (which represents a single tick)
*/
uint16_t COTmrGetMinTime(CO_TMR *tmr, uint32_t unit);

/*! \brief CREATE TIMER
*
*    This function creates the defined action and links this action into
*    the event timer list at the correct timing.
*
* \param tmr
*    Pointer to timer structure
*
* \param startTicks
*    delta time in ticks for the first timer event
*
* \param cycleTicks
*    if != 0, the delta time in ticks for the cyclic timer events
*
* \param func
*    pointer to the action callback function
*
* \param para
*    pointer to the callback function parameter
*
* \retval  >=0    the action identifier
* \retval  <0     an error is detected
*/
int16_t COTmrCreate(CO_TMR      *tmr,
                    uint32_t     startTicks,
                    uint32_t     cycleTicks,
                    CO_TMR_FUNC  func,
                    void        *para);

/*! \brief DELETE TIMER
*
*    This function deletes the defined action and removes the timer, if this
*    was the last action for this timer event.
*
* \param tmr
*    Pointer to timer structure
*
* \param actId
*    action identifier, returned by COTmrCreate()
*
* \retval  =0    successful operation
* \retval  <0    an error is detected
*/
int16_t COTmrDelete(CO_TMR *tmr, int16_t actId);

/*! \brief TIMER SERVICE
*
*    This function is unsed only for cyclic mode, therefore the function shall
*    be called with the minimum cycletime CO_TMR_MIN_DELAY.
*
*    The timer event will be checked to be elapsed and removed from the used
*    timer event list. For further activities (e.g. calling the callback
*    function), the pointer to the event timer data will be posted to a
*    message box.
*
* \param tmr
*    Pointer to timer structure
*
* \retval  =0    no timer elapsed
* \retval  >0    timer was elapsed
* \retval  <0    an error is detected
*/
int16_t COTmrService(CO_TMR *tmr);

/*! \brief PROCESS ELAPSED TIMER ACTIONS
*
*    This function handles all actions after an event timer is elapsed. The
*    function removes the timer info from the used timer event list and
*    performs all actions of the linked action list.
*
*    For all actions with cycletime not equal zero, a new event will be
*    created.
*
* \param tmr
*    Pointer to timer structure
*/
void COTmrProcess(CO_TMR *tmr);

/******************************************************************************
* PROTECTED FUNCTIONS
******************************************************************************/

/*! \brief INITIALIZE TIMER
*
*    This function initializes the timer structure.
*
* \param tmr
*    Pointer to timer structure
*
* \param node
*    Pointer to parent node
*
* \param mem
*    Pointer to memory block array
*
* \param num
*    Length of memory block array
*
* \param freq
*    Frequency of timer clock
*/
void COTmrInit(CO_TMR           *tmr,
               struct CO_NODE_T *node,
               CO_TMR_MEM       *mem,
               uint16_t          num,
               uint32_t          freq);

/*! \brief CLEAR TIMER
*
*    This function deletes all timers, which are created within the CANopen
*    stack. The timers created by the application will still be active after
*    this function call.
*
* \param tmr
*    Pointer to timer structure
*/
void COTmrClear(CO_TMR *tmr);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_TMR_H_ */
