#include "gameCtrl.h"
#include <time.h>

// TODO 
// 1. Implementar o jogador.h e jogador.c
// 2. Implementar a função put_card
// 3. Implementar a função check_round_winner
// 4. Implementar a função check_game_winner
// 5. Guardar qual jogador jogou cada carta na rodada (usar o indice do deck da mesa)

int main(){
    srand(time(NULL));

    gameCtrl_t gameCtrl = new_gameCtrl(4);
    // print_gameCtrl(gameCtrl);
    distribute_cards(gameCtrl);
    print_jogadores(gameCtrl);

    destroy_gameCtrl(gameCtrl);

    return 0;
}