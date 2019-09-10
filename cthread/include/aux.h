#include "support.h"
#include "cdata.h"

#ifndef _AUX_CTHREAD_
#define _AUX_CTHREAD_


TCB_t *allocTCB(int tid, int state, int prio);

ucontext_t *allocCtx();

char *allocStack(int sz);

int InsertWithPrioInFila2(PFILA2 pFila, TCB_t *tcb);

#endif
