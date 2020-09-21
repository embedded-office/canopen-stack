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
/* This file is based on the project 'umm_malloc'. Find the original source code at
*  github: "https://github.com/rhempel/umm_malloc"
*/

#ifndef TS_MEM_H_
#define TS_MEM_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "ts_types.h"
#include "ts_lock.h"

/*!
* \addtogroup ts_frm_mem   Test Framework - Memory Management
* \details  This is a memory allocator specifically designed to work with embedded
*           microcontrollers and should work on any 32 bit controllers, as well as 16 and 8 bit
*           devices. You can even use it on a bigger windows or linux project where a single
*           process might want to manage a large number of smaller objects, and using the system
*           heap might get expensive.
*
*           The memory manager assumes the following things. All memory used by the manager
*
*           + is allocated at link time
*           + it is aligned on a 32 bit boundary and
*           + it is contiguous.
*
*           The fastest linked list implementations use doubly linked lists so that it is possible
*           to insert and delete blocks in constant time. This memory manager keeps track of both
*           free and used blocks in a doubly linked list.
*
*           Most memory managers use some kind of list structure made up of pointers to keep track
*           of used - and sometimes free - blocks of memory. In an embedded system, this can get
*           pretty expensive as each pointer can use up to 32 bits.
*
*           In most embedded systems there is no need for managing large blocks of memory
*           dynamically, so a full 32 bit pointer based data structure for the free and used block
*           lists is wasteful. A block of memory on the free list would use 16 bytes just for the
*           pointers!
*
*           This memory management library sees the memory area as an array of blocks, and uses
*           block numbers to keep track of locations. The block numbers are 15 bits - which allows
*           for up to 32767 blocks of memory. The high order bit marks a block as being either
*           free or in use.
*
*           The result is that a block of memory on the free list uses just 8 bytes instead of 16.
*
*           In fact, we go even one step futher when we realize that the free block index values
*           are available to store data when the block is allocated.
*
*           The overhead of an allocated block is therefore just 4 bytes.
*
*           Each memory block holds 8 bytes, and there are up to 32767 blocks available, for about
*           256K of managed memory area. If that's not enough, you can always add more data bytes
*           to the body of the memory block at the expense of free block size overhead.
* @{
*/

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This type contains information data of a memory blocks within one memory control area.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct TS_MEM_INFO_T {
    uint16_t TotalEntries;                            /*!< Number of elements in lists            */
    uint16_t UsedEntries;                             /*!< Number of elements in used list        */
    uint16_t FreeEntries;                             /*!< Number of elements in free list        */
    uint16_t TotalBlocks;                             /*!< Number of blocks in memory area        */
    uint16_t UsedBlocks;                              /*!< Number of used memory blocks           */
    uint16_t FreeBlocks;                              /*!< Number of free memory blocks           */
    uint16_t FreeContBlocks;                          /*!< Number of continuous free blocks       */
    uint16_t BlockSize;                               /*!< Size of a single block in byte         */
} TS_MEM_INFO;

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This type contains linked list identifier. The intention of this definition is to save
*          memory overhead for the memory block management. A pointer would require 4 byte. This
*          management is for embedded systems and is good enought with 2 byte index values.
*/
/*------------------------------------------------------------------------------------------------*/
STRUCT_PACKED_PRE typedef struct ts_id_list_t
{
    uint16_t Next;                                    /*!< Index of next memory block             */
    uint16_t Prev;                                    /*!< Index of previous memory block         */
} STRUCT_PACKED_SUF TS_ID_LIST;

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This type contains the management of a single memory block. We add an information
*          header to each memory block.
*/
/*------------------------------------------------------------------------------------------------*/
STRUCT_PACKED_PRE typedef struct ts_block_t
{
    union {                                           /*!< This union holds header data           */
        TS_ID_LIST Used;                              /*!< List of used blocks                    */
    } Head;
    union {                                           /*!< This union holds the memory body       */
        TS_ID_LIST Free;                              /*!< If not used: list of free blocks       */
        uint8_t    Data[4];                           /*!< If used: 4 byte of payload data        */
    } Body;
} STRUCT_PACKED_SUF TS_BLOCK;

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This type contains the management of a memory area, called memory control.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct TS_MEM_CTRL_T {
    TS_BLOCK *Start;                                  /*!< Start address of memory area           */
    uint16_t  NumBlocks;                              /*!< Number of blocks in memory area        */
    uint16_t  Spare;                                  /*!< Spare to get a 8 byte alignment        */
} TS_MEM_CTRL;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function setups the lock and unlock function for the memory management.
*
* \param   lock
*          The reference to the memory locking function
*
* \param   unlock
*          The reference to the memory unlock function
*/
/*------------------------------------------------------------------------------------------------*/
void TS_MemSetup(TS_LOCK_FUNC lock, TS_UNLOCK_FUNC unlock);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function set the whole memory area as one free block. We should setup the
*          0th `TS_BLOCK`, since it's special: the 0th `TS_BLOCK` is the head of the free
*          block list. It's a part of the management.
*
* \param   mem
*          Memory control structure.
*
* \param   start
*          Start address of memory area.
*
* \param   size
*          Size of memory area in bytes
*/
/*------------------------------------------------------------------------------------------------*/
void TS_MemInit (TS_MEM_CTRL *mem, void *start, uint32_t size);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function allocates the requested memory size (in bytes).
*
* \param   mem
*          Memory control structure
*
* \param   size
*          Required memory size in bytes.
*
* \return  The reference to the allocated memory portion. If no memory is available, NULL will 
*          be returned.
*/
/*------------------------------------------------------------------------------------------------*/
void *TS_MemGet (TS_MEM_CTRL *mem, uint32_t size);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function releases the memory blocks.
*
* \param   mem
*          Memory control structure
*
* \param   ptr
*          Start of data pointer within the memory block collection.
*/
/*------------------------------------------------------------------------------------------------*/
void TS_MemPut (TS_MEM_CTRL *mem, void *ptr);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This helper function clears the given memory area with 0.
*
* \param   start
*          Start address of memory area.
*
* \param   size
*          Size of memory area in bytes.
*/
/*------------------------------------------------------------------------------------------------*/
void TS_MemClear (void *start, uint32_t size);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This helper function scans the memory area, corresponding to the given
*          memory control structure and fills the given memory info structure.
*
* \param   mem
*          Memory control structure.
*
* \param   info
*          Memory information structure.
*/
/*------------------------------------------------------------------------------------------------*/
void TS_MemInfo (TS_MEM_CTRL *mem, TS_MEM_INFO *info);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif /* TS_MEM_H_ */
/*! @} */
