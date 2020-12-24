//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Free.h"
#include "Used.h"

// Add code here
Used::Used(const uint32_t blocksize)
	: pUsedNext(0),
	pUsedPrev(0),
	mType(Block::Used),
	mAboveBlockFree(false),
	mBlockSize(blocksize)
{ }

// ---  End of File ---------------
