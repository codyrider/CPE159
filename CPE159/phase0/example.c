// example.c
// cons_xxx functions from SPEDE (help coding only, not used for processes in our OS)

#define LOOP 1666666      // LOOP to time 1 second (LOOP x .6 microseconds)
                           // but this number depends when using virtual hosts
		          // since simulated results vary

#include <spede/stdio.h>  // cons_xxx to debug runtime
#include <spede/flames.h> // cons_xxx needs
int main(void) {
	int i;
        char ch;
        printf("Hello, World! (on Linux PC.)\n");
        cons_printf("Hello, World, too! (on Target PC.)\n");
        cons_putchar('A'); // cons_putchar() prints one character
        cons_putchar('B');
        cons_putchar('C');
        while(1) {
	        if( cons_kbhit() ) {             // poll keyboard, returns 1/true if pressed
	                ch = cons_getchar();          // read key
                        if(ch == 'b')breakpoint();    // breakpoint() will go to GDB
                 }
                 for(i=0; i<LOOP; i++) asm("inb $0x80"); // each inb delays CPU .6 microsecond
                 cons_putchar('X');
        }

        return 0;
}
