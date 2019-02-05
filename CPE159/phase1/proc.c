// proc.c, 159
// all user processes are coded here
// processes do not R/W kernel data or code, must use syscalls

#include "k-const.h"
#include "k-data.h"

void Delay(void) {  // delay CPU for half second by 'inb $0x80'
   int i;
   for(i=0; i<LOOP/2; i++) asm("inb $0x80");  // loop to try to delay CPU for about half second
}

void ShowChar(int row, int col, char ch) { // show ch at row, col
/*
   unsigned short *p = ...   // upper-left corner of display
   ...
   ...
   ...
*/
	cons_gotoRC(row, col);
	cons_printf(ch);

}

void InitProc(void) {
	while(1)
	{
        	cons_printf("\r.\r");
		Delay();

        	cons_printf("\r \r");
		Delay();
   	}
}

void UserProc(void) {
	cons_gotoRC(run_pid+1, 0);
   while(1) {
	if(run_pid < 10)
		cons_printf("0");
	cons_printf("%d", run_pid);
	Delay();

	cons_printf("\r\r  \r\r");
	Delay();
   }
}
