
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
    state.spike_count = 5;
    
    state.spikes[0] = (Spike){180, 175, 25, 25, false};
    state.spikes[1] = (Spike){300, 175, 25, 25, false};
    state.spikes[2] = (Spike){325, 175, 25, 25, false};
    state.spikes[3] = (Spike){550, 175, 25, 25, false};
    state.spikes[4] = (Spike){750, 175, 25, 25, false};

    // Block
    state.block_count = 3;
    
    state.blocks[0] = (Block){420, 170, 30, 30};
    state.blocks[1] = (Block){620, 150, 50, 50};
    state.blocks[2] = (Block){850, 170, 30, 30};

    state.camera_x = 0;

    // Initialize VGA graphics
    graphics_init();

    while (1) {
		graphics_render(&state);
        graphics_swap_buffers();

        // Move camera one pixel forward
        state.camera_x++;

        // Restart the simulation
        if(state.camera_x >= 900) {
            state.camera_x = 0;
        }

        // Temporary delay to control scrolling speed
        for(volatile int i = 0; i < 25000; i++) {
        }
		


}

    return 0;
}
