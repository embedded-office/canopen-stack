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

#ifndef TS_CONTEXT_H_
#define TS_CONTEXT_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include <setjmp.h>
#include "ts_types.h"

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This type is used to store a context of the CPU.
*/
/*------------------------------------------------------------------------------------------------*/
typedef jmp_buf TS_CONTEXT;

/******************************************************************************
* PUBLIC MACROS
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This macro will set the scratch register, which is used as function result, to 0.
*          Afterwards the current context is stored in the given memory area.
*
* \param   ctx
*          This argument must be a reference to a variable of type @ref TS_CONTEXT
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_PROTECT(ctx)   (setjmp(ctx) == 0)

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This macro is used to restore the context from the @ref TS_CONTEXT memory area.
*          Afterwards the scratch register, which is used as function result, is set to 1.
*
* \param   ctx
*          This argument must be a reference to a variable of type @ref TS_CONTEXT
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_ABORT(ctx)   do { \
        longjmp((ctx), 1u);  \
    } while(0)

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function saves registers, additional to the common context. This may be useful
*          for CPU dependent registers, like FPU control, MPU control etc.
*/
/*------------------------------------------------------------------------------------------------*/
void TS_SaveMachineRegs(void);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function restores the registers, additional to the common context. This may be
*          useful for CPU dependent registers, like FPU control, MPU control etc.
*/
/*------------------------------------------------------------------------------------------------*/
void TS_RestoreMachineRegs(void);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif /* TS_CONTEXT_H_ */
