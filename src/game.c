#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>
#include <stdlib.h>
#include "int.h"
#include "globals.h"
#include "level_data.h"
#include "sprites.h"
#include "text_interface.h"

uint8_t lifes;
unsigned char lifes_buffer[2]; // for printing the score with padded numbers
uint8_t level;
unsigned char level_buffer[2]; // for printing the score with padded numbers
uint16_t score;
unsigned char score_buffer[6]; // for printing the score with padded numbers
uint8_t bullet_cooldown;
uint8_t levelstart_cooldown;
uint8_t in_game;
struct level_definition current_level;
uint8_t collision_with_tile;
unsigned char key_pressed;

// Load level sprites
void load_sprites() {

    // create pad sprite
    create_pad();

    // create ball sprite
    create_ball();
}

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
    bullet_cooldown = 255;
    levelstart_cooldown = 255;
    in_game = 1;
    collision_with_tile=0;
}

// draw the current level
void draw_level() {

    // Clear the game area
    sp1_Invalidate(&game_area);
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
    sp1_SetPrintPos(&game_text_area, 0, 5);
    sp1_PrintString(&game_text_area, paused_text);
    sp1_UpdateNow();
    in_wait_nokey();
    in_wait_key();
    in_wait_nokey();

    // clear text
    sp1_ClearRectInv(&game_text_rect, INK_WHITE | PAPER_BLACK, ' ', SP1_RFLAG_TILE | SP1_RFLAG_COLOUR);
    sp1_UpdateNow();
}

// request confirmation and abort the game if the player wants to
void abort_game() {

    
}

// Upadte the score text
void update_score(uint16_t value) {
    
    score+=value;

    pad_numbers(score_buffer, 6, score);
    sp1_SetPrintPos(&game_score_area, 0, 0);
    sp1_PrintString(&game_score_area, score_buffer);
}

// Update the lifes text
void update_lifes() {
    
    pad_numbers(lifes_buffer, 2, lifes);
    sp1_SetPrintPos(&game_lifes_area, 0, 0);
    sp1_PrintString(&game_lifes_area, lifes_buffer);
}

// Update the level text
void update_level() {
    
    pad_numbers(level_buffer, 2, level + 1);
    sp1_SetPrintPos(&game_level_area, 0, 0);
    sp1_PrintString(&game_level_area, level_buffer);
}

// Play the game
void play_game() {

    init_gameplay();

    draw_level();

    position_sprites();

    update_pad();
    update_ball();
    
    // write initial text information
    update_score(score);
    update_lifes();
    update_level();

    wait();
    sp1_UpdateNow();
    
    // show Ready...?
    sp1_SetPrintPos(&game_text_area, 0, 7);
    sp1_PrintString(&game_text_area, ready_text);
    sp1_UpdateNow();
    wait_seconds(2);
    // clear Ready...?
    sp1_ClearRectInv(&game_text_rect, INK_WHITE | PAPER_BLACK, ' ', SP1_RFLAG_TILE | SP1_RFLAG_COLOUR);
    sp1_UpdateNow();

    while(in_game) {

        // get player input
        if (in_key_pressed( game_keys.left )) {
            // Left, move pad
            pad_sprite.pos.x -= pad_sprite.velocity;
            update_pad();
            
        } else if(in_key_pressed( game_keys.right )) {
            // OR right, move pad
            pad_sprite.pos.x += pad_sprite.velocity;
            update_pad();
        }
        if(in_key_pressed( game_keys.fire )) {
            // Fire
            
        }
        if(in_key_pressed( game_keys.pause )) pause_game(); // Pause the gamme
        if(in_key_pressed( game_keys.abort )) { // Abort the game
            
        } 
            
        if (levelstart_cooldown == 0) {
            // we're in action

            // Move the ball
            move_ball();

            // Check for ball collisions
            if(collision_ball_with_pad()==1) {
                // collision with pad
            }
            if(collision_ball_with_screen() == 1) {
                // collision with screen
            }
            collision_with_tile = collision_ball_with_tile();
            if(collision_with_tile > 0) {

                if(collision_with_tile == 1) { // tile destroyed

                    update_score(1);
                } else {
                    if(collision_with_tile == 2) { // needs one more hit

                    } else {    // indestructable

                    }
                }
                
            }

            // if(collision_ball_die()==1) { // lost a life

            // }

            update_ball();

            // Check for death condition
                // check for ball hitting down
            
            // check for winning condition
                // check for no more tiles

        } else {
            // in start of level cooldown (overrided by fire)
            levelstart_cooldown--;
            if(in_key_pressed( game_keys.fire )) {
                levelstart_cooldown = 0;
            }
            ball_sprite.pos.x = pad_sprite.pos.x + 10;
            update_ball();
        }

        // wait for vsync and update the screen
        wait();
        sp1_UpdateNow();
    }

    // Move sprites outside screen
    sp1_MoveSprPix(pad_sprite.s, &game_area, 0, 0, 0);
    sp1_MoveSprPix(ball_sprite.s, &game_area, 0, 0, 0);

    // get back to the menu
}

