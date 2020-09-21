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

#ifndef TS_LOCK_H_
#define TS_LOCK_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "ts_types.h"

/*!
* \addtogroup ts_frm_lock   Test Framework - Lock/Unlock Function Interface
* \details  The lock/unlock function interface is used to adapt the test framework to the common
*           way of protecting data in case of concurrent access in asynchronous program flows
*           (e.g. a RTOS or Interrupts).
*/

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This type specifies the lock state register, passed from lock to unlock
*/
/*------------------------------------------------------------------------------------------------*/
typedef uint32_t TS_LOCK_SR;

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This type specifies the interface of the lock function.
*/
/*------------------------------------------------------------------------------------------------*/
typedef TS_LOCK_SR(*TS_LOCK_FUNC)(void);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This type specifies the interface of the unlock function.
*/
/*------------------------------------------------------------------------------------------------*/
typedef void(*TS_UNLOCK_FUNC)(TS_LOCK_SR);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This type specifies the lock management object.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct TS_LOCK_T {
    uint32_t       LockCnt;
    TS_LOCK_FUNC   LockFunc;
    TS_UNLOCK_FUNC UnlockFunc;
} TS_LOCK;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function allows to set the lock/unlock functions for a locking management.
*          It is possible to disable the locking by setting the lock and unlock functions
*          to NULL.
*
* \param   lock
*          Lock management object
*
* \param   lockFunc
*          reference to function, called for locking managed component
*
* \param   unlockFunc
*          reference to function, called for unlocking managed component
*/
/*------------------------------------------------------------------------------------------------*/
void TS_LockSetup(TS_LOCK *lock, TS_LOCK_FUNC lockFunc, TS_UNLOCK_FUNC unlockFunc);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function locks the managed component. 
*
* \param   lock
*          Lock management object
*
* \return  This function returns the locking state, before calling this function.
*/
/*------------------------------------------------------------------------------------------------*/
TS_LOCK_SR TS_Lock(TS_LOCK *lock);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function unlocks the managed component. To allow nested locking, the
*          given lock state will be restored with this function call.
*
* \param   lock
*          Lock management object
*
* \param   state
*          Locking state, which will be restored in this function.
*/
/*------------------------------------------------------------------------------------------------*/
void TS_Unlock(TS_LOCK *lock, TS_LOCK_SR state);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif /* TS_LOCK_H_ */
