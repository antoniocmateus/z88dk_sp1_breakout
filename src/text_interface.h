#ifndef _TEXTINTERFACE_H
#define _TEXTINTERFACE_H

extern unsigned char ready_text[];
extern unsigned char paused_text[];
extern unsigned char quit_text[];

extern void init_text();
extern void pad_numbers(unsigned char *s, unsigned int limit, long number);

#endif