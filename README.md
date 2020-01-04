To compile and run Lab6 exercises, follow the example below: 

`gcc lab6_ex3.c -o ex3 -fopenmp` 

`./ex3`

To compile and run Lab7 and Lab8 exercises, follow the example below: 

`mpicc lab8_ex2.c -o ex1` 

`mpirun --hostfile hostfile --display-map ./ex1`


Use:
- `-lm` when needs `math.h`

- `-lpthread` when needs `pthread.h`