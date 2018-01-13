#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#include "city.h"
#include "simulation.h"

#define PORT 4321

typedef struct {
  int socket;
  FILE* chan;
} server;

server* create_server(int socket);
server* connect_to_server (char *hostname, int port);
void send_current_state (server* serv, city* cit, population* migrants);
population* receive_emigrants(server* serv);
void integrate_emigrants(city* cit, population* emigrants);
void run_client();

#endif
