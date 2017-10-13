LDFLAGS=-g
CFLAGS= -Wall -Wextra -Wno-unused-parameter -Werror
7colors: mainC.o board.o IA.o
	gcc $(LDFLAGS) $^ -o $@
mainC.o: mainC.c board.h IA.h
	gcc $(CFLAGS) -c $<
board.o: board.c board.h
	gcc $(CFLAGS) -c $<
IA.o: IA.c IA.h board.h
	gcc $(CFLAGS) -c $<
clean:
	rm -f mainC.o board.o IA.o 7colors
.PHONY: clean
