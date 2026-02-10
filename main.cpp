#include <iostream>
#include <string>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define PROJECT_NAME "softplayer"

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Surface *surface = nullptr;


int main(int argc, char *args[])
{
    std::cout << "Running!" << std::endl;

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Soft Player", 800, 600, 0);
    surface = SDL_GetWindowSurface(window);
    

    while (true)
    {
        // run foreva!
    }

    std::cout << "Closing!" << std::endl;
    return 0;
}