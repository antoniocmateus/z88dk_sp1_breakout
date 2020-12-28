#include <im2.h>
#include <intrinsic.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>
#include "int.h"

// timer

unsigned char tick;
unsigned char timer;

IM2_DEFINE_ISR_8080(isr)
{

   ++tick;
}

void wait(void)
{
   while (abs(tick - timer) < SPEED)
      intrinsic_halt();
   
   timer = tick;
}

void wait_seconds(char seconds) {

   for (int i = 0; i < seconds * 50; i++)
   {
      intrinsic_halt();
   }
   
}

void setup_int(void)
{
   im2_init((void *)0xd000);
   memset((void *)0xd000, 0xd1, 257);
   
   z80_bpoke(0xd1d1, 0xc3);
   z80_wpoke(0xd1d2, (unsigned int)isr);
   intrinsic_ei();
}
