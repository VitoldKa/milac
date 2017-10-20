#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <syslog.h>
#include <sys/time.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>

#include "mila.h"
#include "mila_tread.h"

#define STATE_WAIT_FOR_HELO 1
#define STATE_WAIT_FOR_MAIL 2
#define STATE_WAIT_FOR_RCPT 3
#define STATE_WAIT_FOR_DATA 4
#define STATE_WAIT_FOR_DOT 5
#define STATE_WAIT_FOR_QUIT 6
#define STATE_QUIT 7

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
	clock_t begin = clock();
	time_t rawtime; struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	printf("%s\n", asctime (timeinfo));
	smila *lmila = arg;
	
	char *buffer = malloc(EMAIL_SIZE+1);
		if(!buffer)
			printf("Error buffer malloc\n");
	
	int n=0;
	//printf("->New Connection\n");
	const char outbuf[] = "220 mila7.xxx.com ESMTP Postfix (Ubuntu)\r\n";
	int ret = write(lmila->socket, outbuf, strlen(outbuf));
	{
		//printf("%x::ret : %d\n", (unsigned int)lmila->tid, ret);
	}
	int state = STATE_WAIT_FOR_HELO;
	int run = 1;
	char from[256]="", to[256]="";
	char *data = malloc(EMAIL_SIZE+1);
		if(!data)
			printf("Error data malloc\n");
	data[0]=0;
	data[EMAIL_SIZE]=0;
	char *data_pointer = data;
	bzero(from, 255);
	bzero(to, 255);
//	bzero(data, EMAIL_SIZE+1);
	while(run)
	{
		bzero(buffer, 100);
//buffer[EMAIL_SIZE]=0;
		n = recv(lmila->socket,buffer,EMAIL_SIZE, 0);
		buffer[n] = 0;
		if(n == -1)
		{
			fprintf(stderr, "%x::socket() failed: %m\n", (unsigned int)lmila->tid);
			close(lmila->socket);
			run = 0;
			break;
		}
		else if(n == 0)
		{
			printf("%x::Connetion closed by remote\n", (unsigned int)lmila->tid);
			close(lmila->socket);
			run = 0;
			state = STATE_QUIT;
		}	
	
			
		// parce the buffer

		if(state != STATE_WAIT_FOR_DOT)
		{
			strlwr(buffer);
			printf("->%s\n", buffer);
		}

		if(state == STATE_WAIT_FOR_DOT)
		{
				// test for remainig size in data
				
				if(EMAIL_SIZE-(data_pointer-data) >= n)
				{			
					memcpy(data_pointer, buffer, n);
					data_pointer += n;
					data_pointer[0] = 0;
				}
				else
				{
					// discard the data
				}
//				//printf("%x::buff %s", (unsigned int)lmila->tid, buffer+n-4);
//printf("data size %d\n", n);

//				printf("%x::buff %s", (unsigned int)lmila->tid, buffer);

	
				if(strstr(buffer, "\r\n.\r\n"))
				{
 					printf("end of body\n");
					clock_t end = clock();
					double time_smtp = (double)(end - begin) / CLOCKS_PER_SEC;
					printf("smtp time befor mila: %f\n\n", time_smtp);

					mila(data, data_pointer-data, to+1);

					end = clock();
					time_smtp = (double)(end - begin) / CLOCKS_PER_SEC;
					printf("smtp time after mila: %f\n\n", time_smtp);
					const char loutbuf2[] = "250 Ok\r\n";
					//printf("%x::<-%s", (unsigned int)lmila->tid, loutbuf2);
					ret = write(lmila->socket, loutbuf2, sizeof(loutbuf2)-1);															
					state = STATE_WAIT_FOR_QUIT;				
				}
		}

		else
		{
			if(strcmp(buffer, "") != 0)
			{
	//				//printf("%d\n", n);
					//printf("%x::->%s", (unsigned int)lmila->tid, buffer);
			}		
			if(strncmp(buffer, "ehlo", 4)==0)
			{
	// 			printf("->EHLO\n");
	//			if(state = STATE_WAIT_FOR_HELO)
	//			{
					const char loutbuf[] = "250-mila7.xxx.com\r\n250-8BITMIME\r\n250 SIZE 157286400\r\n";
					printf("%x::<-%s", (unsigned int)lmila->tid, loutbuf);
					ret = write(lmila->socket, loutbuf, sizeof(loutbuf)-1);

	/*				char loutbuf2[] = "250-8BITMIME\r\n";
					//printf("<-%s", loutbuf2);
					ret = write(lmila->socket, loutbuf2, sizeof(loutbuf2));

					char loutbuf3[] = "250-SIZE 157286400\r\n";
					//printf("<-%s", loutbuf3);
				
	//				250-PIPELINING
	//				250 SMTPUTF8
					ret = write(lmila->socket, loutbuf3, sizeof(loutbuf3));
	*/
					state = STATE_WAIT_FOR_MAIL;
	//			}
			}

			if( ( strncmp(buffer, "mail from:", 10)==0 ))
			{
	// 			printf("mail from:\n");
	//			if(state = STATE_WAIT_FOR_MAIL)
	//			{
					const char loutbuf[] = "250 Ok\r\n";
					printf("%x::<-%s", (unsigned int)lmila->tid, loutbuf);
					strcpy(from, buffer+10);
					ret = write(lmila->socket, loutbuf, sizeof(loutbuf)-1);
					state = STATE_WAIT_FOR_RCPT;
	//			}
			}

			if(strncmp(buffer, "rcpt to:", 8)==0)
			{
	// 			printf("->%s\n", buffer);
				//printf("%x::->%s\n", (unsigned int)lmila->tid, buffer);
				if(state = STATE_WAIT_FOR_RCPT)
				{
					if( ( isemailexist(buffer+8+1))) // TODO: remove "\r\n"
					{
						strcpy(to, buffer+8);
						static const char loutbuf[] = "250 Recipient ok.\r\n";
						printf("%x::<-%s", (unsigned int)lmila->tid, loutbuf);
						ret = write(lmila->socket, loutbuf, sizeof(loutbuf)-1);				
						state = STATE_WAIT_FOR_DATA;
					}
					else
					{
						static const char loutbuf[] = "554 5.7.1 Relay access denied\r\n";
						printf("%x::<-%s", (unsigned int)lmila->tid, loutbuf);
						printf("%x::%s", (unsigned int)lmila->tid, buffer+8);
						ret = write(lmila->socket, loutbuf, sizeof(loutbuf)-1);										
						state = STATE_QUIT;
					}
				}
			}




			if(strcmp(buffer, "data\r\n")==0)
			{
	// 			printf("->%s\n", buffer);
	//			if(state = STATE_WAIT_FOR_DATA)
	//			{
					static const char loutbuf[] = "354 Enter mail, end with \".\" on a line by itself\r\n";
					printf("%x::<-%s", (unsigned int)lmila->tid, loutbuf);
					ret = write(lmila->socket, loutbuf, sizeof(loutbuf)-1);															
					state = STATE_WAIT_FOR_DOT;
					// check for the dot
	//			}
			}


			if(strcmp(buffer, ".\r\n")==0)
			{
	// 			printf("->%s\n", buffer);
	//			if(state = STATE_WAIT_FOR_DOT)
				{
					static const char loutbuf[] = "250 Ok\r\n";
					//printf("%x::<-%s", (unsigned int)lmila->tid, loutbuf);
					ret = write(lmila->socket, loutbuf, sizeof(loutbuf)-1);															
					state = STATE_WAIT_FOR_QUIT;



				}
			}

			if(strcmp(buffer, "quit\r\n")==0)
			{
	// 			printf("->%s\n", buffer);
				static const char loutbuf[] = "221 Closing connection\r\n";
				printf("%x::<-%s", (unsigned int)lmila->tid, loutbuf);
				ret = write(lmila->socket, loutbuf, sizeof(loutbuf)-1);															
				close(lmila->socket);
				run = 0;
				state = STATE_QUIT;
			}
		}

	}
	printf("%x::stop thread\n", (unsigned int)lmila->tid);
	close(lmila->socket);
	
	clock_t end = clock();
	double time_smtp = (double)(end - begin) / CLOCKS_PER_SEC;
//	printf("smtp time: %f\n\n", time_smtp);
	
	if(data)
		free(data);
	if(buffer)
		free(buffer);
	if(lmila)
		free(lmila);
}




int mila_parce(smila *mila)
{
//	clock_t begin = clock();
	//printf("start thread\n");
   	pthread_create(&mila->tid, NULL, &doSomeThing, mila);
//	clock_t end = clock();
//	double time_smtp = (double)(end - begin) / CLOCKS_PER_SEC;
//	printf("thread create time: %f\n\n", time_smtp);
	return 0;
}
