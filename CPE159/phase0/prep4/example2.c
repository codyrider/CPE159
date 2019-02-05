#include <spede/flames.h>
#include <spede/machine/asmacros.h>
#include <spede/machine/io.h>
#include <spede/machine/pic.h>
#include <spede/machine/proc_reg.h>
#include <spede/machine/seg.h>
#include <spede/stdio.h>

#define VID_MASK 0x0f00
#define TIMER_INTR 32
#define PIC_MASK 0x21
#define MASK ~0x01
#define PIC_CONTROL 0x20
#define TIMER_DONE 0x60
#define LOOP 1666666

void TimerEntry(void);
void TimerCode(void);

int main()
{	
	char ch;
	struct i386_gate * intr_table;
	intr_table = get_idt_base();

	fill_gate(&intr_table[TIMER_INTR], (int)TimerEntry, get_cs(), ACC_INTR_GATE, 0);
	outportb(PIC_MASK, MASK);
	asm("sti");

	while(1)
	{

		if( cons_kbhit() ) {             // poll keyboard, returns 1/true if pressed
         		ch = cons_getchar();          // read key
			cons_printf("%c Pressed\n", ch);
         		if(ch == 'b')breakpoint();    // breakpoint() will go to GDB
		}

	}

	return(0);
}

void TimerCode(void)
{
	static int count = 0;
	count++;
	if(count % 50 == 0)
		cons_printf(".");
	if(count % 300 == 0)
		cons_printf("\nCody Rider (press 'b' to return to GDB)\n");
		
	outportb(PIC_CONTROL, TIMER_DONE);
}
