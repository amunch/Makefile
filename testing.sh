#/bin/bash

make clean
touch yo.cpp
touch hello.cpp
touch fact.cpp
touch ./outputs/*

make

time make -f harder_make

touch yo.cpp
touch hello.cpp
touch fact.cpp
touch ./outputs/*

time ./make_file -f harder_make
