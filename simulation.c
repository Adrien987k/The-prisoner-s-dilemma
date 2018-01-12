
#include "simulation.h"

population* create_population(int entity_per_strat) {
  population* pop = malloc(sizeof(population));
  pop->nb_entity = NB_STRATEGY * entity_per_strat;
  pop->generation = 0;
  int i;
  for (i = 0; i < NB_STRATEGY; i++) {
    pop->proportions[i] = entity_per_strat;
  }
  return pop;
}

void simulate_one_generation(city* cit) {
  int scores[NB_STRATEGY];
  double restes[NB_STRATEGY];
  int i, j, k, l;
  strategy* strategies = get_strategies_array();
  result_of_fight* result;

  for (i = 0; i < NB_STRATEGY; i++) scores[i] = 0;

  for (i = 0; i < NB_STRATEGY; i++) {
    for (j = 0; j < cit->pop->proportions[i]; j++) {
      for (k = 0; k < NB_STRATEGY; k++) {
        for (l = (i == k ? 1 : 0); l < cit->pop->proportions[k]; l++) {
          result = fight(strategies[i], strategies[k], 10, cit->parameters);
          scores[i] += result->score_player_0;
        }
      }
    }
  }

  double sum;
  int total_pop;
  int reste;
  int max_reste;
  int index_max;
  sum = 0;
  total_pop = 0;
  for (i = 0; i < NB_STRATEGY; i++) {
    sum += scores[i];
  }
  for (i = 0; i < NB_STRATEGY; i++) {
    double double_pop;
    int int_pop;
    double_pop = (sum != 0 ? cit->pop->nb_entity * (scores[i] / sum) : 0);
    int_pop = double_pop;
    total_pop += int_pop;
    restes[i] = double_pop - int_pop;
    cit->pop->proportions[i] = int_pop;
  }
  reste = cit->pop->nb_entity - total_pop;
  for (i = 0; i < reste; i++) {
    max_reste = 0;
    index_max = 0;
    for (j = 0; j <NB_STRATEGY; j++) {
      if (restes[j]>max_reste) {
        max_reste = restes[j];
        index_max = j;
      }
    }
    cit->pop->proportions[index_max]++;
    restes[index_max] = 0;
  }
  cit->pop->generation++;

  free(strategies);
}

void simulate_population(int max_generation, int entity_per_strat, city_parameters* parameters) {
  bool allowed_strategies[NB_STRATEGY];
  int i;
  for (i = 0; i < NB_STRATEGY; i++) allowed_strategies[i] = true;

  city* cit = create_city(entity_per_strat, allowed_strategies, parameters);

  for (i = 0; i < max_generation; i++)
    simulate_one_generation(cit);

  for (i = 0; i <NB_STRATEGY; i++)
    printf("%d\n", cit->pop->proportions[i]);

  destroy_city(cit);
}

city_parameters* create_city_parameters(int T, int D, int C, int P) {
  city_parameters* parameters = malloc(sizeof(city_parameters));
  parameters->T = T;
  parameters->D = D;
  parameters->C = C;
  parameters->P = P;

  return parameters;
}

city* create_city(int entity_per_strat, bool allowed_strategies[], city_parameters* parameters) {
  city* cit = malloc(sizeof(city));
  cit->pop = create_population(entity_per_strat);

  int i;
  for (i = 0; i < NB_STRATEGY; i++) cit->allowed_strategies[i] = allowed_strategies[i];

  cit->parameters = malloc(sizeof(city_parameters));
  cit->parameters = parameters;

  return cit;
}

void destroy_city(city* cit) {
  free(cit->pop);
  free(cit->parameters);
  free(cit);
}

