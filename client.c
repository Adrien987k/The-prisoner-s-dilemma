
#include "client.h"

server* create_server(int socket) {
  server* serv = malloc(sizeof(server));
  serv->socket = socket;
  serv->chan = fdopen(socket, "rw");
  return serv;
}

// Se connecter au serveur sur hostname:port, renvoie un descripteur
server* connect_to_server (char *hostname, int port) {
  // créer un socket TCP
  int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  // trouver l'adresse IP du serveur
  struct hostent *host_address = gethostbyname(hostname);
  if (!host_address) {
    fprintf(stderr,"Erreur : serveur %s non connu\n",hostname);
    exit(1);
  }

  // bricoler l'adresse avec le port
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;

  memcpy(&server_address.sin_addr, host_address->h_addr_list[0], host_address->h_length);
  server_address.sin_port = htons(port);

  // se connecter au serveur
  printf("Connexion au serveur à %s:%d ...\n",hostname,port);
  while (1) {
    if (connect(s,(struct sockaddr *)&server_address, sizeof server_address) == 0) {
      printf("Connecté !\n");
      return create_server(s);
    }
    sleep(3);
    printf("J'essaie à nouveau...\n");
  }

}

void send_current_state(server* serv, city* cit, population* migrants) {
  int buffer[BUFSIZE];
  int i, n = 0;
  for (i = 0; i < NB_STRATEGY; i++) {
    buffer[n++] = cit->pop->proportions[i];
  }
  buffer[n++] = cit->pop->nb_entity;
  for (i = 0; i < NB_STRATEGY; i++) {
    buffer[n++] = migrants->proportions[i];
  }
  buffer[n++] = migrants->nb_entity;

  buffer[n] = END_OF_BUFFER;

  send(serv->socket, (char*) buffer, BUFSIZE * sizeof(int), 0);
}

population* receive_emigrants(server* serv) {
  population* emigrants = create_population(0, NULL);

  int buffer[BUFSIZE];
  recv(serv->socket, buffer, BUFSIZE * sizeof(int), 0);

  int i;
  for (i = 0; i < NB_STRATEGY; i++) emigrants->proportions[i] = buffer[i];
  emigrants->nb_entity = buffer[i];

  return emigrants;
}

void integrate_emigrants(city* cit, population* emigrants) {
  int i;
  for (i = 0; i < NB_STRATEGY; i++) {
    cit->pop->proportions[i] += emigrants->proportions[i];
  }
  cit->pop->nb_entity += emigrants->nb_entity;
}

void run_client() {
  server* serv = connect_to_server("hostname", PORT);

  bool allowed_strategies[NB_STRATEGY];
  int i;
  for (i = 0; i < NB_STRATEGY; i++) allowed_strategies[i] = true;
  city_parameters* parameters = create_city_parameters(5, 0, 3, 1, allowed_strategies);

  city* cit = create_city(1000, parameters);

  population* emigrants;
  while (true) {
    send_current_state(serv, cit, NULL); /* !!! SELECT PEOPLE TO SEND !!! */

    emigrants = receive_emigrants(serv);

    integrate_emigrants(cit, emigrants);

    simulate_one_generation(cit);
  }
}
