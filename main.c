#include "simulation.h"
#include "strategies.h"

int main () {
  city_parameters* parameters = create_city_parameters(5, 0, 3, 1);

  fight_all_against_all(100, parameters);

  all_score_against_all(100, parameters);

  simulate_population(30, 100, parameters);

  return EXIT_SUCCESS;
}
