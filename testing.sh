#/bin/bash

make clean
touch yo.cpp
touch hello.cpp
touch fact.cpp

make

time make -f hard_make

touch yo.cpp
touch hello.cpp
touch fact.cpp

time ./make_file -f hard_make
