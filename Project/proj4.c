#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define NUM_THR 4

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int n;
double sum;


void* my_pthread_function(void *thr_data) {
	int id = (int) thr_data;
    int iter = ceil((double) n / NUM_THR);

    for(int i = id * iter + 1; i < n + 1 && i < id * iter + iter + 1; i += 1){     
        pthread_mutex_lock(&mutex);
        sum += 1.0 / i;
        pthread_mutex_unlock(&mutex);
    }

    printf("%f on #%d\n", sum, id);
}

int main() {

    printf("Enter a integer number of elements of the sum:\n");
    scanf("%d", &n);
    //fflush(stdout);

    sum = - log(n);
    
	pthread_t thread_array[NUM_THR];
	
	int thr;
	for(int i = 0; i < NUM_THR; i++) {
		thr = pthread_create(&thread_array[i], NULL, my_pthread_function, (void *)i);
		if(thr) {
			printf("Error: return code from pthread_create() - %d", thr);
			exit(-1);
        }
	}

	/* Wait till threads are complete before main continues. Unless we  */
    /* wait we run the risk of executing an exit which will terminate   */
	/* the process and all threads before the threads have completed.   */

	for(int i = 0; i < NUM_THR; i++)	
		pthread_join(thread_array[i], NULL);


    printf("Final result: %f\n", sum);

	return 0;
}

