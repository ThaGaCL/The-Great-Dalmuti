#include "gameCtrl.h"

int main(){

    gameCtrl_t gameCtrl = new_gameCtrl();

    print_gameCtrl(gameCtrl);

    destroy_gameCtrl(gameCtrl);

    return 0;
}