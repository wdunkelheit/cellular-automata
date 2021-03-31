all:
	gcc -Wall -Werror -pedantic cells.c -o automaton.o
	./run.sh
cleanup:
	rm *.o *.out *~
clean:
	rm *.o *.out *~
debug:
	gcc -g cells.c -o automaton.o
	gdb automaton.o
