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

#include "ts_pipe.h"
#include "ts_mem.h"
#include "ts_printf.h"
#include <string.h>

/******************************************************************************
* PRIVATE DEFINES
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

#define TS_PIPE_OUT_OF_MEM   1                        /*!< Error Code: Out of Memory              */

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This define specifies the size (in bytes) of the memory area, used for the pipes.
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_PIPE_MEM_SIZE     1024

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This macro allows the definition of an activity when detecting an usage error. In the
*          test framework we print a message and stop executing the tests.
*/
/*------------------------------------------------------------------------------------------------*/
#define TS_PipeError(code,line) \
    TS_Printf("Abort Test: Pipe Error %d in Line %d", code, line); \
    while(1)

/******************************************************************************
* PRIVATE TYPES
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This structure holds data and relations of a pipe.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct TS_PIPE_T {
    TS_LIST_ITEM  Pipes;                              /*!< the list of existing pipes             */
    TS_LIST_ITEM  ItemHead;                           /*!< head of list of items in this pipe     */
    char         *Name;                               /*!< reference to name of this pipe         */
} TS_PIPE;

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This structure holds data and relations of a single pipe item.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct TS_PIPE_ITEM_T {
    TS_LIST_ITEM  Items;                              /*!< the linked list of pipe items          */
    void         *Data;                               /*!< reference to data memory of this item  */
    uint32_t      Size;                               /*!< size of referenced data memory         */
} TS_PIPE_ITEM;

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

static TS_LIST_ITEM   ts_NamedPipes;                  /*!< head of list of existing named pipes   */
static TS_MEM_CTRL    ts_PipeMemCtrl;                 /*!< dynamic memory control for pipes       */
static uint8_t        ts_PipeMem[TS_PIPE_MEM_SIZE];   /*!< memory used within pipe management     */
static TS_LOCK        ts_PipeLock;                    /*!< pipe locking management                */

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This local function scans the existing pipe list and looks for the given name. If a
*          pipe with the given name is found, the pipe structure is returned, otherwise, this
*          function returns NULL.
*
* \param   name
*          Name of the pipe
*
* \note    The calling function must ensure, that name is valid (not NULL).
*
* \return  The pipe structure reference, or NULL if not found.
*/
/*------------------------------------------------------------------------------------------------*/
static TS_PIPE *TS_PipeFind(char *name);
static TS_PIPE *TS_PipeFind(char *name)
{
    TS_LIST_ITEM *cursor;
    TS_PIPE      *pipe;

    /* find name in pipe management list */
    TS_LIST_FOR_EACH(cursor, &ts_NamedPipes)
    {
        pipe = TS_LIST_DATA(cursor, TS_PIPE, Pipes);
        if (strcmp(pipe->Name, name) == 0) {
            return (pipe);
        }
    }

    return (NULL);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This local function creates a pipe with the given name. If a pipe with the given
*          name already exists, this function returns the corresponding pipe handle.
*
* \param   name
*          Name of the pipe
*
* \note    The calling function must ensure, that name is valid (not NULL).
*
* \return  The created (or already existing) pipe handle.
*/
/*------------------------------------------------------------------------------------------------*/
static TS_PIPE *TS_PipeCreate(char *name);
static TS_PIPE *TS_PipeCreate(char *name)
{
    TS_PIPE *pipe;

    pipe = TS_PipeFind(name);
    if (pipe == NULL) {
        pipe = (TS_PIPE *)TS_MemGet(&ts_PipeMemCtrl, sizeof(TS_PIPE));
        if (pipe != NULL) {
            /* initialize pipe structure */
            TS_ListInit(&pipe->Pipes);
            TS_ListInit(&pipe->ItemHead);
            pipe->Name = name;
            /* put in linked list */
            TS_ListAddHead(&ts_NamedPipes, &pipe->Pipes);
        } else {
            TS_PipeError(TS_PIPE_OUT_OF_MEM, __LINE__);
        }
    }
    return (pipe);
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

void TS_PipeInit(void)
{
    TS_ListInit(&ts_NamedPipes);
    TS_MemInit(&ts_PipeMemCtrl, &ts_PipeMem[0], TS_PIPE_MEM_SIZE);
    TS_LockSetup(&ts_PipeLock, NULL, NULL);
}


void TS_PipeSetup(TS_LOCK_FUNC lock, TS_UNLOCK_FUNC unlock)
{
    TS_LockSetup(&ts_PipeLock, lock, unlock);
}


void TS_PipeAdd(char *name, void *data, uint32_t size)
{
    TS_LOCK_SR    state = 0;
    TS_PIPE      *pipe;
    TS_PIPE_ITEM *item;

    ASSERT_NOT_NULL(name);

    state = TS_Lock(&ts_PipeLock);

    /* ensure pipe is existing */
    pipe = TS_PipeCreate(name);
    if ((size != 0) && (data != NULL)) {
        item = (TS_PIPE_ITEM *)TS_MemGet(&ts_PipeMemCtrl, sizeof(TS_PIPE_ITEM));
        if (item != NULL) {
            item->Data = TS_MemGet(&ts_PipeMemCtrl, size);
            if (item->Data != NULL) {
                /* initialize item structure */
                TS_ListInit(&item->Items);
                memcpy(item->Data, data, size);
                item->Size = size;
                /* put in linked list */
                TS_ListAddTail(&pipe->ItemHead, &item->Items);
            } else {
                TS_MemPut(&ts_PipeMemCtrl, item);
                TS_PipeError(TS_PIPE_OUT_OF_MEM, __LINE__);
            }
        } else {
            TS_PipeError(TS_PIPE_OUT_OF_MEM, __LINE__);
        }
    }
    
    TS_Unlock(&ts_PipeLock, state);
}


bool TS_PipeGet(char *name, void *data, uint32_t size)
{
    TS_LOCK_SR     state = 0;
    TS_PIPE       *pipe;
    TS_LIST_ITEM  *item;
    TS_PIPE_ITEM  *pipedata;
    bool           result = false;

    ASSERT_NOT_NULL(name)(false);

    state = TS_Lock(&ts_PipeLock);
    
    pipe = TS_PipeCreate(name);
    if (!TS_LIST_IS_EMPTY(&pipe->ItemHead)) {
        item     = TS_ListFetchHead(&pipe->ItemHead);
        pipedata = TS_LIST_DATA(item, TS_PIPE_ITEM, Items);
        if (size > pipedata->Size) {
            size = pipedata->Size;
        }
        if ((size > 0) && (data != NULL)) {
            memcpy(data, pipedata->Data, size);
        }
        TS_MemPut(&ts_PipeMemCtrl, pipedata->Data);
        TS_MemPut(&ts_PipeMemCtrl, pipedata);
        result = true;
    }
    
    TS_Unlock(&ts_PipeLock, state);

    return (result);
}


void TS_PipeClear(char *name)
{
    TS_LOCK_SR state = 0;
    bool       result;

    ASSERT_NOT_NULL(name);
    
    state = TS_Lock(&ts_PipeLock);
    
    do {
        result = TS_PipeGet(name, 0, 0);
    } while (result == true);
    
    TS_Unlock(&ts_PipeLock, state);
}


void TS_PipeDelete(char *name)
{
    TS_LOCK_SR  state = 0;
    TS_PIPE    *pipe;

    ASSERT_NOT_NULL(name);

    state = TS_Lock(&ts_PipeLock);
    
    pipe = TS_PipeFind(name);
    if (pipe != NULL) {
        TS_PipeClear(name);
        TS_ListRemove(&pipe->Pipes);
        TS_MemPut(&ts_PipeMemCtrl, pipe);
    }
    
    TS_Unlock(&ts_PipeLock, state);
}


bool TS_PipeIsExisting(char *name)
{
    TS_LOCK_SR state = 0;
    TS_PIPE   *pipe;
    bool       result = false;

    ASSERT_NOT_NULL(name)(false);

    state = TS_Lock(&ts_PipeLock);
    
    pipe = TS_PipeFind(name);
    if (pipe != NULL) {
        result = true;
    }
    
    TS_Unlock(&ts_PipeLock, state);

    return (result);
}


bool TS_PipeIsEmpty(char *name)
{
    TS_LOCK_SR  state = 0;
    TS_PIPE    *pipe;
    bool        result = true;

    ASSERT_NOT_NULL(name)(true);
    
    state = TS_Lock(&ts_PipeLock);
    
    pipe = TS_PipeFind(name);
    if (pipe != NULL) {
        if (!TS_LIST_IS_EMPTY(&pipe->ItemHead)) {
            result = false;
        }
    }
    
    TS_Unlock(&ts_PipeLock, state);

    return (result);
}


uint32_t TS_PipeGetNum(void)
{
    uint32_t      cnt = 0;
    TS_LIST_ITEM *cursor;

    /* count all named pipes */
    TS_LIST_FOR_EACH(cursor, &ts_NamedPipes) {
        cnt++;
    }

    return (cnt);
}


uint32_t TS_PipeGetSize(char *name)
{
    TS_LOCK_SR     state = 0;
    TS_PIPE       *pipe;
    TS_PIPE_ITEM  *pipedata;
    uint32_t       result = 0;

    ASSERT_NOT_NULL(name)(0);

    state = TS_Lock(&ts_PipeLock);
    
    /* ensure pipe is existing */
    pipe = TS_PipeCreate(name);
    if (!TS_LIST_IS_EMPTY(&pipe->ItemHead)) {
        /* get reference to head of pipe */
        pipedata = TS_LIST_HEAD_DATA(TS_PIPE_ITEM, &pipe->ItemHead, Items);
        result = pipedata->Size;
    }

    TS_Unlock(&ts_PipeLock, state);

    return (result);
}


uint32_t TS_PipeMemUsed(void)
{
    TS_MEM_INFO info;
    
    TS_MemInfo(&ts_PipeMemCtrl, &info);
    
    return (info.FreeBlocks * info.BlockSize);
}
