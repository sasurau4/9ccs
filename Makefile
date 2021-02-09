CFLAGS=-std=c11 -g3 -static

9ccs: 9ccs.c

test: 9ccs
		./test.sh

clean:
		rm -f 9cc *.o *~ tmp*

.PHONEY: test clean