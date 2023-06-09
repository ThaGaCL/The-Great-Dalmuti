#include "gameCtrl.h"

// TODO 
// 1. Implementar o jogador.h e jogador.c
// 2. Implementar a função put_card
// 3. Implementar a função check_round_winner
// 4. Implementar a função check_game_winner
// 5. Guardar qual jogador jogou cada carta na rodada (usar o indice do deck da mesa)

int main(){

    gameCtrl_t gameCtrl = new_gameCtrl();

    print_gameCtrl(gameCtrl);

    destroy_gameCtrl(gameCtrl);

    return 0;
}