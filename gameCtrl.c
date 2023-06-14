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
void distribute_cards(gameCtrl_t gameCtrl) {
    int qtdCartas = gameCtrl.deck.size;
    int qtdJogadores = gameCtrl.qtdJogadores;
    int qtdCartasPorJogador = qtdCartas / qtdJogadores;

    for (int i = 0; i < gameCtrl.qtdJogadores; i++) {
        int cartasDistribuidas = 0;  // Movido para dentro do loop for

        while (cartasDistribuidas < qtdCartasPorJogador) {  // Corrigido para < em vez de <=
            int cardType = rand() % 13;

            while (gameCtrl.deck.cards[cardType].amount <= 0) {
                cardType = rand() % 13;
            }

            gameCtrl.jogadores[i].deck.cards[cardType].value = cardType;
            gameCtrl.jogadores[i].deck.cards[cardType].amount++;
            gameCtrl.jogadores[i].deck.size++;
            gameCtrl.deck.cards[cardType].amount--;
            cartasDistribuidas++;
            gameCtrl.deck.size--;
        }
    }

}

void print_jogadores(gameCtrl_t gameCtrl){
    for(int i = 0; i < gameCtrl.qtdJogadores; i++){
        print_jogador(gameCtrl.jogadores[i]);
    }
}

int put_card(gameCtrl_t gameCtrl, int value, int amount, int playerId){

    if((playerId == 1)||((playerId > 0) && (amount >= gameCtrl.table.cards[playerId-1].amount) && (value >= gameCtrl.table.cards[playerId-1].value))){
        gameCtrl.table.cards[playerId].amount = amount;
        gameCtrl.table.cards[playerId].value = value;
        return 1;
    }

    return 0;

}

int check_round_winner(gameCtrl_t gameCtrl){
    int winner = 0;
    
    for(int i = 1; i <= gameCtrl.qtdJogadores; i++){
        if(gameCtrl.table.cards[i].value != -1)
            winner = i;
    }

    return winner;
}
