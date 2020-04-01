myset: myset.o set.o
	gcc -g myset.o set.o -ansi -Wall -pedantic -lm -o myset
myset.o: myset.c myset.h set.h
	gcc -c myset.c -ansi -Wall -pedantic -lm -o myset.o
set.o: set.c set.h
	gcc -c set.c -ansi -Wall -pedantic -lm -o set.o
