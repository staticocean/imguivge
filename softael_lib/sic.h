
#ifndef __SIC__
#define __SIC__

//------------------------------------------------------------------------------
// SIC - Synchronious Interval Call
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//------------------------------------------------------------------------------

typedef struct sic
{
	clock_t ts;
	clock_t interval;
	
}	s_sic;

#define sic_init(inter) (s_sic) { .ts = 0x00, .interval = (clock_t) ((double) inter * (double) CLOCKS_PER_SEC) }
#define sic_call(sic, call) if (clock() - sic.ts > sic.interval) { sic.ts = clock(); call; }

//------------------------------------------------------------------------------


#endif /* __SIC__ */






