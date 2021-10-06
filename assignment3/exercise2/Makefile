#
# example Makefile to build an executable named myprog from myprog.cpp
#

PROGS= $(PROG2_SEQ) $(PROG2) $(PROG2_TEST)
PROG2_SEQ=exercise2_seq
PROG2_DEBUG=exercise2_debug
PROG2_TEST=exercise2_test
PROG2=exercise2

all: $(PROGS)

$(PROG2_SEQ): Game_Of_Life.c
	gcc -std=c11 -Wall -O3 -DOUTPUT Game_Of_Life.c -o $(PROG2_SEQ)

$(PROG2): Game_Of_Life_OpenMP.c
	gcc -std=c11 -Wall -O3 -fopenmp Game_Of_Life_OpenMP.c -o $(PROG2)

$(PROG2_TEST): Game_Of_Life_OpenMP.test.c
	gcc -std=c11 -Wall -O3 -fopenmp Game_Of_Life_OpenMP.test.c -o $(PROG2_TEST)

$(PROG2_DEBUG): Game_Of_Life_OpenMP.c
	gcc -std=c11 -Wall -O2 -fopenmp -fsanitize=thread -g Game_Of_Life_OpenMP.c -o $(PROG2_DEBUG)

clean:
	$(RM) $(PROGS)
