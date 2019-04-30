// proc.c, 159
// all user processes are coded here
// processes do not R/W kernel data or code, must use sys-calls

#include "k-const.h"   // LOOP
#include "sys-call.h"  // all service calls used below
#include "k-data.h"
#include "tools.h"
#include "k-include.h"
#include "proc.h"

void InitTerm(int term_no) {
   int i, j;

   Bzero((char *)&term[term_no].out_q, sizeof(q_t));
   Bzero((char *)&term[term_no].in_q, sizeof(q_t));      // <------------- new
   Bzero((char *)&term[term_no].echo_q, sizeof(q_t));    // <------------- new
   term[term_no].out_mux = MuxCreateCall(Q_SIZE);
   term[term_no].in_mux = MuxCreateCall(0);              // <------------- new

   outportb(term[term_no].io_base+CFCR, CFCR_DLAB);             // CFCR_DLAB is 0x80
   outportb(term[term_no].io_base+BAUDLO, LOBYTE(115200/9600)); // period of each of 9600 bauds
   outportb(term[term_no].io_base+BAUDHI, HIBYTE(115200/9600));
   outportb(term[term_no].io_base+CFCR, CFCR_PEVEN|CFCR_PENAB|CFCR_7BITS);

   outportb(term[term_no].io_base+IER, 0);
   outportb(term[term_no].io_base+MCR, MCR_DTR|MCR_RTS|MCR_IENABLE);
   for(i=0; i<LOOP/2; i++)asm("inb $0x80");
   outportb(term[term_no].io_base+IER, IER_ERXRDY|IER_ETXRDY); // enable TX & RX intr
   for(i=0; i<LOOP/2; i++)asm("inb $0x80");

   for(j=0; j<25; j++) {                           // clear screen, sort of
      outportb(term[term_no].io_base+DATA, '\n');
      for(i=0; i<LOOP/30; i++)asm("inb $0x80");
      outportb(term[term_no].io_base+DATA, '\r');
      for(i=0; i<LOOP/30; i++)asm("inb $0x80");
   }
/*  // uncomment this part for VM (Procomm logo needs a key pressed, here reads it off)
   inportb(term[term_no].io_base); // clear key cleared PROCOMM screen
   for(i=0; i<LOOP/2; i++)asm("inb $0x80");
*/
}

void InitProc(void) {
   int i;

   vid_mux = MuxCreateCall(1);  // create/alloc a mutex, flag init 1

   InitTerm(0);
   InitTerm(1);

   while(1) {
      ShowCharCall(0, 0, '.');
      for(i=0; i<LOOP/2; i++) asm("inb $0x80");  // this can also be a kernel service

      ShowCharCall(0, 0, ' ');
      for(i=0; i<LOOP/2; i++) asm("inb $0x80");
   }
}


void Aout(int device)
{
	int i;
	int rand_num;
        int my_pid = GetPidCall();
        char str[] = ") Hello, World!\n\r";
	char str2[STR_SIZE];
        char my_alpha[STR_SIZE];
	my_alpha[0] = my_pid + 64;

	Itoa(str2, my_pid);
        WriteCall(device, str2);

	WriteCall(device, " (");
	WriteCall(device, my_alpha);

        WriteCall(device, str);

	PauseCall();

        for(i = 0; i < 70; i++)
        {
                ShowCharCall(my_pid, i, my_alpha[0]);
		rand_num = RandCall() % 20 + 5;
                SleepCall(rand_num);
                ShowCharCall(my_pid, i, ' ');
        }
        ExitCall(my_pid * 100);
}


void UserProc(void) {
	int i;
   	int device;
   	int my_pid = GetPidCall();  // get my PID
	int fork_pid;
	int exit_code;

	char my_alpha[STR_SIZE];
   	char input[STR_SIZE];
	char exitCode_s[STR_SIZE];
	char cur_pid[STR_SIZE];
	char forkPid_s[STR_SIZE];

	Itoa(cur_pid, my_pid);

   	device = my_pid % 2 == 1? TERM0_INTR : TERM1_INTR;

	SignalCall(SIGINT, (int)Ouch);

   	while(1) {
 		WriteCall(device, "PID ");  // prompt for terminal input     <-------------- new
		WriteCall(device, cur_pid);
		WriteCall(device, " > ");
      		ReadCall(device, input);   // read terminal input           <-------------- new
 		if(StrCmp(input, "race") == FALSE) //compare str2 and "fork", if not the same -> "continue;"
			continue;

		for(i=0; i < 5; i++)
		{
			fork_pid = ForkCall();

			if(fork_pid == NONE)
			{
				WriteCall(device, "Couldn't Fork!");
				continue;
			}
		
			if(fork_pid == 0)
			{
				ExecCall((int)Aout, device); //child code
			}
		
			Itoa(forkPid_s, fork_pid);
			WriteCall(device, "Child PID: "); //parent code
			WriteCall(device, forkPid_s);
			WriteCall(device, "\n\r");
		}
	
		SleepCall(300);
		KillCall(0, SIGGO);

		for(i=0; i < 5; i++)
		{
			exit_code = WaitCall();
			my_alpha[0] = (exit_code/100)+64;

			Itoa(exitCode_s, exit_code);
			WriteCall(device, "Child Exit Code: ");
			WriteCall(device, exitCode_s);
			WriteCall(device, " ");
			WriteCall(device, my_alpha);
			WriteCall(device, " arrives!\n\r");
		}
	}
}

void Ouch(int device)
{
	WriteCall(device, "Can't touch that!\n\r");
}

void Wrapper(int handler, int arg)
{
	func_p_t2 func = (func_p_t2)handler;

	asm("pushal");	//save regs
	func(arg);	//call signal handler with arg
	asm("popal");	//restore regs
	asm(	"movl %%ebp, %%esp; 
		popl %%ebp; 
		ret $8"
		:	//no output
		:
	);	//skip handler & arg
}
