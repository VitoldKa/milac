#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "test.h"
#include "mila.h"
#include "log.h"

int DRY_RUN = 1;

pthread_barrier_t barrier;

void* doTheTest(void *arg)
{
	for(int c = 0; c<1000; c++)
		GENERAL(LOG_LEVEL_GENERAL, "Test %d", c);
		
	pthread_barrier_wait(&barrier);
	return NULL;
}


int main(int argc, char *argv[])
{

	logging_SetLogFile("log_ok.log");
	logging_SetLevel(-1);
	logging_SetFacilities(-1);


	// simple test
	GENERAL(LOG_LEVEL_GENERAL, "Test %d", 5);



	// multithread test
	pthread_t tid[100];

	pthread_barrier_init(&barrier, NULL, 101);

	for(int c = 0; c<100; c++)
		pthread_create(&tid[c], NULL, &doTheTest, mila);

	pthread_barrier_wait(&barrier);

	TEST_END;		
}	
