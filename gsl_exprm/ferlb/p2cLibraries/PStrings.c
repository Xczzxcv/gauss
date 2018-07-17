/*---------------------------------------------------------------------------*
 |                                                                           |
 |                <<< PStrings.c - Pascal string routines >>>                |
 |                                                                           |
 |                   Copyright Apple Computer, Inc. 1994-5                	 |
 |                            All rights reserved.                           |
 |                                                                           |
 *---------------------------------------------------------------------------*/

/* 

This file defines the following routines:

PStr_Set(dst, src) 							- Assign one Pascal string to another
PStr_Cmp(s1, s2)								- Compare two Pascal strings
PStr_Pos(sub, src)							- Position from left of substring in source string
PStr_Copy(dst, src, amount)			-	Copy src string chars to dst and form a string
PStr_Insert(dst, ins, start)		- Insert a string into another
PStr_Delete(dst, start, amount)	- Delete characters from a string
PStr_Concat(dst, s1,...,NULL)		-	Concat arbitrary number of strings
	
*/

/*
#include <Types.h>
#include <CType.h>
#include <String.h>
#include <StdArg.h>
*/
#include "types.h"
#include <varargs.h>


/*------------------------------------------------*
 | PStr_Set - Assign one Pascal string to another |
 *------------------------------------------------*

 	Assigns value of second string to first.
*/
 
void PStr_Set(unsigned char *destS, unsigned char *srcS)
{
	memcpy(destS, srcS, *srcS + 1);								/* + 1 for length byte */
}


/*---------------------------------------*
 | PStr_Cmp - Compare two Pascal strings |
 *---------------------------------------*

 Compares the contents of two Pascal strings and returns <0 if s1 < s2, 0 if
 s1 == s2, and >0 if s1 > s2.
*/
 
short PStr_Cmp(unsigned char *s1, unsigned char *s2)
{
	register int32_t n = (unsigned char)*s1++;
	int32_t m = (unsigned char)*s2++;
	
	if ((m = n - m) != 0) return (m);
	
	for (; n && *s1 == *s2; n--, s1++, s2++);
	return (n ? *s1 - *s2 : 0);
}


/*-----------------------------------------------*
 | PStr_Pos - Find 1st occurrence of a substring |
 *-----------------------------------------------*

 Find the first occurrence of Pascal string sub in the Pascal string src and
	return the index of the first occurrence.
*/

short PStr_Pos(unsigned char *sub, unsigned char *src)
{
		register int32_t sublen, limit;
		register unsigned char *p, *q, n;
		unsigned char *origSrc = src;
	
		sublen = (unsigned char)*sub++;
		if (sublen == 0) return (0);
		
		limit = (unsigned char)*src++ - sublen;
		if (limit < 0) return (0);
		
		while (limit-- >= 0) {
			q = src;
			p = sub;
			n = sublen;
			while (n--) if (*p++ != *q++) goto next;
			return ((short)(src - origSrc));
next: src++; 
		}
		return (0);
}


/*--------------------------------------------------*
 | PStr_Copy - Copy a (sub)string to another string |
 *--------------------------------------------------*

	Copy amount characters (or up to the last character of the src Pascal string)
	from the src Pascal string starting at startPos in src to the dst Pascal string
	and return a pointer to the dst Pascal string.  It is up to the caller to make
	sure enough space is reserved in the dst string.
*/

unsigned char *PStr_Copy(unsigned char *dst, unsigned char *src,
											 	int32_t startPos, int32_t amount)
{
	unsigned char *start = src + startPos;
	unsigned char *p = dst++;
	unsigned char *q = src + *src + 1;

	if (start >= q) {
		*p = '\0';
		return(p);
	}
	
	if (start+amount > q)
		amount = q - start;
	
	while (amount--)
		*dst++ = *start++;
	*p = dst - p - 1;
	return (p);
}


/*----------------------------------------------*
 | PStr_Insert - Insert one string into another |
 *----------------------------------------------*

 Insert the ins Pascal string into the dst string starting at position
 startPos in the dst string.  The function returns original dst
 Pascal string with the insert as its value.  It is up to the caller to make
 sure enough space is reserved in the dst string for the insertion.
*/

unsigned char *PStr_Insert(unsigned char *dst, unsigned char *ins,
													int32_t startPos)
{
	unsigned char *start = dst + startPos;
	register unsigned char *q, *p = dst + (unsigned char)*dst + 1;
	register int32_t inslen;

	*dst += inslen = (unsigned char)*ins;
	
	q = p + inslen;
	while (--p >= start) *--q = *p;
	
	while (inslen--) *++p = *++ins;
	return (dst);
}


/*-----------------------------------------------*
 | PStr_Delete - Delete characters from a string |
 *-----------------------------------------------*

 Delete amount characters (or all remaining characters) from the Pascal string
 starting at the character indicated by startPos.
*/

unsigned char *PStr_Delete(unsigned char *dst, int32_t startPos, int32_t amount)
{
	unsigned char *start = dst + startPos;
	register unsigned char *p;
	register int32_t n;
	
	if (amount <= 0) return (dst);
	
	p = start + amount;
	n = dst + (unsigned char)*dst - p + 1;
	
	if (n <= 0) {
		*dst = start - dst - 1;
	} else {
		while (n--) *start++ = *p++;
		*dst -= amount;
	}

	return (dst);
}


/*-----------------------------------*
 | PStr_Concat - Concatenate strings |
 *-----------------------------------*

 Concatenate all the Pascal strings specified in the argument list together
 with the dst Pascal string and return a pointer to the dst string.  It is up
 to the caller to make sure enough space is reserved in the dst string for all
 the concatenations.	No check is made to see that the string length exceeds
 255 characters.
*/

/*
unsigned char *PStr_Concat(unsigned char *dst, unsigned char *src1, ...)
*/
unsigned char *PStr_Concat(va_alist)
va_dcl
{
	va_list ap;
	register unsigned char n, *p, *dst, *q = dst + 1;
	
	*dst = 0;
	
	va_start(ap);
        dst = va_arg(ap,unsigned char *);
	
	while (p = va_arg(ap, unsigned char *)) 
		for (*dst += (n = (unsigned char)*p); n; n--) 
			*q++ = *++p;
			
	va_end(ap);
	return (dst);
}
