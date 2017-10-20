#include <stdio.h>
#include <string.h>

#include "test.h"
#include "mila.h"

#define STRINGIZE(x) #x "/apns_registering_server.conf"
#define SSTRINGIZE(x) STRINGIZE(x)
#define CONFIGFILENAME SSTRINGIZE(ETC_DIR)

char str1[] = "                           <form name=\"userForm\" action=\"/friendaccept/107244\" method=\"POST\">\r\n                                <input type=\"hidden\" name=\"_csrf\"\r\n    \r\n     value=\"rfHkxaSdrgjzAatFqCAEDtUTO8sS7ZcO2a+jY=\"\r\n     \r\n\r\n    >\r\n\r\n                                <input type=\"submit\" class=\"btn btn-brand phs\" value=\"Accepter\" style=\"min-width: 145px;\" />\r\n                            </form>\t\t\t\r\n";
char str2[] = "                            <form name=\"userForm\" action=\"/friendaccept/107742\" method=\"POST\">\r\n                                <input type=\"hidden\" name=\"_csrf\"\r\n\r\n     value=\"a2omLlbFNlIWZgUl/fkus8aSbndKgfAX52ANo=\"\r\n\r\n\r\n    >\r\n\r\n                                <input type=\"submit\" class=\"btn btn-brand phs\" value=\"Accepter\" style=\"min-width: 145px;\" />\r\n                            </form>\r\n                        </div>\r\n                        <div class=\"col-3-12\">\r\n                            <form name=\"userForm\" action=\"/frienddeclinerequest/107742\" method=\"POST\">\r\n                                <input type=\"hidden\" name=\"_csrf\"\r\n\r\n     value=\"a2omLlbFNlIWZgUl/fkus8aSbndKgfAX52ANo=\"\r\n\r\n\r\n    >\r\n\r\n                                <input type=\"submit\" class=\"btn btn-unstyle phs\" value=\"D\u00E9cliner\" style=\"border:1px solid #4f8fcd; min-width: 145px;\" />\r\n                            </form>\r\n";
int mila_accept_ok(int argc, char *argv[])
{
	int profile;

	TEST_EQUAL(mila_accept(str1, 9), 0);
//	TEST_EQUAL(mila_accept(str2, 10), 0);


	
	
	TEST_END;		
}	
