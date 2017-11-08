#include <stdio.h>
#include <string.h>

#include "test.h"
#include "mila.h"

int DRY_RUN = 1;



int main(int argc, char *argv[])
{
	int profile;

	// unkwon 
	TEST_NEQUAL(getfromemail("arny.ka@mila11.vitou.com", &profile), 0);
	TEST_EQUAL(profile, 0);


	// empty
	TEST_NEQUAL(getfromemail("", &profile), 0);
	TEST_EQUAL(profile, 0);


	// null
	TEST_NEQUAL(getfromemail(NULL, &profile), 0);
	TEST_EQUAL(profile, 0);

	
	
	TEST_END;		
}	
