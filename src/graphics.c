#include "graphics.h"
#include "config.h"


#define FRAME_A_ADDRESS 0x08000000
#define FRAME_B_ADDRESS 0x08012C00

#define VGA_DMA_ADDRESS 0x04000100

#define VGA_BUFFER_REG 0
#define VGA_BACKBUFFER_REG 1
#define VGA_RESOLUTION_REG 2
#define VGA_STATUS_REG 3


#ifdef GRAPHICS_TEST
static uint8_t test_buffer[VGA_WIDTH * VGA_HEIGHT];
static volatile uint8_t *draw_buffer = test_buffer;
#else 
static volatile uint8_t *draw_buffer;
static volatile uint32_t *vga_control = (volatile uint32_t *) VGA_DMA_ADDRESS;
#endif



void graphics_draw_pixel(int x, int y, uint8_t color) {
    if (x < 0 || x >= VGA_WIDTH || y < 0 || y >= VGA_HEIGHT) {
        return;
    }

    draw_buffer[y * VGA_WIDTH + x] = color;
}

#ifdef GRAPHICS_TEST
uint8_t graphics_get_pixel(int x, int y) {
    if (x < 0 || x >= VGA_WIDTH || y < 0 || y >= VGA_HEIGHT) {
        return 0;
    }

    return draw_buffer[y * VGA_WIDTH + x];
}
#endif

void graphics_clear(uint8_t color) {
    for(int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        draw_buffer[i] = color;
    }
}

void graphics_draw_rect(int x, int y, int width, int height, uint8_t color) {
    if(width <= 0 || height <= 0) { 
        return;
    }

    //Calculate rectangle boundaries
    int x_start = x;
    int y_start = y;
    int x_end = x + width;
    int y_end = y + height;

    //Clip to screen boundaries
    if(x_start < 0) x_start = 0;
    if(y_start < 0) y_start = 0;
    if(x_end > VGA_WIDTH) x_end = VGA_WIDTH;
    if(y_end > VGA_HEIGHT) y_end = VGA_HEIGHT;

    //Rectangle is completely outside the screen
    if((x_start >= x_end) ||(y_start >= y_end)) {
        return;
    }

    //Draw directly into framebuffer
    for( int py = y_start; py < y_end; py++) {
        int row_offset = py * VGA_WIDTH;

        for(int px = x_start; px < x_end; px++) {
            draw_buffer[row_offset + px] = color;
        }
    }
}

static void graphics_draw_horizontal_line(int x, int y, int width, uint8_t color) {
    if( width <= 0 || y < 0 || y >= VGA_HEIGHT) {
        return;
    }

    int x_start = x;
    int x_end = x + width;

    if(x_start < 0) x_start = 0;
    if(x_end > VGA_WIDTH) x_end = VGA_WIDTH;
    if(x_start >= x_end) return;

    int row_offset = y * VGA_WIDTH;

    for(int px = x_start; px < x_end; px++) {
        draw_buffer[row_offset + px] = color;
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

       graphics_draw_horizontal_line(x + offsetX, y + dy, rowWidth, color);
    }
}

void graphics_init(void) {
#ifdef GRAPHICS_TEST
    draw_buffer = test_buffer;
#else
    
    vga_control[VGA_BACKBUFFER_REG] = FRAME_A_ADDRESS;
    vga_control[VGA_BUFFER_REG] = 0;

    while(vga_control[VGA_STATUS_REG] & 1) {}

    vga_control[VGA_BACKBUFFER_REG] = FRAME_B_ADDRESS;
    draw_buffer = (volatile uint8_t *) FRAME_B_ADDRESS;
#endif

}

void graphics_swap_buffers(void) {
#ifndef GRAPHICS_TEST    
    vga_control[VGA_BUFFER_REG] = 0;

    while(vga_control[VGA_STATUS_REG] & 1) {}

    draw_buffer = (volatile uint8_t *) vga_control[VGA_BACKBUFFER_REG];
#endif
}

void graphics_render(const GameState *state) {
    
    graphics_clear(COLOR_BACKGROUND); //initialize with black screen

    graphics_draw_rect(
        0, 
        GROUND_Y, 
        VGA_WIDTH, 
        GROUND_HEIGHT, 
        COLOR_GROUND
    ); //Start creating ground 

    graphics_draw_rect(
        0,
        GROUND_Y,
        VGA_WIDTH,
        2,
        COLOR_GROUND_LINE
    );



    graphics_draw_rect(
        state->player.x, 
        state->player.y, 
        state->player.width, 
        state->player.height, 
        COLOR_PLAYER_BORDER
    ); //Draw the player 

    graphics_draw_rect(
        state->player.x + PLAYER_BORDER_WIDTH, 
        state->player.y + PLAYER_BORDER_WIDTH, 
        state->player.width - (2 * PLAYER_BORDER_WIDTH), 
        state->player.height - (2 * PLAYER_BORDER_WIDTH), 
        COLOR_PLAYER
    ); 


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
            COLOR_SPIKE_BORDER, 
            state->spikes[i].upside_down
        ); //Draw the spikes

        graphics_draw_triangle(
            screen_x + SPIKE_BORDER_WIDTH,
            state->spikes[i].y + SPIKE_BORDER_WIDTH,
            state->spikes[i].width - (2 * SPIKE_BORDER_WIDTH),
            state->spikes[i].height - (2 * SPIKE_BORDER_WIDTH),
            COLOR_SPIKE,
            state->spikes[i].upside_down
        );
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
            COLOR_BLOCK
        );
    }
}
