#
# example Makefile to build an executable named myprog from myprog.cpp
#

PROGS= $(PROG2)
PROG1=hello_mpi
PROG2=sieve_p2p

all: $(PROGS)

$(PROG1): $(PROG1).c
	mpicc -Wall $(PROG1).c -o $(PROG1)

$(PROG2): $(PROG2).cpp
	mpic++ -std=c++0x -Wall -O3 $(PROG2).cpp -o $(PROG2)

clean:
	$(RM) $(PROGS)
