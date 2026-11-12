
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "graphics.h"
#include "config.h"

int main(void) {
    graphics_init();

    // ============================================================
    // Test 1: Normal rectangle
    // ============================================================
    graphics_clear(0x00);
    graphics_draw_rect(10, 20, 30, 15, 0xFF);

    assert(graphics_get_pixel(10, 20) == 0xFF);
    assert(graphics_get_pixel(39, 34) == 0xFF);
    assert(graphics_get_pixel(40, 34) == 0x00);
    assert(graphics_get_pixel(39, 35) == 0x00);

    printf("Test 1 passed: Normal rectangle\n");


    // ============================================================
    // Test 2: Rectangle clipping
    // ============================================================
    graphics_clear(0x00);
    graphics_draw_rect(-10, 20, 30, 10, 0xFF);

    assert(graphics_get_pixel(0, 20) == 0xFF);
    assert(graphics_get_pixel(19, 29) == 0xFF);
    assert(graphics_get_pixel(20, 20) == 0x00);

    printf("Test 2 passed: Rectangle clipping\n");


    // ============================================================
    // Test 3: Completely off-screen rectangle
    // ============================================================
    graphics_clear(0x00);
    graphics_draw_rect(350, 20, 30, 10, 0xFF);

    assert(graphics_get_pixel(319, 20) == 0x00);

    printf("Test 3 passed: Off-screen rectangle\n");


    // ============================================================
    // Test 4: Zero-width rectangle
    // ============================================================
    graphics_clear(0x00);
    graphics_draw_rect(10, 20, 0, 10, 0xFF);

    assert(graphics_get_pixel(10, 20) == 0x00);

    printf("Test 4 passed: Zero-width rectangle\n");


    // ============================================================
    // Test 5: Normal triangle
    // ============================================================
    graphics_clear(0x00);
    graphics_draw_triangle(100, 50, 7, 4, 0xFF, false);

    assert(graphics_get_pixel(103, 50) == 0xFF);
    assert(graphics_get_pixel(100, 53) == 0xFF);
    assert(graphics_get_pixel(106, 53) == 0xFF);
    assert(graphics_get_pixel(100, 50) == 0x00);

    printf("Test 5 passed: Normal triangle\n");


    // ============================================================
    // Test 6: Upside-down triangle
    // ============================================================
    graphics_clear(0x00);
    graphics_draw_triangle(100, 50, 7, 4, 0xFF, true);

    assert(graphics_get_pixel(100, 50) == 0xFF);
    assert(graphics_get_pixel(106, 50) == 0xFF);
    assert(graphics_get_pixel(103, 53) == 0xFF);
    assert(graphics_get_pixel(100, 53) == 0x00);

    printf("Test 6 passed: Upside-down triangle\n");


    // ============================================================
    // Test 7: Triangle clipping
    // ============================================================
    graphics_clear(0x00);
    graphics_draw_triangle(-3, 50, 7, 4, 0xFF, false);

    assert(graphics_get_pixel(0, 53) == 0xFF);
    assert(graphics_get_pixel(3, 53) == 0xFF);
    assert(graphics_get_pixel(4, 53) == 0x00);

    printf("Test 7 passed: Triangle clipping\n");


    // ============================================================
    // Test 8: Single-row triangle
    // ============================================================
    graphics_clear(0x00);
    graphics_draw_triangle(100, 50, 7, 1, 0xFF, false);

    assert(graphics_get_pixel(100, 50) == 0xFF);
    assert(graphics_get_pixel(106, 50) == 0xFF);
    assert(graphics_get_pixel(103, 49) == 0x00);
    assert(graphics_get_pixel(103, 51) == 0x00);

    printf("Test 8 passed: Single-row triangle\n");


    // ============================================================
    // Test 9: Ground rendering and highlight
    // ============================================================
    GameState ground_state = {0};

    graphics_render(&ground_state);

    // Background above the ground
    assert(graphics_get_pixel(100, GROUND_Y - 1)
           == COLOR_BACKGROUND);

    // Two-pixel ground highlight
    assert(graphics_get_pixel(100, GROUND_Y)
           == COLOR_GROUND_LINE);
    assert(graphics_get_pixel(100, GROUND_Y + 1)
           == COLOR_GROUND_LINE);

    // Regular ground below the highlight
    assert(graphics_get_pixel(100, GROUND_Y + 2)
           == COLOR_GROUND);
    assert(graphics_get_pixel(100, VGA_HEIGHT - 1)
           == COLOR_GROUND);

    printf("Test 9 passed: Ground rendering and highlight\n");


    // ============================================================
    // Test 10: Player rendering with border
    // ============================================================
    GameState player_state = {0};

    player_state.player.x = 50;
    player_state.player.y = 100;
    player_state.player.width = 20;
    player_state.player.height = 20;

    graphics_render(&player_state);

    // Outer border
    assert(graphics_get_pixel(50, 100)
           == COLOR_PLAYER_BORDER);
    assert(graphics_get_pixel(69, 119)
           == COLOR_PLAYER_BORDER);
    assert(graphics_get_pixel(51, 110)
           == COLOR_PLAYER_BORDER);

    // Bright interior
    assert(graphics_get_pixel(52, 102)
           == COLOR_PLAYER);
    assert(graphics_get_pixel(67, 117)
           == COLOR_PLAYER);
    assert(graphics_get_pixel(60, 110)
           == COLOR_PLAYER);

    // Outside player
    assert(graphics_get_pixel(49, 100)
           == COLOR_BACKGROUND);
    assert(graphics_get_pixel(70, 100)
           == COLOR_BACKGROUND);
    assert(graphics_get_pixel(50, 120)
           == COLOR_BACKGROUND);

    printf("Test 10 passed: Player rendering with border\n");


    // ============================================================
    // Test 11: Normal spike with border
    // ============================================================
    GameState spike_state = {0};

    spike_state.spike_count = 1;
    spike_state.spikes[0].x = 100;
    spike_state.spikes[0].y = 100;
    spike_state.spikes[0].width = 7;
    spike_state.spikes[0].height = 4;
    spike_state.spikes[0].upside_down = false;

    graphics_render(&spike_state);

    // Outer triangle border
    assert(graphics_get_pixel(103, 100)
           == COLOR_SPIKE_BORDER);
    assert(graphics_get_pixel(100, 103)
           == COLOR_SPIKE_BORDER);
    assert(graphics_get_pixel(106, 103)
           == COLOR_SPIKE_BORDER);

    // Inner triangle
    assert(graphics_get_pixel(103, 102)
           == COLOR_SPIKE);

    // Outside triangle
    assert(graphics_get_pixel(100, 100)
           == COLOR_BACKGROUND);

    printf("Test 11 passed: Normal spike with border\n");


    // ============================================================
    // Test 12: Upside-down spike with border
    // ============================================================
    GameState upside_down_state = {0};

    upside_down_state.spike_count = 1;
    upside_down_state.spikes[0].x = 100;
    upside_down_state.spikes[0].y = 100;
    upside_down_state.spikes[0].width = 7;
    upside_down_state.spikes[0].height = 4;
    upside_down_state.spikes[0].upside_down = true;

    graphics_render(&upside_down_state);

    // Outer triangle border
    assert(graphics_get_pixel(100, 100)
           == COLOR_SPIKE_BORDER);
    assert(graphics_get_pixel(106, 100)
           == COLOR_SPIKE_BORDER);
    assert(graphics_get_pixel(103, 103)
           == COLOR_SPIKE_BORDER);

    // Inner triangle
    assert(graphics_get_pixel(103, 101)
           == COLOR_SPIKE);

    // Outside triangle
    assert(graphics_get_pixel(100, 103)
           == COLOR_BACKGROUND);

    printf("Test 12 passed: Upside-down spike with border\n");


    // ============================================================
    // Test 13: Block rendering
    // ============================================================
    GameState block_state = {0};

    block_state.block_count = 1;
    block_state.blocks[0].x = 100;
    block_state.blocks[0].y = 100;
    block_state.blocks[0].width = 20;
    block_state.blocks[0].height = 20;

    graphics_render(&block_state);

    // Inside block
    assert(graphics_get_pixel(100, 100) == COLOR_BLOCK);
    assert(graphics_get_pixel(119, 119) == COLOR_BLOCK);

    // Outside block
    assert(graphics_get_pixel(99, 100)
           == COLOR_BACKGROUND);
    assert(graphics_get_pixel(120, 100)
           == COLOR_BACKGROUND);

    printf("Test 13 passed: Block rendering\n");


    // ============================================================
    // Test 14: Camera scrolling with spikes
    // ============================================================
    GameState camera_state = {0};

    camera_state.camera_x = 50;
    camera_state.spike_count = 1;
    camera_state.spikes[0].x = 150;
    camera_state.spikes[0].y = 100;
    camera_state.spikes[0].width = 7;
    camera_state.spikes[0].height = 4;
    camera_state.spikes[0].upside_down = false;

    graphics_render(&camera_state);

    // World x = 150, camera x = 50 -> screen x = 100
    assert(graphics_get_pixel(103, 100)
           == COLOR_SPIKE_BORDER);

    // Original world position should be empty
    assert(graphics_get_pixel(153, 100)
           == COLOR_BACKGROUND);

    printf("Test 14 passed: Camera scrolling with spikes\n");


    // ============================================================
    // Test 15: Completely off-screen spike
    // ============================================================
    GameState offscreen_state = {0};

    offscreen_state.spike_count = 1;
    offscreen_state.spikes[0].x = 400;
    offscreen_state.spikes[0].y = 100;
    offscreen_state.spikes[0].width = 20;
    offscreen_state.spikes[0].height = 20;

    graphics_render(&offscreen_state);

    assert(graphics_get_pixel(319, 100)
           == COLOR_BACKGROUND);

    printf("Test 15 passed: Off-screen spike\n");


    // ============================================================
    // Test 16: Camera scrolling with blocks
    // ============================================================
    GameState camera_block_state = {0};

    camera_block_state.camera_x = 80;
    camera_block_state.block_count = 1;
    camera_block_state.blocks[0].x = 150;
    camera_block_state.blocks[0].y = 100;
    camera_block_state.blocks[0].width = 20;
    camera_block_state.blocks[0].height = 20;

    graphics_render(&camera_block_state);

    // World x = 150, camera x = 80 -> screen x = 70
    assert(graphics_get_pixel(70, 100) == COLOR_BLOCK);
    assert(graphics_get_pixel(89, 119) == COLOR_BLOCK);

    // Original world position should be empty
    assert(graphics_get_pixel(150, 100)
           == COLOR_BACKGROUND);

    printf("Test 16 passed: Camera scrolling with blocks\n");


    // ============================================================
    // Test 17: Frame clearing
    // ============================================================
    GameState first_frame = {0};
    GameState second_frame = {0};

    first_frame.player.x = 50;
    first_frame.player.y = 100;
    first_frame.player.width = 20;
    first_frame.player.height = 20;

    graphics_render(&first_frame);

    // Player exists in first frame
    assert(graphics_get_pixel(50, 100)
           == COLOR_PLAYER_BORDER);
    assert(graphics_get_pixel(60, 110)
           == COLOR_PLAYER);

    // Render second frame without a player
    graphics_render(&second_frame);

    // Old player pixels should now be background
    assert(graphics_get_pixel(50, 100)
           == COLOR_BACKGROUND);
    assert(graphics_get_pixel(60, 110)
           == COLOR_BACKGROUND);

    // Ground should still exist
    assert(graphics_get_pixel(50, GROUND_Y)
           == COLOR_GROUND_LINE);

    printf("Test 17 passed: Frame clearing\n");


    printf("\nAll 17 graphics tests passed!\n");

    return 0;
}
