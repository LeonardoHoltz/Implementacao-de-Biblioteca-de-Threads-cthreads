/*
- 	Programa para verificar a exclusão mútua.
- é utilizado um semáforo contador com valor inicial 1 para simular
- um semáforo binário
- 	São criadas várias threads que tentam utilizar
- esse semáforo e várias threads independentes.
*/
#include "../include/support.h"
#include "../include/cthread.h"
#include <stdio.h>
#include <stdlib.h>

#define MAXT 50

csem_t mutex;

void uso_de_recurso(void *id)
{
	if( cwait(&mutex) == 0 )
	{
		printf("Sou a thread %d usando o recurso e chamando cyield\n",(int)id);
		cyield();
		printf("Sou a thread %d liberando o recurso e terminando\n",(int)id);
		csignal(&mutex);
	}
	
	return;
}

void outro_trabalho(void *id)
{
	int tid = (int)id;
	printf("Sou a thread %d fazendo outra coisa e chamando cyield\n",tid);
	cyield();
	printf("Sou a thread %d terminando\n", tid);

	return;
}

int main()
{
	int workers[MAXT];
	int i=0;

	csem_init(&mutex,1);
	
	for(;i<MAXT; i++)
	{
		if(i%2)
			workers[i] = ccreate(uso_de_recurso, (void *)i, 0);
		else
			workers[i] = ccreate(outro_trabalho, (void *)i, 0);
		
		if(workers[i] == -1)
		{ 
			printf("ERRO CRIACAO DE THREAD\n"); 
			exit(-1); 
		}
	}

	for(i=0;i<MAXT; i++)
	{
		cjoin(workers[i]);
	}

	exit(0);
}
