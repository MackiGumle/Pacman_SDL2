#include "grafika.h"

void init_menu(SDL_Renderer *renderer, struct text *menuHead, struct text *menuText, struct text *menuScore, struct text *menuLives, struct text *menuSpeed, struct text *menuPower, struct vars s, int score, TTF_Font *font, TTF_Font *hFont)
{    
    // Header
    menuHead->color.r = 255;
    menuHead->color.g = 255;
    menuHead->color.b = 0;
    menuHead->color.a = 255;
    
    menuHead->surface = TTF_RenderText_Solid(hFont, "Pac-man", menuHead->color);
    menuHead->texture = SDL_CreateTextureFromSurface(renderer, menuHead->surface);

    menuHead->text.x = WINDOW_WIDTH / 2 - FONT_HSIZE * 3;
    menuHead->text.y = menuHead->surface->h;
    menuHead->text.w = menuHead->surface->w;
    menuHead->text.h = menuHead->surface->h;

    SDL_FreeSurface(menuHead->surface);

    // text
    menuText->color.r = 255;
    menuText->color.g = 255;
    menuText->color.b = 0;
    menuText->color.a = 255;
    
    menuText->surface = TTF_RenderText_Solid(font, "Press ENTER to play", menuText->color);
    menuText->texture = SDL_CreateTextureFromSurface(renderer, menuText->surface);

    menuText->text.x = WINDOW_WIDTH / 2 - FONT_SIZE * 8;
    menuText->text.y = menuText->surface->h;
    menuText->text.w = menuText->surface->w;
    menuText->text.h = menuText->surface->h;

    SDL_FreeSurface(menuText->surface);

    // Score
    char text[100];
    menuScore->color.r = 255;
    menuScore->color.g = 255;
    menuScore->color.b = 0;
    menuScore->color.a = 255;
    
    sprintf(text, "Score: %d", score);
    menuScore->surface = TTF_RenderText_Solid(hFont, text, menuScore->color);
    menuScore->texture = SDL_CreateTextureFromSurface(renderer, menuScore->surface);

    menuScore->text.x = WINDOW_WIDTH / 2 - FONT_HSIZE * 5;
    menuScore->text.y = WINDOW_HEIGT / 2 + FONT_HSIZE - 10;
    menuScore->text.w = menuScore->surface->w;
    menuScore->text.h = menuScore->surface->h;

    SDL_FreeSurface(menuScore->surface);

    // Lives
    menuLives->color.r = 0;
    menuLives->color.g = 255;
    menuLives->color.b = 0;
    menuLives->color.a = 255;
    
    sprintf(text, "lives: %d", s.var[0]);
    menuLives->surface = TTF_RenderText_Solid(font, text, menuLives->color);
    menuLives->texture = SDL_CreateTextureFromSurface(renderer, menuLives->surface);

    menuLives->text.x = 50;
    menuLives->text.y = WINDOW_HEIGT - FONT_SIZE * 3 - 10;
    menuLives->text.w = menuLives->surface->w;
    menuLives->text.h = menuLives->surface->h;

    SDL_FreeSurface(menuLives->surface);

    // Speed
    menuSpeed->color.r = 255;
    menuSpeed->color.g = 255;
    menuSpeed->color.b = 0;
    menuSpeed->color.a = 255;
    
    sprintf(text, "speed: %d", s.var[1]);
    menuSpeed->surface = TTF_RenderText_Solid(font, text, menuSpeed->color);
    menuSpeed->texture = SDL_CreateTextureFromSurface(renderer, menuSpeed->surface);

    menuSpeed->text.x = 50;
    menuSpeed->text.y = WINDOW_HEIGT - FONT_SIZE * 2 - 10;
    menuSpeed->text.w = menuSpeed->surface->w;
    menuSpeed->text.h = menuSpeed->surface->h;

    SDL_FreeSurface(menuSpeed->surface);

    // Power
    menuPower->color.r = 0;
    menuPower->color.g = 255;
    menuPower->color.b = 255;
    menuPower->color.a = 255;
    
    sprintf(text, "power: %ds", s.var[2]);
    menuPower->surface = TTF_RenderText_Solid(font, text, menuPower->color);
    menuPower->texture = SDL_CreateTextureFromSurface(renderer, menuPower->surface);

    menuPower->text.x = 50;
    menuPower->text.y = WINDOW_HEIGT - FONT_SIZE - 10;
    menuPower->text.w = menuPower->surface->w;
    menuPower->text.h = menuPower->surface->h;

    SDL_FreeSurface(menuPower->surface);
}

void draw_menu(SDL_Renderer *renderer,const struct text *menuHead, const struct text *menuText, struct text *menuScore, struct text *menuLives, struct text *menuSpeed, struct text *menuPower, SDL_Texture *sprites, struct vars s)
{
    SDL_Rect source;
    source.x = 128;
    source.y = 32;
    source.w = source.h = 32;
    
    SDL_Rect dest;
    dest.x = 250;
    dest.y = WINDOW_HEIGT - FONT_SIZE * (3 - s.sel) - 10;
    dest.w = dest.h = 32;  
    
    SDL_RenderCopy(renderer, menuHead->texture, NULL, &menuHead->text);
    SDL_RenderCopy(renderer, menuText->texture, NULL, &menuText->text);
    SDL_RenderCopy(renderer, menuScore->texture, NULL, &menuScore->text);
    SDL_RenderCopy(renderer, menuLives->texture, NULL, &menuLives->text);
    SDL_RenderCopy(renderer, menuSpeed->texture, NULL, &menuSpeed->text);
    SDL_RenderCopy(renderer, menuPower->texture, NULL, &menuPower->text);
    SDL_RenderCopy(renderer, sprites, &source, &dest);

}

void init_stats(SDL_Renderer *renderer, struct text *textLives, struct text *textScore, struct text *textPower, struct text *textPoints, TTF_Font *font, struct game *game)
{
    char text[100];

    // Lives
    textLives->color.r = 0;
    textLives->color.g = 255;
    textLives->color.b = 0;
    textLives->color.a = 255;
    
    sprintf(text, "lives: %d", game->pacman.lives);

    textLives->surface = TTF_RenderText_Solid(font, text, textLives->color);
    textLives->texture = SDL_CreateTextureFromSurface(renderer, textLives->surface);

    textLives->text.x = FONT_SIZE * 2;
    textLives->text.y = FONT_SIZE - 15;
    textLives->text.w = textLives->surface->w;
    textLives->text.h = textLives->surface->h;

    SDL_FreeSurface(textLives->surface);

    // Score
    textScore->color.r = 255;
    textScore->color.g = 255;
    textScore->color.b = 0;
    textScore->color.a = 255;

    sprintf(text, "score: %d", game->score);
    
    textScore->surface = TTF_RenderText_Solid(font, text, textScore->color);
    textScore->texture = SDL_CreateTextureFromSurface(renderer, textScore->surface);

    textScore->text.x = WINDOW_WIDTH - 11 * FONT_SIZE;
    textScore->text.y = FONT_SIZE - 15;
    textScore->text.w = textScore->surface->w;
    textScore->text.h = textScore->surface->h;

    SDL_FreeSurface(textScore->surface);

    // Power
    textPower->color.r = 0;
    textPower->color.g = 255;
    textPower->color.b = 255;
    textPower->color.a = 255;

    sprintf(text, "power: %.2fs", game->pacman.power_pill_timer / 1000.0);
    
    textPower->surface = TTF_RenderText_Solid(font, text, textPower->color);
    textPower->texture = SDL_CreateTextureFromSurface(renderer, textPower->surface);

    textPower->text.x = FONT_SIZE * 2;
    textPower->text.y = FONT_SIZE * 2;
    textPower->text.w = textPower->surface->w;
    textPower->text.h = textPower->surface->h;

    SDL_FreeSurface(textPower->surface);

    // Points
    textPoints->color.r = 255;
    textPoints->color.g = 0;
    textPoints->color.b = 0;
    textPoints->color.a = 255;

    sprintf(text, "points: %d", game->points);
    
    textPoints->surface = TTF_RenderText_Solid(font, text, textPoints->color);
    textPoints->texture = SDL_CreateTextureFromSurface(renderer, textPoints->surface);

    textPoints->text.x = WINDOW_WIDTH - 11 * FONT_SIZE;
    textPoints->text.y = FONT_SIZE * 2;
    textPoints->text.w = textPoints->surface->w;
    textPoints->text.h = textPoints->surface->h;

    SDL_FreeSurface(textPoints->surface);
}

void draw_stats(SDL_Renderer *renderer, struct text *textLives, struct text *textScore, struct text *textPower, struct text *textPoints)
{
    SDL_RenderCopy(renderer, textLives->texture, NULL, &textLives->text);
    SDL_RenderCopy(renderer, textScore->texture, NULL, &textScore->text);
    SDL_RenderCopy(renderer, textPower->texture, NULL, &textPower->text);
    SDL_RenderCopy(renderer, textPoints->texture, NULL, &textPoints->text);
}

void draw_map(SDL_Renderer *renderer, SDL_Texture *sprites, const struct game *game)
{
    bool ghost = false; // Ghost was drawn
    int i; // id of ghost

    SDL_Rect source;
    source.x = 0;
    source.y = 0;
    source.w = source.h = 32;
    
    SDL_Rect dest;
    dest.x = 0;
    dest.y = WINDOW_HEIGT - 608;
    dest.w = dest.h = 32;  

    for(int y = 0; y < MAP_HEIGHT; y++) {
        for(int x = 0; x < MAP_WIDTH; x++) {

            for (i = 0; i < 4; i++) {
                if(game->ghosts[i].pos.x == x && game->ghosts[i].pos.y == y && !ghost) {
                    ghost = true;
                    break;
                }
            }

            if(ghost) {
                if(game->pacman.power_pill_timer > 0)
                    source.x = 128;
                else
                    source.x = 32 * i;
                
                source.y = 64;
                ghost = false;
                SDL_RenderCopy(renderer, sprites, &source, &dest);
                dest.x += 32;
                continue;
            }

            if(game->pacman.pos.x == x && game->pacman.pos.y == y) {
                source.x = 32 * game->pacman.dir;
                source.y = 32 * ((game->pacman.pos.x + game->pacman.pos.y) % 2);  
                SDL_RenderCopy(renderer, sprites, &source, &dest);
                dest.x += 32;
                continue;
            }

            switch (game->map.cells[x][y]) {
                case CELL_EMPTY:
                    source.x = 160;
                    source.y = 32;
                    break;
                case CELL_POINT:
                    source.x = 128;
                    source.y = 32;
                    break;
                case CELL_POWER_PILL:
                    source.x = 128;
                    source.y = 0;
                    break;
                case CELL_WALL:
                    source.x = 160;
                    source.y = 0;
                    break;
            }

            SDL_RenderCopy(renderer, sprites, &source, &dest);
            dest.x += 32;
        }

        dest.x = 0;
        dest.y += 32;
    }
}