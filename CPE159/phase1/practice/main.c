#include <spede/flames.h>
#include <spede/machine/asmacros.h>
#include <spede/machine/io.h>
#include <spede/machine/pic.h>
#include <spede/machine/proc_reg.h>
#include <spede/machine/seg.h>
#include <spede/stdio.h>

#define LOOP 1666666 

int main(void) {
	int i = 0;
	cons_gotoRC(0, 0);
   while(1) {
        cons_printf("\r.\r");
	for(i=0; i<LOOP/2; i++) asm("inb $0x80"); // each inb delays CPU .6 microsecond

	cons_printf("\b \r");
        for(i=0; i<LOOP/2; i++) asm("inb $0x80"); // each inb delays CPU .6 microsecond

   }
	return 0;
}

