#ifndef CARD_H
#define CARD_H

#include <stdio.h>
#include <stdlib.h>

// O valor máximo de uma carta é 12, o valor 0 representa os coringas
#define CARD_MAX_VALUE 12

typedef struct card {
    int value;
    int amount;
} card_t;

card_t new_card(int value);

void print_card(card_t card);

#endif