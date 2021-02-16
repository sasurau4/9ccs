CFLAGS=-std=c11 -g3 -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

9ccs: $(OBJS)
	$(CC) -o 9ccs $(OBJS) $(LDFLAGS)

$(OBJS): 9ccs.h

test: 9ccs
		./test.sh

clean:
		rm -f 9ccs *.o *~ tmp*

.PHONEY: test clean