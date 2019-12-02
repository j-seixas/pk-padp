#include <stdlib.h>
#include <stdio.h>
#include "mpi.h" 
 
int main(int argc, char **argv) 
{ 
    int rank,ranksent,size,source,dest,tag,i,len;
    char name[20];
    len=20;
    MPI_Status status; 
    MPI_Init(&argc,&argv); 
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); 
    MPI_Comm_size(MPI_COMM_WORLD,&size); 
    MPI_Get_processor_name(name,&len);
    if(rank==0) 
    { 
        dest=0; 
        tag=0;
        for(i=1;i<size;i++) 
            MPI_Send(&rank,1,MPI_INT,i,tag,MPI_COMM_WORLD);
    } 
    else 
    { 
        MPI_Recv(&ranksent,1,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status); 
        printf("Data to process %d recieved from %d on %s\n",rank,status.MPI_SOURCE,name); 

    } 
    
    MPI_Finalize(); 
    return(0); 
}