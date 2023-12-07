myio: myio.c
	gcc -g -c  -Wall -pedantic -o  myio myio.c

rtest: rtest.c
	gcc -g -Wall -pedantic  -o rtest rtest.c myio.c

wtest: wtest.c
	gcc -g -Wall -pedantic -o wtest wtest.c myio.c

ctest: ctest.c
	gcc -Wall -pedantic -o ctest ctest.c myio.c

dtest: dtest.c
	gcc -Wall -pedantic -g -o dtest dtest.c myio.c
.PHONY: clean
clean:
	rm -f myio rtest wtest ctest dtest
