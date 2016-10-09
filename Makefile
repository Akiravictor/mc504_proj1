CC = gcc
CFLAGS = -Wall -g -pthread

PROGRAMS = new_phil \

all: $(PROGRAMS)

clean:
	rm -f *~ $(PROGRAMS)
