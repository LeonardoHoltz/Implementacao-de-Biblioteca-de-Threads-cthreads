#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/auxiliar.h"

int ccreate (void (*start)(void*), void *arg, int prio)
{

	if( init() == -INIT_ERROR)
		return -1;
	FILA_EXEC->prio += stopTimer();

	// criar contexto de execução da nova thread
	// com nova stack, etc. que ao retornar chame
	// uma função de limpeza/função que cheque cjoin/csignal

	static unsigned int tid = 1; // A cada chamada nova o tid inicia o valor incrementado da última chamada
	TCB_t nova_thread = allocTCB(tid, PROCST_APTO);
	tid++;
	
	getcontext(nova_thread->context);
	
	// Setup da pilha do contexto
	nova_thread->context->uc_stack->ss_sp = (void *) malloc( sizeof(SIGSTKSZ) );
	nova_thread->context->uc_stack->ss_size = SIGSTKSZ;
	
	//uc_link
	nova_thread->context->uc_link = &cleanupCtx;
	
	// Modifica o contexto
	makecontext(&(nova_thread->context), &start, 0);

	// colocar nova thread na lista de aptos
	InsertTCB(FILA_APTO, nova_thread);

	// retorna o identificador da thread criada
	return nova_thread->tid;
}

int cyield(void)
{

	if( init() == -INIT_ERROR)
		return -1;
	FILA_EXEC->prio += stopTimer();

	// guarda na fila de aptos
	TCB_t *curr = popEXEC();
	InsertTCB(FILA_APTO, curr);

	return escalonador(curr);
}

int cjoin(int tid)
{

	if( init() == -INIT_ERROR )
		return -1;
	FILA_EXEC->prio += stopTimer();

	// procura a thread com tid especificado
	TCB_t *found = findTCB(FILA_APTO, tid);
	if(found == NULL)
		found = findTCB(FILA_BLOQ, tid);

	if(found != NULL)
	{
		if(found->join_check)
			return -1;

		// guarda na fila de bloqueados
		TCB_t *curr = popEXEC();
		curr->state = PROCST_BLOQ;
		InsertTCB(FILA_BLOQ,curr);

		// guarda informação de retorno na thread procurada
		found->join_check = 1;
		found->join_tid = curr->tid;

		// se voltar a executar (FILA_EXEC não vazia) retorna.
		return escalonador(curr);
	}
	else
		return -1;
}

int csem_init(csem_t *sem, int count)
{
	if( init() == -INIT_ERROR )
		return -1;
	FILA_EXEC->prio += stopTimer();

	int ret = 0;

	sem = (csem_t *) malloc( sizeof(csem_t) );
	if(sem != NULL)
	{
		sem->count = count;
		ret = CreateFila2(sem->fila);
	}
	else
		ret = -1;

	startTimer();
	return ret;
}

int cwait(csem_t *sem)
{
	if( init() == -INIT_ERROR )
		return -1;
	FILA_EXEC->prio += stopTimer();
	// checa se o recurso está livre
	if(sem->count > 0)
	// se sim, volta a executar
	{
		sem->count--;
		startTimer();
		return 0;
	}
	// senão, bloqueia, colocando na fila do semáforo e na fila de bloqueados
	else
	{
		sem->count--;
		
		// tira da fila de executando
		TCB_t *thread_atual = popEXEC();
		thread_atual->state = PROCST_BLOQ;
		
		// colcoca na fila do semáforo
		AppendFila2(sem->fila, (void *) thread_atual);
		
		// coloca na fila de bloqueados
		AppendFila2(FILA_BLOQ, (void *) thread_atual);
		
		return escalonador(thread_atual);
	}
}

int csignal(csem_t *sem)
{
	if( init() == -INIT_ERROR )
		return -1;
	FILA_EXEC->prio += stopTimer();

	// aumenta a quantidade de recurso
	sem->count++;

	// checa se há alguém esperando o recurso (count <= 0)
	if(sem->count <= 0)
	// caso sim, acorda ele (nao-preemptivo)
	{
		// tira da fila do semáforo o primeiro (FCFS)
		FirstFila2(sem->fila);
		TCB_t *front = (TCB_t *) GetAtIteratorFila2(sem->fila);
		DeleteAtIteratorFila2(sem->fila);

		// tira da fila de bloqueados
		SetIteratorAtTCB(FILA_BLOQ, front->tid);
		DeleteAtIteratorFila2(FILA_BLOQ);
	
		// coloca na fila de aptos
		front->state = PROCST_APTO;
		InsertTCB(FILA_APTO, front);
	}

	// volta a executar
	startTimer();
	return 0;
}

int cidentify (char *name, int size)
{
	strncpy (name, "Augusto Dalcin Peiter - 287685\nErik Bardini da Rosa - 303693\nLeonardo Holtz de Oliveira - 287702", size);
	return 0;
}


