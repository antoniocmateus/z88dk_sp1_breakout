#ifndef _SPRITES_H
#define _SPRITES_H

#include <stdlib.h>

extern void create_pad();
extern void update_pad();
extern void position_sprites_init();
extern void create_ball();
extern void update_ball();
extern void move_ball();
extern void collision_ball_with_pad(int left, int right);
extern void collision_ball_with_screen();
extern void collision_ball_with_tile();
extern void move_sprites_outside();
extern uint8_t all_tiles_done();

#endif