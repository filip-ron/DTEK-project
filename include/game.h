#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "config.h"

typedef struct {
    int x;
    int y;
    int height;
    int width;
} Player;


typedef struct {
    int x;
    int y;
    int height;
    int width;
    bool upside_down;
} Spike;

typedef struct {
    int x;
    int y;
    int height;
    int width;
} Block;

typedef struct {
    Player player;
    Spike spikes[MAX_SPIKES];
    int spike_count;
    Block blocks[MAX_BLOCKS];
    int block_count;
    int camera_x;
} GameState;


#endif