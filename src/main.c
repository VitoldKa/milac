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
#include <netinet/in.h>
#include <arpa/inet.h>

#include <signal.h>

#include <curl/curl.h>

#include "mila.h"
#include "mila_tread.h"


#define MAX_PID 8

#define VERSION_MAJOR	0
#define VERSION_MINOR	0
#define VERSION_PATCH	0

// adresse OVH
//#define INADDRESS "147.135.182.245"
//#define INADDRESS "178.33.43.38"
// adresse amazon AWS
#define INADDRESS "172.31.40.44"

int sfd;

int run;

void sig_handler(int signum)
{
    printf("\nReceived signal %d\n", signum);
    run = 0;
	close(sfd);
	printf("Stoping Mila_accept\n");
}


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
	printf("mila time: %f\n\n", mila_smtp);
//			printf("size: %d\n", size);
//    fclose(fstdin);

	return 0;
}

int main (int argc, char *argv[])
{
	signal(SIGINT, sig_handler);
	printf("Starting Mila_accept\n");
	
	pthread_mutex_init(&mutex, NULL);
	
	/*	struct addrinfo hints;
		struct addrinfo *result, *rp;
	
		struct sockaddr_storage peer_addr;
		socklen_t peer_addr_len;
		ssize_t nread;
		char buf[BUF_SIZE];
	*/
	int s, clilen, newsockfd;
	struct sockaddr_in serv_addr, cli_addr;
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(25);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	inet_aton(INADDRESS, &serv_addr.sin_addr);

	if((sfd = socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		//error
		printf("socket error\n");
		fprintf(stderr, "socket() failed: %m\n");
		close(sfd);
	}
	int option = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	if (bind(sfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		// error
		printf("bind error\n");
		fprintf(stderr, "socket() failed: %m\n");
		perror("socket");
		close(sfd);
		return 1;
	}

	if(listen(sfd, 1000)==-1)
	{
		printf("listen error\n");
		close(sfd);
			// error
	}

	clilen = sizeof(cli_addr);
	run = 1;
	while(run)
	{
		usleep(10);
//		printf("listen\n");
		if((newsockfd = accept(sfd, (struct sockaddr *) &cli_addr, (socklen_t*)&clilen)))
		{
			// create thread
			smila *lsmila = malloc(sizeof(smila));
			lsmila->socket = newsockfd;
			mila_parce(lsmila);
//			printf("accept\n");
		}
	}
	// wait for all tread to terminate
//	unlink(sfd);
//	unlink(sfd);
	close(sfd);
	
	return 0;
}