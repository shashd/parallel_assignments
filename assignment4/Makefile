#
# example Makefile to build an executable named myprog from myprog.cpp
#

PROGS= $(PROG2) $(PROG3)
PROG1=hello_mpi
PROG2=sieve_p2p
PROG3=exercise1_bcast_reduce

all: $(PROGS)

$(PROG1): $(PROG1).c
	mpicc -Wall $(PROG1).c -o $(PROG1)

$(PROG2): $(PROG2).cpp
	mpic++ -std=c++0x -Wall -O3 $(PROG2).cpp -o $(PROG2)

$(PROG3): $(PROG3).cpp
	mpic++ -std=c++0x -Wall -O3 $(PROG3).cpp -o $(PROG3)

clean:
	$(RM) $(PROGS)
