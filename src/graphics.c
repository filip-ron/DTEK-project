#include "graphics.h"

#define VGA_WIDTH 320
#define VGA_HEIGHT 240

#define FRAME_A_ADDRESS 0x08000000
#define FRAME_B_ADDRESS 0x08012C00

#define VGA_DMA_ADDRESS 0x04000100

#define VGA_BUFFER_REG 0
#define VGA_BACKBUFFER_REG 1
#define VGA_RESOLUTION_REG 2
#define VGA_STATUS_REG 3

static volatile uint8_t *draw_buffer;
static volatile uint32_t *vga_control = (volatile uint32_t *) VGA_DMA_ADDRESS;

void graphics_draw_pixel(int x, int y, uint8_t color) {
    if (x < 0 || x >= VGA_WIDTH || y < 0 || y >= VGA_HEIGHT) {
        return;
    }

    draw_buffer[y * VGA_WIDTH + x] = color;
}

void graphics_clear(uint8_t color) {
    for(int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        draw_buffer[i] = color;
    }
}

void graphics_draw_rect(int x, int y, int width, int height, uint8_t color) {
    for(int dy = 0; dy < height; dy++) {
        for(int dx = 0; dx < width; dx++) {
            graphics_draw_pixel(x + dx,y + dy,color);
        }
    }
}

void graphics_draw_triangle(int x, int y, int width, int height, uint8_t color, bool upside_down) {
    if(width <= 0 || height <= 0) {
        return;
    }

    if (height == 1) {
        graphics_draw_rect(x, y, width, 1, color);
        return;
    }

    for(int dy = 0; dy < height; dy++){
        int distance_from_tip = upside_down
            ? height - 1 - dy
            : dy;

        int rowWidth = 1 +  (((width - 1) * distance_from_tip) / (height - 1));
        int offsetX = (width - rowWidth) / 2;

        graphics_draw_rect(x + offsetX, y + dy, rowWidth, 1, color);
    }
}

void graphics_init(void) {
    vga_control[VGA_BACKBUFFER_REG] = FRAME_A_ADDRESS;

    vga_control[VGA_BUFFER_REG] = 0;

    while(vga_control[VGA_STATUS_REG] & 1) {}

    vga_control[VGA_BACKBUFFER_REG] = FRAME_B_ADDRESS;

    draw_buffer = (volatile uint8_t *) FRAME_B_ADDRESS;


}