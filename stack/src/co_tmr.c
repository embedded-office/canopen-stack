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
* \file        co_tmr.c
*
* \brief       HIGHSPEED TIMER MANAGEMENT
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_tmr.c#4 $
*
*              This file implements a highspeed timer management. The time events are
*              stored in a linked list with delta-times between the events. Each timer
*              event holds the root of another linked list, which contains the neccessary
*              actions for this event. See the following figure for an overview:
* \code
*              FreeTmr -> T4 -> T5 -> T6 -> T7 -> 0
*              FreeAct -> A5 -> A6 -> A7 -> 0
*
*                         |--dt--|
*              UsedTmr -> T1 -> T3 -> T2 -> T0 -> 0
*                         ||    ||    ||    |+-> A0 -> 0
*                         ||    ||    ||    +----^
*                         ||    ||    |+-------> A3 -> 0
*                         ||    ||    +----------^
*                         ||    |+-------------> A4 -> 0
*                         ||    +----------------^
*                         |+-------------------> A1 -> A2 -> 0
*                         +----------------------------^
* \endcode
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

/*
****************************************************************************************************
*                                            INCLUDES
****************************************************************************************************
*/

#include "co_core.h"

/*
****************************************************************************************************
*                                     LOCAL FUNCTION PROTOTYPES
****************************************************************************************************
*/

static CO_TMR_TIME *CO_TmrInsert(CO_TMR *tmr, uint32_t dTnew, CO_TMR_ACTION *action);
static void         CO_TmrRemove(CO_TMR *tmr, CO_TMR_TIME *tx);

/*
****************************************************************************************************
*                                            FUNCTIONS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief CREATE TIMER
*
* \ingroup API
*
*          This function creates the defined action and links this action into the event
*          timer list at the correct timing.
*
* \param[in,out]   tmr         Pointer to timer structure
*
* \param[in]       startTime   delta time in ticks for the first timer event
*
* \param[in]       cycleTime   if != 0, the delta time in ticks for the cyclic timer events
*
* \param[in]       func        pointer to the action callback function
*
* \param[in]       para        pointer to the callback function parameter
*
* \retval   >=0    the action identifier
* \retval   <0     an error is detected
*/
/*------------------------------------------------------------------------------------------------*/
int16_t COTmrCreate(CO_TMR *tmr, uint32_t startTime, uint32_t cycleTime, CO_TMR_FUNC func, void *para)
{
    CO_TMR_ACTION *act;                               /* Local: pointer to an action              */
    CO_TMR_TIME   *tn;                                /* Local: pointer to timer info             */
    int16_t     result;                            /* Local: function result                   */
                                                      /*------------------------------------------*/
    CPU_SR_ALLOC();                                   /* allocate space for status                */

    if (tmr == 0) {                                   /* see, if obj ptr parameters is invalid    */
        CO_NodeFatalError();                          /* inform user                              */
        return -1;                                    /* abort further processing                 */
    }
    if (startTime == 0) {                             /* see, if starttime is 0 (zero)            */
        startTime = cycleTime;                        /* last chance: set starttime to cycletime  */
    }
    if ((startTime == 0) &&                           /* see, if starttime and cycletime are 0    */
        (cycleTime == 0)) {
        return -1;                                    /* abort further processing                 */
    }
    if (tmr->Node == 0) {                             /* see, if timer module is not initialized  */
        CO_NodeFatalError();                          /* inform user                              */
        return -1;                                    /* abort further processing                 */
    }
    if (func == 0) {                                  /* see, if callback function pointer is bad */
        tmr->Node->Error = CO_ERR_BAD_ARG;            /* set timer error code                     */
        return -1;                                    /* abort further processing                 */
    }
    if (tmr->Acts == 0) {                             /* see, if no actions are possible          */
        tmr->Node->Error = CO_ERR_TMR_NO_ACT;         /* set timer error code                     */
        return -1;                                    /* abort further processing                 */
    }                                                 /*------------------------------------------*/
    CPU_CRITICAL_ENTER();                             /* lock shared timer resources              */
                                                      /*------------------------------------------*/
    act            = tmr->Acts;                       /* fetch first action from action pool list */
    tmr->Acts      = act->Next;                       /* set first action to next action in list  */
    act->Next      = 0;                               /* clear action list pointer                */
    act->Func      = func;                            /* set action callback function             */
    act->Para      = para;                            /* set callback parameter                   */
    act->CycleTime = cycleTime;                       /* set action cycletime                     */
                                                      /*------------------------------------------*/
    tn = CO_TmrInsert(tmr, startTime, act);           /* insert event timer with created action   */
    if (tn == (CO_TMR_TIME*)0) {                      /* see, if an error is detected             */
        act->CycleTime   = 0;                         /* clear action cycletime                   */
        act->Para        = 0;                         /* clear callback parameter                 */
        act->Func        = (CO_TMR_FUNC)0;            /* clear action callback function           */
        act->Next        = tmr->Acts;                 /* put back the action into action pool     */
        tmr->Acts        = act;
        tmr->Node->Error = CO_ERR_TMR_INSERT;         /* set timer error                          */
        result           = -1;                        /* indicate function error to caller        */
    } else {                                          /* otherwise: no error detected             */
        result = (int16_t)(act->Id);               /* return action identifier                 */
    }
    CPU_CRITICAL_EXIT();                              /* unlock shared timer resources            */
                                                      /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief DELETE TIMER
*
* \ingroup API
*
*          This function deletes the defined action and removes the timer, if this was the
*          last action for this timer event.
*
* \param[in,out]   tmr         Pointer to timer structure
*
* \param[in]       actId       action identifier, returned by COTmrCreate()
*
* \retval  =0      successful operation
* \retval  <0      an error is detected
*/
/*------------------------------------------------------------------------------------------------*/
int16_t COTmrDelete(CO_TMR *tmr, int16_t actId)
{
    CO_TMR_TIME   *tx;                                /* Local: pointer to timer event            */
    CO_TMR_ACTION *act;                               /* Local: pointer to action info            */
    CO_TMR_ACTION *prev;                              /* Local: pointer to previous action info   */
    CO_TMR_ACTION *del    = 0;                        /* Local: pointer to action info            */
    int16_t     result = -1;                       /* Local: function result                   */
                                                      /*------------------------------------------*/
    CPU_SR_ALLOC();                                   /* allocate space for status                */
    if ( (actId < 0) ||                               /* see, if actId is not in range            */
         (actId >= (int16_t)(tmr->Max)) ) {
        return -1;                                    /* error                                    */
    }
    CPU_CRITICAL_ENTER();                             /* lock shared timer resources              */
                                                      /*------------------------------------------*/
    tx    = tmr->Use;                                 /* set timer pointer to root of timer list  */
    while ((tx != 0) && (del == 0)) {                 /* loop through timer list until act found  */
        act = tx->Action;                             /* get root of action list of timer event   */
        if (act->Id == (uint16_t)actId) {           /*--- FIRST ACTION IN ACTION LIST ---       */
            del        = act;                         /* remember action info for deletion        */
            tx->Action = act->Next;                   /* remove action from action list           */
        } else {                                      /* otherwise: first action does not match   */
            prev       = act;                         /* set previous action info                 */
            act        = act->Next;                   /* set next action info                     */
            while ((act != 0) && (del == 0)) {        /* until end of list OR del action found    */
                if (act->Id == (uint16_t)actId) {   /* see, if action id matches given id       */
                    del        = act;                 /*  remember action info for deletion       */
                    prev->Next = act->Next;           /* remove action from list                  */
                    if (act->Next == 0) {             /*--- LAST ACTION IN ACTION LIST ---        */
                        tx->ActionEnd = prev;         /* update pointer to end of action list     */
                    }
                }
                prev = act;                           /* update previous action in action list    */
                act  = act->Next;                     /* switch to next action in action list     */
            }
        }                                             /*------------------------------------------*/
        if (del == 0) {                               /* see, if action is not found              */
            tx = tx->Next;                            /* switch to next timer event in timer list */
        }
    }                                                 /*------------------------------------------*/
    if (del == 0) {                                   /* see, if action is not found in used list */
        tx = tmr->Elapsed;                            /* set timer pointer to root of elapsed list*/
        while ((tx != 0) && (del == 0)) {             /* loop through timer list until act found  */
            act = tx->Action;                         /* get root of action list of timer event   */
            if (act->Id == (uint16_t)actId) {       /*--- FIRST ACTION IN ACTION LIST ----------*/
                del        = act;                     /* remember action info for deletion        */
                tx->Action = act->Next;               /* remove action from action list           */
            } else {                                  /* otherwise: first action does not match   */
                prev       = act;                     /* set previous action info                 */
                act        = act->Next;               /* set next action info                     */
                while ((act != 0) && (del == 0)) {    /* until end of list OR del action found    */
                    if (act->Id == (uint16_t)actId) { /* see, if action id matches given id     */
                        del        = act;             /*  remember action info for deletion       */
                        prev->Next = act->Next;       /* remove action from list                  */
                        if (act->Next == 0) {         /*--- LAST ACTION IN ACTION LIST -----------*/
                            tx->ActionEnd = prev;     /* update pointer to end of action list     */
                        }
                    }
                    prev = act;                       /* update previous action in action list    */
                    act  = act->Next;                 /* switch to next action in action list     */
                }
            }                                         /*------------------------------------------*/
            if (del == 0) {                           /* see, if action is not found              */
                tx = tx->Next;                        /* switch to next timer event in timer list */
            }
        }                                             /*------------------------------------------*/
    }
    if (del != 0) {                                   /* see, if delete action is found           */
        del->CycleTime = 0;                           /* clear cycletime of deleted action        */
        del->Para      = 0;                           /* clear callback function parameter        */
        del->Func      = (CO_TMR_FUNC)0;              /* clear pointer to callback function       */
        del->Next      = tmr->Acts;                   /* link freed action in front of            */
        tmr->Acts      = del;                         /*   free action list                       */
                                                      /*------------------------------------------*/
        if (tx != 0) {                                /* see, if timer is valid                   */
            if (tx->Action == (CO_TMR_ACTION*)0) {    /* see, if action was last action for timer */
                tx->ActionEnd = 0;                    /* clear action list end pointer            */
                CO_TmrRemove(tmr, tx);                /* yes: remove timer from used timer list   */
            }
            result = 0;                               /* indicate successfull operation           */
        }
    }                                                 /*------------------------------------------*/
    CPU_CRITICAL_EXIT();                              /* unlock shared timer resources            */
    return (result);                                  /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief TIMER SERVICE
*
* \ingroup API
*
*          This function is unsed only for cyclic mode, therefore the function shall be called
*          with the minimum cycletime CO_TMR_MIN_DELAY.
*
*          The timer event will be checked to be elapsed and removed from the used timer event
*          list. For further activities (e.g. calling the callback function), the pointer to
*          the event timer data will be posted to a message box.
*
* \param[in,out]   tmr        Pointer to timer structure
*
* \retval  =0      no timer elapsed
* \retval  >0      timer was elapsed
* \retval  <0      an error is detected
*/
/*------------------------------------------------------------------------------------------------*/
int16_t COTmrService(CO_TMR *tmr)
{
    CO_TMR_TIME *tn;                                  /* Local: pointer to elapsed timer event    */
    int16_t   result = 0;                          /* Local: function result                   */
    CPU_SR_ALLOC();                                   /* allocate space for status                */
                                                      /*------------------------------------------*/
    if (tmr == 0) {                                   /* see, if obj ptr parameters is invalid    */
        CO_NodeFatalError();                          /* inform user                              */
        return -1;                                    /* abort further processing                 */
    }

    CPU_CRITICAL_ENTER();                             /* lock shared timer resources              */
    if (tmr->Delay > 0) {                             /* see, if a timer event delay is present   */
        CO_TMR_UPDATE(tmr);                           /* decrement remaining event delay          */
        if (tmr->Delay == 0) {                        /* see, if event is elapsed                 */
            tn       = tmr->Use;                      /* get pointer to timer event data          */
            tmr->Use = tn->Next;                      /* and remove this event from event list    */
            tn->Next = 0;
            if (tmr->Use != 0) {                      /* see, if further timer events exists      */
                CO_TMR_RELOAD(tmr, tmr->Use->Delta);  /* yes: reload event delay counter          */
            } else {                                  /* otherwise: no further event in list      */
                CO_TMR_STOP(tmr);                     /* stop timer                               */
            }
            if (tmr->Elapsed == 0) {                  /* see, if elapsed timer list is empty      */
                tmr->Elapsed = tn;                    /* yes: set timer in elapsed list front     */
            } else {                                  /* otherwise: elapsed list is not empty     */
                tn->Next     = tmr->Elapsed;          /* place elapsed timer in front of list     */
                tmr->Elapsed = tn;
            }
            result = 1;                               /* indicate elapsed timer                   */
        }
    }
    CPU_CRITICAL_EXIT();                              /* unlock shared timer resources            */
                                                      /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief PROCESS ELAPSED TIMER ACTIONS
*
* \ingroup API
*
*          This function handles all actions after an event timer is elapsed. The function
*          removes the timer info from the used timer event list and performs all actions of
*          the linked action list.
*
*          For all actions with cycletime not equal zero, a new event will be created.
*
* \param[in,out]   tmr        Pointer to timer structure
*/
/*------------------------------------------------------------------------------------------------*/
void COTmrProcess(CO_TMR *tmr)
{
    CO_TMR_TIME   *tn;                                /* Local: pointer to timer info             */
    CO_TMR_TIME   *res;                               /* Local: pointer to timer info             */
    CO_TMR_ACTION *act;                               /* Local: pointer to current action info    */
    CO_TMR_ACTION *next;                              /* Local: pointer to next action info       */
    CO_TMR_FUNC    func;                              /* Local: pointer to callback function      */
    void          *para;                              /* Local: pointer to callback parameter     */
                                                      /*------------------------------------------*/
    CPU_SR_ALLOC();                                   /* allocate space for status                */
                                                      /*------------------------------------------*/
    while (tmr->Elapsed != 0) {                       /* loop through all elapsed timer           */
                                                      /*------------------------------------------*/
        CPU_CRITICAL_ENTER();                         /* lock shared timer resources              */
        tn            = tmr->Elapsed;                 /* get first elapsed timer event            */
        tmr->Elapsed  = tn->Next;                     /* update elapsed timer list                */

        act           = tn->Action;                   /* get action list out of timer event       */
        tn->Action    = 0;                            /* clear action list in timer event         */
        tn->ActionEnd = 0;                            /* clear pointer to end of action list      */
        tn->Delta     = 0;                            /* clear delta time in timer event          */
        tn->Next      = tmr->Free;                    /* link free timer event in front           */
        tmr->Free     = tn;                           /*   of free timer event list               */
        CPU_CRITICAL_EXIT();                          /* release shared timer resources           */

        while (act != 0) {                            /* traverse through linked action list      */
            next      = act->Next;                    /* hold pointer to next action              */
            act->Next = 0;                            /* clear next action pointer in action      */
            func      = act->Func;                    /* get callback function pointer            */
            para      = act->Para;                    /* get pointer to callback parameter        */
                                                      /*------------------------------------------*/
            if (act->CycleTime == 0) {                /* see, if timer event is a single event    */
                act->Para = 0;                        /* clear callback parameter pointer         */
                act->Func = (CO_TMR_FUNC)0;           /* clear callback function pointer          */
                CPU_CRITICAL_ENTER();                 /* lock shared timer resources              */
                act->Next = tmr->Acts;                /* link free action structure in front      */
                tmr->Acts = act;                      /*   of free action list                    */
                CPU_CRITICAL_EXIT();                  /* release shared timer resources           */
                                                      /*------------------------------------------*/
            } else {                                  /* otherwise: cyclic timer event            */
                CPU_CRITICAL_ENTER();                 /* lock shared timer resources              */
                res = CO_TmrInsert(tmr, act->CycleTime, act); /* insert new timer event           */
                CPU_CRITICAL_EXIT();                  /* release shared timer resources           */
                if (res == (CO_TMR_TIME*)0) {
                    tmr->Node->Error = CO_ERR_TMR_CREATE;/* set timer error                       */
                }
            }                                         /*------------------------------------------*/
            func(para);                               /* execute callback function                */
            act = next;                               /* switch to next action in list            */
        }
    }
}

/*
****************************************************************************************************
*                                        INTERNAL FUNCTION
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief INITIALIZE TIMER
*
* \ingroup INTERNAL
*
*          This function initializes the timer structure.
*
* \param[in,out]   tmr        Pointer to timer structure
*
* \param[in]       node       Pointer to parent node
*
* \param[in]       mem        Pointer to memory block array
*
* \param[in]       num        Length of memory block array
*/
/*------------------------------------------------------------------------------------------------*/
void CO_TmrInit(CO_TMR *tmr, CO_NODE *node, CO_TMR_MEM *mem, uint16_t num)
{
    tmr->Node  = node;                                /* store parent node information            */
    tmr->Max   = num;                                 /* set number of elements in act/tmr-lists  */
    tmr->TPool = &mem->Tmr;                           /* set root of timer pool                   */
    tmr->APool = &mem->Act;                           /* set root of action pool                  */
                                                      /*------------------------------------------*/
    CO_TmrReset(tmr);                                 /* reset timers (create linked list)        */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief RESET TIMER
*
* \ingroup INTERNAL
*
*          This function creates the linked lists of the unused action info and event time
*          structures.
*
* \param[in,out]   tmr        Pointer to timer structure
*/
/*------------------------------------------------------------------------------------------------*/
void CO_TmrReset(CO_TMR *tmr)
{
    CO_TMR_MEM    *mem = (CO_TMR_MEM *)tmr->APool;    /* Local: pointer to memory block           */
    CO_TMR_ACTION *ap  = tmr->APool;                  /* Local: pointer to action array           */
    CO_TMR_TIME   *tp  = tmr->TPool;                  /* Local: pointer to timer array            */
    uint16_t     id  = 0;                           /* Local: action identifier                 */
    uint16_t     blk;                               /* Local: loop counter through memory block */
                                                      /*------------------------------------------*/
    CPU_SR_ALLOC();                                   /* allocate space for status                */
                                                      /*------------------------------------------*/
    CPU_CRITICAL_ENTER();                             /* lock shared timer resources              */
    tmr->Delay   = 0;                                 /* clear delay counter (for cyclic mode)    */
    tmr->Use     = 0;                                 /* clear used timer list                    */
    tmr->Elapsed = 0;                                 /* clear elapsed timer list                 */
    tmr->Free    = tmr->TPool;                        /* set root of free timer list              */
    tmr->Acts    = tmr->APool;                        /* set root of free action list             */
                                                      /*------------------------------------------*/
    for (blk = 1; blk <= tmr->Max; blk++) {           /* loop through memory blocks               */
        if (blk < tmr->Max) {                         /* see, if next memory block exists         */
            ap->Next  = &mem[blk].Act;                /* connect action to next action            */
            tp->Next  = &mem[blk].Tmr;                /* connect time to next time                */
        } else {                                      /* otherwise: last element in list          */
            ap->Next  = 0;                            /* finish linked list with a NULL pointer   */
            tp->Next  = 0;                            /* finish linked list with a NULL pointer   */
        }
        ap->Id        = id;                           /* set unique action identifier             */
        ap->Func      = (CO_TMR_FUNC)0;               /* clear callback function pointer          */
        ap->Para      = 0;                            /* clear callback function parameter        */
        ap->CycleTime = 0;                            /* clear cycletime                          */
        tp->Delta     = 0;                            /* clear delta time to this event           */
        tp->Action    = (void*)0;                     /* clear root pointer to action list        */
        tp->ActionEnd = (void*)0;                     /* clear action list end pointer            */
        ap            = ap->Next;                     /* switch to next action info               */
        tp            = tp->Next;                     /* switch to next time info                 */
        id++;                                         /* increment action identifier              */
    }
    CPU_CRITICAL_EXIT();                              /* release shared timer resources           */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief CLEAR TIMER
*
* \ingroup INTERNAL
*
*          This function deletes all timers, which are created within the CANopen stack. The
*          timers created by the application will still be active after this function call.
*
* \param[in,out]   tmr        Pointer to timer structure
*/
/*------------------------------------------------------------------------------------------------*/
void CO_TmrClear(CO_TMR *tmr)
{
    CO_NODE    *node;                                 /* Local: ptr to parent node                */
#if CO_TPDO_N > 0
    CO_TPDO    *pdo;                                  /* Local: ptr to transmit PDO list          */
    uint16_t  num;                                  /* Local: number of PDO                     */
#endif
                                                      /*------------------------------------------*/
    CPU_SR_ALLOC();                                   /* allocate space for status                */
                                                      /*------------------------------------------*/
    node = tmr->Node;                                 /* get parent node                          */
                                                      /*------------------------------------------*/
                                                      /*                   N M T                  */
                                                      /*------------------------------------------*/
    CPU_CRITICAL_ENTER();                             /* lock shared timer resources              */
    COTmrDelete(tmr, node->Nmt.Tmr);                  /* Clear heartbeat timer                    */
    node->Nmt.Tmr = -1;                               /* indicate no timer for heartbeat          */
    CPU_CRITICAL_EXIT();                              /* release shared timer resources           */
                                                      /*------------------------------------------*/
                                                      /*                  T P D O                 */
                                                      /*------------------------------------------*/
#if CO_TPDO_N > 0
    for (num = 0; num < CO_TPDO_N; num++) {           /* loop through all possible supported TPDO */
        pdo = &node->TPdo[num];                       /* get ptr to TPDO in list                  */
        if (pdo->EvTmr > -1) {                        /* see, if a timer is set for timer event   */
            CPU_CRITICAL_ENTER();                     /* lock shared timer resources              */
            COTmrDelete(tmr, pdo->EvTmr);             /* Clear event timer                        */
            pdo->EvTmr = -1;                          /* indicate no timer for timer event        */
            CPU_CRITICAL_EXIT();                      /* release shared timer resources           */
        }
        if (pdo->InTmr > -1) {                        /* see, if a timer is set for inhibit time  */
            CPU_CRITICAL_ENTER();                     /* lock shared timer resources              */
            COTmrDelete(tmr, pdo->InTmr);             /* Clear inhibit timer                      */
            pdo->InTmr = -1;                          /* indicate no timer for inhibit timing     */
            CPU_CRITICAL_EXIT();                      /* release shared timer resources           */
        }
    }
#endif
}

/*
****************************************************************************************************
*                                          LOCAL FUNCTION
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief INSERT TIMER
*
* \ingroup INTERNAL
*
*          This function inserts an action into the used timer list. First, this function
*          checks for an existing timer event on the same time and links the action to the
*          existing timer info. If no timer event exists on the same time, a new timer event
*          will be created and inserted into the used timer list in a way, that the resulting
*          used timer list is a sorted list with precalculated delta times between the time
*          events.
*
* \param[in,out]   tmr         Pointer to timer structure
*
* \param[in]       dTnew       deltatime for new action
*
* \param[in]       action      pointer to action info structure
*
* \return  returns the pointer to the event timer, which contains the new action.
*/
/*------------------------------------------------------------------------------------------------*/
static CO_TMR_TIME *CO_TmrInsert(CO_TMR *tmr, uint32_t dTnew, CO_TMR_ACTION *action)
{
    uint32_t   dTx;                                 /* Local: delta time to next event timer    */
    CO_TMR_TIME *tx;                                  /* Local: pointer to timer info             */
    CO_TMR_TIME *tn = 0;                              /* Local: pointer to resulting timer info   */
                                                      /*------------------------------------------*/
    dTx = CO_TMR_DELAY(tmr);                          /* get remaining delta time to next event   */
    tx  = tmr->Use;                                   /* get used timer list                      */
    if (tx == 0) {                                    /* --- FIRST TIMER ---                      */
        tn            = tmr->Free;                    /* yes: get free timer                      */
        tmr->Free     = tn->Next;                     /* remove timer from free list              */
        tn->Delta     = dTnew;                        /* set delta Time                           */
        tn->Action    = action;                       /* link action to new timer                 */
        tn->ActionEnd = action;                       /* update end of action list pointer        */
        tn->Next      = 0;                            /* last element in used list                */
        tmr->Use      = tn;                           /* set timer in used list                   */

        CO_TMR_RELOAD(tmr, tn->Delta);                /* set delta time to first event            */
        CO_TMR_START(tmr);                            /* start timer                              */
    } else {

        while ((dTnew > dTx) && (tn == 0)) {
            if (tx->Next == 0) {                      /* --- BEHIND LAST TIMER ---                */
                tn            = tmr->Free;            /* yes: get free timer                      */
                tmr->Free     = tn->Next;             /* remove timer from free list              */
                tn->Delta     = dTnew - dTx;          /* set delta Time                           */
                tn->Action    = action;               /* link action to new timer                 */
                tn->ActionEnd = action;               /* update end of action list pointer        */
                tn->Next      = 0;                    /* last element in used list                */
                tx->Next      = tn;                   /* set timer at end of used list            */

            } else {                                  /* --- BETWEEN TWO TIMERS ---               */
                dTx += tx->Next->Delta;               /* calc. delta time from now to next tmr    */
                if (dTnew < dTx) {                    /* see, if new tmr elapses before next tmr  */
                    tn              = tmr->Free;      /* yes: get free timer                      */
                    tmr->Free       = tn->Next;       /* remove timer from free list              */
                    tn->Next        = tx->Next;
                    tx->Next        = tn;
                    tn->Delta       = dTnew -
                        (dTx - tn->Next->Delta);
                    tn->Action      = action;         /* link action to new timer                 */
                    tn->ActionEnd   = action;         /* update end of action list pointer        */
                    tn->Next->Delta = dTx - dTnew;
                } else {
                    tx = tx->Next;                    /* yes: goto next timer                     */
                }
            }
        }
        if (tn == 0) {
            if (dTnew == dTx) {                       /* --- EQUAL EXISTING TIMER ---             */
                tx->ActionEnd->Next = action;
                tx->ActionEnd       = action;
                tn                  = tx;             /* set function result                      */

            } else if (dTnew < dTx) {                 /* --- BEFORE FIRST TIMER ---               */
                tn            = tmr->Free;            /* yes: get free timer                      */
                tmr->Free     = tn->Next;             /* remove timer from free list              */
                tn->Delta     = dTnew;                /* set delta Time                           */
                tn->Action    = action;               /* link action to new timer                 */
                tn->ActionEnd = action;               /* update end of action list pointer        */
                tn->Next      = tx;                   /* first element in used list               */
                tmr->Use      = tn;                   /* set timer in used list                   */
                tx->Delta     = dTx - dTnew;          /* correct next delta time                  */

                CO_TMR_RELOAD(tmr, tn->Delta);        /* set delta time to first event            */
            }
        }
    }

    return tn;
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief REMOVE TIMER
*
* \ingroup INTERNAL
*
*          This function removes timer info from the used timer list.
*
* \param[in,out]   tmr         Pointer to timer structure
*
* \param[in]       tx          pointer to timer info structure, which shall be removed from the list
*/
/*------------------------------------------------------------------------------------------------*/
static void CO_TmrRemove(CO_TMR *tmr, CO_TMR_TIME *tx)
{
    CO_TMR_TIME *tn;                                  /* Local: pointer to timer info             */
                                                      /*------------------------------------------*/
    if (tx != 0) {                                    /* see, if parameter is valid pointer       */
        if (tmr->Use == tx) {
            if (tx->Next == 0) {                      /* --- LAST ENTRY IN LIST ---               */
                CO_TMR_STOP(tmr);                     /* yes: stop timer                          */
                tmr->Use = tx->Next;                  /* remove first timer from list             */

            } else {                                  /* --- FIRST ENTRY ---                      */
                tx->Next->Delta += tmr->Delay;        /* calculate new delta time                 */
                tmr->Use = tx->Next;                  /* remove first timer from list             */
                CO_TMR_RELOAD(tmr, tmr->Use->Delta);  /* reload timer with new delta time         */
            }
            tx->Next  = tmr->Free;                    /* append timer to free list                */
            tmr->Free = tx;
        } else {
            tn = tmr->Use;
            do {
                if (tn->Next == tx) {                 /* --- ENTRY WITHIN LIST ---                */
                    tn->Next   = tx->Next;            /* remove entry from list                   */
                    if (tx->Next != 0) {              /* --- LAST ENTRY IN LIST ---               */
                        tn->Next->Delta += tx->Delta; /* calculate following delta time           */
                    }
                    tx->Next   = tmr->Free;           /* append timer to free list                */
                    tmr->Free  = tx;
                    tx         = 0;                   /* indicate timer is removed                */
                }
                tn = tn->Next;
            } while((tn != 0) && (tx != 0));          /* loop until end of list or entry removed  */
        }
    }
}
