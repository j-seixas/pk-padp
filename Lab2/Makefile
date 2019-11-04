#C compiler
CCOMP = gcc

#C linker
LOADER = gcc

# optymalization options
OPT = -O3

# localization of header files
INC = -I./inc

# libraries
LIB = -L./lib -ltmeas

# dependancies and commands
lab2: lab2.o
	$(LOADER) $(OPT) lab2.o -o lab2 $(LIB)

# how to obtain a file lab2.o
lab2.o: lab2.c
	$(CCOMP) -c $(OPT) lab2.c $(INC)

tmeas.o: tmeas.c
	$(CCOMP) -c $(OPT) tmeas.c $(INC)

clean:
	rm -f *.o
