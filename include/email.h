#ifndef _EMAIL_H
#define _EMAIL_H

#include "curl/curl.h"

typedef struct  {

	CURL *curl;
	
} temail;
typedef temail* pemail;


typedef struct  {
	char *filename;
	
} tattachement;
typedef tattachement* pattachement;




int email_send(const char *body, int n, ...);



#endif