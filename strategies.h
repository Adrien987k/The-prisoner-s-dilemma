#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define T 5
#define D 0
#define C 3
#define P 1

#define BETRAY 0
#define COOPERATE 1

#define NB_STRATEGY 11

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
  int score_winner;
  int score_looser;
  int score_player_0;
  int score_player_1;
} result_of_fight;

strategy* get_strategies_array();
int majority(int plays[], int n, int mou);
int play(strategy strat, int player, int turn, previous_plays* prev);
result_of_fight* fight(strategy strat1, strategy strat2, int n);
void fight_all_against_all(int n);
int score_against_all(strategy strat, int n);