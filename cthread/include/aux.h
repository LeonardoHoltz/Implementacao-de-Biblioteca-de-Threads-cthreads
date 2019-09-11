/*
 * aux.h: Funções auxiliares para alocação de recursos e manipulação de estruturas.
 *
 */
#ifndef _AUX_CTHREAD_
#define _AUX_CTHREAD_

#include "support.h"
#include "cdata.h"

extern PFILA2 FILA_APTO;
extern PFILA2 FILA_BLOQ;
extern TCB_t *FILA_EXEC;

/*------------------------------------------------------------------------------
Função: inicializa as estruturas (filas de tcb's) caso a biblioteca não tenha
	sido inicializada.

Ret:	==0, se conseguiu
	??? (a decidir)
------------------------------------------------------------------------------*/
int init();

/*------------------------------------------------------------------------------
Função:	insere TCB na fila apontada por pFila, levando em conta
	as prioridades.
Ret:	==0, se conseguiu
	!=0, caso contrário (erro)
------------------------------------------------------------------------------*/
int InsertTCB(PFILA2 pFila, TCB_t *tcb);

/*------------------------------------------------------------------------------
Função: aloca um TCB com os parâmetros dados.
Ret:	ponteiro para a estrutura, se conseguiu
	NULL, caso contrário
------------------------------------------------------------------------------*/
TCB_t *allocTCB(int tid, int state, int prio);

#endif
