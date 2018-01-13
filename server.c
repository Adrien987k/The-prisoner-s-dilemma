
#include "server.h"

client* create_client(int socket) {
  client* clt = malloc(sizeof(client));
  clt->fd = socket;
  clt->chan = fdopen(socket, "rw");
  return clt;
}

client_pool* create_empty_client_pool() {
  client_pool* clients = malloc(sizeof(client_pool));
  clients->clients = malloc(0);
  clients->nb_client = 0;
  return clients;
}

void add_client(client_pool* clients, client* client) {
  clients->nb_client++;
  clients->clients = realloc(clients->clients, clients->nb_client * sizeof(client));
  clients->clients[clients->nb_client - 1] = *client;
}

client_pool* wait_for_clients(int port, int total_client) {
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

  client_pool* client_pool = create_empty_client_pool();

  printf("J'attends une connexion sur le port %d ...\n",port);
  while (client_pool->nb_client < total_client) {
     // laisser un client se connecter
     int client_socket = accept(s, NULL, NULL);

     if (client_socket < 0) {
       if (errno == EINTR || errno == EWOULDBLOCK) continue;
       fprintf(stderr,"Erreur : connexion client échouée\n");
       exit(1);
     }

     printf("Connexion acceptée !\n");

     add_client(client_pool, create_client(s));
  }

  return client_pool;
}

population* get_migrants(int socket) {
  population* migrants = create_population(0, NULL);

  int buffer[BUFSIZE];
  recv(socket, buffer, BUFSIZE * sizeof(int), 0);

  int i;
  for (i = 0; i < NB_STRATEGY; i++) migrants->proportions[i] = buffer[i];
  migrants->nb_entity = buffer[i];

  return migrants;
}

void run_server() {
  client_pool* client_pool = wait_for_clients(PORT, NB_CLIENT);

  struct pollfd fds[NB_CLIENT];
  int i;
  for (i = 0; i < NB_CLIENT; i++) {
    fds[i].fd = client_pool->clients[i].fd;
    fds[i].events = POLLIN;
  }

  int nb_client_ok;
  bool clients_ok[NB_CLIENT];
  population* migrants = malloc(NB_CLIENT * sizeof(population));

  while (true) {
    for (i = 0; i < NB_CLIENT; i++) clients_ok[i] = 0;

    while (true) {
      nb_client_ok = 0;

      if (poll(fds, NB_CLIENT, -1) < 0)
        fprintf(stderr, "Error poll: %d", errno);

      for (i = 0; i < NB_CLIENT; i++) {
        if (fds[i].revents == POLLIN) {
          if (!clients_ok[i]) {
            clients_ok[i] = true;
            migrants[i] = *get_migrants(fds[i].fd);

            nb_client_ok++;
          }
        }
      }

      if (nb_client_ok == NB_CLIENT) {
        //Répartir les population dans les autres villes
      }
    }

  }

}
