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

   unsigned short *p = VID_HOME + row*80 + col;   // upper-left corner of display
   *p = ch + VID_MASK;
}

void InitProc(void) {
	while(1)
	{
        	ShowChar(0, 0, '.');
		Delay();

        	ShowChar(0, 0, ' ');
		Delay();
   	}
}

void UserProc(void) {
	while(1) {
  		ShowChar(run_pid, 0, (run_pid/10)+48);
		ShowChar(run_pid, 1, (run_pid % 10)+48);
		Delay();

		ShowChar(run_pid, 0, ' ');
		ShowChar(run_pid, 1, ' ');
		Delay();
   	}
}
