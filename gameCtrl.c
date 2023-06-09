#include "gameCtrl.h"

gameCtrl_t new_gameCtrl(){
    gameCtrl_t gameCtrl;

    gameCtrl.deck = new_deck();

    return gameCtrl;
}


void print_gameCtrl(gameCtrl_t gameCtrl){
    print_deck(gameCtrl.deck);
}

void destroy_gameCtrl(gameCtrl_t gameCtrl){
    destroy_deck(gameCtrl.deck);
}
