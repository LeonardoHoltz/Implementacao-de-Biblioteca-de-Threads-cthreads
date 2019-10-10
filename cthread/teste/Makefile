#
# Makefile para programas exemplo
# 

CC=gcc
LIB_DIR=../lib

all: exemplo mandel teste_vetor barbeiro filosofos prodcons series

filosofos: filosofos.c $(LIB_DIR)/libcthread.a
	$(CC) -o filosofos filosofos.c -L$(LIB_DIR) -lcthread -lm -Wall
	
clean:
	rm -rf exemplo mandel teste_vetor barbeiro filosofos prodcons series *.o *~




