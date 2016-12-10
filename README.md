MAKEFILE
--------

By: Andrew Munch (amunch) and Mark Pruitt (mpruitt1)

**Dependencies**

Dependent on Boost, so make sure that is installed.  Other libraries are standard and everything runs well on the student machines and Gitlab pipeline.

**Oranization of this Repository**

Source code is held in makefile.cpp (main), Make_Graph.h (Class Declaration), and Make_Graph.cpp (Class Implementation).  There are 4 Makefiles, easy_make, medium_make, hard_make, and harder_make, that are used to test our implementation against the Unix implementation.

Our Makefile tests the output every time make test is run against the make utility on all four of the above Makefiles.  It also verifies that there are no memory leaks and tests that the time does not run greater than a second.  

There are several directories in this repository.  'outputs' contains the make utility outputs that we compare our implementation against.  'test_files' contains test files our makefiles compile. 'time_tests' contains outputs of timing our implementation against the make utility using 'testing.sh', a very simple bash script to automate time testing with the 'time' utility.

**Setup**

Please type make to run the Makefile we provided that will compile our make utility.

**Running**

To run the makefile, simply type:

    ./make_file

This will by default run whatever is in a file called sample_make.  If you would like to test on another file, it uses the same syntax as the Linux make utility:

    ./make_file -f <desired makefile>

So, for example, on the Makefile 'hard_make', type:
    
    ./make_file -f hard_make

If you only want to run one target, and all of its dependencies, you can type something like:

    ./make_file -f hard_make yo.o

In the same way, to clean:

    ./make_file -f hard_make clean
