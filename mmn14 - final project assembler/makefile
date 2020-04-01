assembler: assembler.o file_parser.o input_parser.o data_storage.o utilities.o
	gcc -g assembler.o file_parser.o input_parser.o data_storage.o utilities.o -ansi -Wall -pedantic -lm -o assembler
assembler.o: assembler.c data_storage.h file_parser.h
	gcc -c assembler.c -ansi -Wall -pedantic -lm -o assembler.o
file_parser.o: file_parser.c utilities.h data_storage.h input_parser.h file_parser.h
	gcc -c file_parser.c -ansi -Wall -pedantic -lm -o file_parser.o
input_parser.o: input_parser.c utilities.h input_parser.h
	gcc -c input_parser.c -ansi -Wall -pedantic -lm -o input_parser.o
data_storage.o: data_storage.c utilities.h data_storage.h
	gcc -c data_storage.c -ansi -Wall -pedantic -lm -o data_storage.o
utilities.o: utilities.c utilities.h
	gcc -c utilities.c -ansi -Wall -pedantic -lm -o utilities.o
