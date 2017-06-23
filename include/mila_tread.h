#ifndef _MILA_TREAD_H
#define _MILA_TREAD_H

typedef  struct {
	int socket;
	pthread_t tid;
	char *inbuf;
} smila;

int mila_parce(smila *mila);

#endif