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

#ifndef CO_TYPES_H_
#define CO_TYPES_H_

/******************************************************************************
* INCLUDES
******************************************************************************/

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/******************************************************************************
* PUBLIC MACROS
******************************************************************************/

#ifdef _DEBUG
/* For unit testing and debug builds, the marked implementation of functions
 * are included as weak symbol in the CANopen library. This weak symbols help
 * us to improve testability by allowing stubbing functions in the test cases
 * with stub functions of the same symbol name.
 */
#ifndef WEAK_TEST
#define WEAK_TEST    __attribute__((weak))
#endif
#else
#define WEAK_TEST
#endif
#define WEAK    __attribute__((weak))

#define CO_UNUSED(var)    (void)(var)

#define ASSERT_PTR_ERR(ptr,err)    \
    if ((void*)(ptr) == NULL) { return (err); }

#define ASSERT_EQU_ERR(var,val,err)    \
    if ((var) != (val)) { return (err); }

#define ASSERT_LOWER(var,val)    \
    if ((var) >= (val)) { return; }

#define ASSERT_LOWER_ERR(var, val, err)    \
    if ((var) >= (val)) { return (err); }

#define ASSERT_NOT(var,val)    \
    if ((var) == (val)) { return; }

#define ASSERT_NOT_ERR(var,val,err)    \
    if ((var) == (val)) { return (err); }

#define ASSERT_PTR(ptr)    \
    if ((void*)(ptr) == NULL) { return; }

#define ASSERT_PTR_FATAL(ptr)    \
    if ((void*)(ptr) == NULL) { CONodeFatalError(); return; }

#define ASSERT_PTR_FATAL_ERR(ptr, err)    \
    if ((void*)(ptr) == NULL) { CONodeFatalError(); return(err); }

#endif /* ifndef CO_TYPES_H_ */
