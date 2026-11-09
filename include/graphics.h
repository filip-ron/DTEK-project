#ifndef GRAPHICS_H
#define GRAPhICS_H

#include <stdint.h>
#include <stdbool.h>

void graphics_init(void);
void graphics_clear(uint8_t color);
void graphics_draw_pixel(int x, int y, uint8_t color);
void graphics_draw_rect(int x, int y, int width, int height, uint8_t color);
void graphics_draw_triangle(int x, int y, int width, int height, uint8_t color, bool upside_down);
void graphics_swap_buffers(void);

#endif