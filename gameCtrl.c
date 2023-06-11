#include "gameCtrl.h"

gameCtrl_t new_gameCtrl(int qtdJogadores){
    gameCtrl_t gameCtrl;

    gameCtrl.qtdJogadores = qtdJogadores;
    gameCtrl.jogadores = malloc(sizeof(jogador_t) * qtdJogadores);
    
    for(int i = 0; i < qtdJogadores; i++){
        gameCtrl.jogadores[i] = new_jogador(i+1);
    }

    gameCtrl.deck = new_deck();
    gameCtrl.table = new_table();

    return gameCtrl;
}


void print_gameCtrl(gameCtrl_t gameCtrl){
    print_deck(gameCtrl.deck);
}


void destroy_gameCtrl(gameCtrl_t gameCtrl){
    destroy_deck(gameCtrl.deck);
}

// Distribui as cartas aleatoriamente para os jogadores
void distribute_cards(gameCtrl_t gameCtrl){
    // int qtdCartas = gameCtrl.deck.size;
    // int qtdJogadores = gameCtrl.qtdJogadores;
    // int qtdCartasPorJogador = qtdCartas / qtdJogadores;
    // int qtdCartasRestantes = qtdCartas % qtdJogadores;
    // int cartasDistribuidas = 0;
    // int jogadorAtual = 0;

    // for(int i = 0; i < qtdCartas; i++){
    //     if(cartasDistribuidas < qtdCartasRestantes){
    //         gameCtrl.jogadores[jogadorAtual].deck.cards[i] = gameCtrl.deck.cards[i];
    //         cartasDistribuidas++;
    //     }else{
    //         gameCtrl.jogadores[jogadorAtual].deck.cards[i] = gameCtrl.deck.cards[i];
    //     }

    //     if(i % qtdCartasPorJogador == 0 && i != 0){
    //         jogadorAtual++;
    //     }
    // }
}

void print_jogadores(gameCtrl_t gameCtrl){
    for(int i = 0; i < gameCtrl.qtdJogadores; i++){
        print_jogador(gameCtrl.jogadores[i]);
    }
}