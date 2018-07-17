/*---------------------------------------------------------------------------*
 |                                                                           |
 |                 <<< Builtins.c - p2c builtin routines >>>                 |
 |                                                                           |
 |                    Copyright Apple Computer, Inc. 1994                 	 |
 |                            All rights reserved.                           |
 |                                                                           |
 *---------------------------------------------------------------------------*/

/* 

This file defines the following routines:

InitArgCArgV(theArgC, theArgV, theEnv, cvtToPString)	- initialize global argv vector

PB_Power(base, n)	- raise base to the n-th power
	
*/

#include "Builtins.h"
#include <strings.h>

int32_t ArgC;
char **ArgV;
char **Env;

void InitArgCArgV(int32_t theArgC, char **theArgV, char **theEnv, Boolean cvtToPString)
{
	int32_t i;
	
	ArgC = theArgC;
	ArgV = theArgV;
	Env = theEnv;
	
	if (cvtToPString) {			/* change to pStrings if needed	*/
		for (i = 0; i < theArgC; ++i)	
			c2pstr(theArgV[i]);
		if (theEnv)
			for (i = 0; theEnv[i]; ++i)	
				c2pstr(theEnv[i]);
	}
}

int32_t PB_Power(int32_t base, int32_t n)
{
	int32_t i, p = 1;
	
	for (i = 1; 1 <= n; ++i)
		p = p * base;
	
	return p;
}
