#ifndef CONFIG_H
#define CONFIG_H

// VGA display
#define VGA_WIDTH 320
#define VGA_HEIGHT 240

// Ground
#define GROUND_Y 200
#define GROUND_HEIGHT (VGA_HEIGHT - GROUND_Y)

// Player
#define PLAYER_WIDTH 20
#define PLAYER_HEIGHT 20

// RGB332 colors
#define COLOR_BLACK 0x00
#define COLOR_WHITE 0xFF
#define COLOR_RED 0xE0
#define COLOR_GREEN 0x1C
#define COLOR_BLUE 0x03

// Game object colors
#define BACKGROUND_COLOR COLOR_BLACK
#define PLAYER_COLOR COLOR_BLUE
#define GROUND_COLOR COLOR_GREEN
#define SPIKE_COLOR COLOR_RED
#define BLOCK_COLOR COLOR_WHITE

// Level
#define MAX_SPIKES 20
#define MAX_BLOCKS 40


#endif