//ENG_20170104_001 : changement de l en k
//ENG_20170104_002 : changement de j en k
//ENG_20170104_003 : modif de sum
//ENG_20170104_004 : changement de nom de population en pop (confusion entre la var et le type)
//ENG_20170104_005 : gestion des restes pour la proportion
//ENG_20170104_006 : changement de 1 en 100 (nb de tours pour fight) (finalement 10 parce que 100 c'est trop long)


#include "simulation.h"

population* create_population(int entity_per_strat) {
  population* pop = malloc(sizeof(population));    //ENG_20170104_004
  pop->nb_entity = NB_STRATEGY * entity_per_strat;
  pop->generation = 0;
  int i;
  for (i = 0; i < NB_STRATEGY; i++) {
    pop->proportions[i] = entity_per_strat;
  }
  return pop;
}

population* simulate_one_generation(population* pop) {
  int scores[NB_STRATEGY];
  double restes[NB_STRATEGY];    //ENG_20170104_005
  int i, j, k, l;
  strategy* strategies = get_strategies_array();
  result_of_fight* result;

  for (i = 0; i < NB_STRATEGY; i++) scores[i] = 0;

  for (i = 0; i < NB_STRATEGY; i++) {
    for (j = 0; j < pop->proportions[i]; j++) {
      for (k = 0; k < NB_STRATEGY; k++) {
        for (l = (i == k ? 1 : 0); l < pop->proportions[k]; l++) {   //ENG_20170104_001
          result = fight(strategies[i], strategies[k], 10);    //ENG_20170104_002 ENG_20170104_006
          scores[i] += result->score_player_0;
        }
      }
    }
  }

  double sum;     //ENG_20170104_003
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
    double_pop = (sum != 0 ? pop->nb_entity * (scores[i] / sum) : 0);
    int_pop = double_pop;
    total_pop += int_pop;
    restes[i] = double_pop - int_pop;
    pop->proportions[i] = int_pop;
  }
  reste = pop->nb_entity - total_pop;
  for (i = 0; i < reste; i++) {
    max_reste = 0;
    index_max = 0;
    for (j = 0; j <NB_STRATEGY; j++) {
      if (restes[j]>max_reste) {
        max_reste = restes[j];
        index_max = j;
      }
    }
    pop->proportions[index_max]++;
    restes[index_max] = 0;
  }
  pop->generation++;

  free(strategies);

  return pop;
}

void simulate_population(int max_generation, int entity_per_strat) {
  population* pop = create_population(entity_per_strat);
  int i;
  for (i = 0; i < max_generation; i++) {
    pop = simulate_one_generation(pop);
  }
  for (i = 0; i <NB_STRATEGY; i++) {
    printf("%d\n",pop->proportions[i]);
  }
  free(pop);
}
