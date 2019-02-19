// k-sr.h, 159

#ifndef __K_SR__
#define __KSR__

#include "k-type.h"

void NewProcSR(func_p_t);
void CheckWakeProc(void);
void TimerSR(void);
int GetPidSR(void);
void ShowCharSR(void);
void SleepSR(int);

#endif