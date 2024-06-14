all: disassemble

CC = clang

NAME = disassemble

CFLAGS = -Wall

LIBRARY = -lm

OBJECTS = main.o file_handler.o disassembler.o log.o 

SRC_FILE = ../src/main.c ../src/disassembler.c ../src/file_handler.c

INCLUDE = -I../include/



disassemble: $(OBJECTS)
	$(CC) $(INCLUDE) $(LIBRARY) $(OBJECTS) -O2 -o disassemble

file_handler.o: ../src/file_handler.c
	$(CC) $(INCLUDE) -c ../src/file_handler.c

disassembler.o: ../src/disassembler.c
	$(CC) $(INCLUDE) -c ../src/disassembler.c

main.o: ../src/main.c
	$(CC) $(INCLUDE) -c ../src/main.c

log.o: ../src/log.c
	$(CC) $(INCLUDE) -c ../src/log.c
clean:
	rm *.o disassemble

