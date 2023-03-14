CC = gcc
CFLAGS = -Wall -Werror
OUTPUT = threads.o
DEBUG = -g
SOURCES = threads.c
 
all:
	$(CC) $(SOURCES) $(CFLAGS) -o $(OUTPUT)
debug:
	$(CC) $(SOURCES) $(CFLAGS) -o $(OUTPUT)
clean:
	rm -f $(OUTPUT)
fresh:
	make clean
	make all
