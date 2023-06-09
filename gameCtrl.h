#ifndef gameCtrl_H
#define gameCtrl_H
#include "deck.h"
#include "jogador.h"

typedef struct gameCtrl{
    deck_t deck;
    deck_t table;
} gameCtrl_t;

gameCtrl_t new_gameCtrl();

void print_gameCtrl(gameCtrl_t gameCtrl);

// Adiciona uma carta na mesa
void put_card(gameCtrl_t* gameCtrl, int value);

// Compara o valor das cartas na mesa e retorna o vencedor
void check_round_winner(gameCtrl_t* gameCtrl);

void destroy_gameCtrl(gameCtrl_t gameCtrl);


#endif