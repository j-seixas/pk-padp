#include<stdio.h>
#include<mpi.h>
#include<string.h>

typedef struct {
	char name[20];
	int age;
    float height;
    int shoeSize;
    int numChildren;
} Data;


int main(int argc, char **argv) {
	int rank, size, r;
	int src, dst, tag, i;	
	MPI_Status status;
	MPI_Datatype new_type;
	MPI_Datatype type[5] = { MPI_CHAR, MPI_INT, MPI_FLOAT, MPI_INT, MPI_INT };
	int blen[5] = { 20, 1 , 1, 1, 1};
	MPI_Aint disp[5];
	MPI_Aint base, addr;
	Data tabrecord,tabrecieved;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(rank == 0) 
		printf("MPI_Type_create_struct()\n");

	MPI_Get_address(&tabrecord, &base);
	MPI_Get_address(&(tabrecord.name), &addr);
	disp[0] = addr - base;
	MPI_Get_address(&(tabrecord.age), &addr);
	disp[1] = addr - base;
    MPI_Get_address(&(tabrecord.height), &addr);
	disp[2] = addr - base;
    MPI_Get_address(&(tabrecord.shoeSize), &addr);
	disp[3] = addr - base;
    MPI_Get_address(&(tabrecord.numChildren), &addr);
	disp[4] = addr - base;
	
	MPI_Type_create_struct(5, blen, disp, type, &new_type);
	MPI_Type_commit(&new_type);

	if(rank != 0)
	{
		strcpy(tabrecieved.name, "Peter");
		tabrecieved.age = 14;
        tabrecieved.height = 1.72;
        tabrecieved.shoeSize = 44;
        tabrecieved.numChildren = 0;
		printf("In process %d my name is %s and my age is %d, height %f, shoe size %d, number of children %d\n", rank, 
            tabrecieved.name,tabrecieved.age, tabrecieved.height, tabrecieved.shoeSize, tabrecieved.numChildren);		
		MPI_Recv(&tabrecieved, 1, new_type, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf("In process %d I have received name: %s ", rank, tabrecieved.name);
		printf("and age: %d and height %f, shoe size %d, number of children %d\n", tabrecieved.age, tabrecieved.height, 
            tabrecieved.shoeSize, tabrecieved.numChildren);
	}
	else
	{
		strcpy(tabrecord.name, "John");
		tabrecord.age = 51;
        tabrecord.height = 1.8;
        tabrecord.shoeSize = 45;
        tabrecord.numChildren = 1;
        double start = MPI_Wtime();
		for (r=1; r<size; r++)
			MPI_Send(&tabrecord, 1, new_type, r, 0, MPI_COMM_WORLD);
        double end = MPI_Wtime();
        printf("\n\nTime: %lf\n", end - start);
	}
	//MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}

