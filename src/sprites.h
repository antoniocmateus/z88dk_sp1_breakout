#ifndef _SPRITES_H
#define _SPRITES_H

#include <stdlib.h>

extern void create_pad();
extern void update_pad();
extern void position_sprites_init();
extern void create_ball();
extern void update_ball();
extern void move_ball();
extern uint8_t collision_ball_with_pad();
extern uint8_t collision_ball_with_screen();
extern uint8_t collision_ball_with_tile();
// extern uint8_t collision_ball_die();

#endif