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

#include "ts_env.h"
#include "ts_context.h"
#include "ts_output.h"
#include "ts_version.h"
#include "ts_pipe.h"

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This define specifies the maximum length of a test suite name.
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_NAME_MAX   32u

/******************************************************************************
* PRIVATE TYPES
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This type contains the runtime setup environment.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct TS_SETUP_T {
    TS_FUNC    BeforeAll;                             /*!< Called before ALL test suites          */
    TS_FUNC    AfterAll;                              /*!< Called after ALL test suites           */
    TS_FUNC    BeforeSuite;                           /*!< Called before EACH test suites         */
    TS_FUNC    AfterSuite;                            /*!< Called after EACH test suites          */
    TS_FUNC    BeforeTest;                            /*!< Called before EACH test case           */
    TS_FUNC    AfterTest;                             /*!< Called after EACH test case            */
    uint32_t   Disable[TS_GROUP_MAX];                 /*!< Bitwise enable/disable of tests        */
} TS_SETUP;

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This type contains all test data for a single test run.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct TS_CURRENT_T {
    TS_CONTEXT Context;                               /*!< Memory for general machine context     */
    TS_CHAR    SuiteName[TS_NAME_MAX];                /*!< Name of test suite                     */
    TS_LITERAL TestName;                              /*!< Name of test function                  */
    uint16_t   Line;                                  /*!< Line of test function                  */
    uint16_t   TestCnt;                               /*!< Number of tests in test suite          */
    uint16_t   FailCnt;                               /*!< Number of failed tests                 */
    uint16_t   PassCnt;                               /*!< Number of passed tests                 */
    uint16_t   SkipCnt;                               /*!< Number of ignored test                 */
    bool       Fail;                                  /*!< Flag: current test is failed           */
    bool       Skip;                                  /*!< Flag: current test is ignored          */
} TS_CURRENT;

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This type contains all collected summary data since last reset of statistic.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct TS_SUMMARY_T {
    uint16_t   FailCnt;                               /*!< Total number of failed tests           */
    uint16_t   TestCnt;                               /*!< Total number of tests                  */
    uint16_t   SkipCnt;                               /*!< Total number of ignored tests          */
} TS_SUMMARY;

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This type is the container with all data of the test framework.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct TS_DATA_T {
    TS_SETUP   Env;                                   /**< Test runtime environment setup         */
    TS_CURRENT Cur;                                   /**< Data of current test run               */
    TS_SUMMARY Sum;                                   /**< Collected test summary                 */
} TS_DATA;

/******************************************************************************
* PUBLIC VARIABLES
******************************************************************************/

TEST_SECTION_START_DEF
TEST_SECTION_START_ALLOC

TEST_SECTION_END_DEF
TEST_SECTION_END_ALLOC

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details Allocated test data container.
*/
/*------------------------------------------------------------------------------------------------*/
static TS_DATA TestData;

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function is the last function, called after the execution of each test case
*          function. If the test execution was successful, this function prints a useful message.
*          In any case the statistics are updated by this function as well.
*
* \param   line
*          Line number of executed test case function
*/
/*------------------------------------------------------------------------------------------------*/
static void TS_Result(TS_LINE line)
{
    if (TestData.Cur.Skip == true) {
        TestData.Cur.SkipCnt++;
    } else if (!TestData.Cur.Fail) {
        TS_Printf("%s:%d:%s:PASS\n",
                  TestData.Cur.SuiteName,
                  line,
                  TestData.Cur.TestName);
    } else {
        TestData.Cur.FailCnt++;
    }

    TestData.Cur.Fail = false;
    TestData.Cur.Skip = false;
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function analyzes and sets the test suite name. With the analysis we want to
*          detect, if there is a path with filename given. In this case, we want to set the
*          file name only (filter the paths). Due to the nature of the RCS keywords, we copy
*          the file name to the given string memory.
*
* \param   name
*          Pointer to name for the test suite
*
* \param   string
*          Pointer to destination string memory
*/
/*------------------------------------------------------------------------------------------------*/
static void TS_SetSuiteName(TS_LITERAL name, TS_STRING string)
{
    TS_LITERAL ptr = name;
    uint16_t   cnt = 0u;
    bool      path = false;

    if (ptr != NULL) {
        while (*ptr != 0u) {                          /* goto end of string                       */
            if ( (*ptr == '\\') || (*ptr ==  '/') ) { /* see, if path delimiter is existing       */
                path = true;
            }
            ptr++;
        }
        if (path == true) {                           /* see, if path delimiter was found         */
            while ((*ptr != '\\') && (*ptr != '/')) { /* go back until file delimiter detected    */
                ptr--;
            }
            ptr++;                                    /* move behind file delimiter               */
        } else {
            ptr = name;
        }

        while ((*ptr != 0u ) && (cnt < TS_NAME_MAX)) { /* end of string or destination memory     */
            if ((path == true) && (*ptr == ' ')) {
                break;
            }
            *string = *ptr;
            string++;
            ptr++;
            cnt++;
        }
    }
    *string = 0u;                                     /* ensure correct end of string             */
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function checks, that the test suite with the given identifier is not explicitly
*          disabled (see TS_DisableSuite). The function returns true if the test must be executed.
*          Otherwise the function returns false.
*
* \param   group
*          Test suite group
*
* \param   suite
*          Test suite in group
*/
/*------------------------------------------------------------------------------------------------*/
static bool TS_IsEnabled(uint8_t group, uint8_t suite)
{
    bool result = true;

    if ((group < TS_GROUP_MAX) && (suite < TS_SUITE_MAX)) {
        if ((TestData.Env.Disable[group] & (1uL << suite)) != 0) {
            /* Test suite is disabled */
            result = false;
        }
    }

    return result;
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function checks, that all test suites are enabled. The function returns true if
*          all test are executed. Otherwise the function returns false.
*/
/*------------------------------------------------------------------------------------------------*/
static bool TS_IsAllEnabled(void)
{
    bool    result = true;
    uint8_t group;

    for (group = 0; group < TS_GROUP_MAX; group++) {
        if (TestData.Env.Disable[group] != 0) {
            /* Not all test suites are enabled */
            result = false;
        }
    }

    return result;
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \details this function is called before each test suite execution
*/
/*------------------------------------------------------------------------------------------------*/
static void TS_SuiteBefore(void)
{
    if (TestData.Env.BeforeSuite) {
        TestData.Env.BeforeSuite();
    }
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function is called after each test suite execution
*/
/*------------------------------------------------------------------------------------------------*/
static void TS_SuiteAfter(void)
{
    if (TestData.Env.AfterSuite) {
        TestData.Env.AfterSuite();
    }
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function initializes the overall test statistics and prints a welcome message
*          to the linked output stream.
*/
/*------------------------------------------------------------------------------------------------*/
static void TS_SummaryInit(void)
{
    if (TestData.Env.BeforeAll) {
        TestData.Env.BeforeAll();
    }

    TestData.Sum.TestCnt = 0u;
    TestData.Sum.FailCnt = 0u;
    TestData.Sum.SkipCnt = 0u;
    TS_Printf("-----------------------\n");
    TS_Printf("    S T A R T\n");
    TS_Printf("-----------------------\n");
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function prints the overall statistic of all executed tests.
*/
/*------------------------------------------------------------------------------------------------*/
static int32_t TS_Summary(void)
{
    int32_t result = TestData.Sum.FailCnt;

    TS_Printf("-----------------------\n");
    TS_Printf("    S U M M A R Y\n");
    TS_Printf("-----------------------\n");
    TS_Printf("%d Tests %d Failures %d Ignored\n",
              TestData.Sum.TestCnt,
              TestData.Sum.FailCnt,
              TestData.Sum.SkipCnt);
    if (TestData.Sum.FailCnt == 0u) {
        TS_Printf("OK\n");
    } else {
        TS_Printf("FAIL\n");
    }

    if (!TS_IsAllEnabled()) {
        TS_Printf("Info: Some Tests are disabled.\n");
    }

    if (TestData.Env.AfterAll) {
        TestData.Env.AfterAll();
    }

    return (result);
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

void TS_TestRun(TS_FUNC func, TS_LITERAL name, TS_LINE line)
{
    TestData.Cur.TestName = name;
    TestData.Cur.Line     = (uint16_t)line ;
    TestData.Cur.TestCnt++;

    if (TestData.Env.BeforeTest) {
        TestData.Env.BeforeTest();                    /* setup function before test               */
    }
    TS_SaveMachineRegs();
    if (TS_PROTECT(TestData.Cur.Context)) {           /* store current context                    */
        func();                                       /* call test function                       */
    }
    if (TestData.Env.AfterTest) {
        TestData.Env.AfterTest();                     /* setup function after test                */
    }

    TS_Result(line);
}


void TS_Finish (void)
{
    TS_RestoreMachineRegs();
    TS_ABORT(TestData.Cur.Context);
}


void TS_Fail(TS_LITERAL msg, TS_LINE line)
{
    if (TestData.Cur.Skip) {
        return;
    }

    TS_Printf("%s:%d:%s:FAIL",
              TestData.Cur.SuiteName,
              line,
              TestData.Cur.TestName);
    if (msg != NULL) {
        TS_Printf(":%s", msg);
    }
    TS_Printf("\n");

    TestData.Cur.Fail = true;
    TS_Finish();
}


void TS_Skip(TS_LITERAL msg, TS_LINE line)
{
    if (TestData.Cur.Skip) {
        return;
    }

    TS_Printf("%s:%d:%s:IGNORE",
              TestData.Cur.SuiteName,
              line,
              TestData.Cur.TestName);
    if (msg != NULL) {
        TS_Printf(":%s", msg);
    }
    TS_Printf("\n");

    TestData.Cur.Skip = true;
    TS_Finish();
}


void TS_DisableSuite(uint8_t group, uint8_t suite)
{
    if ((group < TS_GROUP_MAX) &&
        (suite < TS_SUITE_MAX)) {
        TestData.Env.Disable[group] |= ((uint32_t)1u << suite);
    }
}


void TS_EnableSuite(uint8_t group, uint8_t suite)
{
    if ((group < TS_GROUP_MAX) &&
        (suite < TS_SUITE_MAX)) {
        TestData.Env.Disable[group] &= (uint32_t)~((uint32_t)1u << suite);
    }
}


void TS_DisableGroup(uint8_t group)
{
    if (group < TS_GROUP_MAX) {
        TestData.Env.Disable[group] = (uint32_t)0xFFFFFFFF;
    }
}


void TS_EnableGroup(uint8_t group)
{
    if (group < TS_GROUP_MAX) {
        TestData.Env.Disable[group] = (uint32_t)0;
    }
}


void TS_DisableAll(void)
{
    uint8_t group;

    for (group = 0; group < TS_GROUP_MAX; group++) {
        TestData.Env.Disable[group] = (uint32_t)0xFFFFFFFF;
    }
}


void TS_EnableAll(void)
{
    uint8_t group;

    for (group = 0; group < TS_GROUP_MAX; group++) {
        TestData.Env.Disable[group] = (uint32_t)0;
    }
}


void TS_SetupCase(TS_FUNC before, TS_FUNC after)
{
    TestData.Env.BeforeTest = before;
    TestData.Env.AfterTest  = after;
}


void TS_SetupSuite(TS_FUNC before, TS_FUNC after)
{
    TestData.Env.BeforeSuite = before;
    TestData.Env.AfterSuite  = after;
}


void TS_SetupAll(TS_FUNC before, TS_FUNC after)
{
    TestData.Env.BeforeAll = before;
    TestData.Env.AfterAll  = after;
}


void TS_Begin(TS_LITERAL name)
{
    TS_SetSuiteName(name, &(TestData.Cur.SuiteName[0]));
    TestData.Cur.TestName    = "";
    TestData.Cur.TestCnt     = 0u;
    TestData.Cur.FailCnt     = 0u;
    TestData.Cur.SkipCnt     = 0u;
    TestData.Cur.Fail        = false;
    TestData.Cur.Skip        = false;
}


void TS_End(void)
{
    TS_Printf("-----------------------\n");
    TS_Printf("%d Tests %d Failures %d Ignored\n",
              TestData.Cur.TestCnt,
              TestData.Cur.FailCnt,
              TestData.Cur.SkipCnt);

    TestData.Sum.TestCnt += TestData.Cur.TestCnt;
    TestData.Sum.FailCnt += TestData.Cur.FailCnt;
    TestData.Sum.SkipCnt += TestData.Cur.SkipCnt;

    if (TestData.Cur.FailCnt == 0u) {
        TS_Printf("OK\n");
    } else {
        TS_Printf("FAIL\n");
    }
}


void TS_Init (void)
{
    TS_PrintfInit( NULL, TS_PutChar );                /* set standard interface for test report   */
    TS_PipeInit();

    TS_SetupAll  ( NULL, NULL );                      /* disable all setup functions              */
    TS_SetupSuite( NULL, NULL );
    TS_SetupCase ( NULL, NULL );

    TS_EnableAll ( );                                 /* enable all test groups and suites        */
}

int32_t TS_Start (void)
{
    const TS_INFOFUNC *test;
    int32_t result = 0;
    TS_INFO Info;

    TS_SummaryInit();                                 /* reset summary statistic                  */

    test = (const TS_INFOFUNC *)&TEST_SECTION_START;
    while (test < (const TS_INFOFUNC *)&TEST_SECTION_END) {
        if (*test) {
            (*test)(&Info);                           /* get test suite info data                 */
            if (TS_IsEnabled(Info.Group, Info.Suite)) {
                TS_SuiteBefore();
                Info.Runner();                        /* call test suite runner                   */
                TS_SuiteAfter();
            }
        }
        test++;
    }

    result = TS_Summary();                            /* show test summary statistic              */

    return (result);
}
