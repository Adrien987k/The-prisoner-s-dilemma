
#include "client.h"

client* create_client(int socket) {
  client* client;
  client->fd = socket;
  client->chan = fdopen(socket, "rw");
  return client;
}

clients* create_clients() {
  clients* clts = malloc(sizeof(clients));
  clts->clients = malloc(0);
  clts->nb_client = 0;
  return clts;
}

void add_client(clients* clts, client* clt) {
  clts->nb_client++;
  clts->clients = realloc(clts->nb_client * sizeof(client));
  clts->clients[clts->nb_client - 1] = clt;
  return clts;
}

clients* wait_for_clients(int port) {
  struct sockaddr_in server_address;

  // créer un socket TCP
  int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  // connecter le socker au port choisi
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(port);

  if (bind(s,(struct sockaddr *)&server_address,sizeof server_address)) {
    fprintf(stderr,"Erreur : bind échoue, " "port %d encore occupé ?\n", port);
    exit(1);
  }

  // mettre le socket en mode écoute
  listen(s, 1);

  clients* clients = create_clients();

  printf("J'attends une connexion sur le port %d ...\n",port);
  while (1) {
     // laisser un client se connecter
     int client_socket = accept(s, NULL, NULL);

     if (client_socket < 0) {
       if (errno == EINTR || errno == EWOULDBLOCK) continue;
       fprintf(stderr,"Erreur : connexion client échouée\n");
       exit(1);
     }

     printf("Connexion acceptée !\n");

     add_client(clients, create_client(s));
  }

  return clients;
}

void run() {
  //clients* clients = wait_for_clients();

  //
}
