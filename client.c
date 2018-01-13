
#include "client.c"

server* create_server(int socket) {
  server* serv = malloc(sizeof(server));
  serv->fd = socket;
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
