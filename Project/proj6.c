#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#define NUM_THR 4
#define N 3

int main() {
    omp_set_num_threads(NUM_THR);
    omp_lock_t lock;
    double time_start, time_stop;
    int n;
    
    printf("Enter a integer number for the upper limit to calculate prime numbers:\n");
    scanf("%d", &n);
    //fflush(stdout);

    int nums[n - 1];

    for(int i = 2; i < n + 1; i++)
        nums[i - 2] = i;
    
    int tmp = (int) ceil(sqrt(n));

    time_start = omp_get_wtime();

    int i, j;
    #pragma omp parallel for schedule(static) shared(nums) private(i, j)
    for(i = 0; i < tmp; i++) {
        omp_set_lock(&lock);
        if(nums[i] != 0) {
            for(j = pow(nums[i], 2) - 2; j < n - 1; j += nums[i]){
                printf("#%d -> nums[i]:%d j:%d -> %d\n", omp_get_thread_num(), nums[i], j, nums[j]);
                if(nums[j] != 0) {
                    nums[j] = 0;
                }
                
            }
        }
        omp_unset_lock(&lock);
    }
    
    omp_destroy_lock(&lock);
    time_stop = omp_get_wtime();
    
    int counter = 0;
    printf("Prime numbers:");
    for(i = 0; i < n - 1; i++) {
        if(nums[i] != 0) {
            printf(" %d", nums[i]);
            counter++;
        }
    }
    printf("\nFound %d prime numbers in %f.\n", counter, time_stop - time_start);

    return 0;
}