myio: myio.c
	gcc -o myio myio.c

test: test.c
	gcc -Wall -pedantic -c -o test test.c



.PHONY: clean
clean:
	rm -f myio test
