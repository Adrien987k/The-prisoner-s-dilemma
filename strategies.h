
#ifndef STRATEGIES_H
#define STRATEGIES_H

#include "city.h"

#define BETRAY 0
#define COOPERATE 1

typedef enum {
  GENTILLE,
  MECHANTE,
  DONNANT_DONNANT,
  RANCUNIERE,
  PERIODIQUE_MECHANTE,
  PERIODIQUE_GENTILLE,
  MAJORITE_MOU,
  MEFIANTE,
  MAJORITE_DUR,
  SONDEUR,
  DONNANT_DONNANT_DUR
} strategy;

typedef struct {
  int* player_0;
  int* player_1;
  bool player_0_betrayed;
  bool player_1_betrayed;
} previous_plays;

typedef struct {
  int winner;
  int score_player_0;
} result_of_fight;


strategy* get_strategies_array(city_parameters* parameters);
int majority(int plays[], int n, int mou);
int play(strategy strat, int player, int turn, previous_plays* prev);
result_of_fight* fight(strategy strat1, strategy strat2, int nb_turn, city_parameters* parameters);
void fight_all_against_all(int nb_turn, city_parameters* parameters);
int score_against_all(strategy strat, int nb_turn, city_parameters* parameters);
void all_score_against_all(int nb_turn, city_parameters* parameters);

#endif
