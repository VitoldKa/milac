#ifndef _MILA_H
#define _MILA_H

#include <semaphore.h> 
#include <curl/curl.h>

extern pthread_mutex_t mutex;

typedef struct {
	char *email;
	char *bearer_key;
	char *credentials;
	char *cookiejar;
	char *forward;

	CURL *curl;
	CURL *curl_api;
	struct curl_slist *header_api_list;
	struct curl_slist *list;
	
	pthread_mutex_t mutex;
} s_mila_profile;

int mila_init(void);
int mila_free(void);
int mila (char *inbuf, int size, char *to);

int isemailexist(char*);
int getfromemail(char*, int*);

#endif
