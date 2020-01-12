#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#define NUM_THR 4

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int a, b, n;
double h;
double sum;
double f(double x);
double xi(int i);

void* my_pthread_function(void *thr_data) {
	int id = (int) thr_data;

    for(int i = id; i < n - 2; i += NUM_THR){
        
        pthread_mutex_lock(&mutex);
        sum += f(xi(i + 1));
        pthread_mutex_unlock(&mutex);

    }
    printf("%f on #%d\n", sum, id);
}

double f(double x){
    return 1.0 / (1 + pow(x, 2));
}

double xi(int i) {
    return a + i * h;
}

int main() {
    a = 0;
    b = 1;
    printf("Enter a integer number of subintervals:\n");
    scanf("%d", &n);
    //fflush(stdout);

    double h = (b - a) / (double) n;
    sum = (f(a) + f(b)) / 2.0;
    
	pthread_t thread_array[NUM_THR];
	
    clock_t begin = clock();
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

    sum = 4 * h * sum;

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Final result: %f, duration %lf\n", sum, time_spent);

	return 0;
}

