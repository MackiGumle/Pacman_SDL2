#ifndef PACMAN_H
#define PACMAN_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAP_WIDTH  19
#define MAP_HEIGHT 19


enum cell_type {
    CELL_EMPTY,
    CELL_WALL,
    CELL_POINT,
    CELL_POWER_PILL
};

enum direction {
    RIGHT,
    DOWN,
    LEFT,
    UP
};

struct map {
    enum cell_type cells[MAP_WIDTH][MAP_HEIGHT];
};

struct pos {
    int x;
    int y;
};

struct vars { // 0 lives,1 speed,2 power
    int var[3];
    int sel;
};

struct pacman {
    // Position
    struct pos pos;
    // Direction
    int dir;
    // Spawn
    struct pos spawn;

    int lives;
    int power_pill_timer; // In milliseconds
};

struct ghost {
    // Position
    struct pos pos;
    // Direction
    int dir;
    // Traget
    struct pos tar;
    // Spawn
    struct pos spawn;

    int stun_timer; // In milliseconds  
};

struct game
{
    struct map map;
    struct pacman pacman;
    struct ghost ghosts[4];
    struct pos dir[4]; // directions
    int points;
    int score;
    bool game_over;
};


int map_load(struct game *game, const char *filename); // Loads a map from a file
void game_init(struct game *game, struct vars s); // Initializes the game

int turn_back(int dir); // Returns oposite direction
bool is_path(const struct game *game, struct pos pos); // Checks if path is valid
void pacman_move(struct game *game, struct vars s); // Handles pacman movement and collisions
void ghosts_move(); // Handles ghost movement
void ghost_respawn(struct ghost *ghost); // Moves ghost to spawnpos
void pacman_respawn(struct game *game); // Moves pacman and ghosts to spawnpos
void hit_check(struct game *game); // Handles pacman-ghost collision

void game_update(struct game *game, int frameTime); // Updates game state
void print_game(const struct game* game); // Prints game state in text format

#endif