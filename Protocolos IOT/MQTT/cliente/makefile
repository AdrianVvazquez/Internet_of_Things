CC = gcc
CFLAGS = -Wall -Werror
OUTPUT = cliente.o
DEBUG = -g
SOURCES = cliente.c 
 
all:
	$(CC) $(SOURCES) $(CFLAGS) -o $(OUTPUT) header.h
debug:
	$(CC) $(SOURCES) $(CFLAGS) -o $(OUTPUT)
clean:
	rm -f $(OUTPUT)
fresh:
	make clean
	make all
