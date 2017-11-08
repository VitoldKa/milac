#include <stdio.h>
#include <string.h>

#include "test.h"
#include "mila.h"

#include "email.h"

#include "log.h"

int DRY_RUN = 1;


int main(int argc, char *argv[])
{
	logging_SetLogFile("log_ok.log");
	logging_SetLevel(-1);
	logging_SetFacilities(-1);

	GENERAL(LOG_LEVEL_GENERAL, "\nemail_ok");

	TEST_EQUAL(email_send("test", 0), 0);
	TEST_EQUAL(email_send("test", 1, "test test", "test.txt", "text/plain; charset=utf-8"), 0);
	TEST_EQUAL(email_send("test", 2, "test test", "test.txt", "text/plain; charset=utf-8", 
									"test2 test2", "test2.txt", "text/plain; charset=utf-8"), 0);
	
	TEST_END;		
}
