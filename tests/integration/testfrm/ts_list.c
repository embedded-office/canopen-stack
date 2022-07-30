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

#include "ts_list.h"
#include "ts_lock.h"

/******************************************************************************
* PRIVATE MACROS
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This assertion ensures, that the given pointer is not NULL.
*
* \param   ptr
*          Pointer which is assumed to be not NULL.
*/
/*------------------------------------------------------------------------------------------------*/
#define ASSERT_NOT_NULL(ptr)   if ((ptr) == NULL) return

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This assertion ensures, that the given item is initialized, but not used in a linked
*          list.
*
* \note    This assertion can be used after @ref ASSERT_NOT_NULL(item)
*
* \param   item
*          Item which is assumed to be initialized and not used.
*/
/*------------------------------------------------------------------------------------------------*/
#define ASSERT_NOT_USED(item)  if (((item)->Next == NULL) || ((item)->Next != (item))) return

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This assertion ensures, that the given item is initialized and used in a linked list.
*
* \note    This assertion can be used after @ref ASSERT_NOT_NULL(item)
*
* \param   item
*          Item which is assumed to be initialized and linked in any list.
*/
/*------------------------------------------------------------------------------------------------*/
#define ASSERT_IS_USED(item)   if (((item)->Next == NULL) || ((item)->Next == (item))) return

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

static TS_LOCK ts_ListLock = { 0 };                   /*!< List lock management                   */

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

void TS_ListSetup(TS_LOCK_FUNC lock, TS_UNLOCK_FUNC unlock)
{
    TS_LockSetup(&ts_ListLock, lock, unlock);
}

void TS_ListInit( TS_LIST_ITEM *item )
{
    ASSERT_NOT_NULL( item );

    item->Prev = item;
    item->Next = item;
}


void TS_ListAddHead(TS_LIST_ITEM *head, TS_LIST_ITEM *item)
{
    TS_LOCK_SR state = 0;
    
    ASSERT_NOT_NULL( head );
    ASSERT_NOT_NULL( head->Next );
    ASSERT_NOT_NULL( item );
    ASSERT_NOT_USED( item );
    
    state = TS_Lock(&ts_ListLock);
    
    item->Next       = head->Next;
    item->Prev       = head;
    item->Next->Prev = item;
    head->Next       = item;
    
    TS_Unlock(&ts_ListLock, state);
}


void TS_ListAddTail(TS_LIST_ITEM *head, TS_LIST_ITEM *item)
{
    TS_LOCK_SR state = 0;

    ASSERT_NOT_NULL( head );
    ASSERT_NOT_NULL( head->Next );
    ASSERT_NOT_NULL( item );
    ASSERT_NOT_USED( item );

    state = TS_Lock(&ts_ListLock);

    item->Next       = head;
    item->Prev       = head->Prev;
    item->Prev->Next = item;
    head->Prev       = item;

    TS_Unlock(&ts_ListLock, state);
}


void TS_ListRemove(TS_LIST_ITEM *item)
{
    TS_LOCK_SR state = 0;

    ASSERT_NOT_NULL( item );
    ASSERT_IS_USED ( item );

    state = TS_Lock(&ts_ListLock);

    item->Prev->Next = item->Next;
    item->Next->Prev = item->Prev;

    TS_Unlock(&ts_ListLock, state);
}


TS_LIST_ITEM *TS_ListFetchHead(TS_LIST_ITEM *head)
{
    TS_LOCK_SR state = 0;
    TS_LIST_ITEM *item = NULL;

    ASSERT_NOT_NULL( head )(NULL);
    ASSERT_IS_USED ( head )(NULL);

    state = TS_Lock(&ts_ListLock);

    item             = head->Next;
    item->Next->Prev = head;
    head->Next       = item->Next;

    TS_Unlock(&ts_ListLock, state);

    return item;
}


TS_LIST_ITEM *TS_ListFetchTail(TS_LIST_ITEM *head)
{
    TS_LOCK_SR state = 0;
    TS_LIST_ITEM *item = NULL;

    ASSERT_NOT_NULL( head )(NULL);
    ASSERT_IS_USED ( head )(NULL);

    state = TS_Lock(&ts_ListLock);
    
    item             = head->Prev;
    item->Prev->Next = head;
    head->Prev       = item->Prev;

    TS_Unlock(&ts_ListLock, state);
    
    return item;
}


bool TS_ListContains(TS_LIST_ITEM *head, TS_LIST_ITEM *item)
{
    TS_LOCK_SR state = 0;
    bool result = false;
    TS_LIST_ITEM *cursor;

    ASSERT_NOT_NULL( head )(false);
    ASSERT_IS_USED ( head )(false);
    ASSERT_NOT_NULL( item )(false);
    ASSERT_IS_USED ( item )(false);

    state = TS_Lock(&ts_ListLock);

    TS_LIST_FOR_EACH(cursor, head) {
        if (cursor == item) {
            result = true;
            break;
        }
    }

    TS_Unlock(&ts_ListLock, state);

    return result;
}
