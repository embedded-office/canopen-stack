/*
****************************************************************************************************
* (c) copyright by
*     Embedded Office GmbH & Co. KG       Tel : (07522) 97 00 08-0
*     Friedrich-Ebert-Str. 20/1           Fax : (07522) 97 00 08-99
*     D-88239 Wangen                      Mail: info@embedded-office.de
*                                         Web : http://www.embedded-office.de
*
* All rights reserved. Confidential and Proprietary. Protected by international copyright laws.
* Knowledge of the source code may not be used to write a similar product.
* This file may only be used in accordance with a license and should not be
* redistributed in any way.
****************************************************************************************************
*/
/*!
****************************************************************************************************
* \file     co_tmr.h
*
* \brief    HIGHSPEED TIMER MANAGEMENT DEFINITIONS
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_tmr.h#3 $
*
*           This include file defines the symbolic constants and data types for the timer
*           management.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_TMR_H_
#define CO_TMR_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
****************************************************************************************************
*                                            INCLUDES
****************************************************************************************************
*/

#include "cpu.h"
#include "co_cfg.h"

/*
****************************************************************************************************
*                                             DEFINES
****************************************************************************************************
*/

#if CO_TMR_TICKS_PER_SEC <= 1000
#define CO_TMR_MIN_DELTA  (1000/CO_TMR_TICKS_PER_SEC) /*!< minimum time difference in ms          */
#else
#define CO_TMR_MIN_DELTA  1                           /*!< minimum time difference in ms          */
#endif

/*
****************************************************************************************************
*                                              MACROS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief RELOAD TIMER
*
* \ingroup INTERNAL
*
*          This macro reloads the timer value to the next delay.
*
* \note    This macro provides the advanced configuration support to hardware driven timers. In
*          typical application, this macro needs not to be changed.
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_TMR_RELOAD(tmr,x)     { tmr->Delay=x; tmr->Time+=x; }

/*------------------------------------------------------------------------------------------------*/
/*! \brief GET TIMER VALUE
*
* \ingroup INTERNAL
*
*          This macro reads the timer value.
*
* \note    This macro provides the advanced configuration support to hardware driven timers. In
*          typical application, this macro needs not to be changed.
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_TMR_DELAY(tmr)        tmr->Delay

/*------------------------------------------------------------------------------------------------*/
/*! \brief STOP TIMER
*
* \ingroup INTERNAL
*
*          This macro stops the timer.
*
* \note    This macro provides the advanced configuration support to hardware driven timers. In
*          typical application, this macro needs not to be changed.
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_TMR_STOP(tmr)         { tmr->Delay=0; tmr->Time=0; }

/*------------------------------------------------------------------------------------------------*/
/*! \brief START TIMER
*
* \ingroup INTERNAL
*
*          This macro (re-)starts the timer.
*
* \note    This macro provides the advanced configuration support to hardware driven timers. In
*          typical application, this macro needs not to be changed.
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_TMR_START(tmr)

/*------------------------------------------------------------------------------------------------*/
/*! \brief UPDATE TIMER
*
* \ingroup INTERNAL
*
*          This macro updates the timer value.
*
* \note    This macro provides the advanced configuration support to hardware driven timers. In
*          typical application, this macro needs not to be changed.
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_TMR_UPDATE(tmr)       { tmr->Delay--; }

/*------------------------------------------------------------------------------------------------*/
/*! \brief CALCULATE TIMER TICKS
*
* \ingroup INTERNAL
*
*          This macro calculates the number of timer ticks for a given time in ms.
*/
/*------------------------------------------------------------------------------------------------*/
#define CO_TMR_TICKS(ms)     ((ms)/(1000/CO_TMR_TICKS_PER_SEC))

/*
****************************************************************************************************
*                                          TYPE DEFINITIONS
****************************************************************************************************
*/

typedef void (*CO_TMR_FUNC)(void *arg_p);             /*!< Timer Callback function prototype      */

/*------------------------------------------------------------------------------------------------*/
/*! \brief TIMER ACTION
*
* \ingroup TMR
*
*          This structure holds all data, which are needed for managing a timed action.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_TMR_ACTION_T {
    CPU_INT16U              Id;                       /*!< unique action identifier               */
    struct CO_TMR_ACTION_T *Next;                     /*!< link to next action                    */
    CO_TMR_FUNC             Func;                     /*!< pointer to callback function           */
    void                   *Para;                     /*!< callback function parameter            */
    CPU_INT32U              CycleTime;                /*!< action cycle time in ticks             */

} CO_TMR_ACTION;

/*------------------------------------------------------------------------------------------------*/
/*! \brief TIMER EVENT
*
* \ingroup TMR
*
*          This structure holds all data, which are needed for managing a timer event.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_TMR_TIME_T {
    struct CO_TMR_TIME_T   *Next;                     /*!< link to next timer                     */
    struct CO_TMR_ACTION_T *Action;                   /*!< root of linked action list             */
    struct CO_TMR_ACTION_T *ActionEnd;                /*!< last element in linked action list     */
    CPU_INT32U              Delta;                    /*!< delta ticks from previous timer event  */

} CO_TMR_TIME;

/*------------------------------------------------------------------------------------------------*/
/*! \brief TIMER MEMORY
*
* \ingroup TMR
*
*          This structure is intended to simplify the memory allocation in the application. The
*          number of action and timer structures are always the same, therefore we can reduce the
*          configuration effort to the memory array, and the length of this memory array.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_TMR_MEM_T {                         /*!< This structure is used for easy        *
                                                       *   allocation of sufficient memory for    *
                                                       *   timer internal usage.                  */
    CO_TMR_ACTION           Act;                      /*!< memory portion for action info         */
    CO_TMR_TIME             Tmr;                      /*!< memory portion for timer info          */

} CO_TMR_MEM;

/*------------------------------------------------------------------------------------------------*/
/*! \brief TIMER MANAGEMENT
*
* \ingroup TMR
*
*          This structure holds all data, which are needed for managing the highspeed timer events.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_TMR_T {
    struct CO_NODE_T       *Node;                     /*!< Link to parent node                    */
    CPU_INT32U              Max;                      /*!< Num. of elements in action/event pool  */
    struct CO_TMR_ACTION_T *APool;                    /*!< Timer action pool                      */
    struct CO_TMR_TIME_T   *TPool;                    /*!< Timer event pool                       */
    struct CO_TMR_ACTION_T *Acts;                     /*!< Timer action free linked list          */
    struct CO_TMR_TIME_T   *Free;                     /*!< Timer event free linked list           */
    struct CO_TMR_TIME_T   *Use;                      /*!< Timer event used linked list           */
    struct CO_TMR_TIME_T   *Elapsed;                  /*!< Timer event elapsed linked list        */
    CPU_INT32U              Time;                     /*!< Time ticks of next event since create  */
    CPU_INT32U              Delay;                    /*!< Time ticks of next event from now      */

} CO_TMR;

/*
****************************************************************************************************
*                                        FUNCTION PROTOTYPES
****************************************************************************************************
*/

CPU_INT16S COTmrCreate   (CO_TMR *tmr, CPU_INT32U start, CPU_INT32U cycle, CO_TMR_FUNC func, void *para);
CPU_INT16S COTmrDelete   (CO_TMR *tmr, CPU_INT16S actId);
CPU_INT16S COTmrService  (CO_TMR *tmr);
void       COTmrProcess  (CO_TMR *tmr);

/*
****************************************************************************************************
*                                    INTERNAL FUNCTION PROTOTYPES
****************************************************************************************************
*/

void       CO_TmrInit    (CO_TMR *tmr, struct CO_NODE_T *node, CO_TMR_MEM *mem, CPU_INT16U num);
void       CO_TmrReset   (CO_TMR *tmr);
void       CO_TmrClear   (CO_TMR *tmr);

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_TMR_H_ */
