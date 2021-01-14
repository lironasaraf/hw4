all: frequency

frequency: frequency.o
	gcc -Wall -g -o frequency frequency.o

frequency.o: frequency.c
	gcc -Wall -g -c frequency.c

.PHONY: clean all

clean:
	rm -f *.o frequency
