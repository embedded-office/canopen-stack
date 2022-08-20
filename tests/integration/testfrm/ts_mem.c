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

#include "ts_mem.h"

/******************************************************************************
* PRIVATE DEFINES
******************************************************************************/

#define TS_MEM_FREE_MASK       (0x8000)
#define TS_MEM_ID_MASK         (0x7FFF)

#define TS_MEM_USED_N(b)       (mem->Start[b].Head.Used.Next)
#define TS_MEM_USED_P(b)       (mem->Start[b].Head.Used.Prev)
#define TS_MEM_FREE_N(b)       (mem->Start[b].Body.Free.Next)
#define TS_MEM_FREE_P(b)       (mem->Start[b].Body.Free.Prev)
#define TS_MEM_DATA_PTR(b)     (mem->Start[b].Body.Data)

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

static TS_LOCK ts_MemLock = { 0 };

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

void TS_MemSetup(TS_LOCK_FUNC lock, TS_UNLOCK_FUNC unlock)
{
    /* setup locking functions */
    TS_LockSetup(&ts_MemLock, lock, unlock);
}

void TS_MemInit (TS_MEM_CTRL *mem, void *start, uint32_t size)
{
    TS_LOCK_SR state = 0;
    
    /* argument checks */
    if ( (mem   == NULL) ||
         (start == NULL) ||
         (size  == 0   ) ) {
        return;
    }

    mem->Start     = (TS_BLOCK *)start;
    mem->NumBlocks = (uint16_t)(size / sizeof(TS_BLOCK));

    TS_MemClear(mem->Start, size);              /* clear whole memory area      */

    state = TS_Lock(&ts_MemLock);
                                                /*-- setup the 0th block -------*/
    TS_MEM_USED_N(0) = 1;                       /* which just points to the 1st */
    TS_MEM_FREE_N(0) = 1;
                                                /*-- setup the 1st block -------*/
    TS_MEM_USED_N(1) = (mem->NumBlocks - 1) |   /* last block of area is last   */
                TS_MEM_FREE_MASK;               /* this area is free            */
    TS_MEM_USED_P(1) = 0;
    TS_MEM_FREE_N(1) = 0;                       /* this is the last free area   */
    TS_MEM_FREE_P(1) = 0;
                                                /*-- setup the last block ------*/
    TS_MEM_USED_N(mem->NumBlocks - 1) = 0;      /* no more block areas          */
    TS_MEM_USED_P(mem->NumBlocks - 1) = 1;      /* the 1st block                */

    TS_Unlock(&ts_MemLock, state);
}


void *TS_MemGet (TS_MEM_CTRL *mem, uint32_t size)
{
    TS_LOCK_SR state = 0;
    uint16_t blocks;
    uint16_t blockSize = 0;
    uint16_t cf;
#if defined TS_MEM_BEST_FIT
    uint16_t bestSize;
    uint16_t bestBlock;
#endif

    /* argument checks */
    if ( (mem  == NULL) ||
         (size == 0   ) ) {
        return ((void *)NULL);
    }
    if ( (mem->Start     == NULL) ||
         (mem->NumBlocks == 0   ) ) {
        return ((void *)NULL);
    }

    /*
     * the very first thing we do is figure out if we're being asked to allocate
     * a size of 0 - and if we are we'll simply return a null pointer. if not
     * then reduce the size by 1 byte so that the subsequent calculations on
     * the number of blocks to allocate are easier...
     */

    if( size <= (sizeof(((TS_BLOCK *)0)->Body)) ) {
        blocks = 1;
    } else {
        size  -= (1 + (sizeof(((TS_BLOCK *)0)->Body)));
        blocks = (uint16_t)(2 + size/(sizeof(TS_BLOCK)));
    }

    /*
     * Now we can scan through the free list until we find a space that's big
     * enough to hold the number of blocks we need.
     *
     * This part may be customized to be a best-fit, worst-fit, or first-fit
     * algorithm
     */

    state = TS_Lock(&ts_MemLock);

    cf = TS_MEM_FREE_N(0);

    while (cf) {
        blockSize = (TS_MEM_USED_N(cf) & TS_MEM_ID_MASK) - cf;
        if (blockSize >= blocks) {
            break;
        }
        cf = TS_MEM_FREE_N(cf);
    }

    if( (TS_MEM_USED_N(cf) & TS_MEM_ID_MASK) &&
         blockSize >= blocks ) {
        /*
         * This is an existing block in the memory heap, we just need to split off
         * what we need, unlink it from the free list and mark it as in use, and
         * link the rest of the block back into the free list as if it was a new
         * block on the free list...
         */

        if( blockSize == blocks ) {
            /* It's an exact fit and we don't need to split off a block. */

            /* Disconnect this block from the FREE list */
            TS_MEM_FREE_N(TS_MEM_FREE_P(cf)) = TS_MEM_FREE_N(cf);
            TS_MEM_FREE_P(TS_MEM_FREE_N(cf)) = TS_MEM_FREE_P(cf);

            /* And clear the free block indicator */
            TS_MEM_USED_N(cf) &= (~TS_MEM_FREE_MASK);
        } else {
            /* It's not an exact fit and we need to split off a block. */
            /*
             * split current free block `cf` into two blocks. The first one will be
             * returned to user, so it's not free, and the second one will be free.
             */
            TS_MEM_USED_N(cf+blocks) = (TS_MEM_USED_N(cf) & TS_MEM_ID_MASK) | TS_MEM_FREE_MASK;
            TS_MEM_USED_P(cf+blocks) = cf;
            TS_MEM_USED_P(TS_MEM_USED_N(cf) & TS_MEM_ID_MASK) = (cf+blocks);
            TS_MEM_USED_N(cf)                                 = (cf+blocks);

            /* previous free block */
            TS_MEM_FREE_N(TS_MEM_FREE_P(cf)) = cf + blocks;
            TS_MEM_FREE_P(cf + blocks)       = TS_MEM_FREE_P(cf);

            /* next free block */
            TS_MEM_FREE_P(TS_MEM_FREE_N(cf)) = cf + blocks;
            TS_MEM_FREE_N(cf + blocks)       = TS_MEM_FREE_N(cf);
        }
    } else {
        /* Out of memory */
        TS_Unlock(&ts_MemLock, state);
        return( (void *)NULL );
    }

    TS_Unlock(&ts_MemLock, state);

    return( (void *)&TS_MEM_DATA_PTR(cf) );
}


void TS_MemPut (TS_MEM_CTRL *mem, void *ptr)
{
    TS_LOCK_SR state = 0;
    uint16_t c;
    uint16_t cf;

    /* argument checks */
    if ( (mem == NULL) ||
         (ptr == NULL) ) {
        return;
    }
    if ( (mem->Start     == NULL) ||
         (mem->NumBlocks == 0   ) ) {
        return;
    }

    /* Figure out which block we're in. Note the use of truncated division... */

    c = (uint16_t)((((char *)ptr)-(char *)(&(mem->Start[0])))/sizeof(TS_BLOCK));

    /* Now let's assimilate this block with the next one if possible. */

    state = TS_Lock(&ts_MemLock);
    
    if( TS_MEM_USED_N(TS_MEM_USED_N(c)) & TS_MEM_FREE_MASK ) {
        /*
         * The next block is a free block, so assimilate up and remove it from
         * the free list
         */

        /* Disconnect the next block from the FREE list */
        cf = TS_MEM_USED_N(c);

        TS_MEM_FREE_N(TS_MEM_FREE_P(cf)) = TS_MEM_FREE_N(cf);
        TS_MEM_FREE_P(TS_MEM_FREE_N(cf)) = TS_MEM_FREE_P(cf);

        /* And clear the free block indicator */
        TS_MEM_USED_N(cf) &= (~TS_MEM_FREE_MASK);

        /* Assimilate the next block with this one */
        TS_MEM_USED_P(TS_MEM_USED_N(TS_MEM_USED_N(c)) & TS_MEM_ID_MASK) = c;
        TS_MEM_USED_N(c) = TS_MEM_USED_N(TS_MEM_USED_N(c)) & TS_MEM_ID_MASK;
    }

    /* Then assimilate with the previous block if possible */

    if( TS_MEM_USED_N(TS_MEM_USED_P(c)) & TS_MEM_FREE_MASK ) {
        /*
         * The previous block is a free block, so assimilate down.
         */
        TS_MEM_USED_N(TS_MEM_USED_P(c)) = TS_MEM_USED_N(c) | TS_MEM_FREE_MASK;
        TS_MEM_USED_P(TS_MEM_USED_N(c)) = TS_MEM_USED_P(c);
        c = TS_MEM_USED_P(c);
    } else {
        /*
         * The previous block is not a free block, so add this one to the head
         * of the free list
         */
        TS_MEM_FREE_P(TS_MEM_FREE_N(0)) = c;
        TS_MEM_FREE_N(c)         = TS_MEM_FREE_N(0);
        TS_MEM_FREE_P(c)         = 0;
        TS_MEM_FREE_N(0)         = c;
        TS_MEM_USED_N(c)        |= TS_MEM_FREE_MASK;
    }

    TS_Unlock(&ts_MemLock, state);
}


void TS_MemClear(void *start, uint32_t size)
{
    uint8_t  *ptr8;
    uint16_t *ptr16;
    uint32_t *ptr32;

    ptr8 = (uint8_t*)start;
    if ( (((intptr_t)ptr8 & 0x1) != 0) &&
         (size >= 1) ) {
        *ptr8 = (uint8_t)0;
        ptr8++;
        size--;
    }
    ptr16 = (uint16_t*)ptr8;
    if ( (((intptr_t)ptr16 & 0x3) != 0) &&
          (size         >= 2) ) {
        *ptr16 = (uint16_t)0;
        ptr16++;
        size -= 2;
    }
    ptr32 = (uint32_t*)ptr16;
    while (size >= 4) {
        *ptr32 = (uint32_t)0;
        size -= 4;
        ptr32++;
    }
    ptr16 = (uint16_t*)ptr32;
    if (size >= 2) {
        *ptr16 = (uint16_t)0;
        ptr16++;
        size -= 2;
    }
    ptr8 = (uint8_t*)ptr16;
    if (size >= 1) {
        *ptr8 = (uint8_t)0;
        ptr8++;
        size--;
    }
}


void TS_MemInfo (TS_MEM_CTRL *mem, TS_MEM_INFO *info)
{
    TS_LOCK_SR state = 0;
    uint16_t blockNo = 0;
    uint16_t curBlocks;
    
    /*
     * Clear out all of the entries in the TS_MEM_INFO structure before doing
     * any calculations..
     */
    TS_MemClear(info, sizeof(TS_MEM_INFO));

    state = TS_Lock(&ts_MemLock);

    /*
     * Now loop through the block lists, and keep track of the number and size
     * of used and free blocks. The terminating condition is an nb pointer with
     * a value of zero...
     */
    blockNo = TS_MEM_USED_N(blockNo) & TS_MEM_ID_MASK;

    while( TS_MEM_USED_N(blockNo) & TS_MEM_ID_MASK ) {
        curBlocks = (TS_MEM_USED_N(blockNo) & TS_MEM_ID_MASK ) - blockNo;

        ++info->TotalEntries;
        info->TotalBlocks += curBlocks;

        /* Is this a free block? */
        if( TS_MEM_USED_N(blockNo) & TS_MEM_FREE_MASK ) {
            ++info->FreeEntries;
            info->FreeBlocks += curBlocks;

            if (info->FreeContBlocks < curBlocks) {
                info->FreeContBlocks = curBlocks;
            }
        } else {
            ++info->UsedEntries;
            info->UsedBlocks += curBlocks;
        }

        blockNo = TS_MEM_USED_N(blockNo) & TS_MEM_ID_MASK;
    }
    
    TS_Unlock(&ts_MemLock, state);

    /*
     * Update the accounting totals with information from the last block, the
     * rest must be free!
     */
    curBlocks = mem->NumBlocks - blockNo;
    info->FreeBlocks  += curBlocks;
    info->TotalBlocks += curBlocks;

    if (info->FreeContBlocks < curBlocks) {
        info->FreeContBlocks = curBlocks;
    }

    /* set block size, to make some plausibility checks in tests possible */
    info->BlockSize = sizeof(TS_BLOCK);
}
