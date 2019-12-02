#include <stdlib.h>
#include <stdio.h>
#include "mpi.h" 
 
int main(int argc, char **argv) { 
    int rank, ranksent, size, source, dest, tag, i, len, value ;
    char name[20];
    len = 20;
    MPI_Status status; 
    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); 
    MPI_Comm_size(MPI_COMM_WORLD,&size); 
    MPI_Get_processor_name(name,&len);
    int root = 0;

    do {
        if(rank == 0) {             
            printf("Enter a number:\n");
            fflush(stdout);
            scanf("%d", &value);
            printf("\n");
 
        } 

        MPI_Bcast(&value, 1, MPI_INT, root, MPI_COMM_WORLD);
        printf("Data to process %d recieved in %d, from %d on %s\n", value, rank, root, name);
    } while(value >= 0);

    MPI_Finalize(); 
    return(0); 
}