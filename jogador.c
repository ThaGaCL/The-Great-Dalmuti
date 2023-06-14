#include "jogador.h"

jogador_t new_jogador(int id){
    jogador_t jogador;

    jogador.id = id;
    jogador.score = 0;
    jogador.deck = new_table(); // Utiliza a função de criar mesa, pois assim é criado com cartas vazias

    return jogador;
}


void print_jogador(jogador_t jogador){
    printf("Jogador %d\n", jogador.id);
    printf("Score: %d\n", jogador.score);
    printf("Tamanho do Deck: %d\n", jogador.deck.size);
    print_deck(jogador.deck);
}