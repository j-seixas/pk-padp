#include<stdio.h>
#include<omp.h>

omp_lock_t lock;

int main()
{
	omp_init_lock(&lock);
	srand(time(NULL));
	int a=0,i;
	omp_set_num_threads(4);
	printf("Loop with firstprivate:\n");
	#pragma omp parallel for firstprivate(a) 
		for(i=0;i<10;i++)
		{
			printf("Thread %d a=%d\n",omp_get_thread_num(),a);			
			a++;						
		}
	printf("After loop with firstprivate a=%d\n\n",a);
	
	printf("Loop with private:\n");
	#pragma omp parallel for private(a) 
		for(a=0;a<10;a++)
		{			
			printf("Thread %d a=%d\n",omp_get_thread_num(),a);
		}
	printf("After loop with private a=%d\n\n",a);
	printf("Loop with lastprivate:\n");
	#pragma omp parallel for lastprivate(a) 
		for(i=0;i<10;i++)
		{
			//1. try to change it to random number and notice how it works
			a=omp_get_thread_num();		
			//a= rand()%11;
            printf("Thread %d a=%d\n",omp_get_thread_num(),a);
		}
	printf("After loop with lastprivate a=%d\n\n",a);
	
	printf("Loop with shared:\n");	
	a=0;
	#pragma omp parallel for shared(a) 
	for(i=0;i<10;i++)
	{
		//2. What happens when we disable locks?
		//omp_set_lock(&lock);
		a=omp_get_thread_num();
		printf("Thread %d a=%d\n",omp_get_thread_num(),a);	
		//omp_unset_lock(&lock);				
	}
	//3. What is the value of "a" after several runs? 
	printf("After loop with shared a=%d\n\n",a);
	
	printf("Loop without data-sharing attribute clause:\n");	
	a=0;
	#pragma omp parallel for 
	for(i=0;i<100;i++)
	{		
		a++;
		//printf("Thread %d Iteration %d a=%d\n",omp_get_thread_num(),i,a);
	}
	//4. What is the default data scope without any clauses? 
	printf("After loop without data-sharing attribute clause a=%d\n",a);	
}	
