#include "pacman.h"
#include <string.h>

int map_load(struct game *game, const char *filename)
{
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        return -1;
    }

    game->points = 0;
    int ghostCount = 0;
    int pacmanCount = 0;
    int c;
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            c = fgetc(file);
            switch (c) {
                case ' ':
                    game->map.cells[x][y] = CELL_EMPTY;
                    break;
                case '#':
                    game->map.cells[x][y] = CELL_WALL;
                    break;
                case '.':
                    game->map.cells[x][y] = CELL_POINT;
                    game->points++;
                    break;
                case 'o':
                    game->map.cells[x][y] = CELL_POWER_PILL;
                    break;
                case 'G':
                        game->map.cells[x][y] = CELL_EMPTY;
                    if(ghostCount < 4){
                        game->ghosts[ghostCount].spawn.x = x;
                        game->ghosts[ghostCount].spawn.y = y;
                        ghostCount++;
                    }
                    break;
                case 'C':
                        game->map.cells[x][y] = CELL_EMPTY;
                    if(pacmanCount == 0){
                        game->pacman.spawn.x = x;
                        game->pacman.spawn.y = y;
                        pacmanCount++;
                    }
                    break;
                default:
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid map data at (%d,%d) = '%c'\nMust be: ' '/'#'/'.'/'o'\n", x, y, c);
                    fclose(file);
                    return -1;
            }
        }

        // Skip '\n'
        fgetc(file);
    }


    fclose(file);
    return 0;
}

void game_init(struct game *game, struct vars s)
{
    // Loads map data from file
    if (map_load(game, "map.txt") < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load map.");
        exit(1);
    }

    game->dir[UP].x = 0;
    game->dir[UP].y = -1;
    game->dir[RIGHT].x = 1;
    game->dir[RIGHT].y = 0;
    game->dir[LEFT].x = -1;
    game->dir[LEFT].y = 0;
    game->dir[DOWN].x = 0;
    game->dir[DOWN].y = 1;

    game->pacman.lives = s.var[0];
    game->pacman.power_pill_timer = 0;
    game->pacman.pos.x = game->pacman.spawn.x;
    game->pacman.pos.y = game->pacman.spawn.y;

    // Load ghosts
    for(int i = 0; i < 4; i++)
    {
        game->ghosts[i].pos.x = game->ghosts[i].spawn.x;
        game->ghosts[i].pos.y = game->ghosts[i].spawn.y;
        game->ghosts[i].tar = game->pacman.pos;
        game->ghosts[i].dir = 0;
        game->ghosts[i].stun_timer = 1000;
    }

    game->score = 0;
    game->game_over = false;
}

int turn_back(int dir)
{
    switch (dir)
    {
        case UP:
            return DOWN;
        case DOWN:
            return UP;
        case RIGHT:
            return LEFT;
        case LEFT:
            return RIGHT;
    }
}

bool is_path(const struct game *game, struct pos pos)
{
    if (pos.x < 0 || pos.x >= MAP_WIDTH || pos.y < 0 || pos.y >= MAP_HEIGHT || game->map.cells[pos.x][pos.y] == CELL_WALL)
        return false;
    else 
        return true;
}

void pacman_move(struct game *game, struct vars s)
{
    struct pos move; // Position to move to
       
    move.x = game->pacman.pos.x + game->dir[game->pacman.dir].x;
    move.y = game->pacman.pos.y + game->dir[game->pacman.dir].y;
    
    if (!is_path(game, move)) {
        // Pacman hit a wall
        return;
    }
    
    // Update Pacman position
    game->pacman.pos = move;

    // Point or power pill collision
    switch (game->map.cells[move.x][move.y]) {
        case CELL_POINT:
            game->score += 1;
            game->points--;
            game->map.cells[move.x][move.y] = CELL_EMPTY;
            break;            
        case CELL_POWER_PILL:
            game->score += 5;
            game->map.cells[move.x][move.y] = CELL_EMPTY;
            game->pacman.power_pill_timer += s.var[2] * 1000;
            break;
    }
}

void ghosts_move(struct game *game)
{
    int disTemp[2]; //Temp 0-distance, 1-direction
    int dis[2][4]; // 0-Distance, 1-direction
    struct pos move[4]; // Paths up, right, left, down
    bool moved;

    for (int i = 0; i < 4; i++) { // For each ghost
        if (game->ghosts[i].stun_timer != 0) // Skips turn of stunned ghost
            continue;

        moved = false;

        for (int j = 0; j < 4; j++) { // Scans positions
            // Set destination for ghost 
            move[j].x = game->ghosts[i].pos.x + game->dir[j].x;
            move[j].y = game->ghosts[i].pos.y + game->dir[j].y;
            
            // Calculate distance
            dis[0][j] = abs(game->ghosts[i].tar.x - move[j].x) + abs(game->ghosts[i].tar.y - move[j].y);
            // Save direction
            dis[1][j] = j;
        }
     
        // Sorts distances in ascending order
        for (int j = 0; j < 4; j++) {
           for (int k = j + 1; k < 4; k++) {
                if (dis[0][j] > dis[0][k]) {
                    
                    disTemp[0] = dis[0][j];
                    disTemp[1] = dis[1][j];

                    dis[0][j] = dis[0][k];
                    dis[1][j] = dis[1][k];

                    dis[0][k] = disTemp[0];
                    dis[1][k] = disTemp[1];
                }
            }  
        }
        
        // Moves ghost in valid direction
        for (int j = 0; j < 4; j++) {
            if (is_path(game, move[dis[1][j]]) && dis[1][j] != turn_back(game->ghosts[i].dir)) {
                game->ghosts[i].dir = dis[1][j];
                game->ghosts[i].pos = move[game->ghosts[i].dir];
                moved = true;
                break;
            }
        }

        // Ghost got stuck
        if(!moved) {
            game->ghosts[i].dir = turn_back(game->ghosts[i].dir);
            game->ghosts[i].pos = move[game->ghosts[i].dir];      
            moved = true;
        }
    }

}

void ghost_respawn(struct ghost *ghost)
{
    ghost->pos.x = ghost->spawn.x;
    ghost->pos.y = ghost->spawn.y;
    ghost->stun_timer = 2000;
}

void pacman_respawn(struct game *game)
{
    game->pacman.pos.x = game->pacman.spawn.x;
    game->pacman.pos.y = game->pacman.spawn.y;

    for(int i = 0; i < 4; i++){
        ghost_respawn(&game->ghosts[i]);
    }
}

void hit_check(struct game *game)
{
    // Collision with ghost
    for (int i = 0; i < 4; i++) {
        if(game->ghosts[i].pos.x == game->pacman.pos.x && game->ghosts[i].pos.y == game->pacman.pos.y)
            if(game->pacman.power_pill_timer == 0 && !(game->ghosts[i].stun_timer > 0)) { // Ghost isnt stunned and pacman has no power
                game->pacman.lives--;
                pacman_respawn(game);
            }
            else if(!(game->ghosts[i].stun_timer > 0)) { // Ghost eaten if not stunned
                // Respawn eaten ghost
                ghost_respawn(&game->ghosts[i]);
                game->score += 10;
            } 
    }
}

void game_update(struct game *game, int deltaTime)
{
    srand(SDL_GetTicks());

    // Power pill time
    if(game->pacman.power_pill_timer > 0) {
        game->pacman.power_pill_timer -= deltaTime;

        // Ghost panic
        for(int i = 0; i < 4; i++) {
            game->ghosts[i].tar.x = rand() % MAP_WIDTH;
            game->ghosts[i].tar.y = rand() % MAP_WIDTH;
            
            // Scared ghosts spawn points
            if(rand() % 900 == 0)
                game->map.cells[game->ghosts[i].pos.x][game->ghosts[i].pos.y] = CELL_POWER_PILL; 
            else if(game->map.cells[game->ghosts[i].pos.x][game->ghosts[i].pos.y] != CELL_POINT && game->map.cells[game->ghosts[i].pos.x][game->ghosts[i].pos.y] != CELL_POWER_PILL) {
                game->map.cells[game->ghosts[i].pos.x][game->ghosts[i].pos.y] = CELL_POINT; 
                game->points++;
            }
        }
    } else {
        game->pacman.power_pill_timer = 0;

        game->ghosts[0].tar = game->pacman.pos;

        game->ghosts[1].tar.x = game->pacman.pos.x + game->dir[game->pacman.dir].y*2;
        game->ghosts[1].tar.y = game->pacman.pos.y + game->dir[game->pacman.dir].x*2;

        game->ghosts[2].tar.x = game->pacman.pos.x + game->dir[turn_back(game->pacman.dir)].y;
        game->ghosts[2].tar.y = game->pacman.pos.y + game->dir[turn_back(game->pacman.dir)].x;

        game->ghosts[3].tar.x = game->pacman.pos.x + game->dir[game->pacman.dir].x;
        game->ghosts[3].tar.y = game->pacman.pos.y + game->dir[game->pacman.dir].y;
    }

    for(int i = 0; i < 4; i++) {
        if(game->ghosts[i].stun_timer > 0)
                game->ghosts[i].stun_timer -= deltaTime; 
            else
                game->ghosts[i].stun_timer = 0;
    }

    // Game over
    game->game_over = (game->pacman.lives <= 0 || game->points == 0) ? true : false;
}

void print_game(const struct game* game){
    bool ghost = false; // Ghost was printed
    //bool tar = false; // Target was printed
    int j;

    
        for (j = 0; j < 4; j++)
          printf("ghost: %d\t|", j);
    
        printf("\n");

        for(j = 0; j < 4; j++)
            printf("x:%d y:%d \t|", game->ghosts[j].pos.x, game->ghosts[j].pos.y);
        
        printf("\n");

        for(j = 0; j < 4; j++)
            printf("dir:%d \t|", game->ghosts[j].dir);
        
        printf("\n");

        for (j = 0; j < 4; j++)
          printf("stun: %d \t|", game->ghosts[j].stun_timer);

        printf("\n");


    printf("score: %d\t|points: %d\t|lives: %d\t|power: %d\t|\n"
            , game->score, game->points, game->pacman.lives, game->pacman.power_pill_timer);

    printf("game_over: %s\n", game->game_over ? "true" : "false");

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            // Check position of ghosts
            for (int i = 0; i < 4; i++){
                if(game->ghosts[i].pos.x == x && game->ghosts[i].pos.y == y && !ghost) {
                    ghost = true;
                    break;
                }
                
                /*if(game->ghosts[i].tar.x == x && game->ghosts[i].tar.y == y) {
                    tar = true;   
                }*/
            }

            if(ghost){
                printf("G");
                ghost = false;
                continue;
            }

            /*if(tar){
                printf("x");
                tar = false;
                continue;
            }*/

            if(game->pacman.pos.x == x && game->pacman.pos.y == y){
                printf("C");
                continue;
            }
            
            switch (game->map.cells[x][y]){
                case CELL_EMPTY:
                    printf(" ");
                    break;
                case CELL_POINT:
                    printf(".");
                    break;
                case CELL_POWER_PILL:
                    printf("o");
                    break;
                case CELL_WALL:
                    printf("#");
                    break;
            }
        }
        printf("\n");
    }
    
    printf("\e[2J"); // \x1b[2J
}


