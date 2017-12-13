#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "test.h"
#include "mila.h"
#include "mila_tread.h"
#include "email.h"

#include "log.h"

int DRY_RUN = 1;


int main(int argc, char *argv[])
{

	logging_SetLogFile("log_ok.log");
	logging_SetLevel(-1);
	logging_SetFacilities(-1);

	GENERAL(LOG_LEVEL_GENERAL, "\nmila_ok");

	double time_accept;
	clock_t begin;


	main_srv();
	
// 	email_send("mila return", 2, retbuf->str, "mila.html", "text/html; charset=utf-8",
// 								 buf, "message.eml", "message/rfc822; charset=utf-8");

	TEST_END;		
}
