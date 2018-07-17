/*---------------------------------------------------------------------------*
 |                                                                           |
 |                   <<< PSets.c - Pascal set routines >>>                   |
 |                                                                           |
 |                    Copyright Apple Computer, Inc. 1994                 	 |
 |                            All rights reserved.                           |
 |                                                                           |
 *---------------------------------------------------------------------------*/

/* 

This file provides support for Pascal sets with more than 32 elements.
Note that sets are zero based.

This file defines the following routines:

PSet_Init(theSet, nbrWords)												- initialize a large set to the empty set
PSet_Set(theSet, nbrWords, theElem)								- "turn on" an element in a large set
PSet_SetRange(theSet, nbrWords, elem1, elem2)			- "turn on" a range of elements in a large set
PSet_Test(theSet, nbrWords, theElem)							- test an element in a large set
PSet_Eq(set1, set2, nbrWords)											- test two sets for equality
PSet_Subset(set1, set2, nbrWords)									- tests if first set is a subset of the second
PSet_Subset2(set1, set2)													- tests if first small (<= 32 bit) set is a subset of the second
PSet_Difference(*dest, *set1, *set2, nbrWords)		- assign difference of two sets to a destination set
PSet_Intersection(*dest, *set1, *set2, nbrWords)	- assign intersection of two sets to a destination set
PSet_Union(*dest, *set1, *set2, nbrWords)					- assign union of two sets to a destination set
	
*/

/*
#include <Types.h>
#include <Memory.h>
#include <string.h>
*/

#include <string.h>  /* removed from comment above, May 2010 */

#include "types.h"

#include "PSets.h"
 

#define ElemsPerWord 16


/*-----------------------------------------------------*
 | PSet_Init - initialize a large set to the empty set |
 *-----------------------------------------------------*/
 
void PSet_Init(uint16_t *theSet, short nbrWords)
{
	memset(theSet, 0, nbrWords * 2);
}


/*------------------------------------------------*
 | PSet_Set - "turn on" an element in a large set |
 *------------------------------------------------*/
 
void PSet_Set(uint16_t *theSet, short nbrWords, uint32_t theElem)
{
	short idx = (nbrWords - 1) - (theElem / ElemsPerWord);
	theSet[idx] = theSet[idx] | 1 << (theElem % ElemsPerWord);
}


/*--------------------------------------------------------------*
 | PSet_SetRange - "turn on" a range of elements in a large set |
 *--------------------------------------------------------------*/
 
void PSet_SetRange(uint16_t *theSet, short nbrWords, uint32_t elem1, uint32_t elem2)
{
	short idx;

	while (elem1 <= elem2) {
		idx = (nbrWords - 1) - (elem1 / ElemsPerWord);
		theSet[idx] = theSet[idx] | 1 << (elem1 % ElemsPerWord);
		++elem1;
	}
}


/*--------------------------------------------*
 | PSet_Test - test an element in a large set |
 *--------------------------------------------*/
 
Boolean PSet_Test(uint16_t *theSet, short nbrWords, uint32_t theElem)
{
	if ((theElem / ElemsPerWord) > nbrWords)
		return false;
	return (theSet[(nbrWords - 1) - (theElem / ElemsPerWord)] & 1 << (theElem % ElemsPerWord)) != 0;
}


/*--------------------------------------*
 | PSet_Eq - test two sets for equality |
 *--------------------------------------*/
 
Boolean PSet_Eq(uint16_t *set1, uint16_t *set2, short nbrWords)
{
	while (nbrWords) {
		if (*set1++ != *set2++)
			return false;
		--nbrWords;
	};
	return true;
}


/*------------------------------------------------------------*
 | PSet_Subset - tests if first set is a subset of the second |
 *------------------------------------------------------------
 
	Note:  A <= B  ==>  A ^ (A & B) = 0
				 ie, there is nothing in A that is not in both
*/
 
Boolean PSet_Subset(uint16_t *set1, uint16_t *set2, short nbrWords)
{
	short s1;
	while (nbrWords) {
		s1 = *set1++;
		if (s1 ^ (s1 & *set2++) != 0)
			return false;
		--nbrWords;
	};
	return true;
}


/*-------------------------------------------------------------------------------*
 | PSet_Subset2 - tests if first small (<= 32 bit) set is a subset of the second |
 *-------------------------------------------------------------------------------*/
 
Boolean PSet_Subset2(uint32_t set1, uint32_t set2)
{
	return (set1 ^ (set1 & set2) == 0);
}


/*----------------------------------------------------------------------*
 | PSet_Difference - assign difference of two sets to a destination set |
 *----------------------------------------------------------------------*/
 
uint16_t *PSet_Difference(uint16_t *dest, uint16_t *set1, uint16_t *set2, short nbrWords)
{
	uint16_t *origDest = dest;
	
	while (nbrWords) {
		*dest++ = *set1++ & ~*set2++;
		--nbrWords;
	};
	
	return origDest;
}


/*--------------------------------------------------------------------------*
 | PSet_Intersection - assign intersection of two sets to a destination set |
 *--------------------------------------------------------------------------*/
 
uint16_t *PSet_Intersection(uint16_t *dest, uint16_t *set1, uint16_t *set2, short nbrWords)
{
	uint16_t *origDest = dest;
	
	while (nbrWords) {
		*dest++ = *set1++ & *set2++;
		--nbrWords;
	};
	
	return origDest;
}


/*------------------------------------------------------------*
 | PSet_Union - assign union of two sets to a destination set |
 *------------------------------------------------------------*/
 
uint16_t *PSet_Union(uint16_t *dest, uint16_t *set1, uint16_t *set2, short nbrWords)
{
	uint16_t *origDest = dest;
	
	while (nbrWords) {
		*dest++ = *set1++ | *set2++;
		--nbrWords;
	};
	
	return origDest;
}


