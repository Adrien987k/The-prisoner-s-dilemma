
#include "client.h"

#define DEFAULT_INET_ADDR ("127.0.0.1")

/* port = 0 for default port */

int main(int argc, char* argv[]) {
  if (argc < 2 || argc > 3) {
    printf("usage: lauch_client <port> [<inet_addr>]\n");
    return EXIT_FAILURE;
  }
  if (argc == 2) run_client(atoi(argv[1]), DEFAULT_INET_ADDR);
  else if (argc == 3) run_client(atoi(argv[1]), argv[2]);

  return EXIT_SUCCESS;
}
