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

/* get external node specification */
#include "clock_spec.h"

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

/* Allocate a global CANopen node object */
static CO_NODE Clk;

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/* timer callback function */
static void AppClock(void *p_arg)
{
    CO_NODE  *node;
    CO_OBJ   *od_sec;
    CO_OBJ   *od_min;
    CO_OBJ   *od_hr;
    uint8_t   second;
    uint8_t   minute;
    uint32_t  hour;

    /* For flexible usage (not needed, but nice to show), we use the argument
     * as reference to the CANopen node object. If no node is given, we ignore
     * the function call by returning immediatelly.
     */
    node = (CO_NODE *)p_arg;
    if (node == 0) {
        return;
    }

    /* Main functionality: when we are in operational mode, we get the current
     * clock values out of the object dictionary, increment the seconds and
     * update all clock values in the object dictionary.
     */
    if (CONmtGetMode(&node->Nmt) == CO_OPERATIONAL) {

        od_sec = CODictFind(&node->Dict, CO_DEV(0x2100, 3));
        od_min = CODictFind(&node->Dict, CO_DEV(0x2100, 2));
        od_hr  = CODictFind(&node->Dict, CO_DEV(0x2100, 1));

        COObjRdValue(od_sec, node, (void *)&second, sizeof(second));
        COObjRdValue(od_min, node, (void *)&minute, sizeof(minute));
        COObjRdValue(od_hr , node, (void *)&hour  , sizeof(hour));

        second++;
        if (second >= 60) {
            second = 0;
            minute++;
        }
        if (minute >= 60) {
            minute = 0;
            hour++;
        }

        COObjWrValue(od_hr , node, (void *)&hour  , sizeof(hour));
        COObjWrValue(od_min, node, (void *)&minute, sizeof(minute));
        COObjWrValue(od_sec, node, (void *)&second, sizeof(second));
    }
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/* main entry function */
int main(void)
{
    uint32_t ticks;

    /* Initialize your hardware layer */
    /* BSPInit(); */

    /* Initialize the CANopen stack. Stop execution if an
     * error is detected.
     */
    ClkStartNode(&Clk);
    if (CONodeGetErr(&Clk) != CO_ERR_NONE) {
        while(1);
    }

    /* Use CANopen software timer to create a cyclic function
     * call to the callback function 'AppClock()' with a period
     * of 1s (equal: 1000ms).
     */
    ticks = COTmrGetTicks(&Clk.Tmr, 1000, CO_TMR_UNIT_1MS);
    COTmrCreate(&Clk.Tmr, 0, ticks, AppClock, &Clk);

    /* Start the CANopen node and set it automatically to
     * NMT mode: 'OPERATIONAL'.
     */
    CONodeStart(&Clk);
    CONmtSetMode(&Clk.Nmt, CO_OPERATIONAL);

    /* In the background loop we process received CAN frames
     * and executes elapsed action callback functions.
     */
    while (1) {
        CONodeProcess(&Clk);
        COTmrProcess(&Clk.Tmr);
    }
}
