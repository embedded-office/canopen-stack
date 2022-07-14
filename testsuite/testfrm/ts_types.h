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

#ifndef TYPES_H_
#define TYPES_H_

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

#if defined(__STDC__) && (__STDC_VERSION__ >= 199901L) ||       /* ANSI C99 */\
    defined(_MSC_VER) || \
    defined(__cplusplus)

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#else                                                            /* ANSI C90 */

/*-- stddef.h --*/
#define NULL                0u

/*-- stdbool.h --*/
typedef int                 bool;
#define false               0u
#define true                1u

/*-- stdint.h --*/
typedef unsigned char       uint8_t;
typedef   signed char       int8_t;
typedef unsigned short      uint16_t;
typedef   signed short      int16_t;
typedef unsigned long       uint32_t;
typedef   signed long       int32_t;
typedef unsigned long long  uint64_t;
typedef   signed long long  int64_t;

/*-- crtdefs.h --*/
typedef unsigned int        uintptr_t;

#endif

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

#if defined ( _MSC_VER )
#define TEST_SECTION_PRE          __declspec(allocate(".test$u"))
#define TEST_SECTION_DEF          __pragma(section(".test$u", read))
#define TEST_SECTION_SUF
#define TEST_SECTION_START        __start_test
#define TEST_SECTION_END          __stop_test
#define TEST_SECTION_START_DEF    __pragma(section(".test$a", read))
#define TEST_SECTION_START_ALLOC  __declspec(allocate(".test$a")) static const TS_INFOFUNC TEST_SECTION_START = (TS_INFOFUNC)0;
#define TEST_SECTION_END_DEF      __pragma(section(".test$z", read))
#define TEST_SECTION_END_ALLOC    __declspec(allocate(".test$z")) static const TS_INFOFUNC TEST_SECTION_END = (TS_INFOFUNC)0;
#define STRUCT_PACKED_PRE         __pragma(pack(push, 1))
#define STRUCT_PACKED_SUF         __pragma(pack(pop))

#elif defined(__APPLE__) && defined(__MACH__)
#define TEST_SECTION_PRE          __declspec(allocate("__TEST,.test$u"))
#define TEST_SECTION_DEF          __attribute__((section ("__TEST,.test$a")))
#define TEST_SECTION_SUF
#define TEST_SECTION_START        __start_test
#define TEST_SECTION_END          __stop_test
#define TEST_SECTION_START_DEF    __attribute__((section ("__TEST,.test$a")))
#define TEST_SECTION_START_ALLOC  __declspec(allocate("__TEST,.test$a")) static const TS_INFOFUNC TEST_SECTION_START = (TS_INFOFUNC)0;
#define TEST_SECTION_END_DEF      __attribute__((section ("__TEST,.test$z")))
#define TEST_SECTION_END_ALLOC    __declspec(allocate("__TEST,.test$z")) static const TS_INFOFUNC TEST_SECTION_END = (TS_INFOFUNC)0;
#define STRUCT_PACKED_PRE
#define STRUCT_PACKED_SUF
#else
/*
* \note  The testsuite is running with MSVC compiler on the windows host, only. You may
*        adjust the settings here and provide an output channel in ts_output.c to get
*        the tests running on your target, too.
*/
#define TEST_SECTION_PRE          __attribute__((section(".test")))
#define TEST_SECTION_DEF
#define TEST_SECTION_SUF
#define TEST_SECTION_START        __start_test
#define TEST_SECTION_END          __stop_test
#define TEST_SECTION_START_DEF
#define TEST_SECTION_START_ALLOC  static const TS_INFOFUNC TEST_SECTION_START = (TS_INFOFUNC)0;
#define TEST_SECTION_END_DEF
#define TEST_SECTION_END_ALLOC    static const TS_INFOFUNC TEST_SECTION_END = (TS_INFOFUNC)0;
#define STRUCT_PACKED_PRE
#define STRUCT_PACKED_SUF         __attribute__((packed))
#endif

#endif /* TYPES_H_ */
