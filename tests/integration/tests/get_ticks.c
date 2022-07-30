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

TEST_DEF(TS_GetTick_1MHz_0ms)
{
    uint32_t result;

    TS_CreateNode(&node, 1000000u);
    result = COTmrGetTicks(&node.Tmr, 0u, CO_TMR_UNIT_1MS);
    TS_ASSERT(result == 0u);
}

TEST_DEF(TS_GetTick_1MHz_23ms)
{
    uint32_t result;

    TS_CreateNode(&node, 1000000u);
    result = COTmrGetTicks(&node.Tmr, 23u, CO_TMR_UNIT_1MS);
    TS_ASSERT(result == 23000u);
}

TEST_DEF(TS_GetTick_1MHz_456ms)
{
    uint32_t result;

    TS_CreateNode(&node, 1000000u);
    result = COTmrGetTicks(&node.Tmr, 456u, CO_TMR_UNIT_1MS);
    TS_ASSERT(result == 456000u);
}

TEST_DEF(TS_GetTick_1MHz_1000ms)
{
    uint32_t result;

    TS_CreateNode(&node, 1000000u);
    result = COTmrGetTicks(&node.Tmr, 1000u, CO_TMR_UNIT_1MS);
    TS_ASSERT(result == 1000000u);
}

TEST_DEF(TS_GetTick_1MHz_65535ms)
{
    uint32_t result;

    TS_CreateNode(&node, 1000000u);
    result = COTmrGetTicks(&node.Tmr, 65535u, CO_TMR_UNIT_1MS);
    TS_ASSERT(result == 65535000u);
}

/*-------------------------------------------------- Clock 1MHz, Unit: 100us */

TEST_DEF(TS_GetTick_1MHz_100us)
{
    uint32_t result;

    TS_CreateNode(&node, 1000000u);
    result = COTmrGetTicks(&node.Tmr, 1u, CO_TMR_UNIT_100US);
    TS_ASSERT(result == 100u);
}

TEST_DEF(TS_GetTick_1MHz_2300us)
{
    uint32_t result;

    TS_CreateNode(&node, 1000000u);
    result = COTmrGetTicks(&node.Tmr, 23u, CO_TMR_UNIT_100US);
    TS_ASSERT(result == 2300u);
}

TEST_DEF(TS_GetTick_1MHz_72500us)
{
    uint32_t result;

    TS_CreateNode(&node, 1000000u);
    result = COTmrGetTicks(&node.Tmr, 725u, CO_TMR_UNIT_100US);
    TS_ASSERT(result == 72500u);
}

TEST_DEF(TS_GetTick_1MHz_6553500us)
{
    uint32_t result;

    TS_CreateNode(&node, 1000000u);
    result = COTmrGetTicks(&node.Tmr, 65535u, CO_TMR_UNIT_100US);
    TS_ASSERT(result == 6553500u);
}

/*---------------------------------------------------- Clock 1kHz, Unit: 1ms */

TEST_DEF(TS_GetTick_1kHz_0ms)
{
    uint32_t result;

    TS_CreateNode(&node, 1000u);
    result = COTmrGetTicks(&node.Tmr, 0u, CO_TMR_UNIT_1MS);
    TS_ASSERT(result == 0u);
}

TEST_DEF(TS_GetTick_1kHz_23ms)
{
    uint32_t result;

    TS_CreateNode(&node, 1000u);
    result = COTmrGetTicks(&node.Tmr, 23u, CO_TMR_UNIT_1MS);
    TS_ASSERT(result == 23u);
}

TEST_DEF(TS_GetTick_1kHz_456ms)
{
    uint32_t result;

    TS_CreateNode(&node, 1000u);
    result = COTmrGetTicks(&node.Tmr, 456u, CO_TMR_UNIT_1MS);
    TS_ASSERT(result == 456u);
}

TEST_DEF(TS_GetTick_1kHz_1000ms)
{
    uint32_t result;

    TS_CreateNode(&node, 1000u);
    result = COTmrGetTicks(&node.Tmr, 1000u, CO_TMR_UNIT_1MS);
    TS_ASSERT(result == 1000u);
}

TEST_DEF(TS_GetTick_1kHz_65535ms)
{
    uint32_t result;

    TS_CreateNode(&node, 1000u);
    result = COTmrGetTicks(&node.Tmr, 65535u, CO_TMR_UNIT_1MS);
    TS_ASSERT(result == 65535u);
}

/*-------------------------------------------------- Clock 1kHz, Unit: 100us */

TEST_DEF(TS_GetTick_1kHz_100us)
{
    uint32_t result;

    TS_CreateNode(&node, 1000u);
    result = COTmrGetTicks(&node.Tmr, 1u, CO_TMR_UNIT_100US);
    TS_ASSERT(result == 0u);
}

/*--------------------------------------------------- Clock 100Hz, Unit: 1ms */

TEST_DEF(TS_GetTick_100Hz_6ms)
{
    uint32_t result;

    TS_CreateNode(&node, 100u);
    result = COTmrGetTicks(&node.Tmr, 6u, CO_TMR_UNIT_1MS);
    TS_ASSERT(result == 0u);
}

TEST_DEF(TS_GetTick_100Hz_42ms)
{
    uint32_t result;

    TS_CreateNode(&node, 100u);
    result = COTmrGetTicks(&node.Tmr, 42u, CO_TMR_UNIT_1MS);
    TS_ASSERT(result == 4u);
}

TEST_DEF(TS_GetTick_100Hz_789ms)
{
    uint32_t result;

    TS_CreateNode(&node, 100u);
    result = COTmrGetTicks(&node.Tmr, 789u, CO_TMR_UNIT_1MS);
    TS_ASSERT(result == 78u);
}

TEST_DEF(TS_GetTick_100Hz_5000ms)
{
    uint32_t result;

    TS_CreateNode(&node, 100u);
    result = COTmrGetTicks(&node.Tmr, 5000u, CO_TMR_UNIT_1MS);
    TS_ASSERT(result == 500u);
}

TEST_DEF(TS_GetTick_100Hz_65535ms)
{
    uint32_t result;

    TS_CreateNode(&node, 100u);
    result = COTmrGetTicks(&node.Tmr, 65535, CO_TMR_UNIT_1MS);
    TS_ASSERT(result == 6553u);
}

/*------------------------------------------------- Clock 100Hz, Unit: 100us */

TEST_DEF(TS_GetTick_100Hz_100us)
{
    uint32_t result;

    TS_CreateNode(&node, 100u);
    result = COTmrGetTicks(&node.Tmr, 1u, CO_TMR_UNIT_100US);
    TS_ASSERT(result == 0u);
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

    TS_RUNNER(TS_GetTick_1MHz_0ms);
    TS_RUNNER(TS_GetTick_1MHz_23ms);
    TS_RUNNER(TS_GetTick_1MHz_456ms);
    TS_RUNNER(TS_GetTick_1MHz_1000ms);
    TS_RUNNER(TS_GetTick_1MHz_65535ms);
    TS_RUNNER(TS_GetTick_1MHz_100us);
    TS_RUNNER(TS_GetTick_1MHz_2300us);
    TS_RUNNER(TS_GetTick_1MHz_72500us);
    TS_RUNNER(TS_GetTick_1MHz_6553500us);
    TS_RUNNER(TS_GetTick_1kHz_0ms);
    TS_RUNNER(TS_GetTick_1kHz_23ms);
    TS_RUNNER(TS_GetTick_1kHz_456ms);
    TS_RUNNER(TS_GetTick_1kHz_1000ms);
    TS_RUNNER(TS_GetTick_1kHz_65535ms);
    TS_RUNNER(TS_GetTick_1kHz_100us);
    TS_RUNNER(TS_GetTick_100Hz_6ms);
    TS_RUNNER(TS_GetTick_100Hz_42ms);
    TS_RUNNER(TS_GetTick_100Hz_789ms);
    TS_RUNNER(TS_GetTick_100Hz_5000ms);
    TS_RUNNER(TS_GetTick_100Hz_65535ms);
    TS_RUNNER(TS_GetTick_100Hz_100us);

    TS_End();
}
