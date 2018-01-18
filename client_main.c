
#include "client.h"

/* port = 0 for default port */

int main(int argc, char* argv[]) {
  srand(time(NULL));

  if (argc < 2 || argc > 3) {
    printf("usage: lauch_client <inet_addr> [<port>]\n");
    return EXIT_FAILURE;
  }
  if (argc == 2) run_client(0, argv[1]);
  else if (argc == 3) run_client(atoi(argv[2]), argv[1]);

  return EXIT_SUCCESS;
}
