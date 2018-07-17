/*---------------------------------------------------------------------------*
 |                                                                           |
 |                  <<< PHeap.c - Heap Manager Routines >>>                  |
 |                                                                           |
 |                    Copyright Apple Computer, Inc. 1994                 	 |
 |                            All rights reserved.                           |
 |                                                                           |
 *---------------------------------------------------------------------------*/

/* This file contains:

initHeap(size, delta, nonCont, proc)	- Heap Manager inititialization
INITHEAP(size, delta, nonCont)	        - Pascall callable initHeap
New(amount)			        - Allocate amount space on the heap
NEW(amount)			        - Pascal callable version of new
Mark(p)				   - Set p to point at current heap position
MARK(p)				   - Pascal callable version of mark
Release(p)			   - Free heap space back to p
*/

/*
#include <Types.h>
#include <CType.h>
#include <String.h>
#include <Memory.h>
#include <Errors.h>
*/
#include "types.h"
#include <stdio.h>

#define DEFAULTSIZE   10000 /* Default heap size */
#define DEFAULTDELTA  10000 /* Default heap delta */
#define NONCONTIGUOUS	true /* Default for noncontiguous heap chunks*/

int32_t initalHeap	= DEFAULTSIZE;	/* Size of initial heap chunk */
int32_t heapDelta 	= DEFAULTDELTA;	/* Size of additional heap chunks */
Boolean nonContiguous	= NONCONTIGUOUS;/* true ==> allow non-contiguous chunks	*/
void (*memErrProc)() 	= NULL;	/* Ptr to user's memory error proc */
Boolean extendFirst     = false; /* true ==> extend heap before new chunk*/

OSErr heapResult = 0;/* Most recent heap error result code */

typedef struct HeapHdr {  /* Layout of a heap chunk header*/
	struct HeapHdr *nextHeap;  /*   Ptr to next heap chunk */
	int32_t heapSize;             /*   Size of this chunk including hdr */
};

#ifndef __cplusplus
typedef struct HeapHdr HeapHdr;
#endif

static HeapHdr *heapStart = NULL;/* Ptr to 1st heap chunk*/
static HeapHdr *currHeap  = NULL; /* Ptr to current heap chunk	*/
static char *heapPtr = NULL;	/* Ptr to next byte to allocate in heap	*/



/*----------------------------------------*
 | initHeap - Heap Manager Initialization |
 *----------------------------------------*
	
 Call this proc to initialize the Heap Manager.  The parameters detemine
 certain characteristics of the way the heap is allocated.
 
 The initSize parameter determines the initial size (in bytes) of the
 unallocated heap.  When this space if fully allocated an attempt is made to
 extend this space contiguously with the original space.  If that extension is
 unsuccessful, and the parameter nonCont is set to true, a new "chunk" of
 (generally, non-contiguous) heap space is allocated.  The size of this
 additional heap space is specified by the delta parameter (again the size is
 in bytes).
 
 If anything goes wrong during allocation of the heap or space within it, for
 example, additional contiguous space cannot be allocated and nonCont was set
 false, then the Heap Manager uses the errProc to report the error to the
 caller.  The errProc call can be done from initHeap, new, or release, and they
 assume the following declaration:

 void errProc(heapResult)
	 OSErr heapResult;
 {
	 - - -
 }
 
 The heapResult is an error number appropriate to the situation usually
 reported by the underlying system memory management routines (the routines the
 Heap Manager itself uses).  The errProc may or may not return to the Heap
 Manager.  If it does, error reporting from the individual Heap Manager
 routines is the same as if no errProc was supplied which is the case we will
 describe shortly.  Either way, the heapResult code of the most recent Heap
 Manager call is available in the global heapResult.
 
 If there is no errProc, i.e., NULL is specified, or, as we just said, the
 errProc returns, then error reporting is a function of each routine and is
 discussed with the description of that routine.  In the case of initHeap,
 an error code (the same one passed to the errProc) is returned as the
 function result, or 0 if there is no error.

 The only error that could result from calling initHeap is a failure to
 allocate the initial heap of initSize bytes.  It is assumed initHeap is called
 only once, and at the beginning of execution.  Thus, generally you don't have
 to check the return code because, if the initial allocation fails, something
 is seriously wrong with your system, and something else is also bound to fail
 that you will check!
 
 If initHeap is NOT called, then it will be automatically called on the first
 call to new.  The call will be initHeap(10000, 10000, true, NULL) (and, by the
 way, the return code IS checked by new -- new has its own way of reporting
 errors to its caller).

 If initHeap is NOT called, then it will be automatically called on the first
 call to new.  The call will be done as,
 
 initHeap(initalHeap, heapDelta, nonContiguous, memErrProc);
 
 (and, by the way, the return code IS checked by new -- new has its own way of
 reporting errors to its caller).  The parameters are global variables that
 YOU may set.  Thus you may bypass the explicit initHeap, letting new do it,
 with values that you can set.  If you don't set them then they are defaulted
 to give an initHeap call of initHeap(10000, 10000, true, NULL);
*/

OSErr initHeap(int32_t initSize, int32_t delta, Boolean nonCont, void (*errProc)())
{
	int32_t heapSize = initSize + sizeof(HeapHdr);

	heapResult = noErr;
	
	if (heapStart == NULL) {
		heapDelta 		= delta + sizeof(HeapHdr);
		nonContiguous = nonCont;
		memErrProc	  = errProc;
		extendFirst		= false;
		
		if (currHeap = heapStart = (HeapHdr *)NewPtr(heapSize)) {
			heapStart->heapSize = heapSize;
			heapStart->nextHeap = NULL;
			heapPtr = (char *)heapStart + sizeof(HeapHdr);
			return (noErr);
		}
		
		heapResult = MemError();
		if (memErrProc) (*memErrProc)(heapResult);
		return (heapResult);
	}
}

/*------------------------------------------------------------------*
 | INITHEAP - Heap Manager Initialization (Pascal callable version) |
 *------------------------------------------------------------------*
 
 Note: The error return is ignored and the errProc is not supported!
*/

pascal void INITHEAP(int32_t initSize, int32_t delta, Boolean nonCont)
{
	initHeap(initSize, delta, nonCont, NULL);
}

/*---------------------------*
 | New - Allocate Heap Space |
 *---------------------------*

 Allocate amount bytes of heap space.  The amount is rounded up to an even
 value if necessary.  If the space cannot be allocated NULL is returned (over
 and above any error reporting done by the errProc specified to initHeap).
 
 The initHeap routine is called on the first call to new if initHeap was not
 explicitly called prior to calling new.
*/

void *New(int32_t amount)
{
	int32_t newSize, heapSize, amt;
	char *heapPtr0;
	HeapHdr *newHeapChunk;
		
	heapResult = noErr;

	/* Init the heap if not already inited. */
	
	if (!heapStart)
		if (initHeap(initalHeap, heapDelta, nonContiguous, memErrProc))
			return (NULL);
	
	/* Try to carve the space out of our existing heap chunk. */
	
	if ((amt = amount) & 1) amt++;											/* allocate even amount	*/
	
	heapPtr0 = heapPtr;																	/* ptr before updating	*/
	newSize = (heapPtr += amt) - (char *)currHeap;			/* ptr after updating		*/
	
	if (newSize <= currHeap->heapSize) return (heapPtr0);
	
	/* The new amount won't fit in the current heap chunk.  If the user doesn't */
	/* want non-contiguous allocations we try to extend the current heap chunk  */
	/* by the amount we need. If that fails, it's all over! 										*/
	
	/* For non-contiguous allocations we first try to get another heap chunk and*/
	/* carve the space from that.  If we can't get the new chunk, then we again */
	/* try to extend the current heap chunk by what we need as a last ditch try.*/
	/* Failure here again is bad news!  If, on the other hand, the last ditch 	*/
	/* try succeeded, then we assume all future trys at getting new heap chunks	*/
	/* will fail. So we set a switch, extendFirst, to force us to try to extend	*/
	/* the current chunk just as in the contiguous case. The switch is initially*/
	/* false, and only reset to false, once set true, by release() when it has  */
	/* other chenks to free up.																									*/
	
	/* Note: the original algorithm conceptually always had extendFirst set as	*/
	/* true (there was no switch -- but was coded as if the switch was always 	*/
	/* true).  The effect was to always try to extend the current chunk before	*/
	/* going after a new chunk (if non-contiguous -- contiguous is be an error	*/
	/* if the extend failed).  In the days of a small heap that was desirable.	*/
	/* But with larger present day heaps the effect is to successfully extend		*/
	/* the current heap on EVERY New() call (once the initial allocation was 		*/
	/* used up of course). That's because with a larger application heap, there */
	/* is usually pleanty of room to do the extends. So, to avoid this, we just */
	/* allocate the secondary heap chunks and carve the new space out of them.  */
	/* Only if we can't get a chunk do we fall back on the original algorithm 	*/
	/* and do the extends, and continue to do them until release() frees a 			*/
	/* chunk.  Hence the reason for the extendFirst switch.						 					*/
	
	if (!nonContiguous || extendFirst) {								/* contiguous | desprate*/
		SetPtrSize((char *)currHeap, newSize);						/* extend by SetPtrSize	*/
		if ((heapResult = MemError()) == noErr) {					/*           **********	*/
			currHeap->heapSize = newSize;
			return (heapPtr0);
		}
		if (!nonContiguous) {															/* extend failed				*/
			if (memErrProc) (*memErrProc)(heapResult);
			return (NULL);
		}
	}
	
	heapSize = amt + sizeof(HeapHdr);										/* get new heap chunk		*/
	if (heapSize < heapDelta) heapSize = heapDelta;
	
	if (newHeapChunk = (HeapHdr *)NewPtr(heapSize)) {		/* extend by NewPtr			*/
		newHeapChunk->heapSize = heapSize;								/*           ******			*/
		newHeapChunk->nextHeap = NULL;
		currHeap->nextHeap = newHeapChunk;								/* chain chunks together*/
		currHeap = newHeapChunk;
		heapPtr = (heapPtr0 = (char *)newHeapChunk + sizeof(HeapHdr)) + amt;
		return (heapPtr0);
	} else if (!extendFirst) {													/* didn't get heap chunk*/
		SetPtrSize((char *)currHeap, newSize);						/* extend by SetPtrSize	*/
		if ((heapResult = MemError()) == noErr) {					/*           **********	*/
			extendFirst = true;															/* extend 1st next time	*/
			currHeap->heapSize = newSize;
			return (heapPtr0);
		}
	}
	
	heapResult = MemError();
	if (memErrProc) (*memErrProc)(heapResult);
	return (NULL);
}


/*-----------------------------------------------------*
 | NEW - Allocate Heap Space (Pascal callable version) |
 *-----------------------------------------------------*/
 
pascal char *NEW(int32_t amount)
{
	return(New(amount));
}


/*-------------------------------------------------------------------------*
 | Mark - "Mark" the heap, i.e., remember the current top of heap location |
 *-------------------------------------------------------------------------*

 Set a pointer to the heap area.  The current "top" of heap is returned in the
 parameter p and as the function result.  A subsequent call to release will
 free all heap space from this point, p, on.
*/

void *Mark(void **p)
{
	if (!heapStart)
		if (initHeap(initalHeap, heapDelta, nonContiguous, memErrProc))
			return (NULL);

	heapResult = noErr;
	return(*p = heapPtr);
}


/*--------------------------------------------------*
 | MARK - "Mark" the heap (Pascal callable version) |
 *--------------------------------------------------*/

pascal void MARK(void **p)
{
	if (!heapStart)
		if (initHeap(initalHeap, heapDelta, nonContiguous, memErrProc)) {
			*p = NULL;
			return;
		}

	heapResult = noErr;
	*p = heapPtr;
}


/*--------------------------------------------------------*
 | Release - Free heap space back to the specified "mark" |
 *--------------------------------------------------------*

 Deallocate all heap space back to the specified "mark", p.  It is assumed p
 is a value set by a mark call.  All space from allocated after that mark call
 is deallocated.  Pointers to that area should NOT be used again!  Errors are
 not explicitly reported from a call to this proc except perhaps through the
 errProc specified to initHeap.
*/

void Release(char *p)
{
	register HeapHdr *chunkPtr, *nextHeap;

	heapResult = noErr;
	
	if (nonContiguous) {
		for (chunkPtr = heapStart; chunkPtr; chunkPtr = chunkPtr->nextHeap)
			if (p >= (char *)chunkPtr + sizeof(HeapHdr) && 
					p <= (char *)chunkPtr + chunkPtr->heapSize) goto gotIt;
		
		heapResult = memPCErr;
		if (memErrProc) (*memErrProc)(memPCErr);
		return;
	
		gotIt:
		
		currHeap = chunkPtr;
		nextHeap = chunkPtr->nextHeap;
		chunkPtr->nextHeap = NULL;
		for (chunkPtr = nextHeap; chunkPtr; chunkPtr = nextHeap) {
			nextHeap = chunkPtr->nextHeap;
			DisposPtr((char *)chunkPtr);
			extendFirst = false;			/* let New() get chunks 1st again */
		}
	}
	
	heapPtr = p;
	return;
}
