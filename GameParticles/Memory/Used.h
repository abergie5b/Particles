//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef USED_H
#define USED_H

#include "Types.h"
#include "Block.h"

class Free;

class Used : public Align16
{
public:
   // ---------------------------------------------------------
   // Do not reorder, change or add data fields
   //     --> any changes to the data... is a 0 for assignment
   // You can add methods if you wish
   // ---------------------------------------------------------
    Used() = delete;
    Used(const Used&) = delete;
    Used& operator=(const Used&) = delete;
    ~Used() = default;

    Used(const uint32_t blocksize);

    Used       *pUsedNext;       // next used block
    Used       *pUsedPrev;       // prev used block
    Block      mType;            // block type 
    bool       mAboveBlockFree;  // AboveBlock flag:
                                 // if(AboveBlock is type free) -> true 
                                 // if(AboveBlock is type used) -> false
    uint32_t   mBlockSize;       // size of block

};

#endif 

// ---  End of File ---------------
