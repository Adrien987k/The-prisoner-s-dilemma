#include "strategies.h"


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
      if (turn == 0) return COOPERATE;
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


int fight(strategy strat1, strategy strat2, int n) {
  int i;
  int score_player_0 = 0;
  int score_player_1 = 0;
  previous_plays* prev = malloc(sizeof(previous_plays));
  prev->player_0 = malloc(n * sizeof(int));
  prev->player_1 = malloc(n * sizeof(int));
  prev->player_0_betrayed = false;
  prev->player_1_betrayed = false;
  for (i = 0; i < n; i++) {
    prev->player_0[i] = play(strat1, 0, i, prev);
    prev->player_1[i] = play(strat2, 1, i, prev);
    if (!prev->player_0_betrayed && prev->player_0[i] == BETRAY) prev->player_0_betrayed = true;
    if (!prev->player_1_betrayed && prev->player_1[i] == BETRAY) prev->player_1_betrayed = true;

    if (prev->player_0[i] == BETRAY && prev->player_1[i] == BETRAY) {
      score_player_0 += P;
      score_player_1 += P;
    } else if (prev->player_0[i] == COOPERATE && prev->player_1[i] == COOPERATE) {
      score_player_0 += C;
      score_player_1 += C;
    }
    if (prev->player_0[i] == BETRAY && prev->player_1[i] == COOPERATE) {
      score_player_0 += T;
      score_player_1 += D;
    }
    if (prev->player_0[i] == COOPERATE && prev->player_1[i] == BETRAY) {
      score_player_0 += D;
      score_player_1 += T;
    }
  }
  free(prev->player_0);
  free(prev->player_1);
  free(prev);
  return score_player_0 > score_player_1 ? 0 : (score_player_1 > score_player_0 ? 1 : 2);
}
