#!/bin/bash
make clean
make test
rm dest.txt
./test $1 dest.txt
diff $1 dest.txt
