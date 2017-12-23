
#include "simulation.h"

population* create_population(int entity_per_strat) {
  population* population = malloc(sizeof(population));
  population->nb_entity = NB_STRATEGY * entity_per_strat;
  population->generation = 0;
  int i;
  for (i = 0; i < NB_STRATEGY; i++) {
    population->proportions[i] = entity_per_strat;
  }
  return population;
}

population* simulate_one_generation(population* population) {
  int scores[NB_STRATEGY];
  int i, j, k, l;
  strategy* strategies = get_strategies_array();
  result_of_fight* result;

  for (i = 0; i < NB_STRATEGY; i++) scores[i] = 0;

  for (i = 0; i < NB_STRATEGY; i++) {
    for (j = 0; j < population->proportions[i]; j++) {
      for (k = 0; k < NB_STRATEGY; k++) {
        for (l = (i == k ? 1 : 0); l < population->proportions[l]; l++) {
          result = fight(strategies[i], strategies[j], 1);
          scores[i] += result->score_player_0;
        }
      }
    }
  }

  int sum;
  for (i = 0; i < NB_STRATEGY; i++) {
    sum = 0;
    for (j = 0; j < NB_STRATEGY; j++) {
      if (i != j) sum += scores[j];
    }
    population->proportions[i] = sum != 0 ? ceil(population->nb_entity * (scores[i] / sum))
                                          : 0;
  }
  population->generation++;

  free(strategies);

  return population;
}

void simulate_population(int max_generation, int entity_per_strat) {
  population* population = create_population(entity_per_strat);
  int i;
  for (i = 0; i < max_generation; i++) {
    population = simulate_one_generation(population);
  }
  free(population);
}
