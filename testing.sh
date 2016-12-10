#/bin/bash

make clean
touch ./test_files/*

make

echo "OUR MAKEFILE"
echo "============"
time ./make_file -f harder_make

make clean
touch ./test_files/*

echo ""

echo "LINUX MAKEFILE"
echo "=============="

time make -f harder_make
