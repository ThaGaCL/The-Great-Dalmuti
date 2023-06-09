#ifndef DECK_H
#define DECK_H
#include "card.h"


#define DECK_SIZE 80

typedef struct deck {
    card_t* cards;
    int size;
} deck_t;

deck_t new_deck();

deck_t new_table();

void destroy_deck(deck_t deck);

void remove_card(deck_t* deck, int value);

#endif