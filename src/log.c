#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include <glib.h>

#include "log.h"

static 	pthread_mutex_t mutex;

void logging_SetLogFile(char* buf)
{
	strcpy(hlog.logfile, buf);
}

void logging_Init()
{
	pthread_mutex_init(&mutex, NULL);
}

void logging_SetLevel(unsigned int level)
{
	hlog.level = level;
}

void logging_SetFacilities(unsigned int facilities)
{
	hlog.facilities = facilities;
}


void GENERAL(unsigned int facilities, const char *format, ...)
{
	if(hlog.facilities & facilities)
	{
		va_list args;
		va_start(args, format);

		GString * buff = g_string_new(NULL);

		GTimeVal gtime;
		
		g_get_current_time(&gtime);
		gchar *time = g_time_val_to_iso8601(&gtime);
		g_string_append(buff, time);
		g_string_append(buff, "  ");
		g_string_append_vprintf(buff, format, args);
		g_string_append_c(buff, '\n');

		va_end(args);
		
		pthread_mutex_lock(&mutex);
		extern int DRY_RUN;
		if(!DRY_RUN)
		{
			FILE * pFile; 

			pFile = fopen(hlog.logfile, "a");
			if(pFile)
			{
				fwrite(buff->str, sizeof(char), buff->len, pFile);
				fclose(pFile);
			}
			else
			{
				int errsv = errno;
				printf("logfile open error: %d: %s\n", errsv, strerror(errsv));
			}
		}
		else
		{
			printf("%s", buff->str);
		}
		g_free(time);
		g_string_free(buff, TRUE);
		pthread_mutex_unlock(&mutex);
	}
}




void p_LOG(char *header, const char *format, ...)
{
	va_list args;
	va_start(args, format);

	unsigned int len = strlen(header);
	len += strlen(format);
	len += strlen("\n");
	char buff[len];

	strcat(buff, header);
	strcat(buff, format);
	strcat(buff, "\n");

	vsprintf((char*)buff, format, args);
	va_end(args);

}
