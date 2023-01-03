#ifndef GRAFIKA_H
#define GRAFIKA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "pacman.h"

#define WINDOW_HEIGT 700
#define WINDOW_WIDTH 608
#define FONT_HSIZE 50
#define FONT_SIZE 24

struct text {
    SDL_Color color;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect text;
};

void init_menu(SDL_Renderer *renderer, struct text *menuHead, struct text *menuText, struct text *menuScore, struct text *menuLives, struct text *menuSpeed, struct text *menuPower, struct vars s, int score, TTF_Font *font, TTF_Font *hFont);
void draw_menu(SDL_Renderer *renderer,const struct text *menuHead, const struct text *menuText, struct text *menuScore, struct text *menuLives, struct text *menuSpeed, struct text *menuPower, SDL_Texture *sprites, struct vars s);
void init_stats(SDL_Renderer *renderer, struct text *textLives, struct text *textScore, struct text *textPower, struct text *textPoints, TTF_Font *font, struct game *game);
void draw_stats(SDL_Renderer *renderer, struct text *textLives, struct text *textScore, struct text *textPower, struct text *textPoints);
void draw_map(SDL_Renderer *renderer, SDL_Texture *sprites, const struct game *game);

#endif
