#ifndef JOGADOR_H
#define JOGADOR_H
#include "deck.h"

typedef struct jogador{
    deck_t deck;
    int score;
    int id;
} jogador_t;

jogador_t new_jogador(int id);

void print_jogador(jogador_t jogador);

#endif