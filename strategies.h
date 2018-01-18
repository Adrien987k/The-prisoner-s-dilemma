
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

/* return an array that contains all the strategies */
strategy* get_strategies_array(city_parameters* parameters);

/* return BETRAY or COOPERATE according to the previous plays and the strategy */
int majority(int plays[], int n, int mou);

/* return BETRAY or COOPERATE according to the strategy of the player and the previous plays */
int play(strategy strat, int player, int turn, previous_plays* prev);

/* return a result_of_fight that contains the winner (0 or 1) and the score of player 0 */
result_of_fight* fight(strategy strat1, strategy strat2, int nb_turn, city_parameters* parameters);

/* displays the confrontation table for nb_turn rounds*/
void fight_all_against_all(int nb_turn, city_parameters* parameters);

/* return the score of the strategy strat after fighting all the other strategies for nb_turn rounds */
int score_against_all(strategy strat, int nb_turn, city_parameters* parameters);

/* displays the accumulated points table for nb_turn rounds */
void all_score_against_all(int nb_turn, city_parameters* parameters);

#endif
