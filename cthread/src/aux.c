#include <stdio.h>
#include <string.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/aux.h"

static int _init_cthread_ = 0;


int init()
{
	int ret = 0;
	if( !(_init_cthread_) )
	{
		// código de inicialização aqui
		
		_init_cthread_ = 1;
	}
	
	return ret;
}

int InsertTCB(PFILA2 pFila, TCB_t *tcb);

TCB_t *allocTCB(int tid, int state, int prio);
