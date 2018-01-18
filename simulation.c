
#include "simulation.h"

population* create_empty_population() {
  population* pop = malloc(sizeof(population));
  pop->nb_entity = 0;
  pop->generation = 0;
  return pop;
}

population* create_population(int nb_entities_per_strat[], city_parameters* parameters) {
  population* pop = malloc(sizeof(population));
  int i;
  pop->nb_entity = 0;
  pop->generation = 0;
  for (i = 0; i < NB_STRATEGY; i++) {
    pop->nb_entity += nb_entities_per_strat[i];
    pop->proportions[i] = nb_entities_per_strat[i];
  }
  return pop;
}

void simulate_one_generation(city* cit) {
  int nb_strat = cit->parameters->nb_allowed_strategies;
  int* scores = malloc(nb_strat * sizeof(int));
  double* restes = malloc(nb_strat * sizeof(double));
  int i, j, k, l;
  strategy* strategies = get_strategies_array(cit->parameters);
  result_of_fight* result;

  for (i = 0; i < nb_strat; i++) scores[i] = 0;

  for (i = 0; i < nb_strat; i++) {
    for (j = 0; j < cit->pop->proportions[i]; j++) {
      for (k = 0; k < nb_strat; k++) {
        for (l = (i == k ? 1 : 0); l < cit->pop->proportions[k]; l++) {
          result = fight(strategies[i], strategies[k], cit->parameters->nb_turn_per_fight, cit->parameters);
          scores[i] += result->score_player_0;
          free(result);
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
    double_pop = (sum != 0 ? cit->pop->nb_entity * (scores[i] / (sum == 0 ? 1 : sum)) : 0);
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

  int n = 0;
  cit->total_score = 0;
  for (i = 0; i < NB_STRATEGY; i++) {
    if (!cit->parameters->allowed_strategies[i]) {
      cit->scores[i] = 0;
    } else {
      cit->scores[i] = scores[n];
      cit->total_score += scores[n++];
    }
  }

  free(scores);
  free(restes);
  free(strategies);
}

void simulate_population(int max_generation, int nb_entity_per_strat[], city_parameters* parameters) {

  city* cit = create_city(nb_entity_per_strat, parameters);

  int i,j;
  printf("           ");
  for (j = 0; j <NB_STRATEGY; j++) printf("%d       ", j);
  printf("\n0    - ");
  for (j = 0; j <NB_STRATEGY; j++) {
    if (cit->pop->proportions[j]<10)printf("|   %d   ", cit->pop->proportions[j]);
    else if (cit->pop->proportions[j]<100)printf("|   %d  ", cit->pop->proportions[j]);
    else if (cit->pop->proportions[j]<1000)printf("|  %d  ", cit->pop->proportions[j]);
    else if (cit->pop->proportions[j]<10000)printf("|  %d ", cit->pop->proportions[j]);
    else printf("| %d ", cit->pop->proportions[j]);
  }
  printf("|\n");
  for (i = 1; i < max_generation+1; i++){
    simulate_one_generation(cit);
    if (i<10)printf("%d    - ", i);
    else if (i<100)printf("%d   - ", i);
    else if (i<1000)printf("%d  - ", i);
    else printf("%d - ", i);
    for (j = 0; j <NB_STRATEGY; j++){
      if (cit->pop->proportions[j]<10)printf("|   %d   ", cit->pop->proportions[j]);
      else if (cit->pop->proportions[j]<100)printf("|   %d  ", cit->pop->proportions[j]);
      else if (cit->pop->proportions[j]<1000)printf("|  %d  ", cit->pop->proportions[j]);
      else if (cit->pop->proportions[j]<10000)printf("|  %d ", cit->pop->proportions[j]);
      else printf("| %d ", cit->pop->proportions[j]);
    }
    printf("|\n");
  }

  destroy_city(cit);
}

city_parameters* create_city_parameters(int T, int D, int C, int P, bool allowed_strategies[], int nb_turn) {
  city_parameters* parameters = malloc(sizeof(city_parameters));
  parameters->T = T;
  parameters->D = D;
  parameters->C = C;
  parameters->P = P;
  parameters->nb_turn_per_fight = nb_turn;

  int i;
  for (i = 0; i < NB_STRATEGY; i++) {
    parameters->allowed_strategies[i] = allowed_strategies[i];
    if (allowed_strategies[i]) parameters->nb_allowed_strategies++;
  }

  return parameters;
}

city* create_city(int nb_entities_per_strat[], city_parameters* parameters) {
  city* cit = malloc(sizeof(city));
  cit->pop = create_population(nb_entities_per_strat, parameters);

  cit->parameters = malloc(sizeof(city_parameters));
  cit->parameters = parameters;

  int i;
  for (i = 0; i < NB_STRATEGY; i++) cit->scores[i] = 0;
  cit->total_score = 0;

  return cit;
}

void destroy_city(city* cit) {
  free(cit->pop);
  free(cit->parameters);
  free(cit);
}

