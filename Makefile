EXEC=project.exe
EXEC_SERVER=server.exe
EXEC_CLIENT=client.exe

SOURCES_COMMON=strategies.c simulation.c utils.c
SOURCES=$(SOURCES_COMMON) server.c client.c main.c
SOURCES_SERVER=$(SOURCES_COMMON) server.c server_main.c
SOURCES_CLIENT=$(SOURCES_COMMON) client.c client_main.c

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_SERVER=$(SOURCES_SERVER:.c=.o)
OBJECTS_CLIENT=$(SOURCES_CLIENT:.c=.o)
CC=gcc
CFLAGS=-W -Wall -g -lm

.PHONY: default clean

default: $(EXEC) $(EXEC_SERVER) $(EXEC_CLIENT)

strategies.o: strategies.c strategies.h simulation.h
simulation.o: simulation.c simulation.h constant.h utils.h
main.o: main.c strategies.h simulation.h
server.o: server.c server.h simulation.h
client.o: client.c client.h simulation.h
server_main.o: server_main.c
client_main.o: client_main.c
utils.o: utils.c utils.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)


$(EXEC_SERVER): $(OBJECTS_SERVER)
	$(CC) -o $@ $^

$(EXEC_CLIENT): $(OBJECTS_CLIENT)
	$(CC) -o $@ $^

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^

clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~
