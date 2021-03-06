// k-type.h, 159

#ifndef __K_TYPE__
#define __K_TYPE__

#include "k-const.h"

typedef void (*func_p_t)(void); // void-return function pointer type

typedef enum {UNUSED, READY, RUN} state_t;

typedef struct {
   unsigned int reg[8];
   unsigned int eip;
   unsigned int cs;
   unsigned int efl;
} trapframe_t;

typedef struct {
	int run_count;                       // read in 1.html
	state_t state;
	int total_count;
	trapframe_t * trapframe_p;
} pcb_t;                     

typedef struct {             // generic queue type
	int q[Q_SIZE];                        // for a simple queue
	int tail;
} q_t;

#endif
