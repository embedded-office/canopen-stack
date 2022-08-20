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

#include "co_core.h"
#include "acutest.h"

/******************************************************************************
* TEST CASES - SIZE
******************************************************************************/

/*---------------------------------------------------- Clock 1MHz, Unit: 1ms */

void test_1MHz_0ms(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 1000000u;

    result = COTmrGetTicks(&tmr, 0u, CO_TMR_UNIT_1MS);

    TEST_CHECK(result == 0u);
}

void test_1MHz_23ms(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 1000000u;

    result = COTmrGetTicks(&tmr, 23u, CO_TMR_UNIT_1MS);

    TEST_CHECK(result == 23000u);
}

void test_1MHz_456ms(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 1000000u;

    result = COTmrGetTicks(&tmr, 456u, CO_TMR_UNIT_1MS);

    TEST_CHECK(result == 456000u);
}

void test_1MHz_1000ms(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 1000000u;

    result = COTmrGetTicks(&tmr, 1000u, CO_TMR_UNIT_1MS);

    TEST_CHECK(result == 1000000u);
}

void test_1MHz_65535ms(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 1000000u;

    result = COTmrGetTicks(&tmr, 65535u, CO_TMR_UNIT_1MS);

    TEST_CHECK(result == 65535000u);
}

/*-------------------------------------------------- Clock 1MHz, Unit: 100us */

void test_1MHz_100us(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 1000000u;

    result = COTmrGetTicks(&tmr, 1u, CO_TMR_UNIT_100US);

    TEST_CHECK(result == 100u);
}

void test_1MHz_2300us(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 1000000u;

    result = COTmrGetTicks(&tmr, 23u, CO_TMR_UNIT_100US);

    TEST_CHECK(result == 2300u);
}

void test_1MHz_72500us(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 1000000u;

    result = COTmrGetTicks(&tmr, 725u, CO_TMR_UNIT_100US);

    TEST_CHECK(result == 72500u);
}

void test_1MHz_6553500us(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 1000000u;

    result = COTmrGetTicks(&tmr, 65535u, CO_TMR_UNIT_100US);

    TEST_CHECK(result == 6553500u);
}

/*---------------------------------------------------- Clock 1kHz, Unit: 1ms */

void test_1kHz_0ms(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 1000u;

    result = COTmrGetTicks(&tmr, 0u, CO_TMR_UNIT_1MS);

    TEST_CHECK(result == 0u);
}

void test_1kHz_23ms(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 1000u;

    result = COTmrGetTicks(&tmr, 23u, CO_TMR_UNIT_1MS);

    TEST_CHECK(result == 23u);
}

void test_1kHz_456ms(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 1000u;

    result = COTmrGetTicks(&tmr, 456u, CO_TMR_UNIT_1MS);

    TEST_CHECK(result == 456u);
}

void test_1kHz_1000ms(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 1000u;

    result = COTmrGetTicks(&tmr, 1000u, CO_TMR_UNIT_1MS);

    TEST_CHECK(result == 1000u);
}

void test_1kHz_65535ms(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 1000u;

    result = COTmrGetTicks(&tmr, 65535u, CO_TMR_UNIT_1MS);

    TEST_CHECK(result == 65535u);
}

/*-------------------------------------------------- Clock 1kHz, Unit: 100us */

void test_1kHz_100us(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 1000u;

    result = COTmrGetTicks(&tmr, 1u, CO_TMR_UNIT_100US);

    TEST_CHECK(result == 0u);
}

/*--------------------------------------------------- Clock 100Hz, Unit: 1ms */

void test_100Hz_6ms(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 100u;

    result = COTmrGetTicks(&tmr, 6u, CO_TMR_UNIT_1MS);

    TEST_CHECK(result == 0u);
}

void test_100Hz_42ms(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 100u;

    result = COTmrGetTicks(&tmr, 42u, CO_TMR_UNIT_1MS);

    TEST_CHECK(result == 4u);
}

void test_100Hz_789ms(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 100u;

    result = COTmrGetTicks(&tmr, 789u, CO_TMR_UNIT_1MS);

    TEST_CHECK(result == 78u);
}

void test_100Hz_5000ms(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 100u;

    result = COTmrGetTicks(&tmr, 5000u, CO_TMR_UNIT_1MS);
    TEST_CHECK(result == 500u);
}

void test_100Hz_65535ms(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 100u;

    result = COTmrGetTicks(&tmr, 65535, CO_TMR_UNIT_1MS);

    TEST_CHECK(result == 6553u);
}

/*------------------------------------------------- Clock 100Hz, Unit: 100us */

void test_100Hz_100us(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 100u;

    result = COTmrGetTicks(&tmr, 1u, CO_TMR_UNIT_100US);

    TEST_CHECK(result == 0u);
}


TEST_LIST = {
    { "1MHz_0ms",       test_1MHz_0ms       },
    { "1MHz_23ms",      test_1MHz_23ms      },
    { "1MHz_456ms",     test_1MHz_456ms     },
    { "1MHz_1000ms",    test_1MHz_1000ms    },
    { "1MHz_65535ms",   test_1MHz_65535ms   },
    { "1MHz_100us",     test_1MHz_100us     },
    { "1MHz_2300us",    test_1MHz_2300us    },
    { "1MHz_72500us",   test_1MHz_72500us   },
    { "1MHz_6553500us", test_1MHz_6553500us },
    { "1kHz_0ms",       test_1kHz_0ms       },
    { "1kHz_23ms",      test_1kHz_23ms      },
    { "1kHz_456ms",     test_1kHz_456ms     },
    { "1kHz_1000ms",    test_1kHz_1000ms    },
    { "1kHz_65535ms",   test_1kHz_65535ms   },
    { "1kHz_100us",     test_1kHz_100us     },
    { "100Hz_6ms",      test_100Hz_6ms      },
    { "100Hz_42ms",     test_100Hz_42ms     },
    { "100Hz_789ms",    test_100Hz_789ms    },
    { "100Hz_5000ms",   test_100Hz_5000ms   },
    { "100Hz_65535ms",  test_100Hz_65535ms  },
    { "100Hz_100us",    test_100Hz_100us    },
    { NULL, NULL }
};
