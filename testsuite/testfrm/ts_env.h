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

#ifndef TS_ENV_H_
#define TS_ENV_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "ts_types.h"
#include "ts_printf.h"
#include "ts_version.h"

/*!
* \addtogroup ts_frm  Test Framework
* \details    The Embedded Office Test Framework is designed to perform host and target integration
*             tests in an efficient and readable way.
*
*             #### Glossary
*
*             For a common understanding, some wordings are listed below.
*
*             + test suite  - The collection of all test case functions with a specific test focus
*                             is called *test suite*
*             + test group  - Test suites are organized with in test groups. A single test group
*                             can hold up to 32 test suites.
*             + test module - The test module is the collection of files, needed for a single test
*                             suite. The collection may contain test module helper functions for
*                             reuse within this test suite.
*             + test case   - A single test case function, performing the individual checks.
*
*             #### Basic Usage Example
*
*             Before explaining the test framework in detail, a small (super simple) example should
*             show the principles and demonstrate the ease of use. Assume the following module,
*             which should be tested (it is the 'unit-under-test'):
*
*             \code
*             uint32_t AbsSum (int32_t a, int32_t b)
*             {
*                 if (a < 0) {
*                     a = -a;
*                 }
*                 if (b < 0) {
*                     b = -b;
*                 }
*                 return (a + b);
*             }
*             \endcode
*
*             This function should add the absolut values of the two arguments and return the
*             result. To get one aspect of this function tested, the lines for a test case
*             function may look like:
*
*             \code
*             TS_DEF_MAIN( TS_AbsSum_Pos_Pos )
*             {
*                 uint32_t result;
*                 result = AbsSum(22, 20);
*                 TS_ASSERT(result == 42);
*             }
*             \endcode
*
*             Typically we need multiple of these functions for testing one single feature. Just
*             write multiple tests and follow these six rules:
*
*             + Name the test case with a self explaining name, so if this test fails we can
*               locate the problem.One idea for a easy naming convention could be
*               "TS_<test-focus>_<scenario>"
*             + Keep the test short: max. 12 lines of executable test code
*             + Test only a single aspect in a test case, so if a single requirement is changed,
*               only a single test should fail (in theory). Well, all tests which requires the
*               unit-unter-test for preparing the precondition will fail, too. We may identify
*               a suitable way on skipping these tests in the future.
*             + Use doxygen comments to highlight the preparation, test and check regions of
*               your test
*             + The unit-under-test must never be changed to become testable
*             + We must never use the unit-under-test to build the test framework, global
*               test framework helper functions or any other test framework related feature.
*               Reason : if unit-under-test is changed, the test framework itself is broken!
*
*             For executing all the test cases, we need a single function in the test file to
*             control the execution of the test functions:
*
*             \code
*             TS_DEF_SUITE(0, 1)  << organized in: group #0, suite #1
*             {
*                 TS_Begin(__FILE__);
*                 TS_RUNNER(TS_AbsSum_Pos_Pos);
*                   :
*                 TS_End();
*             }
*             \endcode
*
*             #### Test Execution Context
*
*             Test execution takes place in the main() function. This implies the C runtime
*             stack is used for running the test framework. When creating test case functions,
*             the test framework provides different runtime contexts for test execution.
*
*             **Main Context Test**
*
*             The basic test execution context is a pure function, running on C runtime stack.
*             This means the test case function is called in a call-tree, starting from `main()`
*             (e.g. no context switch like it is done by realtime kernels). This context is
*             prepared with the test case definition macro:
*
*             \code
*             TS_DEF_MAIN( myTestFunc )
*             {
*                 << this is the function body, running on system stack >>
*             }
*             \endcode
*
*             The following sequence diagram shows the principle execution flow for this kind
*             of test:
*
*             \startuml Reinhards Diagram.
*             Bob -> Alice
*             \enduml
*
* @{
*/

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This define specifies the type of a line number, given by the predefined compiler
*          define '```__LINE__```'.
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_LINE   const int

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This define specifies the type of a string memory.
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_STRING   char *

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This define specifies the type of a single character.
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_CHAR   char

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This define specifies the type of a literal string.
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_LITERAL   const char *

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This define specifies the maximum number of test suites per group.
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_SUITE_MAX   32

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This define specifies the maximum number of test groups.
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_GROUP_MAX   16

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This type specifies the test case function interface.
*/
/*------------------------------------------------------------------------------------------------*/
typedef void(*TS_CASE)(void);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This type specifies the test callback function interface for the setup functions before
*          and after a certain point.
*/
/*------------------------------------------------------------------------------------------------*/
typedef void(*TS_FUNC)(void);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This type is the container with all test suite info data.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct TS_INFO {
    TS_CASE    Runner;                                /*!< Test case runner function              */
    uint8_t    Group;                                 /*!< Test suite group                       */
    uint8_t    Suite;                                 /*!< Test suite in group                    */
} TS_INFO;

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This type specifies the test suite info function interface for the dynamic running
*          out of a test suite collection.
*/
/*------------------------------------------------------------------------------------------------*/
typedef void(*TS_INFOFUNC)(TS_INFO *);

/******************************************************************************
* PUBLIC MACROS
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This macro creates a constant holding the line number of the test function and declares
*          the test function prototype.
*
* \note    This macro must declare the function with the test case function prototype, matching the
*          type definition: \ref TS_CASE.
*
* \param   name
*          Name of the test case function
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_DEF_MAIN(name)              \
    TS_LINE name ## _Line = __LINE__;  \
    static void name (void);           \
    static void name (void)

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This macro creates a constant holding the line number of the test function and declares
*          the test function prototype for a RTOS task with the test priority \ref TS_TEST_PRIO.
*
* \note    The test case function must be finished with @ref TS_OSFinish().
*
* \param   name
*          Name of the test case function
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_DEF_TASK(name)              \
    TS_LINE name ## _Line = __LINE__;  \
    static void name ## _task (void);  \
    static void name (void);           \
    static void name (void) {          \
        TS_KernelStart(name ## _task); \
    }                                  \
    static void name ## _task (void)

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This macro creates the test suite function. The macro considers the naming and correct
*          function prototype.
*
* \note    For the macro arguments, use lower digits and no prefix '0x' for the hexadecimal digits
*
* \param   group
*          Test group identifier
*
* \param   suite
*          Test suite identifier in this group
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_DEF_SUITE(group,suite)                                             \
    static void TestSuiteRunner(void);                                        \
    static void TestSuiteInfo(TS_INFO *info) {                                \
        info->Group  = group;                                                 \
        info->Suite  = suite;                                                 \
        info->Runner = TestSuiteRunner;                                       \
    }                                                                         \
    TEST_SECTION_DEF                                                          \
    static TEST_SECTION_PRE TS_INFOFUNC Ptr = TestSuiteInfo TEST_SECTION_SUF; \
    uintptr_t UnusedDummy##group##suite = (uintptr_t)&Ptr;                    \
    static void TestSuiteRunner(void)

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This macro creates the test case runner function call for the given test case function.
*
* \param   name
*          Name of the test case function
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_RUNNER(name)                    \
    TS_TestRun(name, #name, name ## _Line)

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This test assert macro evaluates the expected condition and passes, if the condition is
*          true.
*
* \param   condition
*          Expected condition expression
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_ASSERT(condition)                       \
    if (condition) { } else {                      \
        TS_Fail(" Expected condition (" #condition \
                ") is not true.", __LINE__); }

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This test macro ignores the execution of the following lines and prints the given
*          reason.
*
* \param   reason
*          Reason, why the following lines should be ignored
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_IGNORE(reason) do {      \
    if (reason) {                   \
        TS_Skip(reason, __LINE__);  \
    } else {                        \
        TS_Skip(NULL, __LINE__);    \
    } } while(0)

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

#ifdef  __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function stores the current context in a test context buffer and calls the given
*          test function 'function'. The additional arguments 'name' and 'line' are used to print
*          useful messages during test execution.
*
* \param   function
*          Function pointer to test function
*
* \param   name
*          Name of the test function
*
* \param   line
*          Line number of test function
*/
/*------------------------------------------------------------------------------------------------*/
void TS_TestRun(TS_CASE function, TS_LITERAL name, TS_LINE line);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function aborts the current running test by restoring the previously stored
*          context.
*/
/*------------------------------------------------------------------------------------------------*/
void TS_Finish (void);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function prints a useful message of failed test execution and restores the
*          previously stored context from the test context buffer. This results in a jump back to
*          the function \ref TS_TestRun().
*
* \param   message
*          Verbose failure message
*
* \param   line
*          Line number of check for expected value(s)
*/
/*------------------------------------------------------------------------------------------------*/
void TS_Fail(TS_LITERAL message, TS_LINE line);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function prints a useful message of an ignored test execution and restores the
*          previously stored context from the test context buffer. This results in a jump back
*          to the function TS_TestRun().
*
* \param   message
*          Verbose ignore message
*
* \param   line
*          Line number of ignore statement
*/
/*------------------------------------------------------------------------------------------------*/
void TS_Skip(TS_LITERAL message, TS_LINE line);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function resets the statistics of a single test execution. The given string is
*          filtered to result in a pure filename (optionally with revision) and no path.
*
* \param   name
*          Constant string with name of test suite
*/
/*------------------------------------------------------------------------------------------------*/
void TS_Begin(TS_LITERAL name);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function defines the setup functions, which must be called before and/or after
*          the test case execution. If before or after is not wanted, calling with NULL will
*          skip the corresponding setup function call.
*
* \param   before
*          Function called before test case execution
*
* \param   after
*          Function called after test case execution
*/
/*------------------------------------------------------------------------------------------------*/
void TS_SetupCase(TS_FUNC before, TS_FUNC after);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function prints the statistics of a test suite and returns the number of failures
*          during execution of this test suite.
*/
/*------------------------------------------------------------------------------------------------*/
void TS_End(void);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function initializes the test framework.
*/
/*------------------------------------------------------------------------------------------------*/
void TS_Init(void);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function defines the setup functions, which must be called before and/or after
*          the test suite execution. If before or after is not wanted, calling with NULL will
*          skip the corresponding setup function call.
*
* \param   before
*          Function called before test suite execution
*
* \param   after
*          Function called after test suite execution
*/
/*------------------------------------------------------------------------------------------------*/
void TS_SetupSuite(TS_FUNC before, TS_FUNC after);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function defines the setup functions, which must be called before and/or after
*          the overall test execution. If before or after is not wanted, calling with NULL will
*          skip the corresponding setup function call.
*
* \param   before
*          Function called before test initialization starts
*
* \param   after
*          Function called after test summary is executed
*/
/*------------------------------------------------------------------------------------------------*/
void TS_SetupAll(TS_FUNC before, TS_FUNC after);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function is allows enabling all test suites.
*/
/*------------------------------------------------------------------------------------------------*/
void TS_EnableAll(void);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function is allows disabling all test suites.
*/
/*------------------------------------------------------------------------------------------------*/
void TS_DisableAll(void);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function is allows enabling all test suites of a test group.
*
* \param   group
*          Test group identifier
*/
/*------------------------------------------------------------------------------------------------*/
void TS_EnableGroup(uint8_t group);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function is allows disabling all test suites of a test group.
*
* \param   group
*          Test group identifier
*/
/*------------------------------------------------------------------------------------------------*/
void TS_DisableGroup(uint8_t group);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function is allows enabling a test suite.
*
* \param   group
*          Test group identifier
*
* \param   suite
*          Test suite identifier in this group
*/
/*------------------------------------------------------------------------------------------------*/
void TS_EnableSuite(uint8_t group, uint8_t suite);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function is allows disabling a test suite.
*
* \param   group
*          Test group identifier
*
* \param   suite
*          Test suite identifier in this group
*/
/*------------------------------------------------------------------------------------------------*/
void TS_DisableSuite(uint8_t group, uint8_t suite);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function runs the tests.
*
* \return  The function returns the number of failed tests.
*/
/*------------------------------------------------------------------------------------------------*/
int32_t TS_Start(void);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif /* TS_ENV_H_ */
/*! @} */
