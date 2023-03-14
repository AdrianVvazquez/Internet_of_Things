CC = gcc
CFLAGS = -Wall -Werror
OUTPUT = servidor.o
DEBUG = -g
SOURCES = servidor.c 
 
all:
	$(CC) $(SOURCES) $(CFLAGS) -o $(OUTPUT) -pthread header.h
debug:
	$(CC) $(SOURCES) $(CFLAGS) -o $(OUTPUT)
clean:
	rm -f $(OUTPUT)
fresh:
	make clean
	make all
