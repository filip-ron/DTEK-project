#include "graphics.h"
#include "config.h"


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

void graphics_swap_buffers(void) {
    
    vga_control[VGA_BUFFER_REG] = 0;

    while(vga_control[VGA_STATUS_REG] & 1) {}

    draw_buffer = (volatile uint8_t *) vga_control[VGA_BACKBUFFER_REG];
}

void graphics_render(const GameState *state) {
    
    graphics_clear(BACKGROUND_COLOR); //initialize with black screen

    graphics_draw_rect(
        0, 
        GROUND_Y, 
        VGA_WIDTH, 
        GROUND_HEIGHT, 
        GROUND_COLOR
    ); //Start creating ground in green color

    graphics_draw_rect(
        state->player.x, 
        state->player.y, 
        state->player.width, 
        state->player.height, 
        PLAYER_COLOR
    ); //Draw the player

    for(int i = 0; i < state->spike_count; i++) {
        int screen_x = state->spikes[i].x - state->camera_x;  //Finds the relative position of each spike from its world position minus where the camera is

        if((screen_x + state->spikes[i].width) <= 0 || screen_x >= VGA_WIDTH) {
            continue;                  // Skip spikes outside the visible screen
        }

        graphics_draw_triangle(
            screen_x, 
            state->spikes[i].y, 
            state->spikes[i].width, 
            state->spikes[i].height, 
            SPIKE_COLOR, 
            state->spikes[i].upside_down
        ); //Draw the spikes
    }

    for(int i = 0; i < state->block_count; i++) {
        int screen_x = state->blocks[i].x - state->camera_x;

        if((screen_x + state->blocks[i].width) <= 0 || screen_x >= VGA_WIDTH) {
            continue;
        }

        graphics_draw_rect(
            screen_x,
            state->blocks[i].y,
            state->blocks[i].width,
            state->blocks[i].height,
            BLOCK_COLOR
        );
    }
}
