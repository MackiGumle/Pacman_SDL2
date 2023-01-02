#include "pacman.h"
#include <stdbool.h>
#include <assert.h>

void print_map(struct game game)
{
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            printf("%d", game.map.cells[x][y]);
        }
        printf("\n");
    }
}

void print_game(const struct game* game)
{
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {

            for (int i = 0; i < 4; i++){
                if(game->ghosts[i].x == x && game->ghosts[i].y == y)
                    printf("G");
            }

            if(game->pacman.x == x && game->pacman.y == y)
                printf("C");
            else        
                switch (game->map.cells[x][y]){
                    case CELL_EMPTY:
                        printf(" ");
                        break;
                    case CELL_POINT:
                        printf(".");
                        break;
                    case CELL_POWER_PILL:
                        printf("*");
                        break;
                    case CELL_WALL:
                        printf("#");
                        break;
                    default:
                        printf("x");
                        break;
                }
            
        }
        printf("\n");
    }
}

int main()
{
    struct game game;
    
    printf("nacteni mapy: %d\n", map_load(&game, "map.txt"));
    print_map(game);
    getchar();

    return 0;
}
