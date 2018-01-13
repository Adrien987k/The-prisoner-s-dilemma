#include "simulation.h"
#include "strategies.h"

int main () {
  bool allowed_strategies[NB_STRATEGY];
  int i;
  for (i = 0; i < NB_STRATEGY; i++) allowed_strategies[i] = true;

  city_parameters* parameters = create_city_parameters(5, 0, 3, 1, allowed_strategies);

  fight_all_against_all(100, parameters);

  all_score_against_all(100, parameters);

  simulate_population(1000, 1100, parameters);

  return EXIT_SUCCESS;
}
