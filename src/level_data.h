#include <stdint.h>
#include "globals.h"

unsigned char tile_color[9] = {INK_BLUE | BRIGHT, INK_RED | BRIGHT, INK_MAGENTA | BRIGHT, INK_GREEN | BRIGHT, INK_CYAN | BRIGHT, INK_YELLOW | BRIGHT, INK_WHITE | BRIGHT, INK_YELLOW, INK_WHITE};

// level data
// Colors: 
// format: 
// C - C: Color
// if C = 8 it's indestructable and it's yellow
// if c = 9 it takes 2 hits and it's white
uint8_t level_data[MAX_LEVELS][MAX_LINES][MAX_COLS] = {
    {

        {0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0},
        {9,9,9,9,9,9,9,9,9,9,9},
        {1,1,1,1,7,7,7,1,1,1,1},
        {2,2,2,2,7,7,7,2,2,2,2},
        {3,3,3,3,7,7,7,3,3,3,3},
        {4,4,4,4,7,7,7,4,4,4,4},
        {5,5,5,5,7,7,7,5,5,5,5},
        {0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0}
    },
    {
        {0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0},
        {6,6,6,7,2,6,2,7,6,6,6},
        {6,6,6,7,2,6,2,7,6,6,6},
        {4,4,4,8,2,6,2,8,4,4,4},
        {4,4,4,7,2,6,2,7,4,4,4},
        {3,3,3,7,2,6,2,7,3,3,3},
        {3,3,3,8,2,6,2,8,3,3,3},
        {9,9,9,9,9,9,9,9,9,9,9},
        {0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0}
    }
};