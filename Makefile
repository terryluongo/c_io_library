myio: myio.c
	gcc -o myio myio.c

rtest: rtest.c
	gcc -Wall -pedantic  -o rtest rtest.c myio.c

wtest: wtest.c
	gcc -Wall -pedantic -o wtest wtest.c myio.c

.PHONY: clean
clean:
	rm -f myio rtest wtest
