
#ifndef SIMULATION_H
#define SIMULATION_H

#include "constant.h"
#include "utils.h"
#include "city.h"
#include "strategies.h"

population* create_empty_population();
population* create_population(int nb_entities_per_strat[], city_parameters* parameters);
void simulate_one_generation(city* cit);
void simulate_population(int max_generation, int nb_entity_per_strat[], city_parameters* city_parameters);

city_parameters* create_city_parameters(int T, int D, int C, int P, bool allowed_strategies[], int nb_turn);
city* create_city(int nb_entities_per_strat[], city_parameters* parameters);
void destroy_city(city* cit);

#endif
