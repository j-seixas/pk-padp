#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include <time.h>    // time()
#define N 100

int test(double **Matrix1, double **Matrix2, double **Matrix3) {
    int i,j,k,chk=0;
    double tmp,**Mtest;
    
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            tmp = 0;
            for (k = 0; k < N; k++){
                tmp += Matrix1[i][k] * Matrix2[k][j];
            }

            if(Matrix3[i][j]!=tmp) {
                printf("Error in element %d,%d!\n",i,j);
                chk=1;	  
            }
        }
    }
    return chk;
}

int main(void) {
    double **A,**B,**C;
    int i, j, k, chk;
    double start,stop;
    double sum;
    A=(double **)malloc(N*sizeof(double *));
    B=(double **)malloc(N*sizeof(double *));
    C=(double **)malloc(N*sizeof(double *));
    for(i=0;i<N;i++) {
        A[i]=(double *)malloc(N*sizeof(double));
        B[i]=(double *)malloc(N*sizeof(double));
        C[i]=(double *)malloc(N*sizeof(double));
    }
    srand(time(NULL));
    for(i=0;i<N;i++)
        for(j=0;j<N;j++) {
            A[i][j]=rand()%1001/1000.*100;
            B[i][j]=rand()%1001/1000.*100;
        }

    omp_set_num_threads(10);

    // NO PARALLELIZATION

    printf("NO PARALLELIZATION\n");

    start=omp_get_wtime();

    //fragment for paralelization
        
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            sum = 0;
            for (k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
        
    stop=omp_get_wtime();
    printf("Time of matrix multiplification = %lf\n",stop-start); 

    chk=test(A,B,C);
    if(chk==1)
        printf("Program exited with errors\n");

    // SCHEDULE DYNAMIC First LOOP

    printf("SCHEDULE DYNAMIC First LOOP\n");

    start=omp_get_wtime();

    omp_lock_t writelock;

    omp_init_lock(&writelock);

    //fragment for paralelization
    #pragma omp parallel for schedule(dynamic) private(i,j,k, sum) shared(A,B,C)
    for (i = 0; i < N; i++) 
    {
        for (j = 0; j < N; j++){
            //omp_set_lock(&writelock);
            sum = 0;
            //omp_unset_lock(&writelock);
            for (k = 0; k < N; k++){
                //omp_set_lock(&writelock);
                sum += A[i][k] * B[k][j];
                //omp_unset_lock(&writelock);
            }

            C[i][j] = sum;
        }

    }

    omp_destroy_lock(&writelock);
        
    stop=omp_get_wtime();
    printf("Time of matrix multiplification = %lf\n",stop-start);

    chk=test(A,B,C);
    if(chk==1)
        printf("Program exited with errors\n");


    // SCHEDULE DYNAMIC Seconde LOOP

    printf("SCHEDULE DYNAMIC Seconde LOOP\n");

    start=omp_get_wtime();

    //fragment for paralelization
    
    for (i = 0; i < N; i++) 
    {
        #pragma omp parallel for schedule(dynamic) private(j, k, sum) shared(A,B,C, i)
        for (j = 0; j < N; j++){
            sum = 0;
            for (k = 0; k < N; k++){
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
        
    stop=omp_get_wtime();
    printf("Time of matrix multiplification = %lf\n",stop-start);  
    
    chk=test(A,B,C);
    if(chk==1)
        printf("Program exited with errors\n");
    return 0;
}
