#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include "globals.h"

// Declare sprites
struct sprite_gfx pad_sprite;
struct sprite_gfx ball_sprite;

uint8_t has_collision;
uint8_t has_v_collision;
uint8_t collision_type;

void collision_pad_with_screen();

// Create the pad sprite
void create_pad() {

    pad_sprite.s = sp1_CreateSpr(SP1_DRAW_OR1LB, SP1_TYPE_1BYTE, 2, (int)padl_gfx, 0);      // Left side
    sp1_AddColSpr(pad_sprite.s, SP1_DRAW_OR1, SP1_TYPE_1BYTE, (int)padm_gfx, 0);            // middle part
    sp1_AddColSpr(pad_sprite.s, SP1_DRAW_OR1, SP1_TYPE_1BYTE, (int)padr_gfx, 0);            // right side
    sp1_AddColSpr(pad_sprite.s, SP1_DRAW_OR1RB, SP1_TYPE_1BYTE, 0, 0);                      // buffer 8*

    pad_sprite.velocity = 3;
    pad_sprite.color = INK_WHITE;
    pad_sprite.dim.w = 24;          // Dinamic calc: (pad_sprite.s->width-1)*8 - pad_sprite.s->xthresh+1;
    pad_sprite.dim.h = 8;           //               (pad_sprite.s->height-1)*8 - pad_sprite.s->ythresh+1;
}

// Update the pad sprite position
void update_pad() {

    collision_pad_with_screen();                                                            // check for collisions
    sp1_MoveSprPix(pad_sprite.s, &game_area, 0, pad_sprite.pos.x, pad_sprite.pos.y);        // show sprinte in new position
}

// Create the ball sprite and initialize some of the sprite properties
void create_ball() {

    ball_sprite.s = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 2, (int)ball_gfx, 1);
    sp1_AddColSpr(ball_sprite.s, SP1_DRAW_MASK2RB, SP1_TYPE_2BYTE, 0, 0);

    ball_sprite.dim.w = 8;          // Dinamic calc: (pad_sprite.s->width-1)*8 - pad_sprite.s->xthresh+1;
    ball_sprite.dim.h = 8;          //               (pad_sprite.s->height-1)*8 - pad_sprite.s->ythresh+1;
    pad_sprite.color = INK_WHITE;
}

// Update the ball sprite position
void update_ball() {

    sp1_MoveSprPix(ball_sprite.s, &game_area, 0, ball_sprite.pos.x, ball_sprite.pos.y);     // show sprinte in new position
}

// Position the sprites for beginning of the level
void position_sprites_init() {

    // reset the pad
    pad_sprite.pos.x = PAD_START_POS_X;
    pad_sprite.pos.y = PAD_START_POS_Y;

    // reset the ball
    ball_sprite.vel.x = 2;
    ball_sprite.vel.y = 3;
    ball_sprite.dir.dh = 0;
    ball_sprite.dir.dv = 0;
    ball_sprite.velocity = 0;
    ball_sprite.pos.x = pad_sprite.pos.x + 10;
    ball_sprite.pos.y = pad_sprite.pos.y - 8;
}

// Move the ball taking the direction into consideration
void move_ball() {
    if(ball_sprite.dir.dh == 0) {
        ball_sprite.pos.x += ball_sprite.vel.x + ball_sprite.velocity;
    } else {
        ball_sprite.pos.x -= ball_sprite.vel.x + ball_sprite.velocity;
    }

    if(ball_sprite.dir.dv == 0) {
        ball_sprite.pos.y -= ball_sprite.vel.y + ball_sprite.velocity;
    } else {
        ball_sprite.pos.y += ball_sprite.vel.y + ball_sprite.velocity;
    }
}

uint8_t update_tile_hit(uint8_t tile_index) {

    collision_type = 1;

    current_level.tiles[tile_index].hit--;
    if(current_level.tiles[tile_index].hit == 0 && current_level.tiles[tile_index].type < 3) {
        sp1_PrintAtInv(current_level.tiles[tile_index].char_pos.x, current_level.tiles[tile_index].char_pos.y, INK_WHITE, ' ');
        sp1_PrintAtInv(current_level.tiles[tile_index].char_pos.x, current_level.tiles[tile_index].char_pos.y + 1, INK_WHITE, ' ');
    } else {
        collision_type = 2;
    }
    
    return collision_type;
}

// check if pad collides with creen limits
void collision_pad_with_screen() {

    if (pad_sprite.pos.x < MAX_SCR_LEFT) {                                  // collided with left side
        pad_sprite.pos.x = MAX_SCR_LEFT;                                    // reposition pad
    } else {
        if (pad_sprite.pos.x + pad_sprite.dim.w > MAX_SCR_RIGHT) {          // collided with right side
            pad_sprite.pos.x = MAX_SCR_RIGHT - pad_sprite.dim.w;            // reposition pad
        }
    }
}

// check if ball collides with screen limits
uint8_t collision_ball_with_screen() {
    
    has_collision = 0;

    if (ball_sprite.pos.x < MAX_SCR_LEFT) {                                     // collided with left side
        ball_sprite.pos.x = MAX_SCR_LEFT;                                       // reposition ball
        ball_sprite.dir.dh = 0;                                                 // flag the new direction
        has_collision = 1;
    } else {
        if (ball_sprite.pos.x + ball_sprite.dim.w > MAX_SCR_RIGHT) {
            ball_sprite.pos.x = MAX_SCR_RIGHT - ball_sprite.dim.w;
            ball_sprite.dir.dh = 1;
            has_collision = 1;
        }
    }
    if (ball_sprite.pos.y < MAX_SCR_TOP) {
        ball_sprite.pos.y=MAX_SCR_TOP;
        ball_sprite.dir.dv = 1;
        has_collision = 1;
    } else {

        // TO DELETE. IF BALL HITS BOTTOM IT SHOULD BE HANDLED BY collision_ball_die() OR EQUIVALENT
        if (ball_sprite.pos.y + ball_sprite.dim.h > MAX_SCR_BOTTOM) {
            ball_sprite.pos.y = MAX_SCR_BOTTOM - ball_sprite.dim.h;
            ball_sprite.dir.dv = 0;
            has_collision = 1;
        }

    }

    return has_collision;
}

// uint8_t collision_ball_die() {

//     has_collision = 0;

//     if (ball_sprite.pos.y + ball_sprite.dim.h > MAX_SCR_BOTTOM) {
//         ball_sprite.pos.y = MAX_SCR_BOTTOM - ball_sprite.dim.h;
//         ball_sprite.dir.dv = 0;
//         has_collision = 1;
//     }

//     return has_collision;

// }

// check if the ball collided with the pad
uint8_t collision_ball_with_pad() {

    has_collision = 0;

    if(ball_sprite.pos.y > PAD_START_POS_Y - ball_sprite.dim.h) {           // if the ball is at pad Y then we check for collision else skip it

        if(pad_sprite.pos.x < ball_sprite.pos.x + ball_sprite.dim.w &&
            pad_sprite.pos.x + pad_sprite.dim.w > ball_sprite.pos.x &&
            pad_sprite.pos.y < ball_sprite.pos.y + ball_sprite.dim.h &&
            pad_sprite.pos.y + pad_sprite.dim.h > ball_sprite.pos.y) {

            has_collision = 1;

            // Top OR Bottom contact
            if(ball_sprite.pos.y + ball_sprite.dim.h > pad_sprite.pos.y && ball_sprite.pos.y < pad_sprite.pos.y && ball_sprite.dir.dv == 1) {       // contact at the top

                ball_sprite.dir.dv = 0;
                ball_sprite.pos.y -=2;
            } else {

                if(ball_sprite.pos.y < pad_sprite.pos.y + pad_sprite.dim.h && ball_sprite.pos.y > pad_sprite.pos.y && ball_sprite.dir.dv == 0) {    // FOR DEBUG PURPOSES. THIS IS ELIMINATE, AS HITTING THE PAD BOTTOM SHOULDN'T HAPPEN

                    ball_sprite.dir.dv = 1;
                    ball_sprite.pos.y += 2;
                }
            }

            // Left OR Right contact
            if(ball_sprite.pos.x + ball_sprite.dim.w > pad_sprite.pos.x + pad_sprite.dim.w && ball_sprite.pos.y + ball_sprite.dim.h > pad_sprite.pos.y && ball_sprite.dir.dh == 1) {

                ball_sprite.dir.dh = 0;
                ball_sprite.pos.x +=2;
            } else {
                if(ball_sprite.pos.x < pad_sprite.pos.x && ball_sprite.pos.y + ball_sprite.dim.h > pad_sprite.pos.y && ball_sprite.dir.dh == 0) {                                   

                    ball_sprite.dir.dh = 1;
                    ball_sprite.pos.x -=2;
                }
            }
        }

    }

    return has_collision;

}

uint8_t collision_ball_with_tile() {

    has_collision = 0;
    has_v_collision = 0;

    for (i = 0; i < current_level.total_tiles; i++)
    {
        if(current_level.tiles[i].hit == 0) continue;

        if(current_level.tiles[i].pixel_pos.x < ball_sprite.pos.x + ball_sprite.dim.w &&
            current_level.tiles[i].pixel_pos.x + TILES_W > ball_sprite.pos.x &&
            current_level.tiles[i].pixel_pos.y < ball_sprite.pos.y + ball_sprite.dim.h &&
            current_level.tiles[i].pixel_pos.y + TILES_H > ball_sprite.pos.y) {

            if(current_level.tiles[i].type == 3) { // indestructable
                has_collision = 3;
            } else {
                has_collision = update_tile_hit(i);
            }

            if(ball_sprite.pos.y < current_level.tiles[i].pixel_pos.y + TILES_H && ball_sprite.pos.y > current_level.tiles[i].pixel_pos.y && ball_sprite.dir.dv == 0) { // ball hit the bottom of the tile
                ball_sprite.dir.dv = 1;
                ball_sprite.pos.y +=2;
                has_v_collision = 1;
            } else {
                if(ball_sprite.pos.y + ball_sprite.dim.h > current_level.tiles[i].pixel_pos.y && ball_sprite.pos.y < current_level.tiles[i].pixel_pos.y && ball_sprite.dir.dv == 1) { // ball hit the top of the tile
                    ball_sprite.dir.dv = 0;
                    ball_sprite.pos.y -=2;
                    has_v_collision = 1;
                }
            }

            if(has_v_collision==0) {
                if(ball_sprite.pos.x + ball_sprite.dim.w > current_level.tiles[i].pixel_pos.x + TILES_W && ball_sprite.pos.y + ball_sprite.dim.h > current_level.tiles[i].pixel_pos.y && ball_sprite.dir.dh == 1) { // hit the tile on the left
                    ball_sprite.dir.dh = 0;
                    ball_sprite.pos.x +=2;
                } else {
                    if(ball_sprite.pos.x < current_level.tiles[i].pixel_pos.x && ball_sprite.pos.y + ball_sprite.dim.h > current_level.tiles[i].pixel_pos.y && ball_sprite.dir.dh == 0) {
                        ball_sprite.dir.dh = 1;
                        ball_sprite.pos.x -=2;
                    }
                }
            }

            break;

        }
    }
    

    return has_collision;
}