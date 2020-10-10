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

#include "def_suite.h"
#include "co_core.h"

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

static CO_NODE node;

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/*---------------------------------------------------- Clock 1MHz, Unit: 1ms */

TEST_DEF(TS_GetMinTime_1MHz_1ms)
{
    uint32_t result;

    TS_CreateNode(&node, 1000000u);
    result = COTmrGetMinTime(&node.Tmr, CO_TMR_UNIT_1MS);
    TS_ASSERT(result == 1u);
}

/*-------------------------------------------------- Clock 1MHz, Unit: 100us */

TEST_DEF(TS_GetMinTime_1MHz_100us)
{
    uint32_t result;

    TS_CreateNode(&node, 1000000u);
    result = COTmrGetMinTime(&node.Tmr, CO_TMR_UNIT_100US);
    TS_ASSERT(result == 1u);
}

/*---------------------------------------------------- Clock 1kHz, Unit: 1ms */

TEST_DEF(TS_GetMinTime_1kHz_1ms)
{
    uint32_t result;

    TS_CreateNode(&node, 1000u);
    result = COTmrGetMinTime(&node.Tmr, CO_TMR_UNIT_1MS);
    TS_ASSERT(result == 1u);
}

/*-------------------------------------------------- Clock 1kHz, Unit: 100us */

TEST_DEF(TS_GetMinTime_1kHz_100us)
{
    uint32_t result;

    TS_CreateNode(&node, 1000u);
    result = COTmrGetMinTime(&node.Tmr, CO_TMR_UNIT_100US);
    TS_ASSERT(result == 10u);
}

/*--------------------------------------------------- Clock 100Hz, Unit: 1ms */

TEST_DEF(TS_GetMinTime_100Hz_1ms)
{
    uint32_t result;

    TS_CreateNode(&node, 100u);
    result = COTmrGetMinTime(&node.Tmr, CO_TMR_UNIT_1MS);
    TS_ASSERT(result == 10u);
}

/*------------------------------------------------- Clock 100Hz, Unit: 100us */

TEST_DEF(TS_GetMinTime_100Hz_100us)
{
    uint32_t result;

    TS_CreateNode(&node, 100u);
    result = COTmrGetMinTime(&node.Tmr, CO_TMR_UNIT_100US);
    TS_ASSERT(result == 100u);
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

static void Setup(void)
{
    TS_CreateMandatoryDir();
}

SUITE_GET_TICKS()
{
    TS_Begin(__FILE__);
    TS_SetupCase(Setup, NULL);

    TS_RUNNER(TS_GetMinTime_1MHz_1ms);
    TS_RUNNER(TS_GetMinTime_1MHz_100us);
    TS_RUNNER(TS_GetMinTime_1kHz_1ms);
    TS_RUNNER(TS_GetMinTime_1kHz_100us);
    TS_RUNNER(TS_GetMinTime_100Hz_1ms);
    TS_RUNNER(TS_GetMinTime_100Hz_100us);

    TS_End();
}
