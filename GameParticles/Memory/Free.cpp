

#include "Used.h"
#include "Free.h"
#include "Block.h"

// Add magic here
Free::Free(uint32_t blockSize)
	: pFreeNext(nullptr),
	pFreePrev(nullptr),
	mType(Block::Free),
	mAboveBlockFree(false),
	mBlockSize(blockSize)
{ }
// ---  End of File ---------------
