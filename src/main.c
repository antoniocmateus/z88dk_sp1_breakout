#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>
#include <stdlib.h>
#include "int.h"
#include "globals.h"
#include "game.h"
#include "text_interface.h"

// variables initialization
struct sp1_Rect game_area = {1, 1, 22, 23}; // define game area (x,y,w,h)
struct keys_struct game_keys = {IN_KEY_SCANCODE_z, IN_KEY_SCANCODE_m, IN_KEY_SCANCODE_l, IN_KEY_SCANCODE_q, IN_KEY_SCANCODE_h}; // define the default keys

uint8_t i;

int main()
{

    zx_border(INK_BLACK); // Set border Black (paper is already set by loader)

    // setup IM2
    setup_int();

    // Initialize SP1
    sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
                   INK_WHITE | PAPER_BLACK, ' ' );
    
    //Invalidate the game area to force sp1 to redraw next update
    sp1_Invalidate(&game_area);
    sp1_UpdateNow();

    // initialize the font and text areas
    init_text();

    // initialize the graphics
    init_graphics();

    // generate a new seed with tick (tick will run 50 per second)
    srand(tick);


    // Lets go to the main loop
    while (1) {

       // show menu
        show_menu();

        play_game();

    }

}