#include "simulation.h"
#include "strategies.h"


int main(int argc, char* argv[]) {
  if (argc != 4) {
    printf("usage: Displays the results table for n1 generations, n2 entities per strategy and n3 rounds per fight\n");
    return EXIT_FAILURE;
  }
  if (argc == 4) {

    bool allowed_strategies[NB_STRATEGY];
    int i;
    int nb_generation;
    int nb_entity_per_strat;
    int nb_turn_per_fight;
    int nb_entities[NB_STRATEGY];

    nb_generation = atoi(argv[1]);
    nb_entity_per_strat = atoi(argv[2]);
    nb_turn_per_fight = atoi(argv[3]);

    for (i = 0; i < NB_STRATEGY; i++) {
      allowed_strategies[i] = true;
      nb_entities[i] = nb_entity_per_strat;
    }

    city_parameters* parameters = create_city_parameters(5, 0, 3, 1, allowed_strategies, nb_turn_per_fight);


    simulate_population(nb_generation, nb_entities, parameters);

    return EXIT_SUCCESS;
  }
}
