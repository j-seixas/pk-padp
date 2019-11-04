#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tmeas.h"
#include <pthread.h>

#define SIZE 12
#define NUM_THR 4
#define WAIT 1000000

/* define structure of thread's data */
typedef struct {
	int id;
	int *a;
	int *b;
	int *c;
} vectors;

vectors vec;

void* my_pthread_function( void *thr_data ) {
	int i, id, numberStart;
	id = (int) thr_data;

    numberStart = (int) ceil((float)SIZE / NUM_THR);
    for(i = id * numberStart; i < SIZE && i < numberStart * id + numberStart; i++)
        vec.c[i] = vec.a[i] + vec.b[i];
	printf("I am %d thread and starting on %d\n", id, numberStart * id);

}

void add() {
	int i;
	for(i=0;i<SIZE;i++)
		vec.c[i]=vec.a[i]+vec.b[i];
}


int main() {
	int *vec1,*vec2,*vec3;
	int i, thr;
	double t;
	
	vec.a=(int *)malloc(sizeof(int)*SIZE);
	vec.b=(int *)malloc(sizeof(int)*SIZE);
	vec.c=(int *)malloc(sizeof(int)*SIZE);
	
	srand(time(NULL));			//random numbers generator initialization;
	
	for(i=0;i<SIZE;i++) {
		vec.a[i]=rand()%1001;		//random number generator
		vec.b[i]=rand()%1001;
		printf("vec.a[%d]=%d,vec.b[%d]=%d\n",i,vec.a[i],i,vec.b[i]);
	}

    
	tstart();
	pthread_t thread_array[NUM_THR];
	
	for( i = 0; i < NUM_THR; i++ ) {
		thr = pthread_create( &thread_array[i], NULL, my_pthread_function, (void *)i );
		if( thr ) {
			fprintf( stderr, "Error: return code from pthread_create() - %d", thr);
			exit(-1);
		}
	}

	/* Wait till threads are complete before main continues. Unless we  */
    /* wait we run the risk of executing an exit which will terminate   */
	/* the process and all threads before the threads have completed.   */

	for( i = 0; i < NUM_THR; i++ )	
		pthread_join( thread_array[i], NULL);

    //add();
	t=tstop();
	for(i=0;i<SIZE;i++)
		printf("vec.c[%d]=%d\n",i,vec.c[i]);
	printf("time=%lf\n",t);

	return 0;
}

