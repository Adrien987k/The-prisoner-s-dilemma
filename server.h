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
#include <poll.h>

#include "simulation.h"

#define PORT 1234
#define NB_CLIENT 4

typedef struct {
  int id;
  int fd;
  FILE* chan;
} client;

typedef struct {
  client* clients;
  int nb_client;
} client_pool;

client* create_client();
client_pool* create_empty_client_pool();
void add_client(client_pool* clts, client* clt);
client_pool* wait_for_clients(int port, int total_client);

void run_server();


#endif
