#include "simulation.h"
#include "strategies.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("usage: Displays the confrontation table and the accumulated points table for n rounds\n");
    return EXIT_FAILURE;
  }
  if (argc == 2) {

    bool allowed_strategies[NB_STRATEGY];
    int i;
    int nb_turn;
    nb_turn = atoi(argv[1]);
    for (i = 0; i < NB_STRATEGY; i++) allowed_strategies[i] = true;

    city_parameters* parameters = create_city_parameters(5, 0, 3, 1, allowed_strategies, 10);

    fight_all_against_all(nb_turn, parameters);

    all_score_against_all(nb_turn, parameters);

    return EXIT_SUCCESS;
  }
}
