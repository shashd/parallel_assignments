#
# example Makefile to build an executable named myprog from myprog.cpp
#

PROGS=$(PROG4_1) $(PROG4_1_DEBUG)

PROG4_1=coarse
PROG4_1_DEBUG=coarse_debug

all: $(PROGS)

$(PROG4_1): sorted_list_coarse.hpp benchmark_example.cpp benchmark.hpp
	g++ -Wall -std=c++11 -pthread -O3 -D COARSE benchmark_example.cpp -o $(PROG4_1)

$(PROG4_1_DEBUG): sorted_list_coarse.hpp benchmark_example.cpp benchmark.hpp
	g++ -Wall -std=c++11 -pthread -fsanitize=thread -D COARSE benchmark_example.cpp -o $(PROG4_1_DEBUG)

clean:
	$(RM) $(PROGS)
