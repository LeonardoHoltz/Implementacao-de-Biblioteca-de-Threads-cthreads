#
# Makefile para programas exemplo
# 

CC=gcc
LIB_DIR=../lib

all: filosofos mutex

filosofos: filosofos.c $(LIB_DIR)/libcthread.a
	$(CC) -o filosofos filosofos.c -L$(LIB_DIR) -lcthread -lm -Wall

mutex: mutex.c $(LIB_DIR)/libcthread.a
	$(CC) -o mutex mutex.c -L$(LIB_DIR) -lcthread -lm -Wall
	
clean:
	rm -rf exemplo mandel teste_vetor barbeiro filosofos prodcons series *.o *~




