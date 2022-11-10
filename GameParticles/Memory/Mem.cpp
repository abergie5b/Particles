

#include "Mem.h"
#include "Heap.h"
#include "Block.h"

// To help with coalescing... not required
struct SecretPtr
{
	Free *pFree;
};

void Mem::initialize()
{
	// Add magic here
	Heap* poHeapTmp = this->getHeap();
	Free* pFreeStart = (Free*)(poHeapTmp + 1);
	Free* pFreeEnd = pFreeStart + 1;

	void* heapBottomAddr = (void*)this->privGetHeapBottomAddr();
	uint32_t blockSize = (uint32_t)((uint32_t)heapBottomAddr - (uint32_t)pFreeEnd);

	#undef new
		Free* pFree = new(pFreeStart) Free(blockSize);

	this->poHeap->pFreeHead = pFree;
	this->poHeap->pNextFit = pFree;

	this->poHeap->currFreeMem = blockSize;
	this->poHeap->currNumFreeBlocks = 1;
}

void Mem::privAddUsedToFront(Used* pUsed) const
{
	assert(pUsed != nullptr);
	Used* pUsedHead = this->poHeap->pUsedHead;
	pUsed->pUsedNext = pUsedHead;
	pUsed->pUsedPrev = nullptr;
	if (pUsedHead != nullptr)
		pUsedHead->pUsedPrev = pUsed;
	this->poHeap->pUsedHead = pUsed;
}

void* Mem::privGetBlockPtr(const Used* pUsed) const
{
	assert(pUsed != nullptr);
	void* p = (void*)(pUsed + 1);
	return p;
}

uint32_t Mem::privGetHeapBottomAddr() const
{
	return (uint32_t)this->poHeap + HEAP_TOTAL_SIZE;
}

void Mem::privSetBlockBelowAboveIsFree(const Used* pUsed, bool status) const
{
	// set mAboveBlockFree on the below block
	Used* pBlockBelowUsed = (Used*)((uint32_t)pUsed + sizeof(Used) + pUsed->mBlockSize);
	if ((uint32_t)pBlockBelowUsed < this->privGetHeapBottomAddr())
		pBlockBelowUsed->mAboveBlockFree = status;
}

void Mem::EncodeSecretPointer(const Free* pFree) const
{
	// encode a pointer to the address of the free head in last byte of the block
	uint32_t* pSecret = (uint32_t*)((uint32_t)pFree + sizeof(Free) + pFree->mBlockSize - sizeof(uint32_t*));
	*pSecret = (uint32_t)(pFree);
}

void Mem::privAddFreeBlock(Free* pFree)
{
	assert(pFree != nullptr);

	if (this->poHeap->pFreeHead != nullptr)
	{
		Free* pNode = nullptr;
		Free* pFreeHead = this->poHeap->pFreeHead;
		Free* pBlockPrev = this->poHeap->pFreeHead;
		while (pFreeHead != nullptr)
		{
			if (pFreeHead > pFree)
			{
				pNode = pFreeHead;
				if (pFreeHead == this->poHeap->pFreeHead)
				{
					pNode->pFreePrev = pFree;
					pFree->pFreeNext = pNode;
					pFree->pFreePrev = nullptr;
					this->poHeap->pFreeHead = pFree;
				}
				else
				{
					pNode->pFreePrev->pFreeNext = pFree;
					pFree->pFreePrev = pNode->pFreePrev;
					pFree->pFreeNext = pNode;
					pNode->pFreePrev = pFree;
				}
				break;
			}
			else
			{
				pBlockPrev = pFreeHead;
				pFreeHead = pFreeHead->pFreeNext;
			}
		}

		if (pNode == nullptr)
		{
			pNode = pBlockPrev;
			pNode->pFreeNext = pFree;
			pFree->pFreePrev = pNode;
			pFree->pFreeNext = nullptr;
		}
	}

	else
	{
		this->poHeap->pFreeHead = pFree;
		this->poHeap->pNextFit = pFree;
		pFree->pFreePrev = nullptr;
		pFree->pFreeNext = nullptr;
	}
}

void Mem::privRemoveFreeBlock(const Free* pFree) const
{
	// remove a block from the free list
	assert(pFree != nullptr);
	if (pFree == this->poHeap->pFreeHead)
		this->poHeap->pFreeHead = 0;

	if (pFree->pFreeNext != nullptr)
	{
		if (pFree->pFreePrev != nullptr)
		{
			pFree->pFreePrev->pFreeNext = pFree->pFreeNext;
			pFree->pFreeNext->pFreePrev = pFree->pFreePrev;
		}
		else
		{
			this->poHeap->pFreeHead = pFree->pFreeNext;
			this->poHeap->pFreeHead->pFreePrev = 0;
		}
	}
	else
	{
		if (pFree->pFreePrev != nullptr)
			pFree->pFreePrev->pFreeNext = 0;
	}
}

Free* Mem::privFindFreeBlock(const uint32_t size) const
{
	Free* pBlock = nullptr;

	// check next fit
	if (this->poHeap->pNextFit != nullptr && this->poHeap->pNextFit->mBlockSize >= size)
		pBlock = this->poHeap->pNextFit;

	if (pBlock == nullptr)
	{
		// check free list
		Free* pFreeTmp = this->poHeap->pFreeHead;
		while (pFreeTmp)
		{
			if ((!pBlock && pFreeTmp->mBlockSize >= size) || (pBlock && pFreeTmp->mBlockSize >= size && pBlock->mBlockSize > pFreeTmp->mBlockSize))
				pBlock = pFreeTmp;
			pFreeTmp = pFreeTmp->pFreeNext;
		}
	}
	return pBlock;
}

void* Mem::privAllocateBlock(Free* pFree, uint32_t size)
{
	// convert the free block to used
	#undef new
		Used* pUsed = new(pFree) Used(size);

	// tell the block below that above is not free
	this->privSetBlockBelowAboveIsFree(pUsed, false);

	// add the used block to list
	this->privAddUsedToFront(pUsed);
	this->privAddUsedAdjustStats(pUsed);

	// get pointer to block 
	return this->privGetBlockPtr(pUsed);
}

Free* Mem::privDeallocateBlock(void* pBlock, uint32_t size)
{
	#undef new
		Free* pFree = new(pBlock) Free(size);

	// add free'd block to free list
	this->privAddFreeBlock(pFree);

	// tell the block below that above is free
	this->privSetBlockBelowAboveIsFree((Used*)pFree, true);

	// encode secret in last bytes of below free block
	this->EncodeSecretPointer(pFree);
	return pFree;
}

void* Mem::malloc(const uint32_t _size)
{
	Free* pFree = this->privFindFreeBlock(_size);
	void* pUsedBlock = nullptr;

	if (pFree != nullptr) // found a block, allocate it
	{
		if (pFree->mBlockSize == _size) // perfect fit
		{
			// remove the free block
			this->privRemoveFreeBlock(pFree);
			this->privRemoveFreeAdjustStats(pFree);

			// adjust the next fit
			this->poHeap->pNextFit = this->poHeap->pFreeHead;

			// allocate the block
			pUsedBlock = this->privAllocateBlock(pFree, pFree->mBlockSize);
			assert(pUsedBlock != nullptr);
		}
		else
		{
			// subdivide the block
			Free* pFreeSub = (Free*)((uint32_t)pFree + sizeof(Free) + _size);

			// deallocate the block
			Free* pFreeSubBlock = this->privDeallocateBlock((void*)pFreeSub, (uint32_t)(pFree->mBlockSize - sizeof(Free) - _size));
			this->privAddFreeBlockAdjustStats(pFreeSubBlock);

			// remove the free block from free list
			this->privRemoveFreeBlock(pFree);
			this->privRemoveFreeAdjustStats(pFree);

			// adjust next fit
			this->poHeap->pNextFit = pFreeSubBlock;

			// allocate the block
			pUsedBlock = this->privAllocateBlock(pFree, (uint32_t)_size);
			assert(pUsedBlock != nullptr);
		}
	}
	return pUsedBlock;
}



void Mem::free(void* const data)
{
	assert(data != nullptr);

	// get the header
	Used* pUsed = (Used*)(data)-1;

	bool isAboveTargetFree = pUsed->mAboveBlockFree;
	uint32_t blockSize = pUsed->mBlockSize;
	
	// update stats
	this->privFreeBlockAdjustStats(pUsed);

	// START remove used block from used list
	if (pUsed->pUsedPrev)
		pUsed->pUsedPrev->pUsedNext = pUsed->pUsedNext;
	if (pUsed->pUsedNext)
		pUsed->pUsedNext->pUsedPrev = pUsed->pUsedPrev;

	if (pUsed == this->poHeap->pUsedHead)
	{
		this->poHeap->pUsedHead = this->poHeap->pUsedHead->pUsedNext;
		if (this->poHeap->pUsedHead)
			this->poHeap->pUsedHead->pUsedPrev = nullptr;
	}
	pUsed->pUsedNext = nullptr;
	pUsed->pUsedPrev = nullptr;
	// END remove used block from list

	// deallocate the block
	//Free* pFree = this->privDeallocateBlock((void*)pUsed, pUsed->mBlockSize);

	// coalesce below if we can
	Free* pAboveBlock = (Free*)((uint32_t)pUsed + sizeof(Free) + blockSize);
	if (pAboveBlock->mType == Block::Free)
	{
		// remove free blocks
		this->privRemoveFreeBlock((Free*)pUsed);
		this->privRemoveFreeBlock(pAboveBlock);

		// deallocate the full block
		this->privDeallocateBlock((void*)pUsed, blockSize + pAboveBlock->mBlockSize + sizeof(Free));

		// update stats
		this->privCoalesceAdjustStats();
	}
	else
		this->privDeallocateBlock((void*)pUsed, pUsed->mBlockSize);


	// coalesce above if we can
	if (isAboveTargetFree)
	{
		// deref the secret pointer
		Free* pNewBlockLocation = (Free*)*((uint32_t*)((uint32_t)pUsed - sizeof(uint32_t*)));;

		//Free* secretAboveNext = ((Free*)pUsed)->pFreeNext;
		//if (secretAboveNext)
		//	secretAboveNext->pFreePrev = pNewBlockLocation;
		//pNewBlockLocation->mBlockSize += (blockSize + sizeof(Free));
		//pNewBlockLocation->pFreeNext = secretAboveNext;

		//if ((Free*)pUsed == this->poHeap->pNextFit)
		//	this->poHeap->pNextFit = pNewBlockLocation;
		//this->EncodeSecretPointer(pNewBlockLocation);
		//this->privCoalesceAdjustStats();

		// remove the free blocks
		this->privRemoveFreeBlock(pNewBlockLocation);

		// deallocate the full block
		this->privDeallocateBlock((void*)pNewBlockLocation, pNewBlockLocation->mBlockSize + blockSize + sizeof(Free));

		// update stats
		this->privCoalesceAdjustStats();
	}
}

void Mem::privRemoveFreeAdjustStats(const Free* pFree) const
{
	assert(pFree != nullptr);
	this->poHeap->currFreeMem -= pFree->mBlockSize;
	this->poHeap->currNumFreeBlocks--;
}

void Mem::privAddUsedAdjustStats(const Used* pUsed) const
{
	assert(pUsed != nullptr);
	this->poHeap->currUsedMem += pUsed->mBlockSize;
	this->poHeap->currNumUsedBlocks++;

	// adjust peak statistics
	if (poHeap->currUsedMem > poHeap->peakUsedMemory)
		poHeap->peakUsedMemory = poHeap->currUsedMem;

	if (poHeap->currNumUsedBlocks > poHeap->peakNumUsed)
		poHeap->peakNumUsed = poHeap->currNumUsedBlocks;
}

void Mem::privAddFreeBlockAdjustStats(const Free* pFree) const
{
	this->poHeap->currFreeMem += pFree->mBlockSize;
	this->poHeap->currNumFreeBlocks++;
}

void Mem::privFreeBlockAdjustStats(const Used* pUsed) const
{
	assert(pUsed != nullptr);
	this->poHeap->currFreeMem += pUsed->mBlockSize;
	this->poHeap->currNumFreeBlocks++;

	this->poHeap->currUsedMem -= pUsed->mBlockSize;
	this->poHeap->currNumUsedBlocks--;

	// adjust peak statistics
	if (poHeap->currUsedMem > poHeap->peakUsedMemory)
		poHeap->peakUsedMemory = poHeap->currUsedMem;

	if (poHeap->currNumUsedBlocks > poHeap->peakNumUsed)
		poHeap->peakNumUsed = poHeap->currNumUsedBlocks;
}

void Mem::privCoalesceAdjustStats() const
{
	// coalescing two blocks removes a free block and a header
	this->poHeap->currNumFreeBlocks--;
	this->poHeap->currFreeMem += sizeof(Free);
}

// ---  End of File ---------------
