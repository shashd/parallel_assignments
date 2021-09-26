#
# example Makefile to build an executable named myprog from myprog.cpp
#

PROGS=$(PROG4_1) $(PROG4_2) $(PROG4_3) $(PROG4_4)
PROGS_DEBUG = $(PROG4_1_DEBUG) $(PROG4_2_DEBUG) $(PROG4_3_DEBUG) $(PROG4_4_DEBUG) $(TEST_DEBUG)

PROG4_1=coarse
PROG4_1_DEBUG=coarse_debug

PROG4_2=fine
PROG4_2_DEBUG=fine_debug

PROG4_3=tatas_coarse
PROG4_3_DEBUG=tatas_coarse_debug

PROG4_4=tatas_fine
PROG4_4_DEBUG=tatas_fine_debug

TEST_DEBUG=test_debug

all: $(PROGS)

$(PROG4_1): sorted_list_coarse.hpp benchmark_example.cpp benchmark.hpp
	g++ -Wall -std=c++11 -pthread -O3 -D COARSE benchmark_example.cpp -o $(PROG4_1)

$(PROG4_1_DEBUG): sorted_list_coarse.hpp benchmark_example.cpp benchmark.hpp
	g++ -Wall -std=c++11 -pthread -fsanitize=thread -D COARSE benchmark_example.cpp -o $(PROG4_1_DEBUG)

$(PROG4_2): sorted_list_fine.hpp benchmark_example.cpp benchmark.hpp
	g++ -Wall -std=c++11 -pthread -O3 -D FINE benchmark_example.cpp -o $(PROG4_2)

$(PROG4_2_DEBUG): sorted_list_fine.hpp benchmark_example.cpp benchmark.hpp
	g++ -Wall -std=c++11 -pthread -fsanitize=thread -D FINE benchmark_example.cpp -o $(PROG4_2_DEBUG)

$(PROG4_3): sorted_list_tatas_coarse.hpp benchmark_example.cpp benchmark.hpp
	g++ -Wall -std=c++11 -pthread -O3 -D TATAS_COARSE benchmark_example.cpp -o $(PROG4_3)

$(PROG4_3_DEBUG): sorted_list_tatas_coarse.hpp benchmark_example.cpp benchmark.hpp
	g++ -Wall -std=c++11 -pthread -fsanitize=thread -D TATAS_COARSE benchmark_example.cpp -o $(PROG4_3_DEBUG)

$(PROG4_4): sorted_list_tatas_fine.hpp benchmark_example.cpp benchmark.hpp
	g++ -Wall -std=c++11 -pthread -O3 -D TATAS_FINE benchmark_example.cpp -o $(PROG4_4)

$(PROG4_4_DEBUG): sorted_list_tatas_fine.hpp benchmark_example.cpp benchmark.hpp
	g++ -Wall -std=c++11 -pthread -fsanitize=thread -D TATAS_FINE benchmark_example.cpp -o $(PROG4_4_DEBUG)

$(TEST_DEBUG): sorted_list_test.hpp benchmark_example.cpp benchmark.hpp
	g++ -Wall -std=c++11 -pthread -fsanitize=thread -D TEST benchmark_example.cpp -o $(TEST_DEBUG)

clean:
	$(RM) $(PROGS) $(PROGS_DEBUG)
