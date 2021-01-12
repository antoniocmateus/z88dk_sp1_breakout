#ifndef _TEXTINTERFACE_H
#define _TEXTINTERFACE_H

extern unsigned char ready_text[];
extern unsigned char paused_text[];
extern unsigned char quit_text[];
extern unsigned char life_lost[];
extern unsigned char game_over[];
extern unsigned char level_completed[];
extern unsigned char game_completed[];

extern void init_text();
extern void pad_numbers(unsigned char *s, unsigned int limit, long number);
extern void show_message(unsigned char *s, uint16_t row, uint16_t col, int delay);
extern void show_menu();

#endif