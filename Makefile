SHELL=	bash

all: make_file

make_file: Make_Graph.cpp makefile.cpp
	g++ -std=c++11 Make_Graph.cpp makefile.cpp -o make_file

test: test-easy-output test-medium-output test-hard-output test-memory test-time

test-easy-output: make_file
	@echo "Testing Easy Output (Correctness)..."
	@touch hello.cpp
	@touch yo.cpp
	@touch fact.cpp
	@diff --suppress-common-lines -y <(./make_file -f easy_make) ./outputs/test-easy.output

test-medium-output: make_file
	@echo "Testing Medium Output (Correctness)..."
	@touch hello.cpp
	@touch yo.cpp
	@touch fact.cpp
	@diff --suppress-common-lines -y <(./make_file -f medium_make) ./outputs/test-medium.output

test-hard-output: make_file
	@echo "Testing Hard Output (Correctness)..."
	@touch hello.cpp
	@touch yo.cpp
	@touch fact.cpp
	@diff --suppress-common-lines -y <(./make_file -f hard_make) ./outputs/test-hard.output

test-memory: make_file
	@echo "Testing Memory..."
	@[ `valgrind --leak-check=full ./make_file 2>&1 | grep ERROR | awk '{print $$4}'` = 0 ]

test-time: make_file
	@echo "Testing time (hard-make)..."
	@./measure ./make_file -f hard_make 2>&1 | tail -n 1 | awk '{ if ($$1 > 1.0) { print "Time limit exceeded"; exit 1} }'

clean:
	rm *.o hello make_file .*.time
