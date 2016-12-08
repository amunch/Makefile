SHELL=	bash

all: make_file

make_file: Make_Graph.cpp makefile.cpp
	g++ -std=c++11 Make_Graph.cpp makefile.cpp -o make_file

test: test-simple-output test-memory

test-simple-output: make_file
	@echo "Testing Simple Output (Correctness)..."
	@touch hello.cpp
	@touch yo.cpp
	@touch fact.cpp
	@diff --suppress-common-lines -y <(./make_file) ./outputs/test-medium-output.output

test-memory: make_file
	@echo "Testing Memory..."
	@[ `valgrind --leak-check=full ./make_file 2>&1 | grep ERROR | awk '{print $$4}'` = 0 ]

clean:
	rm *.o hello make_file
