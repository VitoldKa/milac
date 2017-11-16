#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <curl/curl.h>
#include <glib.h>

#include "test.h"
#include "mila.h"

#include "log.h"

int DRY_RUN = 1;

int mila_find_emaillink(char *inbuf, char *url);


int main(int argc, char *argv[])
{
	logging_SetLogFile("log_ok.log");
	logging_SetLevel(-1);
	logging_SetFacilities(-1);

	GENERAL(LOG_LEVEL_GENERAL, "mila_find_emaillink_ok");

	int profile;
//printf("%s", str);
	GString *buf = g_string_new(NULL);
	TEST_NEQUAL(buf, 0);
	
	
	
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();


	
	double time_accept;
	clock_t begin;
	
	time_accept = (double)(clock() - begin) / CLOCKS_PER_SEC;
	printf("mila time_accept : %f\n", time_accept);

	




	/////////////////////////////////////////////////////////////////
	// mila_find_emaillink ok
	printf("mila_find_emaillink ok\n");

	{
	DIR *d = opendir("test/match");
	struct dirent *dir;
	
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if(!strstr(dir->d_name, ".") && !strstr(dir->d_name, ".."))
			{
				GENERAL(LOG_LEVEL_GENERAL, "file: %s", dir->d_name);

				char *filebuf;
				char filename[255] = "";
				strcat(filename, "test/match/");
				strcat(filename, dir->d_name);

				if(!fill_buffer_from_file(filename, &filebuf))
				{
					clock_t begin = clock();
					char url[255] = "";
					TEST_EQUAL(mila_find_emaillink(filebuf, url), 0);

					time_accept = (double)(clock() - begin) / CLOCKS_PER_SEC;
					free(filebuf);
				}
			}
		}
	}
	}

	/////////////////////////////////////////////////////////////////
	// mila_find_emaillink nok
	printf("mila_find_emaillink nok\n");

	{
	DIR *d = opendir("test/without_match");
	struct dirent *dir;
	
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if(!strstr(dir->d_name, ".") && !strstr(dir->d_name, ".."))
			{
				GENERAL(LOG_LEVEL_GENERAL, "file: %s", dir->d_name);

				char *filebuf;
				char filename[255] = "";
				strcat(filename, "test/without_match/");
				strcat(filename, dir->d_name);

				if(!fill_buffer_from_file(filename, &filebuf))
				{
					clock_t begin = clock();
					char url[255] = "";
					TEST_NEQUAL(mila_find_emaillink(filebuf, url), 0);

					time_accept = (double)(clock() - begin) / CLOCKS_PER_SEC;
					free(filebuf);
				}
			}
		}
	}
	}









	g_string_free(buf, TRUE);
	curl_easy_cleanup(curl);
	
	TEST_END;		
}	
