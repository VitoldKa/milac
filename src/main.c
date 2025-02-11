#define __USE_POSIX199309

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <syslog.h>
#include <sys/time.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <signal.h>

#include <curl/curl.h>

#include "mila.h"
#include "mila_tread.h"

#include "log.h"


#define MAX_PID 8

#define VERSION_MAJOR	0
#define VERSION_MINOR	0
#define VERSION_PATCH	0

int DRY_RUN = 0;





int main2 (int argc, char *argv[])
{
	int fd;
    char *input = "/dev/stdin";
	char buf[1024*1024+1];

//			printf("size: %d\n", size);
 	int bytesread, offset=0;
	int size = 0;
    while( 1 )
    {
		if((bytesread = fread(  buf, 1, 1024*1024 - 1 , stdin)) > 0)
		{
			size += bytesread;
			buf[bytesread] = '\0';
			buf[1024*1024] = '\0';
//			printf("size: %d\n", size);
		}
		else
			break;
    }

	clock_t begin = clock();
	mila(buf, size, "<xxx@xxx.com>\r\n");
		clock_t end = clock();
	double mila_smtp = (double)(end - begin) / CLOCKS_PER_SEC;
	GENERAL(LOG_LEVEL_GENERAL, "mila time: %f", mila_smtp);
//			printf("size: %d\n", size);
//    fclose(fstdin);

	return 0;
}


int main (int argc, char *argv[])
{

	logging_Init();
	logging_SetLogFile("/var/log/mila_accept.log");
	logging_SetLevel(-1);
	logging_SetFacilities(-1);

	GENERAL(LOG_LEVEL_GENERAL, "Starting Mila_accept");


// 	pthread_mutex_init(&mutex, NULL);
	
	/*	struct addrinfo hints;
		struct addrinfo *result, *rp;
	
		struct sockaddr_storage peer_addr;
		socklen_t peer_addr_len;
		ssize_t nread;
		char buf[BUF_SIZE];
	*/
	
	main_srv();
	return 0;
}