#include "game.hpp"

Game* game = nullptr;

int main(int argc, char* argv[]){

    game = new Game();

    game->init("hello" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED, 800 , 600 , false);

    while(game->GetisRunning()){
        game->handleEvents();
        game->update();
        game->render();
    }

    game->clean();




    return 1;
}