
#include <stdbool.h>
#include "graphics.h"
#include "game.h"

void handle_exception(void) {
    while (1) {}
}

void handle_interrupt(void) {
    while (1) {}
}

int main(void) {
    GameState state;

    // Player
    state.player.x = 50;
    state.player.y = 180;
    state.player.width = 20;
    state.player.height = 20;

    // Spike
    state.spike_count = 1;
    state.spikes[0].x = 180;
    state.spikes[0].y = 175;
    state.spikes[0].width = 30;
    state.spikes[0].height = 25;
    state.spikes[0].upside_down = false;

    // Block
    state.block_count = 1;
    state.blocks[0].x = 240;
    state.blocks[0].y = 170;
    state.blocks[0].width = 30;
    state.blocks[0].height = 30;

    state.camera_x = 0;

    // Initialize VGA graphics
    graphics_init();

    // Render the test scene
    graphics_render(&state);

    // Display the rendered frame
    graphics_swap_buffers();

    // Keep the program running
    while (1) {}

    return 0;
}
