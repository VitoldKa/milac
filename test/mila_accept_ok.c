#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "test.h"
#include "mila.h"


int mila_accept(const char *buf, int lprofile, char *retbuf);

int main(int argc, char *argv[])
{
	int profile;
//printf("%s", str);
	char *buf = malloc(15*1024*1024);
	TEST_NEQUAL(buf, 0);
	
	
	
	/////////////////////////////////////////////////////////////////
	// Test server error
	printf("Test server error\n");
	char *filebuf;
	if(!fill_buffer_from_file("test/return.html", &filebuf))
	{
// 		printf("str: %s\n", filebuf);
		TEST_NEQUAL(mila_accept(filebuf, 10, buf), 0);
	}
	free(filebuf);


	/////////////////////////////////////////////////////////////////
	// Test Ok
	printf("Test Ok\n");
	if(!fill_buffer_from_file("test/return_ok.html", &filebuf))
	{
// 		printf("str: %s\n", filebuf);
		TEST_EQUAL(mila_accept(filebuf, 10, buf), 0);
	}
	free(filebuf);


	/////////////////////////////////////////////////////////////////
	// Test Partial Ok
	printf("Test Partial Ok\n");
	if(!fill_buffer_from_file("test/return_partial_ok.html", &filebuf))
	{
// 		printf("str: %s\n", filebuf);
		TEST_EQUAL(mila_accept(filebuf, 10, buf), 0);
	}
	free(filebuf);





	free(buf);
	TEST_END;		
}	
