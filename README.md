# assignment 2

by Terry Luongo and Dan Urchuk

## myio

This assignemnt implemented our io functions: myread, myseek, myflush, mywrite, myopen, and myclos

## testing

We have a shell script, `ts.sh`, that executes multiple test files: `rtest.c`,`wtest.c`,and `ctest.c`

`rtest.c` tests `myread` while using default syscalls for write.

`wtest.c` tests `mywrite` while using default syscalls for read.

`ctest.c` tests concurrently reading and writing to two files.

### usage 

To call shell script: `./ts.sh <filename.txt>` 

It will run the read and write tests on the provided file, printing out any differences.  Additionally it will run the concurrent test on fixed files that it uses.


## resources used

We consulted the man pages a lot for this.  There wasn't a ton of research online other than using Google Search's AI tool for syntax occasionally.  Not a ton of syscalls used in this, mostly implementation heavy.  


## known bugs

none at the moment. 

