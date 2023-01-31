CC=cc
CFLAGS= -I .. -Wall -Wextra

PROGRAMS=factorial strrev

.PHONY: all test clean

all: $(PROGRAMS)

test: all
	./factorial
	./strrev

clean:
	rm -f $(PROGRAMS)

factorial: factorial.c ../phantasm.h
	$(CC) $(CFLAGS) factorial.c -o factorial

strrev: strrev.c ../phantasm.h
	$(CC) $(CFLAGS) strrev.c -o strrev
