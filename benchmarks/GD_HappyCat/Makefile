TARGETS = happycat
CC = gcc # flag for gcc
LOADLIBES = -lm # flag for libraries

all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS)

happycat: happycat.o pcg_basic.o

pcg_basic.o: pcg_basic.c pcg_basic.h
happycat.o: happycat.c pcg_basic.h