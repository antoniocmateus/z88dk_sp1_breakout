#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include "globals.h"
#include "text_interface.h"

struct sp1_Rect game_text_rect = {17, 1, 22, 1}; // Y, X, W, H
struct sp1_pss game_text_area;
struct sp1_Rect game_score_rect = {15,  25, 6, 1}; // Y, X, W, H
struct sp1_pss game_score_area;
struct sp1_Rect game_lifes_rect = {17,  27, 2, 1}; // Y, X, W, H
struct sp1_pss game_lifes_area;
struct sp1_Rect game_level_rect = {19,  27, 2, 1}; // Y, X, W, H
struct sp1_pss game_level_area;

// Text
unsigned char ready_text[] = "READY...?";
unsigned char paused_text[] = "GAME PAUSED!";
unsigned char quit_text[] = "QUIT? Y OR N.";

// Initialize fonts and text areas
void init_text() {

    for (i=0; i != 10; i++) {
      sp1_TileEntry(48+i, font_numbers+i*8);
    }
    for (i=0; i != 26; i++) {
      sp1_TileEntry(65+i, font_chars+i*8);
    }
    sp1_TileEntry('!', font_special);
    sp1_TileEntry(',', font_special+1*8);
    sp1_TileEntry('.', font_special+2*8);
    sp1_TileEntry('?', font_special+3*8);

    game_text_area.bounds    = &game_text_rect;            // print window
    game_text_area.flags     = SP1_PSSFLAG_INVALIDATE;     // printed characters invalidated so that they are drawn in the next sp1_UpdateNow
    game_text_area.attr_mask = 0;                          // overwrite background colour
    game_text_area.attr      = INK_WHITE | PAPER_BLACK;
    game_text_area.visit     = 0;

    game_score_area.bounds    = &game_score_rect;            // print window
    game_score_area.flags     = SP1_PSSFLAG_INVALIDATE;     // printed characters invalidated so that they are drawn in the next sp1_UpdateNow
    game_score_area.attr_mask = 0;                          // overwrite background colour
    game_score_area.attr      = INK_WHITE | PAPER_BLACK;
    game_score_area.visit     = 0;

    game_lifes_area.bounds    = &game_lifes_rect;            // print window
    game_lifes_area.flags     = SP1_PSSFLAG_INVALIDATE;     // printed characters invalidated so that they are drawn in the next sp1_UpdateNow
    game_lifes_area.attr_mask = 0;                          // overwrite background colour
    game_lifes_area.attr      = INK_WHITE | PAPER_BLACK;
    game_lifes_area.visit     = 0;

    game_level_area.bounds    = &game_level_rect;            // print window
    game_level_area.flags     = SP1_PSSFLAG_INVALIDATE;     // printed characters invalidated so that they are drawn in the next sp1_UpdateNow
    game_level_area.attr_mask = 0;                          // overwrite background colour
    game_level_area.attr      = INK_WHITE | PAPER_BLACK;
    game_level_area.visit     = 0;
}

void pad_numbers(unsigned char *s, unsigned int limit, long number)
{
   s += limit;
   *s = 0;

   // not a fast method since there are two 32-bit divisions in the loop
   // better would be ultoa or ldivu which would do one division or if
   // the library is so configured, they would do special case base 10 code.
   
   while (limit--)
   {
      *--s = (number % 10) + '0';
      number /= 10;
   }
}