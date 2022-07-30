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

#include "ts_lock.h"
#include "ts_printf.h"

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

#define TS_LOCK_UNBALANCED   1

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This macro allows the definition of an activity when detecting an usage error. In the
*          test framework we print a message and stop executing the tests.
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_LockError(code,line) \
    TS_Printf("Abort Test: Lock Error %d in Line %d", code, line); \
    while(1)

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

void TS_LockSetup(TS_LOCK *lock, TS_LOCK_FUNC lockFunc, TS_UNLOCK_FUNC unlockFunc)
{
    if (lockFunc == NULL) {
        unlockFunc = NULL;
    } else if (unlockFunc == NULL) {
        lockFunc = NULL;
    }

    lock->LockCnt    = 0;
    lock->LockFunc   = lockFunc;
    lock->UnlockFunc = unlockFunc;
}

TS_LOCK_SR TS_Lock(TS_LOCK *lock)
{
    TS_LOCK_SR state = 0;

    if (lock->LockFunc != NULL) {
        if (lock->LockCnt == 0) {
            state = lock->LockFunc();
        }
        lock->LockCnt++;
    }

    return (state);
}

void TS_Unlock(TS_LOCK *lock, TS_LOCK_SR state)
{
    if (lock->UnlockFunc != NULL) {
        if (lock->LockCnt > 0) {
            lock->LockCnt--;
            if (lock->LockCnt == 0) {
                lock->UnlockFunc(state);
            }
        } else {
            TS_LockError(TS_LOCK_UNBALANCED, __LINE__);
        }
    }
}
