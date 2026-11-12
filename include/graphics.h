#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <stdbool.h>
#include "game.h"

void graphics_init(void);
void graphics_clear(uint8_t color);
void graphics_draw_pixel(int x, int y, uint8_t color);
void graphics_draw_rect(int x, int y, int width, int height, uint8_t color);
void graphics_draw_triangle(int x, int y, int width, int height, uint8_t color, bool upside_down);
void graphics_swap_buffers(void);
void graphics_render(const GameState *state);

#ifdef GRAPHICS_TEST
uint8_t graphics_get_pixel(int x, int y);
#endif

#endif