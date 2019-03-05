// k-sr.c, 159

#include "k-include.h"
#include "k-type.h"
#include "k-data.h"
#include "k-lib.h"
#include "k-sr.h"

// to create a process: alloc PID, PCB, and process stack
// build trapframe, initialize PCB, record PID to igned short *p = VID_HOME + row*80 + col;   // upper-left corner of display
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

	limit = sleep_q.tail;	

	for(i = 0; i < limit; i++)
	{
		slp_pid = DeQ(&sleep_q);
		if(pcb[slp_pid].wake_centi_sec <= sys_centi_sec)
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
		return;
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
	return run_pid;
}

void ShowCharSR(int row, int col, char ch)
{
	unsigned short *p = VID_HOME + row*80 + col;   // upper-left corner of display
	*p = ch + VID_MASK;

}

void SleepSR(int centi_sec)
{
	pcb[run_pid].wake_centi_sec = sys_centi_sec + centi_sec;
	pcb[run_pid].state = SLEEP;
	EnQ(run_pid, &sleep_q);
	run_pid = NONE;
}

int MuxCreateSR(int flag)
{
	int mux_id;
	mux_id = DeQ(&mux_q);
	Bzero((char *)&sem[mux_id].suspend_q, sizeof(q_t));
	sem[mux_id].flag = flag;
	sem[mux_id].creater = run_pid;
	
	return mux_id;
}

void MuxOpSR(int mux_id, int opcode)
{
	if(opcode == LOCK)
	{
		if(sem[mux_id].flag > 0)
		{
			sem[mux_id].flag--;
		}
		else
		{
			EnQ(run_pid, &sem[mux_id].suspend_q);
			pcb[run_pid].state = SUSPEND;
			run_pid = NONE;
		}
	}
	else if(opcode == UNLOCK)
	{
		if(QisEmpty(&sem[mux_id].suspend_q))
		{
			sem[mux_id].flag++;
		}
		else
		{
			int rdy_proc;
			rdy_proc = DeQ(&sem[mux_id].suspend_q);
			EnQ(rdy_proc, &ready_q);
			pcb[rdy_proc].state = READY;
			
		}
	}
}

void TermSR(int term_no)
{

	//read IIR??
	
	if( == TXRDY)
	{
		TermTxSR(term_no);
	}
	else if( == RXRDY)
	{
		TermRxSR(term_no);
	}

	if(term[term_no].tx_missed)
	{
		TermTxSR(term_no);
	}
}

void TermTxSR(int term_no)
{
	char * ch;
	if(QisEmpty(&term[term_no].out_q))
	{
		term[term_no].tx_missed = TRUE;
		return;
	}
	else
	{
		ch = DeQ(&term[term_no].out_q);
		outportb(term[term_no].io_base+DATA, *ch);
		term[term_no].tx_missed = TRUE
		MuxOpSR(term_no, UNLOCK);
	}	
}
