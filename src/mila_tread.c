#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <syslog.h>
#include <sys/time.h>
#include <ctype.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>

#include <glib.h>

#include "mila.h"
#include "mila_tread.h"

#include "log.h"


// adresse OVH
//#define INADDRESS "147.135.182.245"
// #define INADDRESS "178.33.43.38"
// test.xxx.com
//#define INADDRESS "145.239.227.138"
// adresse amazon AWS
#define INADDRESS "172.31.40.44"


#define STATE_WAIT_FOR_HELO 1
#define STATE_WAIT_FOR_MAIL 2
#define STATE_WAIT_FOR_RCPT 3
#define STATE_WAIT_FOR_DATA 4
#define STATE_WAIT_FOR_DOT 5
#define STATE_WAIT_FOR_QUIT 6
#define STATE_QUIT 7



const char smtp_ok[] = "250 Ok\r\n";
const char smtp_ehlo_replay[] = "250-mila11.xxx.com\r\n250-8BITMIME\r\n250-PIPELINING\r\n250 SIZE 157286400\r\n"; //
const char smtp_helo_replay[] = "250 mila11.xxx.com\r\n"; //
const char smtp_recipient_ok[] = "250 Recipient ok.\r\n";
const char smtp_relay_denied[] = "554 5.7.1 Relay access denied\r\n";
const char smtp_enter_dot[] = "354 Enter mail, end with \".\" on a line by itself\r\n";
const char smtp_closing_connection[] = "221 Closing connection\r\n";
const char smtp_command_not_implemented[] = "502 Command not implemented\r\n";


int sfd;

int run;



void sig_handler(int signum)
{
	GENERAL(LOG_LEVEL_GENERAL, "Received signal %d", signum);
    run = 0;
	close(sfd);
	GENERAL(LOG_LEVEL_GENERAL, "Stoping Mila_accept");
}




char *strlwr(char *str)
{
  unsigned char *p = (unsigned char *)str;

  while (*p) {
     *p = tolower((unsigned char)*p);
      p++;
  }

  return str;
}

#define EMAIL_SIZE 5*1024*1024

void* doSomeThing(void *arg)
{
	smila *lmila = arg;
	clock_t begin = clock();
	time_t rawtime; struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
// 	printf("%s\n", asctime (timeinfo));
	
	char *inbuffer = (char*) malloc(EMAIL_SIZE+1);
		if(!inbuffer)
			GENERAL(LOG_LEVEL_GENERAL, "Error buffer malloc");
	
	int n=0;
	//printf("->New Connection\n");
	const char outbuf[] = "220 mila11.xxx.com ESMTP Postfix (Ubuntu)\r\n";
	int ret = write(lmila->socket, outbuf, strlen(outbuf));
	{
		//printf("%x::ret : %d\n", (unsigned int)lmila->tid, ret);
	}
	int state = STATE_WAIT_FOR_HELO;
	int run = 1;
	char from[256]="", to[256]="";
	char *data = malloc(EMAIL_SIZE+1);
		if(!data)
			GENERAL(LOG_LEVEL_GENERAL, "Error data malloc");
			
	char *loutbuf = malloc(1024+1);;
		if(!data)
			GENERAL(LOG_LEVEL_GENERAL, "Error data malloc");
	loutbuf[0] = 0;
	
	data[0]=0;
	data[EMAIL_SIZE]=0;
	char *data_pointer = data;
	bzero(from, 255);
	bzero(to, 255);
//	bzero(data, EMAIL_SIZE+1);

	char *buffer = NULL;

	while(run)
	{
// 		bzero(inbuffer, 100);
// 		buffer[EMAIL_SIZE]=0;

		n = recv(lmila->socket,inbuffer,EMAIL_SIZE, 0);
		if(n == -1)
		{
			GENERAL(LOG_LEVEL_GENERAL, "%x::socket() failed: %m", (unsigned int)lmila->tid);
			close(lmila->socket);
			run = 0;
			break;
		}
		else if(n == 0)
		{
			GENERAL(LOG_LEVEL_GENERAL, "%x::Connetion closed by remote\n", (unsigned int)lmila->tid);
			close(lmila->socket);
			run = 0;
			state = STATE_QUIT;
			break;
		}	

		inbuffer[n] = 0;
		
		buffer = inbuffer;
			
		// parce the buffer
		if(state != STATE_WAIT_FOR_DOT)
		{
			strlwr(buffer);
			GENERAL(LOG_LEVEL_GENERAL, "%x::->%s\n", (unsigned int)lmila->tid, buffer);
		}

		// DATA mode
		if(state == STATE_WAIT_FOR_DOT)
		{
				// test for remainig size in data
				
				if(EMAIL_SIZE-(data_pointer-data) >= n)
				{			
					GENERAL(LOG_LEVEL_GENERAL, "%x:: data\n", (unsigned int)lmila->tid);
// 					GENERAL(LOG_LEVEL_GENERAL, ":: data: %s\n", buffer);
// 					GENERAL(LOG_LEVEL_GENERAL, ":: data: %x\n", buffer[0]);
					memcpy(data_pointer, buffer, n);
					data_pointer += n;
					data_pointer[0] = 0;
				}
				else
				{
					// discard the data
				}
	
				// TODO: more clever test for end of data
				
//  				printf("n: %ld\n", strlen(buffer));
 				
				char *dot = data_pointer;
 				search_dot:
 				dot = buffer;
 				int len = strlen(buffer);
// 				GENERAL(LOG_LEVEL_GENERAL, "smtp time befor mila: %f\n\n", time_smtp);
				dot = buffer + (len - 3);
				if(dot = strstr(dot, "\x2e")) {
 					GENERAL(LOG_LEVEL_GENERAL, "%x\n", dot[-1]);
 					GENERAL(LOG_LEVEL_GENERAL, "%c\n", dot[0]);
 					GENERAL(LOG_LEVEL_GENERAL, "%x\n", dot[1]);
 					GENERAL(LOG_LEVEL_GENERAL, "%x\n", dot[2]);
					GENERAL(LOG_LEVEL_GENERAL, "dot\n");
					dot++;
				}
//				else
//					GENERAL(LOG_LEVEL_GENERAL, "not dot\n");
				
// 				if(dot = strchr(dot, '.'))
// 				{
// 					GENERAL(LOG_LEVEL_GENERAL, "c: %c", *dot);
// 					if(dot[-1] == '\n')
// 					{
// 						GENERAL(LOG_LEVEL_GENERAL, "c-1: %c", dot[-1]);
// 						if(dot[1] == '\n' || dot[1] == '\r')

				if(strstr(buffer, "\n.\r\n") || 
				   strstr(buffer, "\n.\n\r") || 
				   strstr(buffer, "\n.\n") || 
				   strstr(buffer, "\n.\r") || 
				   (n == 2 && strstr(buffer, ".\n")) || 
				   (n == 3 && strstr(buffer, ".\r\n")) 
				)
						{
							GENERAL(LOG_LEVEL_GENERAL, "%s\n", buffer	);
//							GENERAL(LOG_LEVEL_GENERAL, "c+1%x", dot[1]);
		// 					GENERAL(LOG_LEVEL_GENERAL, "%x::->%s\n", (unsigned int)lmila->tid, buffer);
// 							printf("end of body\n");
							clock_t end = clock();
							double time_smtp = (double)(end - begin) / CLOCKS_PER_SEC;
							GENERAL(LOG_LEVEL_GENERAL, "smtp time befor mila: %f\n\n", time_smtp);

							mila(data, data_pointer-data, to+1);

							end = clock();
							time_smtp = (double)(end - begin) / CLOCKS_PER_SEC;
							GENERAL(LOG_LEVEL_GENERAL, "smtp time after mila: %f\n\n", time_smtp);
							const char loutbuf2[] = "250 Ok\r\n";
							GENERAL(LOG_LEVEL_GENERAL, "%x::<-%s", (unsigned int)lmila->tid, loutbuf2);
							ret = write(lmila->socket, loutbuf2, sizeof(loutbuf2)-1);		
				
							// clear buffer for new message on this connection
							// TODO: have we still a command (QUIT) in the buffer due to piplining 
							data[0] = 0;
							data_pointer = data;
								
							state = STATE_WAIT_FOR_MAIL;				
						}
// 					}
// 				}
// 				if (state == STATE_WAIT_FOR_DOT)
// 				{
// 					dot++;
// 					goto search_dot;
// 				}
		}
		//CMD mode
		else
		{
			char *lpinbuffer = NULL;
// 			printf("cmd recived: %s\n", buffer);
// 			printf("cmd size: %d\n", n);
			while(strchr(buffer, '\n') != NULL)
			{
// 				printf("n remaining: %ld ( %d\n", n-(buffer-inbuffer), n);
// 			printf("cmd recived: %s\n", buffer);
// 				printf("%ld\n",  strstr(buffer, "\n")-buffer);
// 				printf("loop\n");
				if(buffer[0] == 0)
				{
//					//printf("%d\n", n);
					//printf("%x::->%s", (unsigned int)lmila->tid, buffer);
				}		
				else if(strncmp(buffer, "ehlo", 4)==0)
				{
					strcat(loutbuf, smtp_ehlo_replay);
// 					GENERAL(LOG_LEVEL_GENERAL, "%x::<-%s", (unsigned int)lmila->tid, loutbuf);
	//				250 SMTPUTF8
					state = STATE_WAIT_FOR_MAIL;
				}
				else if(strncmp(buffer, "helo", 4)==0)
				{
					strcat(loutbuf, smtp_helo_replay);
// 					GENERAL(LOG_LEVEL_GENERAL, "%x::<-%s", (unsigned int)lmila->tid, loutbuf);
	//				250 SMTPUTF8
					state = STATE_WAIT_FOR_MAIL;
				}

				else if( ( strncmp(buffer, "mail from:", 10)==0 ))
				{
					strcat(loutbuf, smtp_ok);
// 					GENERAL(LOG_LEVEL_GENERAL, "%x::<-%s", (unsigned int)lmila->tid, loutbuf);
					strcpy(from, buffer+10);
					state = STATE_WAIT_FOR_RCPT;
				}

				else if(strncmp(buffer, "rcpt to:", 8)==0)
				{
	// 				GENERAL(LOG_LEVEL_GENERAL, "%x::->%s\n", (unsigned int)lmila->tid, buffer);
					if(state = STATE_WAIT_FOR_RCPT)
					{
						if( ( isemailexist(buffer+8+1))) // TODO: remove "\r\n"
						{
							strcpy(to, buffer+8);
							strcat(loutbuf, smtp_recipient_ok);
// 							GENERAL(LOG_LEVEL_GENERAL, "%x::<-%s", (unsigned int)lmila->tid, loutbuf);		
							state = STATE_WAIT_FOR_DATA;
						}
						else
						{
							strcat(loutbuf, smtp_relay_denied);
// 							GENERAL(LOG_LEVEL_GENERAL, "%x::<-%s", (unsigned int)lmila->tid, loutbuf);
							GENERAL(LOG_LEVEL_GENERAL, "%x::%s", (unsigned int)lmila->tid, buffer+8);					
							state = STATE_QUIT;
						}
					}
				}
				else if(strcmp(buffer, "data\r\n")==0)
				{
					strcat(loutbuf, smtp_enter_dot);
// 					GENERAL(LOG_LEVEL_GENERAL, "%x::<-%s", (unsigned int)lmila->tid, loutbuf);												
					state = STATE_WAIT_FOR_DOT;
				}
				else if(strcmp(buffer, ".\r\n")==0)
				{
					// TODO: to be removed
					strcat(loutbuf, smtp_ok);
// 					GENERAL(LOG_LEVEL_GENERAL, "%x::<-%s", (unsigned int)lmila->tid, loutbuf);												
					state = STATE_WAIT_FOR_QUIT;
				}
				else if(strcmp(buffer, "quit\r\n")==0)
				{
					strcat(loutbuf, smtp_closing_connection);
// 					GENERAL(LOG_LEVEL_GENERAL, "%x::<-%s", (unsigned int)lmila->tid, loutbuf);													
					close(lmila->socket);
					run = 0;
					state = STATE_QUIT;
				}
				else
				{
					strcat(loutbuf, smtp_command_not_implemented);
				}
				
				buffer = strchr(buffer, '\n');
				buffer += 1;
			}
			ret = write(lmila->socket, loutbuf, strlen(loutbuf));															
			GENERAL(LOG_LEVEL_GENERAL, "%x::<-%s", (unsigned int)lmila->tid, loutbuf);													
			loutbuf[0] = 0;
// 			printf("loopend\n");
		}

	}
	GENERAL(LOG_LEVEL_GENERAL, "thread_exit()");													

// 	printf("%x::stop thread\n", (unsigned int)lmila->tid);
	close(lmila->socket);
	
	clock_t end = clock();
	double time_smtp = (double)(end - begin) / CLOCKS_PER_SEC;
//	printf("smtp time: %f\n\n", time_smtp);
	
	if(inbuffer)
		free(inbuffer);
	if(loutbuf)
		free(loutbuf);
	if(data)
		free(data);
	pthread_t ltid = lmila->tid;
	if(lmila)
		free(lmila);
		
	pthread_detach(ltid);
// 	pthread_join(lmila->tid);
	pthread_exit(NULL);
	return NULL;
}




int mila_parce(smila *mila)
{
//	clock_t begin = clock();
	GENERAL(LOG_LEVEL_GENERAL, "start thread\n");
   	pthread_create(&mila->tid, NULL, &doSomeThing, mila);
//	clock_t end = clock();
//	double time_smtp = (double)(end - begin) / CLOCKS_PER_SEC;
//	printf("thread create time: %f\n\n", time_smtp);
	return 0;
}

int main_srv()
{
	signal(SIGINT, sig_handler);

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
		GENERAL(LOG_LEVEL_GENERAL, "socket() failed: %m");
		close(sfd);
	}
	int option = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	if (bind(sfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		// error
		GENERAL(LOG_LEVEL_GENERAL, "socket() failed: %m");
		close(sfd);
		return 1;
	}

	if(listen(sfd, 128)==-1)
	{
		GENERAL(LOG_LEVEL_GENERAL, "listen error");
		close(sfd);
			// error
	}

	clilen = sizeof(cli_addr);
	run = 1;

	GENERAL(LOG_LEVEL_GENERAL, "listening on %s:25", INADDRESS);

	mila_init();

	while(run)
	{
		usleep(10);
//		printf("listen\n");
		if((newsockfd = accept(sfd, (struct sockaddr *) &cli_addr, (socklen_t*)&clilen)))
		{
			GENERAL(LOG_LEVEL_GENERAL, "%d::Connection from %s", newsockfd, inet_ntoa(cli_addr.sin_addr));

			if(newsockfd == -1)
			{
				GENERAL(LOG_LEVEL_GENERAL, "newsockfd error: %s", strerror(errno));
			}
			// create thread
			smila *lsmila = malloc(sizeof(smila));
			lsmila->socket = newsockfd;
			mila_parce(lsmila);
			lsmila = NULL;
//			printf("accept\n");
		}
	}
	// wait for all tread to terminate
	mila_free();
//	unlink(sfd);
//	unlink(sfd);
	close(sfd);

}






