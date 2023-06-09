#ifndef gameCtrl_H
#define gameCtrl_H
#include "deck.h"

typedef struct gameCtrl{
    deck_t deck;

} gameCtrl_t;

gameCtrl_t new_gameCtrl();

void print_gameCtrl(gameCtrl_t gameCtrl);

void destroy_gameCtrl(gameCtrl_t gameCtrl);


#endif