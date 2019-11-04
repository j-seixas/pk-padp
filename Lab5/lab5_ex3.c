#include<stdio.h>
#include<omp.h>

int main() {

	int a=0,i;
    int threads = 4;
    int chunk_size = 3;
    int size = 1500000;
	omp_set_num_threads(threads);
	printf("Loop with firstprivate:\n");


    printf("Schedule (static, chunk)\n");
    double time, time2;
    time = omp_get_wtime();
	#pragma omp parallel for schedule(static, chunk_size) 
		for(i=0;i<size;i++) {
			a*=0.5;						
			//printf("Thread %d, index %d\n",omp_get_thread_num(),i);		
		}
    
    time2 = omp_get_wtime();
    printf("%lf\n", time2 - time);


    printf("Schedule (static, default chunk)\n");
    time = omp_get_wtime();
    #pragma omp parallel for schedule(static) 
        for(i=0;i<size;i++) {
            a*=0.5;						
            //printf("Thread %d, index %d\n",omp_get_thread_num(),i);		
        }

    time2 = omp_get_wtime();
    printf("%lf\n", time2 - time);


    printf("Schedule (dynamic, chunk)\n");
    time = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic, chunk_size) 
        for(i=0;i<size;i++) {
            a*=0.5;						
            //printf("Thread %d, index %d\n",omp_get_thread_num(),i);		
        }
    
    time2 = omp_get_wtime();
    printf("%lf\n", time2 - time);


    printf("Schedule (dynamic, default chunk)\n");
    time = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic) 
        for(i=0;i<size;i++) {
            a*=0.5;						
            //printf("Thread %d, index %d\n",omp_get_thread_num(),i);		
        }

    time2 = omp_get_wtime();
    printf("%lf\n", time2 - time);

}