#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <graphics.h>

int main(void) {
    graphics_init();

    // Start with entierly black framebuffer
    graphics_clear(0x00);

    // Test 1: Draw a rectangle fully inside the screen
    graphics_draw_rect(10, 20, 30, 15, 0xFF);

    assert(graphics_get_pixel(10, 20) == 0xFF);
    assert(graphics_get_pixel(39, 34) == 0xFF);

    // Pixels just outside the rectangle should remain black
    assert(graphics_get_pixel(40, 34) == 0x00);
    assert(graphics_get_pixel(39, 35) == 0x00);

    printf("Test 1 passed: Normal rectangle\n");

    // Test 2: Rectangle partially outside the left edge

    graphics_clear(0x00);
    graphics_draw_rect(-10, 20, 30, 10, 0xFF);

    assert(graphics_get_pixel(0, 20) == 0xFF);
    assert(graphics_get_pixel(19, 29) == 0xFF);
    assert(graphics_get_pixel(20, 20) == 0x00);

    printf("Test 2 passed: Rectangle clipping\n");

    // Test 3: REctangle completely outsde the screen
    graphics_clear(0x00);
    graphics_draw_rect(350, 20, 30, 10, 0xFF);

    assert(graphics_get_pixel(319, 20) == 0x00);

    printf("Test 3 passed: Off-screen rectangle\n");

    //Test 4: Zero-width rectangle
    graphics_clear(0x00);
    graphics_draw_rect(10, 20, 0, 10, 0xFF);

    assert(graphics_get_pixel(10, 20) == 0x00);

    printf("Test 4 passed: Zero-width rectangle\n");

    // Test 5: Normal triangle
graphics_clear(0x00);
graphics_draw_triangle(100, 50, 7, 4, 0xFF, false);

// Tip of the triangle
assert(graphics_get_pixel(103, 50) == 0xFF);

// Bottom row should be completely filled
assert(graphics_get_pixel(100, 53) == 0xFF);
assert(graphics_get_pixel(106, 53) == 0xFF);

// Outside the triangle
assert(graphics_get_pixel(100, 50) == 0x00);

printf("Test 5 passed: Normal triangle\n");

// Test 6: Upside-down triangle
graphics_clear(0x00);
graphics_draw_triangle(100, 50, 7, 4, 0xFF, true);

// Top row should be completely filled
assert(graphics_get_pixel(100, 50) == 0xFF);
assert(graphics_get_pixel(106, 50) == 0xFF);

// Tip should be at the bottom
assert(graphics_get_pixel(103, 53) == 0xFF);

// Outside the triangle
assert(graphics_get_pixel(100, 53) == 0x00);

printf("Test 6 passed: Upside-down triangle\n");

// Test 7: Partially off-screen triangle
graphics_clear(0x00);
graphics_draw_triangle(-3, 50, 7, 4, 0xFF, false);

// The bottom row extends from x = -3 to x = 3.
// Only pixels x = 0 through x = 3 are visible.
assert(graphics_get_pixel(0, 53) == 0xFF);
assert(graphics_get_pixel(3, 53) == 0xFF);
assert(graphics_get_pixel(4, 53) == 0x00);

printf("Test 7 passed: Triangle clipping\n");

// Test 8: Triangle with height 1
graphics_clear(0x00);
graphics_draw_triangle(100, 50, 7, 1, 0xFF, false);

// The triangle should draw a single horizontal row.
assert(graphics_get_pixel(100, 50) == 0xFF);
assert(graphics_get_pixel(106, 50) == 0xFF);

// No pixels should be drawn above or below.
assert(graphics_get_pixel(103, 49) == 0x00);
assert(graphics_get_pixel(103, 51) == 0x00);

printf("Test 8 passed: Single-row triangle\n");

// Test 9: Ground rendering
GameState state = {0};

graphics_render(&state);

// Above the ground should be black
assert(graphics_get_pixel(100, GROUND_Y - 1) == 0x00);

// Ground should have the correct color
assert(graphics_get_pixel(100, GROUND_Y) == 0x1C);
assert(graphics_get_pixel(100, VGA_HEIGHT - 1) == 0x1C);

printf("Test 9 passed: Ground rendering\n");

// Test 10: Player rendering
GameState player_state = {0};

player_state.player.x = 50;
player_state.player.y = 100;
player_state.player.width = 20;
player_state.player.height = 20;

graphics_render(&player_state);

// Top-left and bottom-right corners
assert(graphics_get_pixel(50, 100) == 0x03);
assert(graphics_get_pixel(69, 119) == 0x03);

// Just outside the player
assert(graphics_get_pixel(49, 100) == 0x00);
assert(graphics_get_pixel(70, 100) == 0x00);
assert(graphics_get_pixel(50, 120) == 0x00);

printf("Test 10 passed: Player rendering\n");

// Test 11: Normal spike rendering
GameState spike_state = {0};

spike_state.spike_count = 1;
spike_state.spikes[0].x = 100;
spike_state.spikes[0].y = 100;
spike_state.spikes[0].width = 7;
spike_state.spikes[0].height = 4;
spike_state.spikes[0].upside_down = false;

graphics_render(&spike_state);

// Tip and bottom corners
assert(graphics_get_pixel(103, 100) != 0x00);
assert(graphics_get_pixel(100, 103) != 0x00);
assert(graphics_get_pixel(106, 103) != 0x00);

// Outside the triangle
assert(graphics_get_pixel(100, 100) == 0x00);

printf("Test 11 passed: Normal spike rendering\n");


// Test 12: Upside-down spike rendering
GameState upside_down_state = {0};

upside_down_state.spike_count = 1;
upside_down_state.spikes[0].x = 100;
upside_down_state.spikes[0].y = 100;
upside_down_state.spikes[0].width = 7;
upside_down_state.spikes[0].height = 4;
upside_down_state.spikes[0].upside_down = true;

graphics_render(&upside_down_state);

// Top corners and bottom tip
assert(graphics_get_pixel(100, 100) != 0x00);
assert(graphics_get_pixel(106, 100) != 0x00);
assert(graphics_get_pixel(103, 103) != 0x00);

// Outside the triangle
assert(graphics_get_pixel(100, 103) == 0x00);

printf("Test 12 passed: Upside-down spike rendering\n");


// Test 13: Block rendering
GameState block_state = {0};

block_state.block_count = 1;
block_state.blocks[0].x = 100;
block_state.blocks[0].y = 100;
block_state.blocks[0].width = 20;
block_state.blocks[0].height = 20;

graphics_render(&block_state);

// Inside the block
assert(graphics_get_pixel(100, 100) != 0x00);
assert(graphics_get_pixel(119, 119) != 0x00);

// Outside the block
assert(graphics_get_pixel(99, 100) == 0x00);
assert(graphics_get_pixel(120, 100) == 0x00);

printf("Test 13 passed: Block rendering\n");


// Test 14: Camera scrolling
GameState camera_state = {0};

camera_state.camera_x = 50;

camera_state.spike_count = 1;
camera_state.spikes[0].x = 150;
camera_state.spikes[0].y = 100;
camera_state.spikes[0].width = 7;
camera_state.spikes[0].height = 4;
camera_state.spikes[0].upside_down = false;

graphics_render(&camera_state);

// World x = 150, camera x = 50
// Therefore screen x = 100
assert(graphics_get_pixel(103, 100) != 0x00);

// Spike should not appear at its original world position
assert(graphics_get_pixel(153, 100) == 0x00);

printf("Test 14 passed: Camera scrolling\n");


// Test 15: Object completely off-screen
GameState offscreen_state = {0};

offscreen_state.spike_count = 1;
offscreen_state.spikes[0].x = 400;
offscreen_state.spikes[0].y = 100;
offscreen_state.spikes[0].width = 20;
offscreen_state.spikes[0].height = 20;

graphics_render(&offscreen_state);

// Screen should remain empty above the ground
assert(graphics_get_pixel(319, 100) == 0x00);

printf("Test 15 passed: Off-screen object\n");


// Test 16: Camera scrolling with blocks
GameState camera_block_state = {0};

camera_block_state.camera_x = 80;

camera_block_state.block_count = 1;
camera_block_state.blocks[0].x = 150;
camera_block_state.blocks[0].y = 100;
camera_block_state.blocks[0].width = 20;
camera_block_state.blocks[0].height = 20;

graphics_render(&camera_block_state);

// World x = 150, camera x = 80
// Therefore screen x = 70
assert(graphics_get_pixel(70, 100) != 0x00);
assert(graphics_get_pixel(89, 119) != 0x00);

// Original world position should remain empty
assert(graphics_get_pixel(150, 100) == 0x00);

printf("Test 16 passed: Camera scrolling with blocks\n");


// Test 17: Frame clearing
GameState first_frame = {0};
GameState second_frame = {0};

// Draw a player in the first frame
first_frame.player.x = 50;
first_frame.player.y = 100;
first_frame.player.width = 20;
first_frame.player.height = 20;

graphics_render(&first_frame);
assert(graphics_get_pixel(50, 100) == 0x03);

// Render a second frame without the player
graphics_render(&second_frame);

// Previous player pixels must be cleared
assert(graphics_get_pixel(50, 100) == 0x00);

// Ground should still be present
assert(graphics_get_pixel(50, GROUND_Y) == 0x1C);

printf("Test 17 passed: Frame clearing\n");

printf("\nAll graphics tests passed!\n");

return 0;
}