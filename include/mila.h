#ifndef _MILA_H
#define _MILA_H

typedef struct {
	char *email;
	char *credentials;
} s_mila_profile;


int mila (char *inbuf, int size, char *to);

int isemailexist(char*);
int getfromemail(char*, const s_mila_profile**);

#endif
