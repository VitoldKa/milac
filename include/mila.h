#ifndef _MILA_H
#define _MILA_H

#include <semaphore.h> 
#include <curl/curl.h>

extern pthread_mutex_t mutex;

typedef struct {
	char *email;
	char *credentials;
	char *cookiejar;
	CURL *curl;
} s_mila_profile;

int mila_init(void);
int mila (char *inbuf, int size, char *to);

int isemailexist(char*);
int getfromemail(char*, int*);

#endif
