#!/bin/bash
make clean
make wtest
make rtest
make ctest

rm dest.txt
rm files/ctest_1d.txt
rm files/ctest_2d.txt

echo "rtest"
./rtest $1 dest.txt
diff $1 dest.txt
rm dest.txt

echo "wtest"
./wtest $1 dest.txt
diff $1 dest.txt

echo "ctest"
./ctest 
diff files/ctest_1s.txt files/ctest_1d.txt
diff files/ctest_2s.txt files/ctest_2d.txt
