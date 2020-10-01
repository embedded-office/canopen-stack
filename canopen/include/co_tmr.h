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

#ifdef __cplusplus               /* for compatibility with C++ environments  */               /* for compatibility with C++ environments  */
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

#ifndef CO_TMR_TICKS_PER_SEC
#define CO_TMR_TICKS_PER_SEC     100             /* default tick rate: 100Hz */
#endif

/******************************************************************************
* PUBLIC MACROS
******************************************************************************/

/*! \brief CALCULATE TIMER TICKS
*
*    This macro calculates the number of ticks for a given time in ms.
*/
#if (CO_TMR_TICKS_PER_SEC <= 1000)
#define CO_TMR_TICKS(ms)         ((ms)/(1000/CO_TMR_TICKS_PER_SEC))
#else
#define CO_TMR_TICKS(ms)         ((ms)*(CO_TMR_TICKS_PER_SEC/1000))
#endif

/******************************************************************************
* PRIVATE MACROS
******************************************************************************/

/*! \brief RELOAD TIMER
*
*    This macro reloads the timer value to the next delay.
*
* \internal
*/
#define CO_TMR_RELOAD(tmr,x)     do { tmr->Delay=x; tmr->Time+=x; } while(0)

/*! \brief GET TIMER VALUE
*
*    This macro reads the timer value.
*
* \internal
*/
#define CO_TMR_DELAY(tmr)        tmr->Delay

/*! \brief STOP TIMER
*
*    This macro stops the timer.
*
* \internal
*/
#define CO_TMR_STOP(tmr)         do { tmr->Delay=0; tmr->Time=0; } while(0)

/*! \brief START TIMER
*
*    This macro (re-)starts the timer.
*/
#define CO_TMR_START(tmr)

/*! \brief UPDATE TIMER
*
*    This macro updates the timer value.
*
* \internal
*/
#define CO_TMR_UPDATE(tmr)       do { tmr->Delay--; } while(0)

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
    uint32_t                CycleTime;     /*!< action cycle time in ticks   */

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
    uint32_t                Time;      /*!< Ticks of next event since create */
    uint32_t                Delay;     /*!< Ticks of next event from now     */

} CO_TMR;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*! \brief CREATE TIMER
*
*    This function creates the defined action and links this action into
*    the event timer list at the correct timing.
*
* \param tmr
*    Pointer to timer structure
*
* \param startTime
*    delta time in ticks for the first timer event
*
* \param cycleTime
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
                    uint32_t     start,
                    uint32_t     cycle,
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
* PRIVATE FUNCTIONS
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
* \internal
*/
void COTmrInit(CO_TMR           *tmr,
               struct CO_NODE_T *node,
               CO_TMR_MEM       *mem,
               uint16_t          num);

/*! \brief CLEAR TIMER
*
*    This function deletes all timers, which are created within the CANopen
*    stack. The timers created by the application will still be active after
*    this function call.
*
* \param tmr
*    Pointer to timer structure
*
* \internal
*/
void COTmrClear(CO_TMR *tmr);

/*! \brief RESET TIMER
*
*    This function creates the linked lists of the unused action info and
*    event time structures.
*
* \param tmr
*    Pointer to timer structure
*
* \internal
*/
void COTmrReset(CO_TMR *tmr);

/*! \brief INSERT TIMER
*
*    This function inserts an action into the used timer list. First, this
*    function checks for an existing timer event on the same time and links
*    the action to the existing timer info. If no timer event exists on the
*    same time, a new timer event will be created and inserted into the used
*    timer list in a way, that the resulting used timer list is a sorted list
*    with precalculated delta times between the time events.
*
* \param tmr
*    Pointer to timer structure
*
* \param dTnew
*    deltatime for new action
*
* \param action
*    pointer to action info structure
*
* \return
*    This function returns the pointer to the event timer, which contains the
*    new action.
*
* \internal
*/
CO_TMR_TIME *COTmrInsert(CO_TMR *tmr, uint32_t dTnew, CO_TMR_ACTION *action);

/*! \brief REMOVE TIMER
*
*    This function removes timer info from the used timer list.
*
* \param tmr
*    Pointer to timer structure
*
* \param tx
*    Pointer to timer info structure, which shall be removed from the list
*
* \internal
*/
void COTmrRemove(CO_TMR *tmr, CO_TMR_TIME *tx);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_TMR_H_ */
