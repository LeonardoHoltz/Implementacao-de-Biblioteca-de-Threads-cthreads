#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../include/auxiliar.h"

// HOLTZ: Fiz a alocacao mas n terminei a coisa da stack ainda (acho)
int ccreate (void (*start)(void*), void *arg, int prio)
{

	if( init() == -INIT_ERROR)
		return -1;
	FILA_EXEC->prio += stopTimer();

	// criar contexto de execução da nova thread
	// com nova stack, etc. que ao retornar chame
	// uma função de limpeza/função que cheque cjoin/csignal

	static unsigned int tid = 1; // A cada chamada nova o tid inicia o valor incrementado da última chamada
	TCB_t nova_thread = allocTCB(tid, PROCST_APTO); // ta faltando a stack ainda
	tid++;

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
    else
	// salva o contexto da thread atual

	// guarda na fila de aptos

	// se voltar a executar (FILA_EXEC não vazia) retorna.


	// só retorna em caso de erro.
	return escalonador();
}

int cjoin(int tid)
{

	if( init() == -INIT_ERROR )
		return -1;
	FILA_EXEC->prio += stopTimer();

	// salva o contexto da thread atual

	// guarda na fila de bloqueados salvando o tid que vai esperar

	// se voltar a executar (FILA_EXEC não vazia) retorna.


	// só retorna em caso de erro.
	return escalonador();
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
		if(!ret)
			free( (void *) sem );
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
	int ret = 0;
    if(sem->count > 0)
	// se sim, volta a executar
    {
        sem->count--;
        startTimer();
        return 0;
    }
	// senão, bloqueia
    else
    {
        sem->count--;
        TCB_t *thread_atual = popEXEC();
        thread_atual->state = PROCST_BLOQ;
        ret = InsertTCB(sem->fila, thread_atual);
        return escalonador();
    }
}

int csignal(csem_t *sem)
{
	if( init() == -INIT_ERROR )
		return -1;
	FILA_EXEC->prio += stopTimer();

	// aumenta a quantidade de recurso

	// checa se há alguém esperando o recurso (count < 0)

	// caso sim, acorda ele (nao-preemptivo)

	// volta a executar
}

int cidentify (char *name, int size)
{
	strncpy (name, "Augusto Dalcin Peiter - 287685\nErik Bardini da Rosa - 303693\nLeonardo Holtz de Oliveira - 287702", size);
	return 0;
}


