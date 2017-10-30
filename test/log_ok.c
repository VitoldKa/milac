#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "test.h"
#include "mila.h"
#include "log.h"

int DRY_RUN = 1;


int main(int argc, char *argv[])
{

	logging_SetLogFile("log_ok.log");
	logging_SetLevel(-1);
	logging_SetFacilities(-1);

	GENERAL(LOG_LEVEL_GENERAL, "Test %d", 5);

	TEST_END;		
}	
