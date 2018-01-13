
#include "simulation.h"

population* create_population(int nb_entity, bool allowed_strategies[]) {
  population* pop = malloc(sizeof(population));
  pop->nb_entity = nb_entity;
  pop->generation = 0;
  int entity_per_strat = 0;/* !!! repartition !!! */
  int i;
  for (i = 0; i < NB_STRATEGY; i++) {
    pop->proportions[i] = entity_per_strat;
  }
  return pop;
}

void simulate_one_generation(city* cit) {
  int nb_strat = cit->parameters->nb_allowed_strategies;
  int* scores = malloc(nb_strat * sizeof(int));
  double* restes = malloc(nb_strat * sizeof(int));
  int i, j, k, l;
  strategy* strategies = get_strategies_array(cit->parameters);
  result_of_fight* result;

  for (i = 0; i < nb_strat; i++) scores[i] = 0;

  for (i = 0; i < nb_strat; i++) {
    for (j = 0; j < cit->pop->proportions[i]; j++) {
      for (k = 0; k < nb_strat; k++) {
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
  for (i = 0; i < nb_strat; i++) {
    sum += scores[i];
  }
  for (i = 0; i < nb_strat; i++) {
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
    for (j = 0; j < nb_strat; j++) {
      if (restes[j]>max_reste) {
        max_reste = restes[j];
        index_max = j;
      }
    }
    cit->pop->proportions[index_max]++;
    restes[index_max] = 0;
  }
  cit->pop->generation++;

  free(scores);
  free(restes);
  free(strategies);
}

void simulate_population(int max_generation, int nb_entity, city_parameters* parameters) {

  city* cit = create_city(nb_entity, parameters);

  int i;
  for (i = 0; i < max_generation; i++)
    simulate_one_generation(cit);

  for (i = 0; i <NB_STRATEGY; i++)
    printf("%d\n", cit->pop->proportions[i]);

  destroy_city(cit);
}

city_parameters* create_city_parameters(int T, int D, int C, int P, bool allowed_strategies[]) {
  city_parameters* parameters = malloc(sizeof(city_parameters));
  parameters->T = T;
  parameters->D = D;
  parameters->C = C;
  parameters->P = P;

  int i;
  for (i = 0; i < NB_STRATEGY; i++) {
    parameters->allowed_strategies[i] = allowed_strategies[i];
    if (allowed_strategies[i]) parameters->nb_allowed_strategies;
  }

  return parameters;
}

city* create_city(int nb_entity, city_parameters* parameters) {
  city* cit = malloc(sizeof(city));
  cit->pop = create_population(nb_entity, parameters->allowed_strategies);

  cit->parameters = malloc(sizeof(city_parameters));
  cit->parameters = parameters;

  return cit;
}

void destroy_city(city* cit) {
  free(cit->pop);
  free(cit->parameters);
  free(cit);
}

