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
	(	"movl %2, %%eax;	//move flag into eax
        	int %1;
		movl %%eax, %0"     // after, copy eax to variable 'mux_id'
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
        	: "g" (mux_id), "g" (opcode), "g" (MUX_OP_CALL) // input
        	: "eax", "ebx"
        );

}

void WriteCall(int device, char *str)
{
	int row;
	int col;
	row = GetPidCall();	//get my PID by service call
				//calc my row number
	col = 0;		//column is zero to begin with
	if(device ==  STDOUT)	//if device is STDOUT
	{
		while(*str != '\0')	//while what str points to is not a null character
		{
			ShowCharCall(row, col, *str);	//use an existing service call to show this character, at row and column
			str++;				//increment the str pointer and the column position
			col++;
		}
	}
	else	//device is a terminal
	{
		int term_no;
		if(device == TERM0_INTR)
			term_no = 0;
		else
		{
			term_no = 1;
		}

		while(*str != '\0')
		{
			MuxOpCall(term[term_no].out_mux, LOCK);
			EnQ(*str, &term[term_no].out_q);
			
			if(device == TERM0_INTR)
			{
				asm
				(
					"int %0"
					:	//no output
					: "g" (TERM0_INTR) //input
					:	//no registers
				);
			}
			else
			{
				asm
				(
			                "int %0"
                                        :       //no output
                                        : "g" (TERM1_INTR) //input
                                        :       //no registers
				);
			}
			
			str++;
		}
	}
}
