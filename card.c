#include "card.h"

card_t new_card(int value){
    card_t card;

    // Cada tipo de carta tem uma quantidade inicial igual ao valor da carta
    card.value = value;
    card.amount = value;

    return card;
}

void print_card(card_t card){
    if(card.amount > 0){
        printf("Card value: %d\n", card.value);
        printf("Card amount: %d\n", card.amount);
    }
}