#include <spede/flames.h>
#include <spede/machine/asmacros.h>
#include <spede/machine/io.h>
#include <spede/machine/pic.h>
#include <spede/machine/proc_reg.h>
#include <spede/machine/seg.h>
#include <spede/stdio.h>

#define LOOP 1666666 
#define Q_SIZE 20

typedef struct {             // generic queue type
        int q[20];                        // for a simple queue
        int tail;
}q_t;

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


int main(void) {
/*
	int i = 0;
	cons_gotoRC(0, 0);
   while(1) {
        cons_printf("\r\r..\r\r");
	for(i=0; i<LOOP/2; i++) asm("inb $0x80"); // each inb delays CPU .6 microsecond

	cons_printf("\r\r  \r\r");
        for(i=0; i<LOOP/2; i++) asm("inb $0x80"); // each inb delays CPU .6 microsecond

   }
*/
	int dq_value;
	q_t ready_q;

	q_t * q_ptr;
	q_ptr = &ready_q;

	ptr->tail = 0;
	print_array(q_ptr);
	EnQ(1, q_ptr);
	print_array(q_ptr);
	dq_value = DeQ(q_ptr);
	print_array(q_ptr);
	printf("dq_value = %d\n", dq_value);

	return 0;
}

