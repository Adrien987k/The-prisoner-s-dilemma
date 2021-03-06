
#include "strategies.h"

strategy* get_all_strategies_array() {
  strategy* strategies = malloc(NB_STRATEGY * sizeof(strategy));
  strategies[0] = GENTILLE;
  strategies[1] = MECHANTE;
  strategies[2] = DONNANT_DONNANT;
  strategies[3] = RANCUNIERE;
  strategies[4] = PERIODIQUE_MECHANTE;
  strategies[5] = PERIODIQUE_GENTILLE;
  strategies[6] = MAJORITE_MOU;
  strategies[7] = MEFIANTE;
  strategies[8] = MAJORITE_DUR;
  strategies[9] = SONDEUR;
  strategies[10] = DONNANT_DONNANT_DUR;
  return strategies;
}

strategy* get_strategies_array(city_parameters* parameters) {
  int nb_strat = parameters->nb_allowed_strategies;
  strategy* all_strategies = get_all_strategies_array();
  strategy* strategies = malloc(nb_strat * sizeof(strategy));

  int i, j = 0;
  for (i = 0; i < nb_strat; i++) {
    if (parameters->allowed_strategies[i]) strategies[j++] = all_strategies[i];
  }

  free(all_strategies);
  return strategies;
}

int majority(int plays[], int n, int mou) {
  int nb_coop = 0;
  int nb_betray = 0;
  int i;
  for (i = 0; i < n; i++) {
    if (plays[i] == COOPERATE) nb_coop++;
    if (plays[i] == BETRAY) nb_betray++;
  }
  if (mou) return nb_betray > nb_coop ? BETRAY : COOPERATE;
  else return nb_betray >= nb_coop ? BETRAY : COOPERATE;
}

int play(strategy strat, int player, int turn, previous_plays* prev) {
  switch(strat) {
    case GENTILLE: return COOPERATE;

    case MECHANTE: return BETRAY;

    case DONNANT_DONNANT:
      if (turn == 0) return COOPERATE;
      return (player == 0 ? prev->player_1[turn - 1] : prev->player_0[turn - 1]);

    case RANCUNIERE:
      if (turn == 0) return COOPERATE;
      return player == 0 ? (prev->player_1_betrayed ? BETRAY : COOPERATE)
        : (prev->player_0_betrayed ? BETRAY : COOPERATE);

    case PERIODIQUE_MECHANTE:
      if (turn % 3 == 0 || turn % 3 == 1) return BETRAY;
      else return COOPERATE;

    case PERIODIQUE_GENTILLE:
      if (turn % 3 == 0 || turn % 3 == 1) return COOPERATE;
      else return BETRAY;

    case MAJORITE_MOU:
      if (turn == 0) return COOPERATE;
      return player == 0 ? majority(prev->player_1, turn, 1) : majority(prev->player_0, turn, 1);

    case MEFIANTE:
      if (turn == 0) return BETRAY;
      return (player == 0 ? prev->player_1[turn - 1] : prev->player_0[turn - 1]);

    case MAJORITE_DUR:
      if (turn == 0) return BETRAY;
      return player == 0 ? majority(prev->player_1, turn, 0) : majority(prev->player_0, turn, 0);

    case SONDEUR:
      if (turn == 0) return BETRAY;
      if (turn == 1) return COOPERATE;
      if (turn == 2) return COOPERATE;
      if (player == 0 && prev->player_1[1] == COOPERATE && prev->player_1[2] == COOPERATE)
        return BETRAY;
      else if (player == 1 && prev->player_0[1] == COOPERATE && prev->player_0[2] == COOPERATE)
        return BETRAY;
      return play(DONNANT_DONNANT, player, turn, prev);

    case DONNANT_DONNANT_DUR:
      if (turn == 0) return COOPERATE;
      if (turn == 1 && player == 0) return (prev->player_1[0] == COOPERATE ? COOPERATE : BETRAY);
      if (turn == 1 && player == 1) return (prev->player_0[0] == COOPERATE ? COOPERATE : BETRAY);
      if (player == 0) {
        if (prev->player_1[turn - 1] == BETRAY || prev->player_1[turn - 2] == BETRAY) return BETRAY;
        return COOPERATE;
      }
      if (prev->player_0[turn - 1] == BETRAY || prev->player_0[turn - 2] == BETRAY) return BETRAY;
      return COOPERATE;
  }

  return -1;
}


result_of_fight* fight(strategy strat1, strategy strat2, int nb_turn, city_parameters* parameters) {
  int i;
  int score_player_0 = 0;
  int score_player_1 = 0;
  previous_plays* prev = malloc(sizeof(previous_plays));
  prev->player_0 = malloc(nb_turn * sizeof(int));
  prev->player_1 = malloc(nb_turn * sizeof(int));
  prev->player_0_betrayed = false;
  prev->player_1_betrayed = false;
  for (i = 0; i < nb_turn; i++) {
    prev->player_0[i] = play(strat1, 0, i, prev);
    prev->player_1[i] = play(strat2, 1, i, prev);
    if (!prev->player_0_betrayed && prev->player_0[i] == BETRAY) prev->player_0_betrayed = true;
    if (!prev->player_1_betrayed && prev->player_1[i] == BETRAY) prev->player_1_betrayed = true;

    if (prev->player_0[i] == BETRAY && prev->player_1[i] == BETRAY) {
      score_player_0 += parameters->P;
      score_player_1 += parameters->P;
    } else if (prev->player_0[i] == COOPERATE && prev->player_1[i] == COOPERATE) {
      score_player_0 += parameters->C;
      score_player_1 += parameters->C;
    }
    if (prev->player_0[i] == BETRAY && prev->player_1[i] == COOPERATE) {
      score_player_0 += parameters->T;
      score_player_1 += parameters->D;
    }
    if (prev->player_0[i] == COOPERATE && prev->player_1[i] == BETRAY) {
      score_player_0 += parameters->D;
      score_player_1 += parameters->T;
    }
  }
  free(prev->player_0);
  free(prev->player_1);
  free(prev);
  result_of_fight* result = malloc(sizeof(result_of_fight));
  result->winner = score_player_0 > score_player_1 ? 0 : (score_player_1 > score_player_0 ? 1 : 2);
  result->score_player_0 = score_player_0;
  return result;
}

void fight_all_against_all(int nb_turn, city_parameters* parameters) {
  strategy* strategies = get_strategies_array(parameters);
  result_of_fight* result;
  int i, j, k;
  printf("       ");
  for (k = 0; k < NB_STRATEGY; k++) printf("%d   ", k);
  printf ("\n");
  for (i = 0; i < NB_STRATEGY; i++) {
    printf(i < 10 ? "%d  - " : "%d - ", i);
    for (j = 0; j < NB_STRATEGY; j++) {
      result = fight(strategies[i], strategies[j], nb_turn, parameters);
      printf(result->winner < 2 ? "| %d " : "| E ", result->winner + 1);
      free(result);
      if (j == NB_STRATEGY - 1) printf("|\n");
    }
  }
  printf("\n");
  free(strategies);
}

int score_against_all(strategy strat, int nb_turn, city_parameters* parameters) {
  strategy* strategies = get_strategies_array(parameters);
  result_of_fight* result;
  int i, score;
  score = 0;
  for (i = 0; i < NB_STRATEGY; i++) {
    result = fight(strat, strategies[i], nb_turn, parameters);
    score += result->score_player_0;
    free(result);
  }
  free(strategies);
  return score;
}

void all_score_against_all(int nb_turn, city_parameters* parameters) {
  strategy* strategies = get_strategies_array(parameters);
  int i;
  printf("N = %d\n", nb_turn);
  for (i = 0; i < NB_STRATEGY; i++) {
    printf(i < 10 ? "%d  " : "%d ", i);
    printf("| %d\n", score_against_all(strategies[i], nb_turn, parameters));
  }
  printf("\n");
}
