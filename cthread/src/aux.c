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

int InsertTCB(PFILA2 pFila, TCB_t *tcb)
{
	int err_vazia = 0;
	int err_insert = 0;
	int err_next = 0;
	
	if( pFila->first != NULL)
	{
		err_vazia = FirstFila2(pFila);

		while( !err_insert && !err_next )
		{		
			TCB_t *currTCB = (TCB_t *) GetAtIteratorFila2(pFila);
	
			if(currTCB != NULL)
			{
				if(tcb->prio > currTCB->prio)
				{
					err_insert = InsertBeforeIteratorFila2(pFila);	
				}
			}
			else
				err_insert = 1;

			err_next = NextFila2(pFila);
		}
	}
	else
		err_vazia = 1;

	if(!err_vazia && !err_insert && !err_next)
		return 0;
	else
	if( pFila == NULL || err_next == -NXTFILA_VAZIA)
		return AppendFila2(pFila, (void *) tcb);
	else
		return -1;
}

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
