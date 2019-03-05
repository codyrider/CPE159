// sys-call.h

#ifndef __sys_call__
#define __sys_call__

//insert any dependencies


//prototypes..
int GetPidCall(void);
void ShowCharCall(int, int, char);
void SleepCall(int);
int MuxCreateCall(int);
void MuxOpCall(int, int);
void WriteCall(int, char *);

#endif
