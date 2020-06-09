CFLAGS = -Wall -pedantic
TARGETS = main

all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS)

main: main.o data_get.o way_finder.o agent_movement.o genetic.o pcg_basic.o

main.o: main.c way_finder.h genom.h data_get.h
data_get.o: data_get.c data_get.h
way_finder.o: way_finder.c way_finder.h genom.h data_get.h
agent_movement.o: agent_movement.c agent_movement.h genom.h data_get.h pcg_basic.h
genetic.o: genetic.c genetic.h agent_movement.h genom.h data_get.h pcg_basic.h
pcg_basic.o: pcg_basic.c pcg_basic.h