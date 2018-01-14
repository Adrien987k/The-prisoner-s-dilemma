
#include "utils.h"

int my_randint(int n) {
  if ((n - 1) == RAND_MAX) {
    return rand();
  } else {
    long end = RAND_MAX / n;
    end *= n;
    int r;
    while ((r = rand()) >= end);

    return r % n;
  }
}
