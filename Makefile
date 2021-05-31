CFLAGS=-std=c11 -g3 -static -fno-common
# SRCS=$(filter-out foo.c, $(wildcard *.c))
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

9ccs: $(OBJS)
	$(CC) -o 9ccs $(OBJS) $(LDFLAGS)

$(OBJS): 9ccs.h

test: 9ccs test/test_cases.c
	@./9ccs test/test_cases.c > tmp.s
	@gcc -o tmp tmp.s test_func.o
	@./tmp

debug: 9ccs
		./debug.sh

clean:
		rm -f 9ccs *.o *~ tmp*

.PHONEY: test clean