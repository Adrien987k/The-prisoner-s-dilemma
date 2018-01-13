EXEC=project.exe
SOURCES=main.c strategies.c simulation.c server.c client.c
OBJECTS=$(SOURCES:.c=.o)
CC=gcc
CFLAGS=-W -Wall -g -lm

.PHONY: default clean

default: $(EXEC)

strategies.o: strategies.c strategies.h simulation.h
simulation.o: simulation.c simulation.h constant.h
main.o: main.c strategies.h simulation.h
server.o: server.c server.h simulation.h
client.o: client.c client.h simulation.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^

clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~
