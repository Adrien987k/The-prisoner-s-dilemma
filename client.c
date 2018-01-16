
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
  printf("Connexion au serveur à %s:%d ...\n",hostname, port);
  while (1) {
    if (connect(s,(struct sockaddr *)&server_address, sizeof server_address) == 0) {
      printf("Connecté !\n");
      return create_server(s);
    }
    sleep(3);
    printf("J'essaie à nouveau...\n");
  }

}

void wait_for_confirmation(server* serv) {
  int buffer[BUFSIZE];
  recv(serv->socket, buffer, BUFSIZE * sizeof(int), 0);
}

int* get_num_strategies_allowed(population* pop){
  int i,j;
  int nb_strategies;
  nb_strategies = 1;
  for (i = 0; i < NB_STRATEGY; i++) {
    if (pop->proportions[i] > 0)nb_strategies++;
  }
  int* num_strategies_allowed = malloc(nb_strategies * sizeof(int));
  num_strategies_allowed[0]=nb_strategies;
  j = 1;
  for (i = 0; i < NB_STRATEGY; i++) {
    if (pop->proportions[i] > 0) {
      num_strategies_allowed[j]=i;
      j++;
    }
  }
  return num_strategies_allowed;
}

population* select_migrants(population* pop){
  population* pop_migrants = malloc(sizeof(population));
  int i;
  int max_migrants;
  for (i = 0; i < NB_STRATEGY; i++) {
    pop_migrants->proportions[i] = 0;
  }
  max_migrants = pop->nb_entity / 20;
  int r = my_randint(max_migrants+1);
  pop_migrants->nb_entity = r;
  pop->nb_entity -= r;
  for (i = 0; i < r; i++) {
    int* num_strategies_allowed = get_num_strategies_allowed(pop);
    int j = my_randint(num_strategies_allowed[0]-1) + 1;
    pop_migrants->proportions[num_strategies_allowed[j]]++;
    pop->proportions[num_strategies_allowed[j]]--;
    free(num_strategies_allowed);
  }
  return pop_migrants;
}

void send_current_state(server* serv, city* cit, population* migrants) {
  int buffer[BUFSIZE];
  int i, n = 0;

  buffer[n++] = cit->parameters->T;
  buffer[n++] = cit->parameters->D;
  buffer[n++] = cit->parameters->C;
  buffer[n++] = cit->parameters->P;

  for (i = 0; i < NB_STRATEGY; i++) buffer[n++] = cit->parameters->allowed_strategies[i];

  for (i = 0; i < NB_STRATEGY; i++)
    buffer[n++] = cit->pop->proportions[i];
  buffer[n++] = cit->pop->nb_entity;

  for (i = 0; i < NB_STRATEGY; i++)
    buffer[n++] = migrants->proportions[i];
  buffer[n++] = migrants->nb_entity;

  for (i = 0; i < NB_STRATEGY; i++)
    buffer[n++] = cit->scores[i];
  buffer[n++] = cit->total_score;

  buffer[n] = END_OF_BUFFER;

  send(serv->socket, (char*) buffer, BUFSIZE * sizeof(int), 0);
}

population* receive_emigrants(server* serv) {
  population* emigrants = create_empty_population();

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

void run_client(int port, char* hostname) {
  server* serv = connect_to_server(hostname, (port == 0) ? PORT : port);

  bool allowed_strategies[NB_STRATEGY];
  int i;
  for (i = 0; i < NB_STRATEGY; i++) allowed_strategies[i] = true;

  city_parameters* parameters = create_city_parameters(5, 0, 3, 1, allowed_strategies);
  city* cit = create_city(1000, parameters);

  wait_for_confirmation(serv);

  population* emigrants;
  population* migrants;
  while (true) {
    migrants = select_migrants(cit->pop);
    send_current_state(serv, cit, migrants);
    free(migrants);

    emigrants = receive_emigrants(serv);
    integrate_emigrants(cit, emigrants);
    free(emigrants);

    simulate_one_generation(cit);

  }
}
