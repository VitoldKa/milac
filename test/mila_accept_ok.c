#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <curl/curl.h>
#include <glib.h>

#include "test.h"
#include "mila.h"

#include "log.h"

int DRY_RUN = 1;

int mila_accept(CURL *curl, GString *buf, int lprofile, GString *retbuf);

int main(int argc, char *argv[])
{
	logging_SetLogFile("log_ok.log");
	logging_SetLevel(-1);
	logging_SetFacilities(-1);

	GENERAL(LOG_LEVEL_GENERAL, "\nmila_accept_ok");

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
	DIR *d = opendir("test/web_match/");
	struct dirent *dir;
	
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			printf("file: %s\n", dir->d_name);
			if(strcmp(dir->d_name, ".") && strcmp(dir->d_name, ".."))
			{
				GENERAL(LOG_LEVEL_GENERAL, "file: %s", dir->d_name);

				char *filebuf;
				char filename[255] = "";
				strcat(filename, "test/web_match/");
				strcat(filename, dir->d_name);

				if(!fill_buffer_from_file(filename, &filebuf))
				{
					GString *str = g_string_new(filebuf);
					clock_t begin = clock();
					char url[255] = "";
					TEST_EQUAL(mila_accept(curl, str, 10, buf), 0);

					time_accept = (double)(clock() - begin) / CLOCKS_PER_SEC;
					free(filebuf);
					g_string_free(str, TRUE);
				}

			}
		}
	}
	}
/*	
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
					GString *str = g_string_new(filebuf);
					clock_t begin = clock();
					char url[255] = "";
					TEST_NEQUAL(mila_accept(curl, str, 10, buf), 0);

					time_accept = (double)(clock() - begin) / CLOCKS_PER_SEC;
					free(filebuf);
					g_string_free(str, TRUE);
				}
			}
		}
	}
	}
*/


/*

	
	/////////////////////////////////////////////////////////////////
	// Test server error
	printf("Test server error\n");
	char *filebuf;
	if(!fill_buffer_from_file("test/return_500.html", &filebuf))
	{
// 		printf("str: %s\n", filebuf);
		GString *str = g_string_new(filebuf);

		clock_t begin = clock();
		TEST_NEQUAL(mila_accept(curl, str, 10, buf), 0);
		time_accept = (double)(clock() - begin) / CLOCKS_PER_SEC;
		printf("mila time_accept : %f\n", time_accept);

		g_string_free(str, TRUE);
	}
	free(filebuf);


	/////////////////////////////////////////////////////////////////
	// Test Ok
	printf("Test Ok\n");
	if(!fill_buffer_from_file("test/return_ok.html", &filebuf))
	{
// 		printf("str: %s\n", filebuf);
		GString *str = g_string_new(filebuf);

		clock_t begin = clock();
		TEST_EQUAL(mila_accept(curl, str, 10, buf), 0);
		time_accept = (double)(clock() - begin) / CLOCKS_PER_SEC;
		printf("mila time_accept : %f\n", time_accept);

		g_string_free(str, TRUE);
	}
	free(filebuf);


	/////////////////////////////////////////////////////////////////
	// Test Partial Ok
	printf("Test Partial Ok\n");
	if(!fill_buffer_from_file("test/return_partial_ok.html", &filebuf))
	{
// 		printf("str: %s\n", filebuf);
		GString *str = g_string_new(filebuf);

		clock_t begin = clock();
		TEST_EQUAL(mila_accept(curl, str, 10, buf), 0);
		time_accept = (double)(clock() - begin) / CLOCKS_PER_SEC;
		printf("mila time_accept : %f\n", time_accept);

		g_string_free(str, TRUE);
	}
	free(filebuf);


	/////////////////////////////////////////////////////////////////
	// Test Partial Ok
	printf("Test 1\n");
	if(!fill_buffer_from_file("test/return1.html", &filebuf))
	{
// 		printf("str: %s\n", filebuf);
		GString *str = g_string_new(filebuf);

		clock_t begin = clock();
		TEST_EQUAL(mila_accept(curl, str, 10, buf), 0);
		time_accept = (double)(clock() - begin) / CLOCKS_PER_SEC;
		printf("mila time_accept : %f\n", time_accept);

		g_string_free(str, TRUE);
	}
	free(filebuf);


	/////////////////////////////////////////////////////////////////
	// Test Partial Ok
	printf("Test 2\n");
	if(!fill_buffer_from_file("test/return2.html", &filebuf))
	{
// 		printf("str: %s\n", filebuf);
		GString *str = g_string_new(filebuf);

		clock_t begin = clock();
		TEST_EQUAL(mila_accept(curl, str, 10, buf), 0);
		time_accept = (double)(clock() - begin) / CLOCKS_PER_SEC;
		printf("mila time_accept : %f\n", time_accept);

		g_string_free(str, TRUE);
	}
	free(filebuf);


	/////////////////////////////////////////////////////////////////
	// Test Partial Ok
	printf("Test 3\n");
	if(!fill_buffer_from_file("test/return3.html", &filebuf))
	{
// 		printf("str: %s\n", filebuf);
		GString *str = g_string_new(filebuf);

		clock_t begin = clock();
		TEST_EQUAL(mila_accept(curl, str, 10, buf), 0);
		time_accept = (double)(clock() - begin) / CLOCKS_PER_SEC;
		printf("mila time_accept : %f\n", time_accept);

		g_string_free(str, TRUE);
	}
	free(filebuf);


	/////////////////////////////////////////////////////////////////
	// Test Partial Ok
	printf("Test _1\n");
	if(!fill_buffer_from_file("test/return_1.html", &filebuf))
	{
// 		printf("str: %s\n", filebuf);
		GString *str = g_string_new(filebuf);

		clock_t begin = clock();
		TEST_EQUAL(mila_accept(curl, str, 10, buf), 50);
		time_accept = (double)(clock() - begin) / CLOCKS_PER_SEC;
		printf("mila time_accept : %f\n", time_accept);

		g_string_free(str, TRUE);
	}
	free(filebuf);

*/
	g_string_free(buf, TRUE);
	curl_easy_cleanup(curl);
	
	TEST_END;		
}	
