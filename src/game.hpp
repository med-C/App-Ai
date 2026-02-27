#pragma once
#include <SDL.h>
#include <iostream>

class Game {
public:
    static void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    static void handleEvents();
    static void update();
    static void render();
    static void clean();

private:
    static SDL_Window* window;
    static SDL_Renderer* renderer;
    static bool isRunning;
public:
    bool GetisRunning(){
        return isRunning;
    }
};
