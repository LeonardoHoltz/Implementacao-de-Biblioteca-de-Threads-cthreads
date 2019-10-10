/*
 * auxiliar.h: Funções auxiliares para alocação de recursos e manipulação de estruturas.
 *
 */
#ifndef _AUX_CTHREAD_
#define _AUX_CTHREAD_

#include "support.h"
#include "cdata.h"

extern PFILA2 FILA_APTO;
extern PFILA2 FILA_BLOQ;
extern TCB_t *FILA_EXEC;
extern ucontext_t cleanupCtx;

/*------------------------------------------------------------------------------
Função: inicializa a biblioteca: as estruturas (filas de tcb's), dispara o 
	primeiro timer e cria o tcb da thread main (mas não salva o contexto)

Ret:	==0, se conseguiu
	-INIT_ERROR, caso contrário
------------------------------------------------------------------------------*/
#define INIT_ERROR 1
int init();

/*------------------------------------------------------------------------------
Função: aloca um TCB com os parâmetros dados. É preciso inicializar o 
	contexto manualmente.
Ret:	ponteiro para a estrutura, se conseguiu
	NULL, caso contrário
------------------------------------------------------------------------------*/
TCB_t *allocTCB(int tid, int state);

/*------------------------------------------------------------------------------
Função:	insere TCB na fila apontada por pFila, levando em conta
	as prioridades.
Ret:	==0, se conseguiu
	!=0, caso contrário (erro)
------------------------------------------------------------------------------*/
int InsertTCB(PFILA2 pFila, TCB_t *tcb);

/*------------------------------------------------------------------------------
Função: acha TCB pelo tid na fila apontada por pFila.
Ret:	ponteiro para TCB, se conseguiu
	NULL, caso contrário
------------------------------------------------------------------------------*/
TCB_t *findTCB(PFILA2 pFILA, int tid);

/*------------------------------------------------------------------------------
Função: coloca o iterador da fila apontada por pFila no TCB pelo tid
Ret:	==0, se conseguiu
	!=0, caso contrário
	Fila Vazia	=> -SETIT_VAZIA
	Não Achou	=> -SETIT_NOTFOUND
	outro erro	=> -SETIT_OTHER
------------------------------------------------------------------------------*/
#define SETIT_VAZIA 1
#define SETIT_NOTFOUND 2
#define SETIT_OTHER 3
int SetIteratorAtTCB(PFILA2 pFILA, int tid);

/*------------------------------------------------------------------------------
Função:	Remove o TCB do estado de executando
Ret:	ponteiro para TCB da thread que estava executando
------------------------------------------------------------------------------*/
TCB_t *popEXEC();

/*------------------------------------------------------------------------------
Função:	se fila de execução está vazia, coloca a thread mais prioritária
	para executar e passa o controle a ela, salvando o contexto atual na thread indicada.
Ret:	0, se houve sucesso
	-1, caso contrário
------------------------------------------------------------------------------*/
int escalonador(TCB_t *curr);

/*------------------------------------------------------------------------------
Função que cuida da finalização da thread, limpando as estruturas utilizadas e
acordando a thread que chamou cjoin(), caso exista.
------------------------------------------------------------------------------*/
void cleanupFunc();

void printFila2(PFILA2 pFila);

#endif
