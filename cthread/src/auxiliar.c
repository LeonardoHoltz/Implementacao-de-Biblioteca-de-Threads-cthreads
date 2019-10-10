#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/auxiliar.h"

static int _init_cthread_ = 0;



FILA2 fApto;
PFILA2 FILA_APTO = &fApto;

FILA2 fBloq;
PFILA2 FILA_BLOQ = &fBloq;

TCB_t *FILA_EXEC;
ucontext_t cleanupCtx;

void printFila2(PFILA2 pFila)
{
	if(FILA_EXEC != NULL)
		printf("exec: tid: %d prio: %d\n", FILA_EXEC->tid, FILA_EXEC->prio);

	if (pFila != NULL)
	{
		if( FirstFila2(pFila) == 0 )
		{
			do
			{
				TCB_t *curr = (TCB_t *) GetAtIteratorFila2(pFila);
				printf("tid: %d prio: %d ", curr->tid, curr->prio);
				if(curr->join_check)
					printf("join_tid: %d", curr->join_tid);
				printf("\n");
						
			}
			while( !NextFila2(pFila) );	
			printf("\n");		
		}
	}	
	else
		printf("Ponteiro Nulo\n");
}

int init()
{
	int ret = 0;

	if( !_init_cthread_ )
	{
		TCB_t *mainThread = (TCB_t *) malloc( sizeof(TCB_t) );

		CreateFila2(FILA_APTO);
		CreateFila2(FILA_BLOQ);
		
		if( mainThread != NULL)
		{
			mainThread->tid = 0;
			mainThread->state = PROCST_EXEC;
			mainThread->prio = 0;
			mainThread->join_check = 0;
			getcontext( &(mainThread->context) );

			// contexto fixo usado para quando threads terminam, fazer uc_link apontar para cleanupCtx
			getcontext(&cleanupCtx);
			cleanupCtx.uc_stack.ss_sp = (void *) malloc(SIGSTKSZ);
			cleanupCtx.uc_stack.ss_size = SIGSTKSZ;
			cleanupCtx.uc_link = NULL;
			
			if( cleanupCtx.uc_stack.ss_sp != NULL)
			{
				makecontext(&cleanupCtx, &cleanupFunc, 0);
				
				FILA_EXEC = mainThread;
		
				startTimer();
				_init_cthread_ = 1;
			}
			else
			{
				free( mainThread );
				ret = -INIT_ERROR;
			}
		}
		else
			ret = -INIT_ERROR;
	}
	
	return ret;
}

int InsertTCB(PFILA2 pFila, TCB_t *tcb)
{
	int ret = 0;
	int insert_middle = 0;
	
	if( pFila != NULL && tcb != NULL )
	{
		if( FirstFila2(pFila) != 0 )
			ret = AppendFila2(pFila, (void *) tcb);
		else
		{
			FirstFila2(pFila);
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
					DeleteAtIteratorFila2(pFila);
			}
			while(  !NextFila2(pFila) );
				
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
		pTCB->join_check = 0;
		getcontext( &(pTCB->context) );
		
	}
	return pTCB;
}

TCB_t *popEXEC()
{
	TCB_t *ret = FILA_EXEC;
	FILA_EXEC = NULL;
	return ret;
}

TCB_t *findTCB(PFILA2 pFila, int tid)
{
	TCB_t *ret = NULL;
	TCB_t *curr;

	if( pFila != NULL )
		if( FirstFila2(pFila) == 0 )
			do
			{
				curr = (TCB_t *) GetAtIteratorFila2(pFila);
				if(curr != NULL)
					if( curr->tid == tid )
					{
						ret = curr;
						break;
					}
			}
			while( NextFila2(pFila) == 0 );

	return ret;
}

int SetIteratorAtTCB(PFILA2 pFila, int tid)
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
			while( !NextFila2(pFila) );
		
			if( !found )
				ret = -SETIT_NOTFOUND;
			else
				ret = 0;
		}
		else
			ret = -SETIT_VAZIA;
	}
	else 
		ret = -SETIT_OTHER;
	
	return ret;
}

int escalonador(TCB_t *curr)
{

	if( FirstFila2(FILA_APTO) == 0 )
	{
		//coloca primeiro da fila de aptos na fila de execução
		FirstFila2(FILA_APTO);
		TCB_t *prox = (TCB_t *) GetAtIteratorFila2(FILA_APTO);
		prox->state = PROCST_EXEC;
		FILA_EXEC = prox;
		DeleteAtIteratorFila2(FILA_APTO);


		if( curr != prox )
		{
			if( curr != NULL )
			{
				startTimer();
				swapcontext( &(curr->context), &(prox->context) );			
			}			
			else
				setcontext( &(prox->context) );
		}	
	}
	else
		// nenhuma thread no estado apto, sem trabalho para fazer.
		exit(0);
	
	startTimer();
	return 0;
}

void cleanupFunc()
{
	TCB_t *curr = popEXEC();

	if( curr != NULL)
	{		
		// se alguém chamou cjoin nessa thread
		if(curr->join_check)
		{
			// procura a thread que chamou cjoin na fila de bloqueados
			int found = SetIteratorAtTCB(FILA_BLOQ, curr->join_tid);
			if( found == 0 )
			{
				// tira da fila de bloqueados
				TCB_t *join_thread = (TCB_t *) GetAtIteratorFila2(FILA_BLOQ);
				DeleteAtIteratorFila2(FILA_BLOQ);

				// coloca na fila de aptos
				InsertTCB(FILA_APTO, join_thread);
			}
		}
		
		// limpa a stack usada pela thread
		free(curr->context.uc_stack.ss_sp);
		// limpa o tcb
		free(curr);
	}

	escalonador(NULL);
}
