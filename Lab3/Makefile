#C compiler
CCOMP = gcc

#C linker
LOADER = gcc

# optymalization options
OPT = -O3

# localization of header files
INC = -I./inc

# libraries
LIB = -L./lib -ltmeas -lpthread

# dependancies and commands
lab3_ex3: lab3_ex3.o
	$(LOADER) $(OPT) lab3_ex3.o -o lab3_ex3 $(LIB)

# how to obtain a file lab2.o
lab3_ex3.o: lab3_ex3.c
	$(CCOMP) -c $(OPT) lab3_ex3.c $(INC)

tmeas.o: tmeas.c
	$(CCOMP) -c $(OPT) tmeas.c $(INC)

clean:
	rm -f *.o
