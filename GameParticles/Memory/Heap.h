//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef HEAPHDR_H
#define HEAPHDR_H

#include "Types.h"
#include "Used.h"
#include "Free.h"

class Heap : public Align16
{
public:
	// Big four
	Heap();
	Heap(const Heap&) = delete;
	Heap& operator =(const Heap&) = delete;
	~Heap() = default;

public:
	// Verify that the Heap is 16 byte aligned.

	// allocation links
	Used		*pUsedHead;
	Free		*pFreeHead;

	// Next fit allocation strategy
	Free	    *pNextFit;

	uint32_t    currFreeMem;         // current size of the total free memory
	uint32_t    peakUsedMemory;      // peak size of used memory
	uint32_t    currUsedMem;         // current size of the total used memory

	uint32_t    currNumUsedBlocks;   // number of current used allocations
	uint32_t    peakNumUsed;         // number of peak used allocations
	uint32_t    currNumFreeBlocks;   // number of current free blocks

	uint32_t    pad;

};

#endif 

// ---  End of File ---------------
