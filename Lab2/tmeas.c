#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "tmeas.h"

static struct timespec start,stop;

void tstart(void)
{
	clock_gettime( CLOCK_REALTIME, &start);	
}

double tstop(void)
{
	clock_gettime( CLOCK_REALTIME, &stop);
	return (double)( stop.tv_sec - start.tv_sec ) + (double)( stop.tv_nsec - start.tv_nsec )/1.0e9;
}
