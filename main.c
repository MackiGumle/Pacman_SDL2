#include "pacman.h"
#include "grafika.h"
#include <stdbool.h>
#include <assert.h>


int main()
{
    if (SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGT, SDL_WINDOW_SHOWN);
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

    TTF_Init();

    TTF_Font *font = TTF_OpenFont("font.ttf", FONT_SIZE);
    TTF_Font *hFont = TTF_OpenFont("font.ttf", FONT_HSIZE);
    if (!font || !hFont) {
        fprintf(stderr, "Error: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Event e;
    int fps = 80;
    float msf = 1000.0 / fps; // Desired ms per frame
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    double frameTime = 0;
    struct vars s = {.var = {3, 5, 2}, .sel = 0};

    bool quit = false;
    bool menu = true;
    struct text menuHead;
    struct text menuText;        
    struct text menuScore;
    struct text menuLives;
    struct text menuSpeed;
    struct text menuPower;    

    struct text textLives;
    struct text textScore;    
    struct text textPower;  
    struct text textPoints;

    SDL_Texture *sprites = IMG_LoadTexture(renderer, "./sprites.png");
   
    struct game game;
    
    game_init(&game, s);

    while (!quit){ // game loop
        LAST = NOW;
        
        // Input handling
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT)
                quit = true;
            
            if (e.type == SDL_KEYDOWN){
                    switch (e.key.keysym.sym) {

                        case SDLK_LEFT: 
                            if(menu && s.var[s.sel] > 1) {
                                s.var[s.sel]--;
                            }else
                                game.pacman.dir = LEFT;
                            break;                   

                        case SDLK_RIGHT: 
                            if(menu) {
                                s.var[s.sel]++;
                            }else
                                game.pacman.dir = RIGHT;
                            break;                    

                        case SDLK_UP: 
                            if(menu && s.sel > 0) {
                                s.sel--;
                            }else
                                game.pacman.dir = UP;
                            break;

                        case SDLK_DOWN: 
                            if(menu && s.sel < 2) {
                                s.sel++;
                            }else
                                game.pacman.dir = DOWN;
                            break;

                        case SDLK_RETURN:
                            if(menu) {
                                menu = false;
                                game_init(&game, s);
                            }
                            break;
                    }
            }
        }

        if(!menu) {
            // Limit game speed
            if(frameTime >= 1000 / s.var[1]){
                frameTime = 0;

                ghosts_move(&game);
                hit_check(&game);
                pacman_move(&game, s);
                hit_check(&game);
            }

            game_update(&game, deltaTime);
            //print_game(&game); // prints game in terminal
            init_stats(renderer, &textLives, &textScore, &textPower, &textPoints, font, &game);
            draw_stats(renderer, &textLives, &textScore, &textPower, &textPoints);
            draw_map(renderer, sprites, &game);


            if(game.game_over)
                menu = true;
        } else {
            init_menu(renderer, &menuHead, &menuText, &menuScore, &menuLives, &menuSpeed, &menuPower, s, game.score, font, hFont);
            draw_menu(renderer, &menuHead, &menuText, &menuScore, &menuLives, &menuSpeed, &menuPower, sprites, s);
        }


        SDL_RenderPresent(renderer);  // Prezentace kreslítka
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Nastavení barvy na černou
        SDL_RenderClear(renderer); 

        NOW = SDL_GetPerformanceCounter();
        deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency());
        frameTime += deltaTime;

        // Limit performance
        if(deltaTime < msf){
            SDL_Delay(msf - deltaTime);
        }
    }

    // Cleanup
    TTF_CloseFont(font);
    TTF_CloseFont(hFont);
    TTF_Quit();

    SDL_DestroyTexture(menuHead.texture);
    SDL_DestroyTexture(menuText.texture);
    SDL_DestroyTexture(menuScore.texture);
    SDL_DestroyTexture(menuLives.texture);
    SDL_DestroyTexture(menuSpeed.texture);
    SDL_DestroyTexture(menuPower.texture);

    SDL_DestroyTexture(textLives.texture);
    SDL_DestroyTexture(textScore.texture);
    SDL_DestroyTexture(textPower.texture);
    SDL_DestroyTexture(textPoints.texture);
    SDL_DestroyTexture(sprites);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
