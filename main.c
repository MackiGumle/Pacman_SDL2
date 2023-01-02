#include "pacman.h"
#include <stdbool.h>
#include <assert.h>


int main()
{
    if (SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Event e;
    int fps = 60;
    float msf = 1000.0 / fps; // Desired ms per frame
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    double frameTime = 0;
    bool quit = false;
    struct game game;

    game_init(&game);
    
    while (!quit){ // game loop
        LAST = NOW;

        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT)
                quit = true;
            
            if(e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym) {

                    case SDLK_LEFT: 
                        game.pacman.dir = LEFT;
                        break;                   

                    case SDLK_RIGHT: 
                        game.pacman.dir = RIGHT;
                        break;                    

                    case SDLK_UP: 
                        game.pacman.dir = UP;
                        break;

                    case SDLK_DOWN: 
                        game.pacman.dir = DOWN;
                        break;
                    
                    case SDLK_KP_MULTIPLY:
                        game.pacman.power_pill_timer += 1000;
                        break;
                }
                
            }
        }

        if(frameTime >= 1000 / 8){
            frameTime = 0;

            ghosts_move(&game);
            hit_check(&game);
            pacman_move(&game);
            hit_check(&game);
        }

        game_update(&game, deltaTime);
        print_game(&game);
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Nastavení barvy na černou
        SDL_RenderClear(renderer);                      // Vykreslení pozadí

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Nastavení barvy na červenou
        SDL_RenderDrawLine(renderer, 0, 0, game.score, game.points); // Vykreslení čáry
        
        SDL_RenderPresent(renderer);  // Prezentace kreslítka

        NOW = SDL_GetPerformanceCounter();
        deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() );

        frameTime += deltaTime;

        if(deltaTime < msf){
            SDL_Delay(msf - deltaTime);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
