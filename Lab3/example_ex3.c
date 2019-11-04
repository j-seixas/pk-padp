#include <stdio.h>
#include <stdlib.h>
#include "tmeas.h"
#include <pthread.h>

#define SIZE 12
#define NUM_THR 4

/* define structure of thread's data */
typedef struct {
	int id;
	int *a;
	int *b;
	int *c;
} vectors;

vectors vec;

void add()
{
	int i;
	for(i=0;i<SIZE;i++)
		vec.c[i]=vec.a[i]+vec.b[i];
}


int main()
{
	int *vec1,*vec2,*vec3;
	int i;
	double t;
	
	vec.a=(int *)malloc(sizeof(int)*SIZE);
	vec.b=(int *)malloc(sizeof(int)*SIZE);
	vec.c=(int *)malloc(sizeof(int)*SIZE);
	
	srand(time(NULL));			//random numbers generator initialization;
	
	for(i=0;i<SIZE;i++)
	{
		vec.a[i]=rand()%1001;		//random number generator
		vec.b[i]=rand()%1001;
		printf("vec.a[%d]=%d,vec.b[%d]=%d\n",i,vec.a[i],i,vec.b[i]);
	}
	tstart();
	add();
	t=tstop();
	for(i=0;i<SIZE;i++)
		printf("vec.c[%d]=%d\n",i,vec.c[i]);
	printf("time=%lf\n",t);

	return 0;
}
	
	