//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef FREE_H
#define FREE_H

#include "Types.h"
#include "Block.h"

class Used;

class Free : public Align16
{
public:
    // ---------------------------------------------------------
    // Do not reorder, change or add data fields
    //     --> any changes to the data... is a 0 for assignment
    // You can add methods if you wish
    // ---------------------------------------------------------
    Free() = delete;
    Free(const Free&) = delete;
    Free& operator=(const Free&) = delete;
    ~Free() = default;

    Free(uint32_t blockSize);

    Free     *pFreeNext;        // next free block
    Free     *pFreePrev;        // prev free block
    Block    mType;             // block type 
    bool     mAboveBlockFree;   // AboveBlock flag:
                                // if(AboveBlock is type free) -> true 
                                // if(AboveBlock is type used) -> false
    uint32_t mBlockSize;        // size of block

};

#endif 

// ---  End of File ---------------
