#
# example Makefile to build an executable named myprog from myprog.cpp
#

PROGS= $(PROG1)
PROG1=exercise1

all: $(PROGS)

$(PROG1): $(PROG1).cpp
	g++ -std=c++11 -Wall -fopenmp -O3 $(PROG1).cpp -o $(PROG1)

clean:
	$(RM) $(PROGS)
