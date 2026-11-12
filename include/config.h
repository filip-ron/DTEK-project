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



// Game object colors
#define COLOR_BACKGROUND 0x01
#define COLOR_GROUND 0x49
#define COLOR_GROUND_LINE 0x1A

#define COLOR_PLAYER 0x67
#define COLOR_PLAYER_BORDER 0x21

#define COLOR_SPIKE 0xE1
#define COLOR_SPIKE_BORDER 0xC1

#define COLOR_BLOCK 0x45

#define PLAYER_BORDER_WIDTH 2
#define SPIKE_BORDER_WIDTH 1



// Level
#define MAX_SPIKES 20
#define MAX_BLOCKS 40


#endif