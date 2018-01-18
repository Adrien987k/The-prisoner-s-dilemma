
#ifndef SIMULATION_H
#define SIMULATION_H

#include "constant.h"
#include "utils.h"
#include "city.h"
#include "strategies.h"

/* return an empty population */
population* create_empty_population();

/* return a population with a given number of entities per strategy */
population* create_population(int nb_entities_per_strat[]);

/* change the population of the city cit after one generation using the parameters of the city */
void simulate_one_generation(city* cit);

/* displays the results table for max_generation generations of a city with a given initial population and parameters */
void simulate_population(int max_generation, int nb_entity_per_strat[], city_parameters* city_parameters);

/* create a city_parameters with the parameters given */
city_parameters* create_city_parameters(int T, int D, int C, int P, bool allowed_strategies[], int nb_turn);

/* create a city with the given parameters */
city* create_city(int nb_entities_per_strat[], city_parameters* parameters);

/* free the memory allocated for the city */
void destroy_city(city* cit);

#endif
