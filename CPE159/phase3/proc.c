// proc.c, 159
// all user processes are coded here
// processes do not R/W kernel data or code, must use sys-calls

#include "k-const.h"   // LOOP
#include "sys-call.h"  // all service calls used below
#include "k-data.h"

void InitProc(void) {
   	int i;
	vid_mux = MuxCreateCall(1);
	while(1) {
      		ShowCharCall(0, 0, '.');
      		for(i=0; i<LOOP/2; i++) asm("inb $0x80"); // this is also a kernel service

      		ShowCharCall(0, 0, ' ');
      		for(i=0; i<LOOP/2; i++) asm("inb $0x80"); // this is also a kernel service
	}
}

void UserProc(void) {
	int my_pid = GetPidCall();

	char str1[] = "PID xx is running, nobody else is using video\0";
	char str2[] = "                                                \0";

	str1[4] = '0' + my_pid / 10;
	str1[5] = '0' + my_pid % 10;

  	while(1) {
		MuxOpCall(vid_mux, LOCK);      
      		WriteCall(STDOUT, (char *)&str1);
		SleepCall(50);                              //sleep .5 sec

      		WriteCall(STDOUT, (char *)&str2);
		SleepCall(50);
		MuxOpCall(vid_mux, UNLOCK);
   }
}
