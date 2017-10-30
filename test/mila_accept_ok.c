#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <glib.h>

#include "test.h"
#include "mila.h"

#include "log.h"

int DRY_RUN = 1;

int mila_accept(GString *buf, int lprofile, GString *retbuf);

int main(int argc, char *argv[])
{
	logging_SetLogFile("log_ok.log");
	logging_SetLevel(-1);
	logging_SetFacilities(-1);

	int profile;
//printf("%s", str);
	GString *buf = g_string_new(NULL);
	TEST_NEQUAL(buf, 0);
	
	
	
	/////////////////////////////////////////////////////////////////
	// Test server error
	printf("Test server error\n");
	char *filebuf;
	if(!fill_buffer_from_file("test/return_500.html", &filebuf))
	{
// 		printf("str: %s\n", filebuf);
		GString *str = g_string_new(filebuf);
		TEST_NEQUAL(mila_accept(str, 10, buf), 0);
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
		TEST_EQUAL(mila_accept(str, 10, buf), 0);
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
		TEST_EQUAL(mila_accept(str, 10, buf), 0);
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
		TEST_EQUAL(mila_accept(str, 10, buf), 0);
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
		TEST_EQUAL(mila_accept(str, 10, buf), 0);
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
		TEST_EQUAL(mila_accept(str, 10, buf), 0);
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
		TEST_EQUAL(mila_accept(str, 10, buf), 50);
		g_string_free(str, TRUE);
	}
	free(filebuf);





	free(buf);
	TEST_END;		
}	
