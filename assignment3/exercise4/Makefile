#
# example Makefile to build an executable named myprog from myprog.cpp
#

PROGS= $(PROG4) $(PROG4_DEBUG)
PROG4_SOURCE=GauElim.cpp
PROG4=exercise4
PROG4_DEBUG=exercise4_debug

all: $(PROGS)

$(PROG4): $(PROG4_SOURCE)
	g++ -std=c++11 -fopenmp -O3 -Wall -D TEST3 $(PROG4_SOURCE) -o $(PROG4)

$(PROG4_DEBUG): $(PROG4_SOURCE)
	g++ -std=c++11 -fopenmp -fsanitize=thread -g -O2 -Wall -D TEST3 $(PROG4_SOURCE) -o $(PROG4_DEBUG)

clean:
	$(RM) $(PROGS)
