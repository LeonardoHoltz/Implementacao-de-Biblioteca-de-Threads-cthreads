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
	int err_null = 0
	int err_vazia = 0;
	int err_insert = 0;
	int err_next = 0;
	
	if( pFila == NULL )
		err_null = 1;
	else
	if( pFila->first != NULL)
	{
		err_vazia = FirstFila2(pFila);

		while( !err_insert && !err_next )
		{		
			TCB_t *currTCB = (TCB_t *) GetAtIteratorFila2(pFila);
	
			if(currTCB != NULL)
			{
				if(tcb->prio < currTCB->prio)
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

	if(!err_vazia && !err_insert && !err_next && !err_null)
		return 0;
	else
	if( err_vazia || err_next == -NXTFILA_VAZIA)
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