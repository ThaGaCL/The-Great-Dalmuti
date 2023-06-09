#include "deck.h"


deck_t new_deck(){
    deck_t deck;

    deck.size = DECK_SIZE;
    deck.cards = malloc(sizeof(card_t) * DECK_SIZE);

    for(int i = 0; i <= CARD_MAX_VALUE; i++){
        deck.cards[i] = new_card(i);
    }


    return deck;
}

void print_deck(deck_t deck){
    for(int i = 0; i <= CARD_MAX_VALUE; i++){
        print_card(deck.cards[i]);
    }
}

void destroy_deck(deck_t deck){
    for(int i = 0; i < deck.size; i++){
        deck.cards[i].amount = 0;
        deck.cards[i].value = 0;
    }
    free(deck.cards);
}