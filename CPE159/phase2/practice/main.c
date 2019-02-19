#include <spede/flames.h>
#include <spede/machine/asmacros.h>
#include <spede/machine/io.h>
#include <spede/machine/pic.h>
#include <spede/machine/proc_reg.h>
#include <spede/machine/seg.h>
#include <spede/stdio.h>

#define LOOP 1666666 
#define Q_SIZE 20
#define VID_MASK 0x0f00     // foreground bright white, background black
#define VID_HOME (unsigned short *)0xb8000 // home position, upper-left corner

int run_pid;

typedef struct {             // generic queue type
        int q[20];                        // for a simple queue
        int tail;
}q_t;

void Delay(void) {  // delay CPU for half second by 'inb $0x80'
   int i;
   for(i=0; i<LOOP/2; i++) asm("inb $0x80");  // loop to try to delay CPU for about half second
}

int QisEmpty(q_t *p) { // return 1 if empty, else 0
        if(p->tail == 0)
                return 1;
        else
                return 0;
}

int QisFull(q_t *p) { // return 1 if full, else 0
        if(p->tail == Q_SIZE)
                return 1;
        else
                return 0;
}

int DeQ(q_t *p) { // return -1 if q[] is empty
        int dq_value;
        int i;

        if(QisEmpty(p)) {
                return -1;
        }

        dq_value = p->q[0];
        for(i = 0; i < Q_SIZE; i++)
        {
                if(i < p->tail - 1)
                        p->q[i] = p->q[i+1];
                else
                        p->q[i] = -1;
        }
	p->tail--;
        return dq_value;
}

// if not full, enqueue # to tail slot in queue
void EnQ(int to_add, q_t *p) {
        if(QisFull(p)) {
               cons_printf("Panic: queue is full, cannot EnQ!\n");
 	       return;
        }
        p->q[p->tail] = to_add;
        p->tail++;
}

void print_array(q_t *p)
{
	int i;
	for(i = 0; i < Q_SIZE; i++)
	{
		printf("%d ", p->q[i]);
	}
	printf("\n");
}

void Bzero(char *p, int bytes) {
	int i;
	for(i = 0; i < bytes; i++)
	{
		*(p + i) = 0;
	}	
}

void ShowChar(int row, int col, char ch) { // show ch at row, col

   unsigned short *p = VID_HOME + row*80 + col;   // upper-left corner of display
   *p = ch + VID_MASK;
}

void UserProc(void) {
	int i = 0;
	while(i < 10) {
		ShowChar(run_pid, 0, (run_pid/10)+48);
		ShowChar(run_pid, 1, (run_pid % 10)+48);
		Delay();

		ShowChar(run_pid, 0, ' ');
		ShowChar(run_pid, 1, ' ');
		Delay();
		i++;
   	}
}

int main(void) {

	run_pid = 20;
	UserProc();

/*
	int i = 0;
   while(1) {
        cons_printf("\r\r..\r\r");
	for(i=0; i<LOOP/2; i++) asm("inb $0x80"); // each inb delays CPU .6 microsecond

	cons_printf("\r\r  \r\r");
        for(i=0; i<LOOP/2; i++) asm("inb $0x80"); // each inb delays CPU .6 microsecond

   }
*/

/*
	int dq_value;
	q_t ready_q;

	ready_q.tail = 0;
	Bzero((char *) &ready_q, sizeof(q_t));
	print_array(&ready_q);
	EnQ(1, &ready_q);
	print_array(&ready_q);
        EnQ(2, &ready_q);
        print_array(&ready_q);
        EnQ(3, &ready_q);
        print_array(&ready_q);

	dq_value = DeQ(&ready_q);
	print_array(&ready_q);
	printf("dq_value = %d\n", dq_value);

       dq_value = DeQ(&ready_q);
        print_array(&ready_q);
        printf("dq_value = %d\n", dq_value);

       dq_value = DeQ(&ready_q);
        print_array(&ready_q);
        printf("dq_value = %d\n", dq_value);
*/

	return 0;
}

