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
		TCB_t *mainThread = allocTCB(0, PROCST_EXEC);
		
		if( mainThread == NULL)
			ret = -INIT_ERROR;
		else
			FILA_EXEC = mainThread;

		if( ret == 0 )
		{
			startTimer();
			_init_cthread_ = 1;
		}
	}
	
	return ret;
}

int InsertTCB(PFILA2 pFila, TCB_t *tcb)
{
	int ret = 0;
	int insert_middle = 0;
	
	if( pFila != NULL )
	{
		if( FirstFila2(pFila) != 0 )
			ret = AppendFila2(pFila, (void *) tcb);
		else
		{
			do
			{		
				TCB_t *currTCB = (TCB_t *) GetAtIteratorFila2(pFila);
	
				if(currTCB != NULL)
				{
					if(tcb->prio < currTCB->prio)
					{	
						ret = InsertBeforeIteratorFila2(pFila, (void *) tcb);	
						insert_middle = 1;
						break;
					}
				}
				else
					DeleteAtIterator2(pFila);
			}
			while(  !NextFila2(pFila) )
				
			if( !insert_middle )
				ret = AppendFila2(pFila, (void *) tcb);
		}
	}
	else
		ret = -1;

	return ret;
}

TCB_t *allocTCB(int tid, int state)
{
	TCB_t *pTCB = (TCB_t *) malloc( sizeof(TCB_t) );
	if(pTCB != NULL)
	{
		pTCB->tid = tid;
		pTCB->state = state;
		pTCB->prio = 0;
		getcontext(pTCB->context);
		/* Fazendo o setup da pilha do contexto */
		pTCP->context->uc_stack->ss_sp = (void *) malloc( sizeof(SIGSTKSZ) );
		pTCP->context->uc_stack->ss_size = SIGSTKSZ;
	}
	return pTCB;
}

TCB_t *popEXEC()
{
	TCB_t *ret = FILA_EXEC;
	FILA_EXEC = NULL;
	return ret;
}

TCB_t *findTCB(PFILA2 pFILA, int tid)
{
	TCB_t *ret = NULL;
	TCB_t *curr;

	if( pFila != NULL )
		if( !FirstFila2(pFila) )
			do
			{
				curr = (TCB_t *) GetAtIteratorFila2(pFila);
				if(curr != NULL)
					if( curr->tid = tid )
					{
						ret = curr;
						break;
					}
			}
			while( !NextFila2(pFila) );

	return ret;
}

int SetIteratorAtTCB(PFILA2 pFILA, int tid)
{
	int ret, found = 0;
	TCB_t *curr;

	if( pFila != NULL )
	{
		ret = FirstFila2(pFila);
		if( ret == 0 )
		{
			do
			{
				curr = (TCB_t *) GetAtIteratorFila2(pFila);
				if(curr != NULL)
					if( curr->tid == tid)
					{
						found = 1;
						break;
					}
			}
			while( !NextFila2(pFila) )
		
			if( !found )
				ret = -SETIT_NOTFOUND
			else
				ret = 0;
		}
		else
			ret = -SETIT_VAZIA
	}
	else 
		ret = -SETIT_OTHER;
	
	return ret;
}
