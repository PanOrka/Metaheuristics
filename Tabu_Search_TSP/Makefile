TARGETS = tsp
CC = gcc # flag for gcc
LOADLIBES = -lm # flag for libraries

all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS)

tsp: tsp.o tsp_func.o pcg_basic.o twlist.o

twlist.o: twlist.c twlist.h
pcg_basic.o: pcg_basic.c pcg_basic.h
tsp_func.o: tsp_func.c tsp.h
tsp.o: tsp.c tsp.h