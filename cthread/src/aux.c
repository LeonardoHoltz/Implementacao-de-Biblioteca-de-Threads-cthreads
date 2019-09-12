#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/aux.h"

static int _init_cthread_ = 0;

PFILA2 FILA_APTO;
PFILA2 FILA_BLOQ;
TCB_t *FILA_EXEC;

int init()
{
	int ret = 0;
	if( !_init_cthread_ )
	{
		// código de inicialização aqui
		
		startTimer();

		_init_cthread_ = 1;
	}
	
	return ret;
}

int InsertTCB(PFILA2 pFila, TCB_t *tcb);

TCB_t *allocTCB(int tid, int state)
{
	TCB_t *pTCB = (TCB_t *) malloc( sizeof(TCB_t) );
	if(pTCB != NULL)
	{
		// fazer rotina de randomização e checagem se tid já existe aqui
		// pTCB->tid = novo tid não usado;

		pTCB->state = state;
		pTCB->prio = 0;
	}
	return pTCB;
}
