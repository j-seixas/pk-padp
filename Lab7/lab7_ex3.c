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

    do {

        if(rank != 0) { 
            MPI_Recv(&value, 1, MPI_INT, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
            printf("Data to process %d recieved from %d on %s\n", value, status.MPI_SOURCE, name);
            MPI_Send(&value, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
            
        } else { 
            
            printf("Enter a number:\n");
            fflush(stdout);
            scanf("%d", &value);
            printf("\n");

            MPI_Send(&value, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
            if( rank == 0)
                MPI_Recv(&value, 1, MPI_INT, size - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
        } 

    } while(value >= 0);

    MPI_Finalize(); 
    return(0); 
}