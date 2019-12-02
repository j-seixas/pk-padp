#include<stdio.h>
#include<omp.h>

int main()
{
	long n = 10, i;
	
	#pragma omp parallel num_threads(4) default(none) shared(n)
	{
		//1. enable nowait option and try to notice difference
		#pragma omp sections //nowait
		{
			#pragma omp section
			{
				printf("Sections - Thread %d is working...\n", omp_get_thread_num());
				sleep(2);
			}	

			#pragma omp section
			{
				printf("Sections - Thread %d is working...\n", omp_get_thread_num());
				sleep(2);
			}
		}
		
		//2. Enable barrier option with and without nowait from step 1 - what can you notice? 
		//#pragma omp barrier

		#pragma omp for schedule(dynamic) private(i)
		for (i = 0; i < n; i++) {
			printf("Iteration %d is done by thread %d.\n", i, omp_get_thread_num());
			sleep(2);
		}
	}
	return 0;
}