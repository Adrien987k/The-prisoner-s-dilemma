#ifndef CITY_H
#define CITY_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "constant.h"

typedef struct {
  int proportions[NB_STRATEGY];
  int nb_entity;
  int generation;
} population;

typedef struct {
  int T;
  int D;
  int C;
  int P;
  bool allowed_strategies[NB_STRATEGY];
  int nb_allowed_strategies;
} city_parameters;

typedef struct {
  population* pop;
  city_parameters* parameters;
} city;

#endif