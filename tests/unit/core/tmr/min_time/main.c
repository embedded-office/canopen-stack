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

void test_1MHz_1ms(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 1000000u;

    result = COTmrGetMinTime(&tmr, CO_TMR_UNIT_1MS);

    TEST_CHECK(result == 1u);
}

/*-------------------------------------------------- Clock 1MHz, Unit: 100us */

void test_1MHz_100us(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 1000000u;

    result = COTmrGetMinTime(&tmr, CO_TMR_UNIT_100US);

    TEST_CHECK(result == 1u);
}

/*---------------------------------------------------- Clock 1kHz, Unit: 1ms */

void test_1kHz_1ms(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 1000u;

    result = COTmrGetMinTime(&tmr, CO_TMR_UNIT_1MS);

    TEST_CHECK(result == 1u);
}

/*-------------------------------------------------- Clock 1kHz, Unit: 100us */

void test_1kHz_100us(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 1000u;

    result = COTmrGetMinTime(&tmr, CO_TMR_UNIT_100US);

    TEST_CHECK(result == 10u);
}

/*--------------------------------------------------- Clock 100Hz, Unit: 1ms */

void test_100Hz_1ms(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 100u;

    result = COTmrGetMinTime(&tmr, CO_TMR_UNIT_1MS);

    TEST_CHECK(result == 10u);
}

/*------------------------------------------------- Clock 100Hz, Unit: 100us */

void test_100Hz_100us(void)
{
    CO_TMR   tmr = { 0 };
    uint32_t result;
    tmr.Freq = 100u;

    result = COTmrGetMinTime(&tmr, CO_TMR_UNIT_100US);

    TEST_CHECK(result == 100u);
}


TEST_LIST = {
    { "1MHz_1ms",    test_1MHz_1ms    },
    { "1MHz_100us",  test_1MHz_100us  },
    { "1kHz_1ms",    test_1kHz_1ms    },
    { "1kHz_100us",  test_1kHz_100us  },
    { "100Hz_1ms",   test_100Hz_1ms   },
    { "100Hz_100us", test_100Hz_100us },
    { NULL, NULL }
};
