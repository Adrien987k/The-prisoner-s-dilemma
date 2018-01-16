EXEC=project.exe
EXEC_1=dileme1
EXEC_2=dileme2
EXEC_SERVER=server.exe
EXEC_CLIENT=client.exe

ALL_EXEC=$(EXEC) $(EXEC_1) $(EXEC_2) $(EXEC_SERVER) $(EXEC_CLIENT)

SOURCES_COMMON=strategies.c simulation.c utils.c
SOURCES=$(SOURCES_COMMON) server.c client.c main.c
SOURCES_1=$(SOURCES_COMMON) dileme1.c
SOURCES_2=$(SOURCES_COMMON) dileme2.c
SOURCES_SERVER=$(SOURCES_COMMON) server.c server_main.c
SOURCES_CLIENT=$(SOURCES_COMMON) client.c client_main.c

ALL_SOURCES=$(SOURCES) dileme1.c dileme2.c server_main.c client_main.c

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_1=$(SOURCES_1:.c=.o)
OBJECTS_2=$(SOURCES_2:.c=.o)
OBJECTS_SERVER=$(SOURCES_SERVER:.c=.o)
OBJECTS_CLIENT=$(SOURCES_CLIENT:.c=.o)

ALL_OBJECTS=$(ALL_SOURCES:.c=.o)

CC=gcc
CFLAGS=-W -Wall -g -lm

.PHONY: default clean

default: $(EXEC) $(EXEC_1) $(EXEC_2) $(EXEC_SERVER) $(EXEC_CLIENT)

strategies.o: strategies.c strategies.h simulation.h
simulation.o: simulation.c simulation.h constant.h utils.h
dileme1.o: dileme1.c simulation.h strategies.h
dileme2.o: dileme2.c simulation.h strategies.h
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

$(EXEC_1): $(OBJECTS_1)
	$(CC) -o $@ $^

$(EXEC_2): $(OBJECTS_2)
	$(CC) -o $@ $^

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^

clean:
	rm -rf $(ALL_EXEC) $(ALL_OBJECTS) $(ALL_SOURCES:.c=.c~) $(ALL_SOURCES:.c=.h~) Makefile~
