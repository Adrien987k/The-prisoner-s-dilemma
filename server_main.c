
#include "server.h"

#define DEFAULT_INET_ADDR ("127.0.0.1")

int main(int argc, char* argv[]) {
  srand(time(NULL));

  if (argc > 2) {
    printf("usage: lauch_server <inet_addr>\n");
    return EXIT_FAILURE;
  }
  printf ("DEBUG %s\n", argv[1]);
  if (argc == 2) run_server(argv[1]);
  else run_server(DEFAULT_INET_ADDR);
}
