TARGETS = griewank
CC = gcc # flag for gcc
LOADLIBES = -lm # flag for libraries

all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS)

griewank: griewank.o pcg_basic.o

pcg_basic.o: pcg_basic.c pcg_basic.h
griewank.o: griewank.c pcg_basic.h