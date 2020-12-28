#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <stdint.h>

// SP1
extern struct sp1_Rect game_area;         // define game area (x,y,w,h)
extern struct sp1_Rect game_text_rect; // Y, X, W, H
extern struct sp1_pss game_text_area;
extern struct sp1_Rect game_score_rect;
extern struct sp1_pss game_score_area;
extern struct sp1_Rect game_lifes_rect;
extern struct sp1_pss game_lifes_area;
extern struct sp1_Rect game_level_rect;
extern struct sp1_pss game_level_area;


// INPUT
struct keys_struct {                      // structure to hold the input keys

   uint16_t left;
   uint16_t right;
   uint16_t fire;
   uint16_t abort;
   uint16_t pause;

};
extern struct keys_struct game_keys;        // defines the input keys

// GAME 
#define MAX_SCR_LEFT     8                  // pixel limit on the left side of the screen
#define MAX_SCR_RIGHT    184                // pixel limit on the right side of the screen
#define MAX_SCR_TOP      9                  // pixel limit on the top side of the screen
#define MAX_SCR_BOTTOM   191                // pixel limit on the right side of the screen
#define PAD_START_POS_X  88                 // pad starting x position
#define PAD_START_POS_Y  168                // pad starting y position

extern uint8_t lifes;                       // hold the number of lifes
extern uint8_t level;                       // hold the current level
extern uint16_t score;                      // score
extern uint8_t bullet_cooldown;            // cooldown for the shooting
extern uint8_t levelstart_cooldown;         // cooldown for level start
extern uint8_t has_collision;
extern uint8_t has_v_collision;
extern uint8_t collision_type;
extern uint8_t i;
extern uint8_t collision_with_tile;
extern unsigned char key_pressed;

struct point {                              // defines a point in the screen (can be pixel coord or character coord)
    uint8_t x;  
    uint8_t y;  
};  
struct size {                               // defines the size in pixels of the the sprite graphic (sprite or tile)
   uint8_t w;                               // width
   uint8_t h;                               // height
};  
struct direction {                          // defines the direction the sprite is moving
   unsigned char dh;                        // 0 RIGHT, 1 LEFT
   unsigned char dv;                        // 0 UP, 1 DOWN
};  
struct sprite_gfx {                         // defines a sprite

    struct sp1_ss *s;                       // sprite graphic(s)
    struct point pos;                       // pixel position
    struct point vel;                       // sprite velocity
    struct size dim;                        // sprite dimension (calculated automaticall by: w = (width-1)*8 - xthresh + 1 and h = (height-1)*8 - ythresh + 1)
    struct direction dir;                   // sprite direction: 
    uint8_t velocity;                       // velocity of the sprite
    unsigned char color;                    // color of the sprite
};      
struct tile_gfx {                           // defines a tile

    struct point char_pos;                  // char coord
    struct point pixel_pos;                 // pixel coord
    uint8_t type;                           // Type of the tile (according to map: 0 normal, 1 two hits, 2 indestructable)
    unsigned char color;                    // tiles color
    uint8_t hit;                            // how many hits left (on 0 will be eliminated)
};

// gfx declarations 
extern unsigned char ball_gfx[];            // masked ball gfx left
extern unsigned char padl_gfx[];            // masked ball gfx right
extern unsigned char padm_gfx[];            // pad gfx left
extern unsigned char padr_gfx[];            // pad gfx right
extern unsigned char tilel_gfx[];           // tile gfx left
extern unsigned char tiler_gfx[];           // tile gfx right

// font
extern uint8_t font_numbers[];
extern uint8_t font_chars[];
extern uint8_t font_special[];

extern struct sprite_gfx ball_sprite;       // define the ball sprite
extern struct sprite_gfx pad_sprite;        // define the pad sprite

// level data
#define MAX_TILES       132                 // Max tiles slots available
#define MAX_COLS        11                  // Max cols for tiles positioning
#define MAX_LINES       12                  // Max lines for tiles positioning
#define MAX_LEVELS      2                   // How many levels we'll have
#define TILES_W         16                  // Tiles width
#define TILES_H         8                   // Tiles height
struct level_definition {                           // defines a level
    uint8_t total_tiles;                            // total tiles in the level
    struct tile_gfx tiles[MAX_TILES];               // the tiles object of the level
};
extern struct level_definition current_level;       // holds the current level mapping
extern unsigned char tile_color[];                  // holds the tile color array

#endif