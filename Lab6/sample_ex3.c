#include<stdio.h>
#include<omp.h>

#define N 500

int main()
{
    int square=0, number, i;
        
    printf("Insert number:\n");
    scanf("%d",&number);
    //how to parallelize it with good final result?
    //#pragma omp parallel for 
    for(i=0;i<N;i++) {
        square+=number*number;
    }
    
    printf("Square=%d\n",square);
}
