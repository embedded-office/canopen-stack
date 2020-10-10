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

#ifndef CO_IF_TIMER_H_
#define CO_IF_TIMER_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

struct CO_IF_T;                /* Declaration of interface structure         */

typedef void     (*CO_IF_TIMER_INIT_FUNC  )(uint32_t);
typedef void     (*CO_IF_TIMER_RELOAD_FUNC)(uint32_t);
typedef uint32_t (*CO_IF_TIMER_DELAY_FUNC )(void);
typedef void     (*CO_IF_TIMER_STOP_FUNC  )(void);
typedef void     (*CO_IF_TIMER_START_FUNC )(void);
typedef uint8_t  (*CO_IF_TIMER_UPDATE_FUNC)(void);

typedef struct CO_IF_TIMER_DRV_T {
    CO_IF_TIMER_INIT_FUNC   Init;
    CO_IF_TIMER_RELOAD_FUNC Reload;
    CO_IF_TIMER_DELAY_FUNC  Delay;
    CO_IF_TIMER_STOP_FUNC   Stop;
    CO_IF_TIMER_START_FUNC  Start;
    CO_IF_TIMER_UPDATE_FUNC Update;
} CO_IF_TIMER_DRV;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*! \brief  RELOAD TIMER COUNTER
*
*    This function reloads the hardware timer tick.
*
* \param cif
*    pointer to the interface structure
*
* \param reload
*    the timer tick reload value
*/
void COIfTimerReload(struct CO_IF_T *cif, uint32_t reload);

/*! \brief  GET TIMER DELAY
*
*    This function returns the delay in timer ticks until the next timer 
*    event occurs.
*
* \param cif
*    pointer to the interface structure
*
* \return
*    timer ticks until timer is elapsed
*/
uint32_t COIfTimerDelay(struct CO_IF_T *cif);

/*! \brief  STOP TIMER
*
*    This function stops the hardware timer.
*
* \param cif
*    pointer to the interface structure
*/
void COIfTimerStop(struct CO_IF_T *cif);

/*! \brief  START TIMER
*
*    This function starts the hardware timer.
*
* \param cif
*    pointer to the interface structure
*/
void COIfTimerStart(struct CO_IF_T *cif);

/*! \brief  UPDATE TIMER COUNTER
*
*    This function updates the hardware timer counter value.
*
* \param cif
*    pointer to the interface structure
*
* \retval >0   timer event is elapsed
* \retval =0   timer event is not elapsed
*/
uint8_t COIfTimerUpdate(struct CO_IF_T *cif);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif /* CO_IF_TIMER_H_ */
