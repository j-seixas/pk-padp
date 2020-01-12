#include <stdlib.h>
#include <stdio.h>
#include <math.h> 
#include "mpi.h" 

typedef struct {
    int upperPart;
	int factorialPart;
    int n;
    int x;
    double result;
} Function;


int main(int argc, char **argv) {
	int rank, size;
	int src;	
    int value;

	MPI_Status status;
	MPI_Datatype new_type, type[5] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_DOUBLE};
	int block_lengths_array[5] = {1, 1, 1, 1, 1};

	MPI_Aint disp[5], base, addr;
	Function func;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
    double start;

	if(rank == 0) 
		printf("Creating Function data type for calculations\n");

	MPI_Get_address(&func, &base);
	MPI_Get_address(&(func.upperPart), &addr);
	disp[0] = addr - base;
	MPI_Get_address(&(func.factorialPart), &addr);
	disp[1] = addr - base;
    MPI_Get_address(&(func.n), &addr);
	disp[2] = addr - base;
    MPI_Get_address(&(func.x), &addr);
	disp[3] = addr - base;
    MPI_Get_address(&(func.result), &addr);
	disp[4] = addr - base;
	
	MPI_Type_create_struct(5, block_lengths_array, disp, type, &new_type);
	MPI_Type_commit(&new_type);

    if(rank != 0) {
        MPI_Recv(&func, 1, new_type, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
        start = MPI_Wtime();
        func.n += 1;
        
        if (func.x == 0) {
            func.upperPart = 0;
            func.factorialPart = 1;
            func.result = 1;

        } else {
            func.upperPart *= func.x;
            func.factorialPart *= func.n;

            double m = func.upperPart / (double) func.factorialPart;
            func.result += func.upperPart / (double) func.factorialPart;
        }

        printf("n: %d, result: %f on %d from %d\n", func.n, func.result, rank, status.MPI_SOURCE);
        MPI_Send(&func, 1, new_type, (rank + 1) % size, 0, MPI_COMM_WORLD);
    
    } else {
        printf("Enter a integer number:\n");
        scanf("%d", &value);
        fflush(stdout);
        start = MPI_Wtime();
        func.n = 0;
        func.result = 1;
		func.upperPart = 1;
        func.factorialPart = 1;
        func.x = value;
        
        printf("n: %d, result: %f on %d\n", func.n, func.result, rank);
  
        //double start = MPI_Wtime();
		MPI_Send(&func, 1, new_type, (rank + 1) % size, 0, MPI_COMM_WORLD);
        //double end = MPI_Wtime();
        //printf("\n\nTime: %lf\n", end - start);
        if( rank == 0) {
            MPI_Recv(&func, 1, new_type, size - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("---> Final : %f  Expected : %f\n", func.result, exp(value)); 

        }
    }
    double end = MPI_Wtime();
    printf("Time rank#%d: %lf\n", rank, end - start);
	// MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}

