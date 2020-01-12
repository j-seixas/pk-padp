Student: João Seixas
Course: Parallel and Distributed Programming
Faculty: Politechnika Krakowska

Note: Since I was using Manjaro Linux (Arch-based distro) the following commands may differ to compile and run:

Project 1:
    mpicc proj1.c -o proj1
    mpirun --hostfile hostfile --display-map ./proj1

Project 2:
    gcc proj2.c -o proj2 -lm -lpthread
    ./proj2

Project 3 (was done with MPI):
    mpicc proj3.c -o proj3 -lm
    mpirun --hostfile hostfile --display-map ./proj3

Project 4 (was done with pthreads):
    gcc proj4.c -o proj4 -lm -lpthread
    ./proj4

Project 5:
    gcc proj5.c -o proj5 -fopenmp
    ./proj5

Project 6:
    gcc proj6.c -o proj6 -fopenmp -lm
    ./proj6