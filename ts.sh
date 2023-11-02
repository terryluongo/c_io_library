#!/bin/bash
make clean
make $1
rm dest.txt
./$1 $2 dest.txt
diff $2 dest.txt
