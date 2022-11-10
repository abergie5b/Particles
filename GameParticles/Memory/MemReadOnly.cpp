

#include "Mem.h"
#include "Heap.h"
#include "Block.h"

//------------------------------------------------------
// DO NOT MODIFY
//------------------------------------------------------

#define STUB_PLEASE_REPLACE(x) (x)

#define HEAP_ALIGNMENT			16
#define HEAP_ALIGNMENT_MASK		(HEAP_ALIGNMENT-1)

#define ALLOCATION_ALIGNMENT		16
#define ALLOCATION_ALIGNMENT_MASK	(ALLOCATION_ALIGNMENT-1)


#ifdef _DEBUG
	#define HEAP_HEADER_GUARDS  64
	#define HEAP_SET_GUARDS  	uint32_t *pE = (uint32_t *)((uint32_t)poRawMem + HEAP_TOTAL_SIZE); \
								*pE++ = 0xEEEEEEEE; *pE++ = 0xEEEEEEEE; *pE++ = 0xEEEEEEEE; *pE++ = 0xEEEEEEEE; \
								*pE++ = 0xEEEEEEEE; *pE++ = 0xEEEEEEEE; *pE++ = 0xEEEEEEEE; *pE++ = 0xEEEEEEEE; \
								*pE++ = 0xEEEEEEEE; *pE++ = 0xEEEEEEEE; *pE++ = 0xEEEEEEEE; *pE++ = 0xEEEEEEEE;	\
								*pE++ = 0xEEEEEEEE; *pE++ = 0xEEEEEEEE; *pE++ = 0xEEEEEEEE; *pE++ = 0xEEEEEEEE;


	#define HEAP_TEST_GUARDS	uint32_t *pE = (uint32_t *)((uint32_t)poRawMem + HEAP_TOTAL_SIZE); \
								assert(*pE++ == 0xEEEEEEEE); assert(*pE++ == 0xEEEEEEEE); \
								assert(*pE++ == 0xEEEEEEEE); assert(*pE++ == 0xEEEEEEEE); \
								assert(*pE++ == 0xEEEEEEEE); assert(*pE++ == 0xEEEEEEEE); \
								assert(*pE++ == 0xEEEEEEEE); assert(*pE++ == 0xEEEEEEEE); \
								assert(*pE++ == 0xEEEEEEEE); assert(*pE++ == 0xEEEEEEEE); \
								assert(*pE++ == 0xEEEEEEEE); assert(*pE++ == 0xEEEEEEEE); \
								assert(*pE++ == 0xEEEEEEEE); assert(*pE++ == 0xEEEEEEEE); \
								assert(*pE++ == 0xEEEEEEEE); assert(*pE++ == 0xEEEEEEEE);
#else
	#define HEAP_HEADER_GUARDS  0
	#define HEAP_SET_GUARDS  	
	#define HEAP_TEST_GUARDS			 
#endif
							
Mem::~Mem()
{
	// do not modify this function
	HEAP_TEST_GUARDS
	_aligned_free(this->poRawMem);
}

Heap *Mem::getHeap()
{
	return this->poHeap;
}

Mem::Mem()
{
	// now initialize it.
	this->poHeap = nullptr;
	this->poRawMem = nullptr;

	// Do a land grab --- get the space for the entire heap
	// Since OS have Different Alignments... I forced it to 16 byte aligned
	poRawMem = _aligned_malloc(HEAP_TOTAL_SIZE + HEAP_HEADER_GUARDS, HEAP_ALIGNMENT);
	HEAP_SET_GUARDS

	// verify alloc worked
	assert(poRawMem != nullptr);

	// Guarantee alignemnt
	assert( ((uint32_t)poRawMem & HEAP_ALIGNMENT_MASK) == 0x0 ); 

	// ------------------------------------------------------------------------
	// Instantiate the heap header on the raw memory
	//
	// Placement new will interact with the memory tracking system:
	//
	//      No memory tracking __SHOULD__ take place on placement new
	//      So you need to tell the Memory Tracking system to ignore this placement new
	//      Add 3 lines around placement new to allow it to compile
	//
	//      example:  Dog *pDog = new(address) Dog();  // original (new way below)
	//
	//      #pragma warning( push ) <----- Add 1/3 --------------
	//      #undef new  <----------------- Add 2/3 --------------
	// 
	//      Dog *pDog = new(address) Dog();
	//
	//      #pragma warning( pop )   <----- Add 3/3 --------------
	//
	// -----------------------------------------------------------------------

	#pragma warning( push )
	#undef new      

		Heap *p = new(poRawMem) Heap();

	#pragma warning( pop )
     
	// update it
	this->poHeap = p;
}

void Mem::dump(int count)
{

	fprintf(FileIO::GetHandle(),"\n------- dump %d -------------\n\n",count);

	fprintf(FileIO::GetHandle(), "heapStart: 0x%p     \n", this->poHeap );
	fprintf(FileIO::GetHandle(), "  heapEnd: 0x%p   \n\n", (void *)((uint32_t)this->poHeap+(uint32_t)HEAP_TOTAL_SIZE) );
	fprintf(FileIO::GetHandle(), "pUsedHead: 0x%p     \n", this->poHeap->pUsedHead );
	fprintf(FileIO::GetHandle(), "pFreeHead: 0x%p     \n", this->poHeap->pFreeHead );
	fprintf(FileIO::GetHandle(), " pNextFit: 0x%p   \n\n", this->poHeap->pNextFit);

	fprintf(FileIO::GetHandle(),"Heap Hdr   s: %p  e: %p                            size: 0x%x \n",(void *)this->poHeap, this->poHeap+1, sizeof(Heap) );

	uint32_t p = (uint32_t)(poHeap+1);

	char *type;
	char *typeHdr;

	while( p < ((uint32_t)poHeap+(uint32_t)HEAP_TOTAL_SIZE) )
	{
		Used *used = (Used *)p;
		if( used->mType == Block::Used )
		{
			typeHdr = "USED HDR ";
			type    = "USED     ";
		}
		else
		{
			typeHdr = "FREE HDR ";
			type    = "FREE     ";
		}

		uint32_t hdrStart = (uint32_t)used;
		uint32_t hdrEnd   = (uint32_t)used + sizeof(Used);
		fprintf(FileIO::GetHandle(),"%s  s: %p  e: %p  p: %p  n: %p  size: 0x%x    AF: %d \n",typeHdr, (void *)hdrStart, (void *)hdrEnd, used->pUsedPrev, used->pUsedNext, sizeof(Used), used->mAboveBlockFree );
	
		uint32_t blkStart = hdrEnd;
		uint32_t blkEnd   = blkStart + used->mBlockSize; 
		fprintf(FileIO::GetHandle(),"%s  s: %p  e: %p                            size: 0x%x \n",type, (void *)blkStart, (void *)blkEnd, used->mBlockSize );

		p = blkEnd;
	}
}

// ---  End of File ---------------
