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

#ifndef TS_LIST_H_
#define TS_LIST_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "ts_types.h"
#include "ts_lock.h"

/*!
* \addtogroup ts_frm_list   Test Framework - Dynamic List
* \details  The linked list is a very common way in organizing data. This module uses
*           double-linked lists. As you see, the structure contains two references to
*           instances of the same structure: to the previous and to the next one.
*
*           \code
*           typedef struct TS_LIST_ITEM_T TS_LIST_ITEM;
*           typedef struct TS_LIST_ITEM_T {
*               TS_LIST_ITEM *prev;
*               TS_LIST_ITEM *next;
*           } TS_LIST_ITEM;
*           \endcode
*
*           \startuml
* digraph g {
*           graph [rankdir = "LR"];
*           head  [style=filled, fillcolor = "#ff7777"
*                  shape=record, label="TS_LIST_ITEM\n(head)  | <n> next | <p> prev"];
*           item1 [shape=record, label="TS_LIST_ITEM\n(item1) | <n> next | <p> prev"];
*           item2 [shape=record, label="TS_LIST_ITEM\n(item2) | <n> next | <p> prev"];
*           head  : n->item1 : n
*           item1 : n->item2 : n
*           item1 : p->head  : p
*           item2 : p->item1 : p
*           }
*           \enduml
*
*           This is very nice, but probably not particularly useful. We'd like to have some
*           payload in each object, don't we? The approach is to embed `TS_LIST_ITEM` into
*           any other structure we'd like to chain. For example, assume we have the structure
*           `MY_BLOCK`:
*
*           \code
*           typedef struct MY_BLOCK_T {
*               int field1;
*               int field2;
*               int field3;
*           } MY_BLOCK;
*           \endcode
*
*           And we want to make these structures chainable. First of all, we need to embed
*           `TS_LIST_ITEM` into it. For this example, it would be logical to put it in the
*           beginning of `MY_BLOCK`, but just to emphasize that the structure can be anywhere
*           inside, not only in the beginning, let's put it somewhere in the middle:
*
*           \code
*           typedef struct MY_BLOCK_T {
*               int field1;
*               int field2;
*               TS_LIST_ITEM list_item;  <<<< say, embed it here
*               int field3;
*           } MY_BLOCK;
*           \endcode
*
*           Ok, nice.Now, create some instances of it. And additional, one more crucial point:
*           create a head of the list. A head is just a regular `TS_LIST_ITEM`, but not
*           embedded anywhere:
*
*           \code
*           MY_BLOCK block_first = { ... };
*           MY_BLOCK block_second = { ... };
*           TS_LIST_ITEM my_blocks;
*           \endcode
*
*           Now, we can create a list by code like this(*Of course, it's not wise to always do
*           that manually, it's just for clarity!*) :
*
*           \code
*           my_blocks.next = &block_first.list_item;
*           my_blocks.prev = &block_second.list_item;
*
*           block_first.list_item.next = &block_second.list_item;
*           block_first.list_item.prev = &my_blocks;
*
*           block_second.list_item.next = &my_blocks;
*           block_second.list_item.prev = &block_first.list_item;
*           \endcode
*
*           \startuml
* digraph g {
*           graph[rankdir = "LR"];
*           head[style = filled, fillcolor = "#ff7777"
*                shape = record, label = "TS_LIST_ITEM\n(my_blocks) |<n> next | <p> prev"];
*           subgraph myblock1 {
*             struct1[shape = record, label = "MY_BLOCK\n(block_first) | field1 | field2 || <n> list_item.next | <p> list_item.prev || field3"];
*           }
*           subgraph myblock2 {
*             struct2[shape = record, label = "MY_BLOCK\n(block_second) | field1 | field2 || <n> list_item.next | <p> list_item.prev || field3"];
*           }
*           head    : n->struct1 : n
*           struct1 : n->struct2 : n
*           struct1 : p->head    : p
*           struct2 : p->struct1 : p
*           }
*           \enduml
*
*          From the above, it's clear that we still have chains of `TS_LIST_ITEM`, not of
*          `MY_BLOCK`. But the thing is: the offset from the beginning of `MY_BLOCK` to its
*          `list_item` is constant for all instances of `MY_BLOCK`. So, if we have a pointer
*          to `TS_LIST_ITEM`, and we know that this instance of it is embedded to `MY_BLOCK`,
*          we can subtract the known offset, and we get a pointer to `MY_BLOCK`. 
*
*          #### Getting a List Element
*
*          There is a special macro for that: `TS_LIST_DATA()`
*
*          \code
*          // Given a pointer 'ptr' to the field 'member' embedded into 'type' (usually
*          // a struct), return pointer to the instance of 'type'.
*          #define TS_LIST_DATA(ptr, type, member) \
*              ((type *)((char *)(ptr)-(char *)(&((type *)0)->member)))
*          \endcode
*
*          So given a pointer to `TS_LIST_ITEM`, we can easily get a pointer to wrapping
*          `MY_BLOCK` as follows:
*
*          \code
*          TS_LIST_ITEM *p_item = ... ;
*          MY_BLOCK *p_my_block = TS_LIST_DATA(p_item, MY_BLOCK, list_item);
*          \endcode
*
*          #### Iterate through the List
*
*          The first approach is to write:
*
*          \code
*          TS_LIST_ITEM *p_cursor;
*          MY_BLOCK     *p_cur_block;  
*          for (p_cursor = my_blocks.next; p_cursor != &my_blocks; p_cursor = p_cursor->next) {
*              p_cur_block = TS_LIST_DATA(p_cur, MY_BLOCK, list_item);
*              // now 'p_cur_block' points to the next block in the list
*          }
*          \endcode
*
*          This code works, but it is just a little bit too much to write. It's much better to
*          use a special macro for iterations like this: `TS_LIST_FOR_EACH_DATA()`. This way,
*          we can just hide all the mess and iterate through the list of our `MY_BLOCK` instances
*          like this:
*
*          \code
*          MY_BLOCK *p_cur_block;
*          TS_LIST_FOR_EACH_DATA(p_cur_block, MY_BLOCK, &my_blocks, list_item) {
*              // now 'p_cur_block' points to the next block in the list
*          }
*          \endcode
*
*          So, it's very reliable way to create list of objects. And we can even include the
*          same object in multiple lists: for this, of course, the wrapping structure should
*          have multiple instances of `TS_LIST_ITEM`: for each list that the object can be
*          included into.
* @{
*/

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details Linked list item.
*/
/*------------------------------------------------------------------------------------------------*/
struct TS_LIST_ITEM_T;

typedef struct TS_LIST_ITEM_T {
    struct TS_LIST_ITEM_T *Prev;                      /*!< Pointer to previous item               */
    struct TS_LIST_ITEM_T *Next;                      /*!< Pointer to next item                   */
} TS_LIST_ITEM;

/******************************************************************************
* PUBLIC MACROS
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This macro condition is true, if list is empty.
*
* \param   head
*          The head for your list.
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_LIST_IS_EMPTY(head)                               \
    ( ((head)->Next == (head)) && ((head)->Prev == (head)) )


/*------------------------------------------------------------------------------------------------*/
/*!
* \details This macro loop iterates through list, starting with first item after head.
*
* \param   cursor
*          Pointer to item, used as a loop cursor.
*
* \param   head
*          Pointer to head of list.
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_LIST_FOR_EACH(cursor, head) \
    for (cursor = (head)->Next;        \
         cursor != (head);             \
         cursor = cursor->Next)


/*------------------------------------------------------------------------------------------------*/
/*!
* \details This macro calculates the pointer to data structure for given item.
*
* \param   item
*          Pointer to list item
*
* \param   type
*          Name of data type definition.
*
* \param   member
*          Name of member within data type definition.
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_LIST_DATA(item, type, member)                        \
    ((type *)((char *)(item)-(char *)(&((type *)0)->member)))


/*------------------------------------------------------------------------------------------------*/
/*!
* \details This macro loop iterates through list of given type, starting with first item after
*          head.
*
* \param   cursor
*          Pointer to item, used as a loop cursor.
*
* \param   type
*          Name of data type definition.
*
* \param   head
*          Pointer to head of list.
*
* \param   member
*          Name of member within data type definition.
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_LIST_FOR_EACH_DATA(cursor, type, head, member)           \
    for (cursor = TS_LIST_DATA((head)->Next, type, member);         \
         &cursor->member != (head);                                 \
         cursor = TS_LIST_DATA((cursor)->member.Next, type, member))

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This macro provides the item data pointer for the first item in a list.
*
* \param   type
*          Name of data type definition.
*
* \param   head
*          Pointer to head of list.
*
* \param   member
*          Name of member within data type definition.
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_LIST_HEAD_DATA(type, head, member)    \
         TS_LIST_DATA((head)->Next, type, member)

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This macro provides the item data pointer for the last item in a list.
*
* \param   type
*          Name of data type definition.
*
* \param   head
*          Pointer to head of list.
*
* \param   member
*          Name of member within data type definition.
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_LIST_TAIL_DATA(type, head, member)    \
         TS_LIST_DATA((head)->Prev, type, member)

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function setups the lock and unlock function for the list management.
*
* \param   lock
*          The reference to the list locking function
*
* \param   unlock
*          The reference to the list unlock function
*/
/*------------------------------------------------------------------------------------------------*/
void TS_ListSetup(TS_LOCK_FUNC lock, TS_UNLOCK_FUNC unlock);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function initializes a list item, independent on the current usage or the current
*          initialization stage.
*
* \param   item
*          Pointer to list item
*/
/*------------------------------------------------------------------------------------------------*/
void TS_ListInit(TS_LIST_ITEM *item);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function adds the given list item after the head of list.
*
* \param   head
*          Head for list.
*
* \param   item
*          Pointer to new item.
*/
/*------------------------------------------------------------------------------------------------*/
void TS_ListAddHead(TS_LIST_ITEM *head, TS_LIST_ITEM *item);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function adds the given list item at the end of list.
*
* \param   head
*          Head for list.
*
* \param   item
*          Pointer to new list item.
*/
/*------------------------------------------------------------------------------------------------*/
void TS_ListAddTail(TS_LIST_ITEM *head, TS_LIST_ITEM *item);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function removes the given list item from list.
*
* \param   item
*          Pointer to list item
*
* \note    We shouldn't reset the item itself, because the pointers in the removed item are often
*          needed to get the next item of the list from which it was just removed.
*
* \note    Typical usage: iterate over the list, check some condition and probably remove the
*          item. After removal, we are still able to get the next item.
*/
/*------------------------------------------------------------------------------------------------*/
void TS_ListRemove(TS_LIST_ITEM *item);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function removes and returns the list item from the head of list.
*
* \param   head
*          Pointer to head of list.
*/
/*------------------------------------------------------------------------------------------------*/
TS_LIST_ITEM *TS_ListFetchHead(TS_LIST_ITEM *head);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function removes and returns the list item from the end of list.
*
* \param   head
*          Pointer to head of list.
*/
/*------------------------------------------------------------------------------------------------*/
TS_LIST_ITEM *TS_ListFetchTail(TS_LIST_ITEM *head);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function returns true, if the given list item is existing in list.
*
* \param   head
*          pointer to head of list.
*
* \param   item
*          Pointer to searched item.
*/
/*------------------------------------------------------------------------------------------------*/
bool TS_ListContains(TS_LIST_ITEM *head, TS_LIST_ITEM *item);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}  /* extern "C" */
#endif

#endif /* TS_LIST_H_ */
/*! @} */
