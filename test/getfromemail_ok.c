#include <stdio.h>
#include <string.h>

#include "test.h"
#include "mila.h"

int DRY_RUN = 1;



int main(int argc, char *argv[])
{
	int profile;

	TEST_EQUAL(getfromemail("vitold.ka@mila11.vitou.com", &profile), 0);
	TEST_EQUAL(profile, 10);

	TEST_EQUAL(getfromemail("vitold.ka.4@mila11.vitou.com", &profile), 0);
	TEST_EQUAL(profile, 9);

	TEST_EQUAL(getfromemail("vitold.ka@mila1-i1.vitou.com", &profile), 0);
	TEST_EQUAL(profile, 8);


	
	
	TEST_END;		
}	
