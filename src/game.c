#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>
#include <stdlib.h>
#include "int.h"
#include "globals.h"
#include "level_data.h"
#include "sprites.h"
#include "text_interface.h"

// bonus
// bullets
// sounds
// animations (tiles, pad?)
// enemies
// hi-score


uint8_t lifes;
uint8_t level;
uint16_t score;
unsigned char info_buffer[6]; // for printing the score, lifes, levels with padded numbers
uint8_t bullet_cooldown;
uint8_t levelstart_cooldown;
uint8_t in_game;
struct level_definition current_level;
unsigned char key_pressed;

// Load level sprites
void load_sprites() {

    // create pad sprite
    create_pad();

    // create ball sprite
    create_ball();
}

// initialize graphics
void init_graphics() {

    load_sprites();

    sp1_TileEntry('a', tilel_gfx);
    sp1_TileEntry('b', tiler_gfx);
}

// Initialize all variables
void init_gameplay() {

    lifes = 4;
    level = 0;
    score = 0;
    bullet_cooldown = 0;
    in_game = 1;
}

// draw the current level
void draw_level() {

    // Clear the game area
    sp1_ClearRectInv(&game_area, INK_WHITE | PAPER_BLACK, ' ', SP1_RFLAG_TILE | SP1_RFLAG_COLOUR);
    sp1_UpdateNow();

    uint8_t t;                                      // the tile number being read
    uint16_t x = 1;
    uint16_t y;

    // TODO clear level data
    current_level.total_tiles = 0;
    
    // load level
    for (uint8_t l = 0; l < MAX_LINES; l++)         // for each line
    {

        y=1;

        for (uint8_t c = 0; c < MAX_COLS; c++)      // for each columns
        {

            t = level_data[level][l][c];            // read the tile number
            if(t != 0) {                            // we have a tile

                current_level.tiles[current_level.total_tiles].char_pos.x = x;  // LINE
                current_level.tiles[current_level.total_tiles].char_pos.y = y;  // COL
                current_level.tiles[current_level.total_tiles].pixel_pos.x = current_level.tiles[current_level.total_tiles].char_pos.y*8;
                current_level.tiles[current_level.total_tiles].pixel_pos.y = current_level.tiles[current_level.total_tiles].char_pos.x*8;
                current_level.tiles[current_level.total_tiles].color = tile_color[t-1];

                if(t < 8 ) {                        // is it normal?
                    current_level.tiles[current_level.total_tiles].type = 1;
                    current_level.tiles[current_level.total_tiles].hit = 1;
                } else {

                    if(t == 8 ) {                   // is it indestructable?

                        current_level.tiles[current_level.total_tiles].type = 3;
                        current_level.tiles[current_level.total_tiles].hit = 0;
                        
                    } else {                        // ah, it's just difficult to kill (2 hits)
                        
                        current_level.tiles[current_level.total_tiles].type = 2;
                        current_level.tiles[current_level.total_tiles].hit = 2;
                    }
                    
                }

                current_level.total_tiles++;

            }

            y+=2;

        }

        x++;

    }

    // draw the level
    for (uint8_t i = 0; i < current_level.total_tiles; i++)
    {
        sp1_PrintAtInv(current_level.tiles[i].char_pos.x, current_level.tiles[i].char_pos.y, current_level.tiles[i].color, 'a');
        sp1_PrintAtInv(current_level.tiles[i].char_pos.x, current_level.tiles[i].char_pos.y + 1, current_level.tiles[i].color, 'b');
    }

}

// position the sprites for level start
void position_sprites() {

    // position ball and pad for beginning of the level
    position_sprites_init();
}

// Pause the game until the pause key is pressed again
void pause_game() {

    // show text
    show_message(paused_text, 0, 5, 0);
}

// request confirmation and abort the game if the player wants to
void abort_game() {

    // show Ready...?
    show_message(quit_text, 0, 5, 0);
    if(key_pressed == 'y') {
        in_game = 0;
    }
}

// Upadte the score text
void update_score(uint16_t value) {
    
    score+=value;

    pad_numbers(info_buffer, 6, score);
    sp1_SetPrintPos(&game_score_area, 0, 0);
    sp1_PrintString(&game_score_area, info_buffer);
}

// Update the lifes text
void update_lifes() {
    
    pad_numbers(info_buffer, 2, lifes);
    sp1_SetPrintPos(&game_lifes_area, 0, 0);
    sp1_PrintString(&game_lifes_area, info_buffer);
}

// Update the level text
void update_level() {
    
    pad_numbers(info_buffer, 2, level + 1);
    sp1_SetPrintPos(&game_level_area, 0, 0);
    sp1_PrintString(&game_level_area, info_buffer);
}

// start/restart level
void restart_level() {

    position_sprites();

    update_pad();
    update_ball();
}

// Get pad movement only
void get_player_movement() {

    if (in_key_pressed( game_keys.left )) {
        // Left, move pad
        pad_sprite.pos.x -= pad_sprite.velocity;
        update_pad();
            
    } else if(in_key_pressed( game_keys.right )) {
        // OR right, move pad
        pad_sprite.pos.x += pad_sprite.velocity;
        update_pad();
    }

}

// Cooldown time at the beginning of each level
void cooldown() {

    levelstart_cooldown=255;

    while (levelstart_cooldown > 0) {

        get_player_movement();

        levelstart_cooldown--;
        if(in_key_pressed( game_keys.fire )) {
            levelstart_cooldown = 0;
        }
        ball_sprite.pos.x = pad_sprite.pos.x + 10;
        update_ball();

        wait();
        sp1_UpdateNow();
    }
}

// Play the game
void play_game() {

    init_gameplay();

    draw_level();

    restart_level();
    
    // write initial text information
    update_score(score);
    update_lifes();
    update_level();

    wait();
    sp1_UpdateNow();
    
    // show Ready...?
    show_message(ready_text, 0, 7, 2);

    cooldown();

    while(in_game) {
 
        // get player input
        get_player_movement();
        if(in_key_pressed( game_keys.fire )) {
            // Fire
        }
        if(in_key_pressed( game_keys.pause )) pause_game(); // Pause the gamme
        if(in_key_pressed( game_keys.abort )) abort_game(); // Abort the game

        // Move the ball
        move_ball();

        // Check for ball collisions
        collision_ball_with_pad(in_key_pressed( game_keys.left ), in_key_pressed( game_keys.right ));
        if(collision_type > 0) {

        }
        collision_ball_with_screen();
        if(collision_type > 0) {

            if(collision_type == 1) {
                // Play sound?
                
            } else {
                // Lost a life

                lifes-=1;
                update_lifes();
                move_sprites_outside(); // animate pad perhaps?
                if(lifes > 0) { // just a life lost
                    show_message(life_lost, 0, 3, 5);

                    restart_level();

                    show_message(ready_text, 0, 7, 2);

                    cooldown();
                } else {    // DIE
                    show_message(game_over, 0, 6, 6);
                    in_game=0;
                    continue;
                }

            }
        }
        collision_ball_with_tile();
        if(collision_type > 0) {

            if(collision_type == 1) { // tile destroyed
                // Play sound?
                update_score(5);
                sp1_UpdateNow();

                if(all_tiles_done() == 0) { // level completed!!

                    level++;
                    if(level == MAX_LEVELS) {
                        // GANE WIN
                        move_sprites_outside();
                        update_score(1000);
                        show_message(game_completed, 0, 1, 10);
                        in_game=0;
                        continue;
                    } else {
                        // LEVEL UP
                        move_sprites_outside();
                        update_score(50*level);
                        show_message(level_completed, 0, 3, 5);
                        update_level();
                        draw_level();
                        restart_level();
                        show_message(ready_text, 0, 7, 2);
                        cooldown();
                    }
                }

            } else {
                if(collision_type == 2) { // needs one more hit
                    // Play sound?
                } else {    // indestructable
                    // Play sound?
                }
            }
            
        }

        update_ball();

        // wait for vsync and update the screen
        wait();
        sp1_UpdateNow();
    }

    // Move sprites outside screen
    move_sprites_outside();

    // get back to the menu
}

