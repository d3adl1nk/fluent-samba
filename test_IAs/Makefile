LDFLAGS=-g
CFLAGS= -Wall -Wextra -Wno-unused-parameter -Werror
7colors: main.o board.o IA.o
	gcc $(LDFLAGS) $^ -o $@
main.o: main.c board.h IA.h
	gcc $(CFLAGS) -c $<
board.o: board.c board.h
	gcc $(CFLAGS) -c $<
IA.o: IA.c IA.h board.h
	gcc $(CFLAGS) -c $<
clean:
	rm -f main.o board.o IA.o 7colors
.PHONY: clean
