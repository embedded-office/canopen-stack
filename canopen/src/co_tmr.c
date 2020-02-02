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
* FUNCTIONS
******************************************************************************/

/*
* see function definition
*/
int16_t COTmrCreate(CO_TMR      *tmr,
                    uint32_t     startTime,
                    uint32_t     cycleTime,
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
    if (startTime == 0) {
        startTime = cycleTime;
    }
    if ((startTime == 0) &&
        (cycleTime == 0)) {
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

    act            = tmr->Acts;
    tmr->Acts      = act->Next;
    act->Next      = 0;
    act->Func      = func;
    act->Para      = para;
    act->CycleTime = cycleTime;

    tn = COTmrInsert(tmr, startTime, act);
    if (tn == (CO_TMR_TIME*)0) {
        act->CycleTime   = 0;
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

/*
* see function definition
*/
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
        del->CycleTime = 0;
        del->Para      = 0;
        del->Func      = (CO_TMR_FUNC)0;
        del->Next      = tmr->Acts;
        tmr->Acts      = del;

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

/*
* see function definition
*/
int16_t COTmrService(CO_TMR *tmr)
{
    CO_TMR_TIME *tn;
    int16_t      result = 0;

    if (tmr == 0) {
        CONodeFatalError();
        return -1;
    }

    COTmrLock();
    if (tmr->Delay > 0) {
        CO_TMR_UPDATE(tmr);
        if (tmr->Delay == 0) {
            /* timer is elapsed */
            tn       = tmr->Use;
            tmr->Use = tn->Next;
            tn->Next = 0;
            if (tmr->Use != 0) {
                CO_TMR_RELOAD(tmr, tmr->Use->Delta);
            } else {
                CO_TMR_STOP(tmr);
            }
            if (tmr->Elapsed == 0) {
                tmr->Elapsed = tn;
            } else {
                tn->Next     = tmr->Elapsed;
                tmr->Elapsed = tn;
            }
            result = 1;
        }
    }
    COTmrUnlock();

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

            if (act->CycleTime == 0) {
                act->Para = 0;
                act->Func = (CO_TMR_FUNC)0;
                COTmrLock();
                act->Next = tmr->Acts;
                tmr->Acts = act;
                COTmrUnlock();

            } else {
                COTmrLock();
                res = COTmrInsert(tmr, act->CycleTime, act);
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

/*
* see function definition
*/
void COTmrInit(CO_TMR *tmr, CO_NODE *node, CO_TMR_MEM *mem, uint16_t num)
{
    tmr->Node  = node;
    tmr->Max   = num;
    tmr->TPool = &mem->Tmr;
    tmr->APool = &mem->Act;

    COTmrReset(tmr);
}

/*
* see function definition
*/
void COTmrReset(CO_TMR *tmr)
{
    CO_TMR_MEM    *mem = (CO_TMR_MEM *)tmr->APool;
    CO_TMR_ACTION *ap  = tmr->APool;
    CO_TMR_TIME   *tp  = tmr->TPool;
    uint16_t       id  = 0;
    uint16_t       blk;

    COTmrLock();
    tmr->Delay   = 0;
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
        ap->Id        = id;
        ap->Func      = (CO_TMR_FUNC)0;
        ap->Para      = 0;
        ap->CycleTime = 0;
        tp->Delta     = 0;
        tp->Action    = (void*)0;
        tp->ActionEnd = (void*)0;
        ap            = ap->Next;
        tp            = tp->Next;
        id++;
    }
    COTmrUnlock();
}

/*
* see function definition
*/
void COTmrClear(CO_TMR *tmr)
{
    CO_NODE    *node;
    CO_TPDO    *pdo;
    uint16_t    num;

    node = tmr->Node;


    /* heartbeat timer */
    COTmrLock();
    COTmrDelete(tmr, node->Nmt.Tmr);
    node->Nmt.Tmr = -1;
    COTmrUnlock();

    /* tpdo timer */
    for (num = 0; num < CO_TPDO_N; num++) {
        pdo = &node->TPdo[num];
        if (pdo->EvTmr > -1) {
            /* pdo timer event */
            COTmrLock();
            COTmrDelete(tmr, pdo->EvTmr);
            pdo->EvTmr = -1;
            COTmrUnlock();
        }
        if (pdo->InTmr > -1) {
            /* inhibit timer */
            COTmrLock();
            COTmrDelete(tmr, pdo->InTmr);
            pdo->InTmr = -1;
            COTmrUnlock();
        }
    }
}

/*
* see function definition
*/
CO_TMR_TIME *COTmrInsert(CO_TMR *tmr, uint32_t dTnew, CO_TMR_ACTION *action)
{
    uint32_t     dTx;
    CO_TMR_TIME *tx;
    CO_TMR_TIME *tn = 0;

    /* get remaining delta time to next event */
    dTx = CO_TMR_DELAY(tmr); 
    
    tx  = tmr->Use;
    if (tx == 0) {
        /* first used timer */
        tn            = tmr->Free;
        tmr->Free     = tn->Next;
        tn->Delta     = dTnew;
        tn->Action    = action;
        tn->ActionEnd = action;
        tn->Next      = 0;
        tmr->Use      = tn;

        CO_TMR_RELOAD(tmr, tn->Delta);
        CO_TMR_START(tmr);

    } else {
        while ((dTnew > dTx) && (tn == 0)) {
            /* behind last timer */
            if (tx->Next == 0) {
                tn            = tmr->Free;
                tmr->Free     = tn->Next;
                tn->Delta     = dTnew - dTx;
                tn->Action    = action;
                tn->ActionEnd = action;
                tn->Next      = 0;
                tx->Next      = tn;

            } else {
                /* between two timers */
                dTx += tx->Next->Delta;
                if (dTnew < dTx) {
                    tn              = tmr->Free;
                    tmr->Free       = tn->Next;
                    tn->Next        = tx->Next;
                    tx->Next        = tn;
                    tn->Delta       = dTnew -
                        (dTx - tn->Next->Delta);
                    tn->Action      = action;
                    tn->ActionEnd   = action;
                    tn->Next->Delta = dTx - dTnew;
                } else {
                    tx = tx->Next;
                }
            }
        }
        if (tn == 0) {
            /* equal existing timer */
            if (dTnew == dTx) {
                tx->ActionEnd->Next = action;
                tx->ActionEnd       = action;
                tn                  = tx;

            } else if (dTnew < dTx) {
                /* before first timer */
                tn            = tmr->Free;
                tmr->Free     = tn->Next;
                tn->Delta     = dTnew;
                tn->Action    = action;
                tn->ActionEnd = action;
                tn->Next      = tx;
                tmr->Use      = tn;
                tx->Delta     = dTx - dTnew;

                CO_TMR_RELOAD(tmr, tn->Delta);
            }
        }
    }

    return tn;
}

/*
* see function definition
*/
void COTmrRemove(CO_TMR *tmr, CO_TMR_TIME *tx)
{
    CO_TMR_TIME *tn;

    if (tx != 0) {
        if (tmr->Use == tx) {
            if (tx->Next == 0) {
                /* last entry */
                CO_TMR_STOP(tmr);
                tmr->Use = tx->Next;
            } else {
                /* first entry */
                tx->Next->Delta += tmr->Delay;
                tmr->Use = tx->Next;
                CO_TMR_RELOAD(tmr, tmr->Use->Delta);
            }
            tx->Next  = tmr->Free;
            tmr->Free = tx;
        } else {
            tn = tmr->Use;
            do {
                if (tn->Next == tx) {
                    /* entry within list */
                    tn->Next   = tx->Next;
                    if (tx->Next != 0) {
                        /* not at end of list */
                        tn->Next->Delta += tx->Delta;
                    }
                    tx->Next   = tmr->Free;
                    tmr->Free  = tx;
                    tx         = 0;
                }
                tn = tn->Next;
            } while((tn != 0) && (tx != 0));
        }
    }
}
