#include<stdio.h>
#include<omp.h>
#include <stdlib.h>  // rand(), srand()
#include <time.h>    // time()

#define N 500

int main()
{
   int square=0, number, i;

   double start,stop;
   srand(time(NULL));

   omp_set_num_threads(5);
	
   printf("Insert number:\n");
   scanf("%d",&number);

   //No CLAUSEs

   printf("\n########  Without any Clauses:  ########\n");

   start=omp_get_wtime();

   //how to parallelize it with good final result?
   for(i=0;i<N;i++)
   {
	   square+=number*number;
   }

   stop=omp_get_wtime();
   
   printf("Square=%d\n",square);
   printf("Time of sum = %lf\n",stop-start); 

   square=0;

   #pragma omp barrier

   //REDUCTION CLAUSE

   printf("\n########  Reduction Clause:  ########\n");

   start=omp_get_wtime();

   //how to parallelize it with good final result?
   #pragma omp parallel for reduction(+ : square)
   for(i=0;i<N;i++)
   {
	   square+=number*number;
   }

   stop=omp_get_wtime();
   
   printf("Square=%d\n",square);
   printf("Time of sum = %lf\n",stop-start); 

   square=0;

   #pragma omp barrier

   //OMP SET LOCK

   omp_lock_t writelock;

   omp_init_lock(&writelock);

   printf("\n########  OMP SET LOCK:  ########\n");

   start=omp_get_wtime();

   //how to parallelize it with good final result?
   #pragma omp parallel for
   for(i=0;i<N;i++)
   {
      omp_set_lock(&writelock);
	   square+=number*number;
      omp_unset_lock(&writelock);
   }

   stop=omp_get_wtime();

   omp_destroy_lock(&writelock);
   
   printf("Square=%d\n",square);
   printf("Time of sum = %lf\n",stop-start); 

   square=0;

   #pragma omp barrier

   //ATOMIC

   printf("\n########  ATOMIC:  ########\n");

   start=omp_get_wtime();

   //how to parallelize it with good final result?
   #pragma omp parallel for
   for(i=0;i<N;i++)
   {
      #pragma omp atomic
      square+=number*number;         
   }

   stop=omp_get_wtime();

   printf("Square=%d\n",square);
   printf("Time of sum = %lf\n",stop-start); 

   square=0;

   #pragma omp barrier

   //CRITICAL

   printf("\n########  CRITICAL:  ########\n");

   start=omp_get_wtime();

   //how to parallelize it with good final result?
   #pragma omp parallel for
   for(i=0;i<N;i++)
   {
      #pragma omp critical
      {
	      square+=number*number;         
      }
   }

   stop=omp_get_wtime();

   printf("Square=%d\n",square);
   printf("Time of sum = %lf\n",stop-start); 

   square=0;
}
