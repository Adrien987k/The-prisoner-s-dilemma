
#include "server.h"

client* create_client(int socket) {
  client* clt = malloc(sizeof(client));
  clt->socket = socket;
  return clt;
}

client_pool* create_empty_client_pool() {
  client_pool* clients = malloc(sizeof(client_pool));
  clients->nb_client = 0;
  return clients;
}

void add_client(client_pool* clients, client* client) {
  clients->nb_client++;
  clients->clients[clients->nb_client - 1] = *client;
}

client_pool* wait_for_clients(int port, char* addr) {
  struct sockaddr_in server_address;

  // créer un socket TCP
  int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  // connecter le socker au port choisi
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = inet_addr(addr);
  server_address.sin_port = htons(port);

  if (bind(s,(struct sockaddr *)&server_address,sizeof server_address)) {
    fprintf(stderr,"Erreur : bind échoue, " "port %d encore occupé ?\n", port);
    exit(1);
  }

  // mettre le socket en mode écoute
  listen(s, 1);

  client_pool* client_pool = create_empty_client_pool();

  printf("J'attends une connexion sur le port %d ...\n",port);
  while (client_pool->nb_client < NB_CLIENT) {
     // laisser un client se connecter
     int client_socket = accept(s, NULL, NULL);

     if (client_socket < 0) {
       if (errno == EINTR || errno == EWOULDBLOCK) continue;
       fprintf(stderr,"Erreur : connexion client échouée\n");
       exit(1);
     }

     printf("Connexion acceptée !\n");

     add_client(client_pool, create_client(client_socket));
  }

  return client_pool;
}

void send_confirmation(client_pool* client_pool) {
  int buffer[BUFSIZE];
  buffer[0] = 0;
  int i;
  for (i = 0; i < NB_CLIENT; i++) {
    send(client_pool->clients[i].socket, buffer, BUFSIZE * sizeof(int), 0);
  }
}

void get_current_state_and_migrants(int socket, city* cit, city* migrants_city) {
  int buffer[BUFSIZE];

  recv(socket, buffer, BUFSIZE * sizeof(int), 0);

  int i, n = 0;

  int T = buffer[n++];
  int D = buffer[n++];
  int C = buffer[n++];
  int P = buffer[n++];
  int nb_turn_per_fight = buffer[n++];

  bool allowed_strategies[NB_STRATEGY];
  for (i = 0; i < NB_STRATEGY; i++) allowed_strategies[i] = buffer[n++];

  city_parameters* city_parameters = create_city_parameters(T, D, C, P, allowed_strategies, nb_turn_per_fight);
  *cit = *create_city(0, city_parameters);
  *migrants_city = *create_city(0, city_parameters);

  for (i = 0; i < NB_STRATEGY; i++) cit->pop->proportions[i] = buffer[n++];
  cit->pop->nb_entity = buffer[n++];

  for (i = 0; i < NB_STRATEGY; i++) migrants_city->pop->proportions[i] = buffer[n++];
  migrants_city->pop->nb_entity = buffer[n++];

  for (i = 0; i < NB_STRATEGY; i++) cit->scores[i] = buffer[n++];
  cit->total_score = buffer[n++];
}

void display_current_cities_states(city cities[]) {
  int i, j;
  for (i = 0; i < NB_CLIENT; i++) {
    printf("========== City N° %d ==========\n", i);
    printf("T = %d, D = %d, C = %d, P = %d\n",
           cities[i].parameters->T, cities[i].parameters->D,
           cities[i].parameters->C, cities[i].parameters->P);
    printf("Total score this generation = %d\n", cities[i].total_score);
    printf("Strategies allowed : \n");
    for (j = 0; j < NB_STRATEGY; j++) printf(" %d ", j);
    printf("\n");
    for (j = 0; j < NB_STRATEGY; j++)
      printf(" %d ", cities[i].parameters->allowed_strategies[j] ? 1 : 0);
    printf("\n");

    for (j = 0; j < NB_STRATEGY; j++) printf("|  %d  ", j);
    printf("|\n");
    for (j = 0; j < NB_STRATEGY; j++) printf("| %d  ", cities[i].pop->proportions[j]);
    printf("|\n");
    for (j = 0; j < NB_STRATEGY; j++) printf("| %d  ", cities[i].scores[j]);
    printf("|\n");

    printf("================================\n\n");
  }
}

int* get_num_cities_allowed(int strategy_num, city cities[]){
  int i,j;
  int nb_cities = 1;
  for (i = 0; i < NB_CLIENT; i++) {
    if (cities[i].parameters->allowed_strategies[strategy_num]) nb_cities++;
  }
  int* num_cities_allowed = malloc(nb_cities * sizeof(int));
  num_cities_allowed[0] = nb_cities;
  j = 1;
  for (i = 0; i < NB_CLIENT; i++) {
    if (cities[i].parameters->allowed_strategies[strategy_num]) {
      num_cities_allowed[j]=i;
      j++;
    }
  }
  return num_cities_allowed;
}

city* dispatch_migrants(city migrant_cities[]){
  population* pop_server = malloc(sizeof(population));
  pop_server->nb_entity = 0;
  int i,j;
  for (i = 0; i < NB_STRATEGY; i++) {
    pop_server->proportions[i] = 0;
  }
  for (i = 0; i < NB_CLIENT; i++) {
    pop_server->nb_entity += migrant_cities[i].pop->nb_entity;
    migrant_cities[i].pop->nb_entity = 0;
    for (j = 0; j < NB_STRATEGY; j++) {
      pop_server->proportions[j] += migrant_cities[i].pop->proportions[j];
      migrant_cities[i].pop->proportions[j] = 0;
    }
  }
  for (i = 0; i < NB_STRATEGY; i++) {
    int* num_cities_allowed = get_num_cities_allowed(i, migrant_cities);
    for (j = 0; j < pop_server->proportions[i]; j++) {
      int k = my_randint(num_cities_allowed[0]-1) + 1;
      migrant_cities[num_cities_allowed[k]].pop->proportions[i]++;
      migrant_cities[num_cities_allowed[k]].pop->nb_entity++;
    }
    free(num_cities_allowed);
  }
  free(pop_server);
  return migrant_cities;
}

void send_emigrants(client_pool* client_pool, city emigrant_cities[]) {
  int i, j;
  for (i = 0; i < NB_CLIENT; i++) {

    int buffer[BUFSIZE];
    for (j = 0; j < NB_STRATEGY; j++) {
      buffer[j] = emigrant_cities[i].pop->proportions[j];
    }
    buffer[j] = emigrant_cities[i].pop->nb_entity;

    send(client_pool->clients[i].socket, (char*) buffer, BUFSIZE * sizeof(int), 0);
  }
}

void run_server(char* addr) {
  client_pool* client_pool = wait_for_clients(PORT, addr); /* !!! Change inet addr !!! */

  struct pollfd fds[NB_CLIENT];
  int i;
  for (i = 0; i < NB_CLIENT; i++) {
    fds[i].fd = client_pool->clients[i].socket;
    fds[i].events = POLLIN;
  }

  int nb_client_ok;
  bool clients_ok[NB_CLIENT];

  send_confirmation(client_pool);

  city cities[NB_CLIENT];
  city migrants_cities[NB_CLIENT];
  city* emigrants_cities;
  int generation = 0;
  bool running = true;
  int last_time = get_current_time();
  while (running) {
    for (i = 0; i < NB_CLIENT; i++) clients_ok[i] = 0;

    nb_client_ok = 0;
    while (true) {
      if (get_current_time() - last_time > DELAY_WAITING_CLIENT) {
        printf ("No answer of one or more client since %d seconds. server stop running\n"
                , DELAY_WAITING_CLIENT);
        running = false;
        break;
      }

      if (poll(fds, NB_CLIENT, -1) < 0)
        fprintf(stderr, "Error poll: %d", errno);

      for (i = 0; i < NB_CLIENT; i++) {
        if (fds[i].revents == POLLIN) {
          if (!clients_ok[i]) {
            clients_ok[i] = true;
            get_current_state_and_migrants(fds[i].fd, &cities[i], &migrants_cities[i]);
            nb_client_ok++;
          }
        }
      }

      if (nb_client_ok == NB_CLIENT) {
        last_time = get_current_time();

        for (i = 0; i < 50; i++) printf("\n");
        printf("============= GENERATION %d =============\n", generation++);

        display_current_cities_states(cities);

        emigrants_cities = dispatch_migrants(migrants_cities);

        send_emigrants(client_pool, emigrants_cities);

        sleep(1);
        break;
      }

    }

  }

}
