#ifndef _LOG_H
#define _LOG_H


typedef struct  {
	unsigned int level;
	unsigned int facilities;
	
	char logfile[255];
} tlogging;

typedef tlogging* plogging;

tlogging hlog;

// log level
#define LOG_LEVEL_GENERAL   1
#define LOG_LEVEL_NOTICE    2
#define LOG_LEVEL_VERBOSE   4
#define LOG_LEVEL_ERROR     8
#define LOG_LEVEL_DEBUG     16




#define LOG_GENERAL         1


void logging_Init(void);
void logging_SetLogFile(char*);
void logging_SetLevel(unsigned int);
void logging_SetFacilities(unsigned int);


void GENERAL(unsigned int, const char *format, ...)
									__attribute__ ((format (printf, 2, 3)));


void p_LOG(char*, const char *my_format, ...)
									__attribute__ ((format (printf, 2, 3)));



#endif