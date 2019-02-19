// k-sr.c, 159

#include "k-include.h"
#include "k-type.h"
#include "k-data.h"
#include "k-lib.h"
#include "k-sr.h"

// to create a process: alloc PID, PCB, and process stack
// build trapframe, initialize PCB, record PID to ready_q
void NewProcSR(func_p_t p) {  // arg: where process code starts
   int pid;

   if( QisEmpty(&pid_q) ) {     // may occur if too many been created
      cons_printf("Panic: no more process!\n");
      //breakpoint();                     // cannot continue, alternative: breakpoint();
	return;
   }

   pid = DeQ(&pid_q);                                            // alloc PID (1st is 0)
	Bzero((char *)(&pcb[pid]), sizeof(pcb_t));                               // clear PCB
   Bzero(&proc_stack[pid][0], PROC_STACK_SIZE);                   // clear stack
   pcb[pid].state = READY;                                       // change process state

   if(pid > 0)EnQ(pid, &ready_q);                        // queue to ready_q if > 0


// point trapframe_p to stack & fill it out
   pcb[pid].trapframe_p = (trapframe_t *) &proc_stack[pid][PROC_STACK_SIZE];                // point to stack top
   pcb[pid].trapframe_p--;                   // lower by trapframe size
   pcb[pid].trapframe_p->efl = EF_DEFAULT_VALUE|EF_INTR; // enables intr
   pcb[pid].trapframe_p->cs = get_cs();                  // dupl from CPU
   pcb[pid].trapframe_p->eip = (unsigned int)p;                          // set to code
}

void CheckWakeProc(void)
{
	int i;
	int limit;
	int slp_pid;

	limit = pcb[run_pid].tail;	

	for(i = 0; i < limit; i++)
	{
		slp_pid = DeQ(&sleep_q);
		if(pcb[slp_pid].wake_centi_sec >= sys_centi_sec)
		{
			pcb[slp_pid].state = READY;
			EnQ(slp_pid, &ready_q);
		}
		else
		{
			EnQ(slp_pid, &sleep_q);
		}
	}
}

// count run_count and switch if hitting time slice
void TimerSR(void) {
	outportb(PIC_CONTROL, TIMER_DONE);         // notify PIC timer done

   	sys_centi_sec++;

	CheckWakeProc();
	if(!run_pid)
	{
		....
	}

   	pcb[run_pid].run_count++;                                       // count up run_count
   	pcb[run_pid].total_count++;                              // count up total_count

   	if(pcb[run_pid].run_count == TIME_SLICE){       // if runs long enough
     	EnQ(run_pid, &ready_q);                                    // move it to ready_q
      	pcb[run_pid].state = READY;                                    // change its state
      	run_pid = NONE;                                    // running proc = NONE
   	}
}

int GetPidSR(void)
{

}

void ShowCharSR(void)
{

}

void SleepSR(int centi_sec)
{
	pcb[run_pid].wake_centi_sec = sys_centi_sec + centi_sec
	pcb[run_pid].state = SLEEP;
	EnQ(run_pid, &sleep_q);
	run_pid = NONE;
}
