CC = gcc
CFLAGS = -Wall -g -pthread

PROGRAMS = new_phil \
	working-code \

all: $(PROGRAMS)

clean:
	rm -f *~ $(PROGRAMS)
