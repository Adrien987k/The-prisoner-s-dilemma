#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>

typedef struct {
  int id;
  int fd;
  FILE* chan;
} client;

typedef struct {
  client* clients;
  int nb_client;
} clients;

client* create_client();
clients* create_clients();
void add_client(clients* clts, client* clt);
clients* wait_for_clients(int port);

void run();


#endif SERVER_H
