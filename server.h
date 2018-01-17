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

#define PORT 8888
#define NB_CLIENT 4

typedef struct {
  int socket;
  FILE* chan;
} client;

typedef struct {
  client clients[NB_CLIENT];
  int nb_client;
} client_pool;

client* create_client();
client_pool* create_empty_client_pool();
void add_client(client_pool* clts, client* clt);
client_pool* wait_for_clients(int port, char* hostname);
void send_confirmation(client_pool* client_pool, int client);
void get_current_state_and_migrants(int socket, city* cit, city* migrants_city);
void display_current_cities_states(city cities[]);
city* dispatch_migrants(city migrant_cities[]);
void send_emigrants(client_pool* client_pool, city emigrants_cities[]);
void run_server(char* addr);


#endif
