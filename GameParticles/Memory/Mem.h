//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MEM_H
#define MEM_H

#include "Heap.h"

class Mem
{
public:
	static const unsigned int HEAP_TOTAL_SIZE = (200 * 1000 * 272 * 10);

public:
	Mem();	
	Mem(const Mem &) = delete;
	Mem & operator = (const Mem &) = delete;
	~Mem();

	Heap *getHeap();
	void dump(int count);

	// implement these functions
	void free( void * const data );
	void *malloc( const uint32_t size );
	void initialize();


private:
	Free* privFindFreeBlock(const uint32_t size) const;
	void privRemoveFreeBlock(const Free* pFree) const;
	void privRemoveFreeAdjustStats(const Free* pFree) const;
	void privAddUsedToFront(Used* pUsed) const;
	void privAddUsedAdjustStats(const Used* pUsed) const;
	void* privGetBlockPtr(const Used* pUsed) const;
	void privFreeBlockAdjustStats(const Used* pUsed) const;
	void privAddFreeBlock(Free* pFree);
	void privAddFreeBlockAdjustStats(const Free* pFree) const;
	void privCoalesceAdjustStats() const;
	void EncodeSecretPointer(const Free* pFree) const;
	void privSetBlockBelowAboveIsFree(const Used* pUsed, bool status) const;
	void* privAllocateBlock(Free* pFree, uint32_t size);
	Free* privDeallocateBlock(void* pUsed, uint32_t size);
	uint32_t privGetHeapBottomAddr() const;
	void	*poRawMem;
	Heap	*poHeap;


};

#endif 

// ---  End of File ---------------
