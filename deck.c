#include "deck.h"

deck_t new_deck(){
    deck_t deck;

    deck.size = DECK_SIZE;
    deck.cards = malloc(sizeof(card_t) * DECK_SIZE);

    deck.cards[0] = new_card(0);
    deck.cards[0].amount = 2;

    for(int i = 1; i <= CARD_MAX_VALUE; i++){
        deck.cards[i] = new_card(i);
    }


    return deck;
}

deck_t new_table(){
    deck_t deck;

    deck.size = 0;
    deck.cards = malloc(sizeof(card_t) * (CARD_MAX_VALUE));

    for(int i = 0; i <= CARD_MAX_VALUE; i++){
        deck.cards[i] = new_card(-1);
        deck.cards[i].value = -1;
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

void remove_card(deck_t* deck, int value){
    if(deck->cards[value].amount > 0)
        deck->cards[value].amount--;
}

