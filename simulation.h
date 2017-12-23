
#ifndef SIMULATION_H
#define SIMULATION_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "strategies.h"

typedef struct {
  int proportions[NB_STRATEGY];
  int nb_entity;
  int generation;
} population;

population* create_population(int entity_per_strat);
population* simulate_one_generation(population* population);
void simulate_population(int max_generation, int entity_per_strat);

#endif
