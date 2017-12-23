EXEC=project.exe
SOURCES=main.c strategies.c simulation.c
OBJECTS=$(SOURCES:.c=.o)
CC=gcc
CFLAGS=-W -Wall -g -lm

.PHONY: default clean

default: $(EXEC)

strategies.o: strategies.c strategies.h
simulation.o: simulation.c simulation.h strategies.h
main.o: main.c simulation.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^

clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~
