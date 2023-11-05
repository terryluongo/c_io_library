myio: myio.c
	gcc -Dbuf_max=5 -Wall -pedantic -o  myio myio.c

rtest: rtest.c
	gcc -Wall -pedantic  -o rtest rtest.c myio.c

wtest: wtest.c
	gcc -Wall -pedantic -o wtest wtest.c myio.c

ctest: ctest.c
	gcc -Wall -pedantic -o ctest ctest.c myio.c

.PHONY: clean
clean:
	rm -f myio rtest wtest ctest
