//***********************************
// NAME: CODY RIDER
//
// entry.S of phase 0, prep4, Timer Device
//***********************************

#include <spede/machine/asmacros.h>

ENTRY(TimerEntry)
	pusha
	cld
	call CNAME(TimerCode)
	popa
	iret
