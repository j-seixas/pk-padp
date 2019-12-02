#include<stdio.h>
#include<mpi.h>
#include<string.h>

typedef struct {
	char name[20];
	int age;
} Data;


int main(int argc, char **argv)
{
	int rank, size, r;
	int src, dst, tag, i;	
	MPI_Status status;
	MPI_Datatype new_type;
	MPI_Datatype type[2] = { MPI_CHAR, MPI_INT };
	int blen[2] = { 20, 1 };
	MPI_Aint disp[2];
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
	
	MPI_Type_create_struct(2, blen, disp, type, &new_type);
	MPI_Type_commit(&new_type);

	if(rank != 0)
	{
		strcpy(tabrecieved.name, "Peter");
		tabrecieved.age = 14;
		printf("In process %d my name is %s and my age is %d\n",rank,tabrecieved.name,tabrecieved.age);		
		MPI_Recv(&tabrecieved, 1, new_type, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf("In process %d I have received name: %s ",rank,tabrecieved.name);
		printf("and age: %d\n",tabrecieved.age);
	}
	else
	{
		strcpy(tabrecord.name, "John");
		tabrecord.age = 51;
		for (r=1; r<size; r++)
			MPI_Send(&tabrecord, 1, new_type, r, 0, MPI_COMM_WORLD);
	}
	//MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}

