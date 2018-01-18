#include "simulation.h"
#include "strategies.h"

int main () {
  srand(time(NULL));

  bool allowed_strategies[NB_STRATEGY];
  int entity_per_strat[NB_STRATEGY];
  int i;
  for (i = 0; i < NB_STRATEGY; i++) {
    allowed_strategies[i] = true;
    entity_per_strat[i] = 100;
  }

  city_parameters* parameters = create_city_parameters(5, 0, 3, 1, allowed_strategies, 10);

  fight_all_against_all(100, parameters);

  all_score_against_all(100, parameters);

  simulate_population(30, entity_per_strat, parameters);

  return EXIT_SUCCESS;
}
