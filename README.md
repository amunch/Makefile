MAKEFILE
--------

By: Andrew Munch (amunch) and Mark Pruitt (mpruitt1)

**Dependencies**

Dependent on Boost, so make sure that is installed.  Other libraries are standard and everything runs well on the student machines and Gitlab pipeline.

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
