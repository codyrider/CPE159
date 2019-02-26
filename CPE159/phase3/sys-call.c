// sys-call.c
// calls to kernel services

#include "k-const.h"

int GetPidCall(void) {
   int pid;

   asm("int %1;             // interrupt!
        movl %%eax, %0"     // after, copy eax to variable 'pid'
       : "=g" (pid)         // output
       : "g" (GETPID_CALL)  // input
       : "eax"              // used registers
	);
	return pid;
}

void ShowCharCall(int row, int col, char ch) {
   asm("movl %0, %%eax;     // send in row via eax
        movl %1, %%ebx;            // send in col via ebx
        movl %2, %%ecx;             // send in ch via ecx
        int %3"             // initiate call, %3 gets entry_id
       :                    // no output
       : "g" (row), "g" (col), "g" (ch), "g" (SHOWCHAR_CALL)
       : "eax", "ebx", "ecx"         // affected/used registers
   );
}

void SleepCall(int centi_sec) {  // # of 1/100 of a second to sleep
   asm( "movl %0, %%eax;
	int %1"
	:			// no output
	: "g" (centi_sec), "g" (SLEEP_CALL) // input
	: "eax"
	);
}

int MuxCreateCall(int flag)
{
	int mux_id;

 	asm
	(	"movl %1, %%eax;	//move flag into eax
		int %0;             // interrupt!
        	movl %%eax, %2"     // after, copy eax to variable 'mux_id'
       		: "=g" (mux_id)         // output
       		: "g" (MUX_CREATE_CALL), "g" (flag)  // input
       		: "eax"              // used registers
        );

        return mux_id;

}

void MuxOpCall(int mux_id, int opcode)
{
	asm(
		"movl %0, %%eax;	//move mux_id into eax
		movl %1, %%ebx;		// move opcode into ebx
        	int %2"
        	:                       // no output
        	: "g" (mux_id), "g" (opcode), "g" (SLEEP_CALL) // input
        	: "eax", "ebx"
        );

}

void WriteCall(int device, char *str)
{

}
