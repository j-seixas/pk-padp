#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

/* global variable */
int global_var=0;

/* initialize mutex */
pthread_mutex_t my_mutex = PTHREAD_MUTEX_INITIALIZER;

void* my_pthread_func( void *arg )
{
	int i = 0,j;
	while( i < 20000 ) {
		i++;
		pthread_mutex_lock( &my_mutex );
		global_var += 1;
		pthread_mutex_unlock( &my_mutex );
	}
	return (void*)0;
}

int main(void) 
{
  	pthread_t my_thread;
  	int i;

  	if( pthread_create( &my_thread, NULL, my_pthread_func, NULL) ) {
    		printf("Error while creating the thread.");
    		abort();
  	}

	for ( i = 0; i < 20000; i++ ) {
    		pthread_mutex_lock( &my_mutex );
    		global_var -= 1;
    		pthread_mutex_unlock( &my_mutex );
  	}

  	if( pthread_join ( my_thread, NULL ) ) {
		printf("Error while joining the thread.");
    		abort();
  	}

  	printf("\nGlobal variable %d\n",global_var);

  	exit(0);

}
