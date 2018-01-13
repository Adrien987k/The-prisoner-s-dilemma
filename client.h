#ifndef CLIENT_H
#define CLIENT_H

#inlcude "city.h"

typdef struct {
  int fd;
  FILE* chan;
} server;

server* create_server(int socket);
server* connect_to_server (char *hostname, int port);
void send_current_state (server* serv, city* cit);

#endif
