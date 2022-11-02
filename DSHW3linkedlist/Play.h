#ifndef PLAY_H_
#define PLAY_H_
#include "shogi.h"

static void stdin_cb (EV_P_ ev_io *w, int revents);
static void timeout1_cb (EV_P_ ev_timer *w, int revents);
static void timeout2_cb (EV_P_ ev_timer *w, int revents);

int Play_Chess();
int Enter_move();
bool Enter_Grave(int Grave[]);
int move();



#endif