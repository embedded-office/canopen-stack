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

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_tmr.h"
#include "co_core.h"

/******************************************************************************
* EXTERNAL FUNCTIONS
******************************************************************************/

extern void COTmrLock(void);
extern void COTmrUnlock(void);

/******************************************************************************
* PRIVATE FUNCTION PROTOTYPES
******************************************************************************/

static void         COTmrReset  (CO_TMR *tmr);
static CO_TMR_TIME *COTmrInsert (CO_TMR *tmr, uint32_t dTnew, CO_TMR_ACTION *action);
static void         COTmrRemove (CO_TMR *tmr, CO_TMR_TIME *tx);

/******************************************************************************
* PROTECTED FUNCTIONS
******************************************************************************/

void COTmrInit(CO_TMR *tmr, CO_NODE *node, CO_TMR_MEM *mem, uint16_t num, uint32_t freq)
{
    COTmrLock();
    tmr->Node  = node;
    tmr->Max   = num;
    tmr->TPool = &mem->Tmr;
    tmr->APool = &mem->Act;
    tmr->Freq  = freq;

    COTmrReset(tmr);
    COTmrUnlock();
}

void COTmrClear(CO_TMR *tmr)
{
    CO_NODE    *node = tmr->Node;
    CO_TPDO    *pdo;
    uint16_t    num;

    /* delete heartbeat timer */
    if (node->Nmt.Tmr > -1) {
        COTmrDelete(tmr, node->Nmt.Tmr);
        node->Nmt.Tmr = -1;
    }

    /* check all tpdo timers */
    for (num = 0; num < CO_TPDO_N; num++) {
        pdo = &node->TPdo[num];

         /* delete pdo timer event */
        if (pdo->EvTmr > -1) {
            COTmrDelete(tmr, pdo->EvTmr);
            pdo->EvTmr = -1;
        }

        /* delete inhibit timer */
        if (pdo->InTmr > -1) {
            COTmrDelete(tmr, pdo->InTmr);
            pdo->InTmr = -1;
        }
    }
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

WEAK_TEST
uint32_t COTmrGetTicks(CO_TMR *tmr, uint16_t time, uint32_t unit)
{
    uint32_t ticks = 0u;
    uint32_t freq  = tmr->Freq;

    if (freq == 0u) {
        ticks = 0u;
    } else {
        if (freq <= unit) {
            ticks = (uint32_t)time / (unit / freq);
        } else {
            ticks = (uint32_t)time * (freq / unit);
        }
    }
    return (ticks);
}

uint16_t COTmrGetMinTime(CO_TMR *tmr, uint32_t unit)
{
    uint16_t time  = 1u;
    uint32_t freq  = tmr->Freq;

    if (freq == 0u) {
        time = 0u;
    } else {
        if (freq <= unit) {
            time = (uint16_t)(unit / freq);
        }
    }
    return (time);
}

WEAK_TEST
int16_t COTmrCreate(CO_TMR      *tmr,
                    uint32_t     startTicks,
                    uint32_t     cycleTicks,
                    CO_TMR_FUNC  func,
                    void        *para)
{
    CO_TMR_ACTION *act;
    CO_TMR_TIME   *tn;
    int16_t        result;

    if (tmr == 0) {
        CONodeFatalError();
        return -1;
    }
    if (startTicks == 0) {
        startTicks = cycleTicks;
    }
    if ((startTicks == 0) &&
        (cycleTicks == 0)) {
        return -1;
    }
    if (tmr->Node == 0) {
        CONodeFatalError();
        return -1;
    }
    if (func == 0) {
        tmr->Node->Error = CO_ERR_BAD_ARG;
        return -1;
    }

    COTmrLock();
    if (tmr->Acts == 0) {
        tmr->Node->Error = CO_ERR_TMR_NO_ACT;
        COTmrUnlock();
        return -1;
    }

    act             = tmr->Acts;
    tmr->Acts       = act->Next;
    act->Next       = 0;
    act->Func       = func;
    act->Para       = para;
    act->CycleTicks = cycleTicks;

    tn = COTmrInsert(tmr, startTicks, act);
    if (tn == (CO_TMR_TIME*)0) {
        act->CycleTicks  = 0;
        act->Para        = 0;
        act->Func        = (CO_TMR_FUNC)0;
        act->Next        = tmr->Acts;
        tmr->Acts        = act;
        tmr->Node->Error = CO_ERR_TMR_INSERT;
        result           = -1;
    } else {
        result = (int16_t)(act->Id);
    }

    COTmrUnlock();

    return (result);
}

WEAK_TEST
int16_t COTmrDelete(CO_TMR *tmr, int16_t actId)
{
    CO_TMR_TIME   *tx;
    CO_TMR_ACTION *act;
    CO_TMR_ACTION *prev;
    CO_TMR_ACTION *del    = 0;
    int16_t        result = -1;

    if ( (actId < 0) ||
         (actId >= (int16_t)(tmr->Max)) ) {
        return -1;
    }

    COTmrLock();

    /* search in used timer list */
    tx = tmr->Use;                     
    while ((tx  != 0) && (del == 0)) {
        act = tx->Action;
        if (act->Id == (uint16_t)actId) {
            del        = act;
            tx->Action = act->Next;
        } else {
            prev = act;
            act  = act->Next;
            while ((act != 0) && (del == 0)) {
                if (act->Id == (uint16_t)actId) {
                    del        = act;
                    prev->Next = act->Next;
                    if (act->Next == 0) {
                        tx->ActionEnd = prev;
                    }
                }
                prev = act;
                act  = act->Next;
            }
        }
        if (del == 0) {
            tx = tx->Next;
        }
    }

    /* not found: search in elapsed timer list */
    if (del == 0) {
        tx = tmr->Elapsed;
        while ((tx != 0) && (del == 0)) {
            act = tx->Action;
            if (act->Id == (uint16_t)actId) {
                del        = act;
                tx->Action = act->Next;
            } else {
                prev = act;
                act  = act->Next;
                while ((act != 0) && (del == 0)) {
                    if (act->Id == (uint16_t)actId) {
                        del        = act;
                        prev->Next = act->Next;
                        if (act->Next == 0) {
                            tx->ActionEnd = prev;
                        }
                    }
                    prev = act;
                    act  = act->Next;
                }
            }
            if (del == 0) {
                tx = tx->Next;
            }
        }
    }

    /* delete action */
    if (del != 0) {
        del->CycleTicks = 0;
        del->Para       = 0;
        del->Func       = (CO_TMR_FUNC)0;
        del->Next       = tmr->Acts;
        tmr->Acts       = del;

        if (tx != 0) {
            if (tx->Action == (CO_TMR_ACTION*)0) {
                tx->ActionEnd = 0;
                COTmrRemove(tmr, tx);
            }
            result = 0;
        }
    }
    COTmrUnlock();

    return (result);
}

int16_t COTmrService(CO_TMR *tmr)
{
    CO_TMR_TIME *tn;
    CO_IF       *cif;
    int16_t      result = 0;
    int16_t      elapsed;

    ASSERT_PTR_FATAL_ERR(tmr, -1);

    cif = &tmr->Node->If;
    elapsed = COIfTimerUpdate(cif);
    if (elapsed > 0) {
        /* get elapsed timer */
        tn       = tmr->Use;                            
        tmr->Use = tn->Next;
        tn->Next = 0;

        /* put elapsed timer in list */
        if (tmr->Elapsed == 0) {                
            tmr->Elapsed = tn;
        } else {
            tn->Next     = tmr->Elapsed;
            tmr->Elapsed = tn;
        }

        /* get next timer */
        tn = tmr->Use;

        /* setup next timer event */
        if (tn != 0) {                       
            COIfTimerReload(cif, tn->Delta);
        } else {
            COIfTimerStop(cif);
        }
        result = 1;
    }
    return (result);
}

/*
* see function definition
*/
void COTmrProcess(CO_TMR *tmr)
{
    CO_TMR_TIME   *tn;
    CO_TMR_TIME   *res;
    CO_TMR_ACTION *act;
    CO_TMR_ACTION *next;
    CO_TMR_FUNC    func;
    void          *para;

    while (tmr->Elapsed != 0) {
        COTmrLock();
        tn            = tmr->Elapsed;
        tmr->Elapsed  = tn->Next;

        act           = tn->Action;
        tn->Action    = 0;
        tn->ActionEnd = 0;
        tn->Delta     = 0;
        tn->Next      = tmr->Free;
        tmr->Free     = tn;
        COTmrUnlock();

        /* loop through all actions of elapsed timer event */
        while (act != 0) {
            next      = act->Next;
            act->Next = 0;
            func      = act->Func;
            para      = act->Para;

            if (act->CycleTicks == 0) {
                act->Para = 0;
                act->Func = (CO_TMR_FUNC)0;
                COTmrLock();
                act->Next = tmr->Acts;
                tmr->Acts = act;
                COTmrUnlock();

            } else {
                COTmrLock();
                res = COTmrInsert(tmr, act->CycleTicks, act);
                COTmrUnlock();
                if (res == (CO_TMR_TIME*)0) {
                    tmr->Node->Error = CO_ERR_TMR_CREATE;
                }
            }
            /* execute callback function */
            func(para);
            act = next;
        }
    }
}

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void COTmrReset(CO_TMR *tmr)
{
    CO_TMR_MEM    *mem = (CO_TMR_MEM *)tmr->APool;
    CO_TMR_ACTION *ap  = tmr->APool;
    CO_TMR_TIME   *tp  = tmr->TPool;
    uint16_t       id  = 0;
    uint16_t       blk;

    tmr->Use     = 0;
    tmr->Elapsed = 0;
    tmr->Free    = tmr->TPool;
    tmr->Acts    = tmr->APool;

    for (blk = 1; blk <= tmr->Max; blk++) {
        if (blk < tmr->Max) {
            ap->Next  = &mem[blk].Act;
            tp->Next  = &mem[blk].Tmr;
        } else {
            ap->Next  = 0;
            tp->Next  = 0;
        }
        ap->Id         = id;
        ap->Func       = (CO_TMR_FUNC)0;
        ap->Para       = 0;
        ap->CycleTicks = 0;
        tp->Delta      = 0;
        tp->Action     = (void*)0;
        tp->ActionEnd  = (void*)0;
        ap             = ap->Next;
        tp             = tp->Next;
        id++;
    }
}

static CO_TMR_TIME *COTmrInsert(CO_TMR *tmr, uint32_t dTnew, CO_TMR_ACTION *action)
{
    uint32_t     dTx;
    CO_TMR_TIME *tx;
    CO_TMR_TIME *tn = 0;
    CO_IF       *cif;

    cif = &tmr->Node->If;
    tx  = tmr->Use;

    /* no used timer */
    if (tx == 0) {
        /* fetch a timer */
        tn            = tmr->Free;
        tmr->Free     = tn->Next;
        /* setup first timer */
        tn->Delta     = dTnew;
        tn->Action    = action;
        tn->ActionEnd = action;
        tn->Next      = 0;
        tmr->Use      = tn;
        COIfTimerReload(cif, tn->Delta);
        COIfTimerStart(cif);

    /* add to timer list */
    } else {
        /* get remaining time to first event */
        dTx = COIfTimerDelay(cif);

        /* find position while new time interval is not reached and
         * no is timer added 
         */
        while ((dTnew > dTx) && (tn == 0)) {

            /* last used timer: append at end of list */
            if (tx->Next == 0) {
                /* fetch a timer */
                tn            = tmr->Free;
                tmr->Free     = tn->Next;
                /* setup new timer at the end of list */
                tn->Delta     = dTnew - dTx;
                tn->Action    = action;
                tn->ActionEnd = action;
                tn->Next      = 0;
                tx->Next      = tn;

            /* used timer within list */
            } else {
                /* calculate time interval for next used timer */
                dTx += tx->Next->Delta;

                /* new time interval is before next used timer */
                if (dTnew < dTx) {
                    /* fetch a timer */
                    tn              = tmr->Free;
                    tmr->Free       = tn->Next;
                    /* setup timer in front of next timer */
                    tn->Next        = tx->Next;
                    tx->Next        = tn;
                    tn->Delta       = dTnew -
                        (dTx - tn->Next->Delta);
                    tn->Action      = action;
                    tn->ActionEnd   = action;
                    tn->Next->Delta = dTx - dTnew;
                
                /* new time interval is after next used timer */
                } else {
                    tx = tx->Next;
                }
            }
        }

        /* no new timer added */
        if (tn == 0) {

            /* time interval is equal to a used timer */
            if (dTnew == dTx) {
                /* add action to used timer */
                tx->ActionEnd->Next = action;
                tx->ActionEnd       = action;
                tn                  = tx;

            /* time interval is before first timer */
            } else if (dTnew < dTx) {
                /* fetch a timer */
                tn            = tmr->Free;
                tmr->Free     = tn->Next;
                /* setup timer in front of first timer */
                tn->Delta     = dTnew;
                tn->Action    = action;
                tn->ActionEnd = action;
                tn->Next      = tx;
                tmr->Use      = tn;
                tx->Delta     = dTx - dTnew;
                COIfTimerReload(cif, tn->Delta);
            }
        }
    }
    return tn;
}

static void COTmrRemove(CO_TMR *tmr, CO_TMR_TIME *tx)
{
    CO_TMR_TIME *tn;
    CO_IF       *cif;

    cif = &tmr->Node->If;
    if (tx != 0) {
        /* timer is first in list */
        if (tmr->Use == tx) {
            /* remove last used timer in list */
            if (tx->Next == 0) {
                COIfTimerStop(cif);
                tmr->Use = tx->Next;

            /* remove first used timer in list */
            } else {
                tx->Next->Delta += COIfTimerDelay(cif);
                tmr->Use = tx->Next;
                COIfTimerReload(cif, tmr->Use->Delta);
            }
            /* put timer in free list */
            tx->Next  = tmr->Free;
            tmr->Free = tx;

        /* timer maybe within list */
        } else {

            /* loop through used timers in list until timer is removed */
            tn = tmr->Use;
            do {
                /* remove next timer in list */
                if (tn->Next == tx) {
                    tn->Next = tx->Next;

                    /* timer was within list */
                    if (tx->Next != 0) {
                        tn->Next->Delta += tx->Delta;
                    }
                    /* put timer in free list */
                    tx->Next   = tmr->Free;
                    tmr->Free  = tx;
                    tx         = 0;
                }
                tn = tn->Next;
            } while((tn != 0) && (tx != 0));
        }
    }
}
