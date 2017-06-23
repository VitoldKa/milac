
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <syslog.h>
#include <sys/time.h>

#include <curl/curl.h>

#include "mila.h"
#include "str_replace.h"


#define MAX_BUF 1024*1024

const s_mila_profile mila_profile[] = {
	{"xxx@xxx.com" , "logintoken=d2ab6ace7c46fd55b9bcd6e1227db2c3dc220f533c1fa121ae8985b74330177bacb42121; connect1.sid=s%3AjxnmCOCPQvUiWuffeiO14rBCLgWbXaUD.UnxHmDnov9urmTiowoj9UQTFQRIlm38u%2FyV7UhdgT%2FA; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;"},
	{"vitold.ka@mila-i1.xxx.com" , "logintoken=6ccc163b05550f4088775dcaeae36a6db6d772100ad1b2938b27b88fa79f93f31594f1b2; connect1.sid=s%3AD3vM9bQ94SCra1ZRy_2xPwqYP3pHBMvd.mDTiXBs1NUVb%2B2%2BDd%2BhA61n2lVM%2FKm2v4hzEb94IyfI; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;"}
	};
	
	
char retbuf[1024*1024] = "";
double time_accept;
double time_processing;


int isemailexist(char *email)
{
	const s_mila_profile *profile;
	if(getfromemail(email, &profile) != 0)
		return 1;
	return 0;
}
int getfromemail(char *email, const s_mila_profile **profile)
{
	profile = NULL;
	int size = sizeof(mila_profile);
	for(int c = 0; c < size; c++)
	{
		if(strcmp(mila_profile[c].email, email)==0)
		{
			*profile = &mila_profile[c];
			return 0;
		}
	}
	return 1;
}


int quoat_decode(char *inbuf, int lenght, char *outbuf, int *newlenght)
{

	char *start, *end = inbuf+lenght;
	char *outpinter = outbuf;
	*newlenght = lenght;
	printf("newlenght: %d\n", lenght);	
//		printf("remaining: %d\n", end-start);
	// search for =

//	str_replace(start, lenght-(start-inbuf), "=\r\n", "\n");
	
	start = strstr(inbuf, "quoted-printable\n\n");
	if(!start)
		start = strstr(inbuf, "quoted-printable\r\n");
	if(start!=NULL)
	{
		printf("quote remove\n");
		start += strlen("quoted-printable\n\n");
		memcpy(outpinter, inbuf, start-inbuf);

		str_replace(start, lenght-(start-inbuf), "=\r\n", "");
		str_replace(start, lenght-(start-inbuf), "=\n", "");
		str_replace(start, lenght-(start-inbuf), "=C3", "\xC3");
		str_replace(start, lenght-(start-inbuf), "=A9", "\xA9");
		str_replace(start, lenght-(start-inbuf), "=AA", "\xAA");
		str_replace(start, lenght-(start-inbuf), "=3D", "\x3D");
		str_replace(start, lenght-(start-inbuf), "=A8", "\xA8");
		str_replace(start, lenght-(start-inbuf), "=A0", "\xA0");
		str_replace(start, lenght-(start-inbuf), "=A7", "\xA7");
		str_replace(start, lenght-(start-inbuf), "=C2", "\xC2");
		str_replace(start, lenght-(start-inbuf), "=09", "\x09");
		str_replace(start, lenght-(start-inbuf), "=E2", "\xE2");
		str_replace(start, lenght-(start-inbuf), "=80", "\x80");
		str_replace(start, lenght-(start-inbuf), "=99", "\x99");

		*newlenght -= 12*3 + 2;


/*	
	
	while((end = memchr(start ,'=', end-start))!=NULL )
	{
		printf("start: %x\n", start);
		printf("newlenght: %d\n", *newlenght);
		memcpy(outpinter, start, end-start-1);
		outpinter +=end-start-1;
		*newlenght -= 3;
		start = end+3;
		printf("remaining: %d\n", (inbuf+lenght)-start);
	}
*/	
	
		outbuf[*newlenght]= '\0';
		return 0;
	}
	return 1;
}

int get_accept_url(char *buf, int len, char *lbuf)
{
//	sscanf(buf, "Accepter%s>", lbuf);
/*	char lsearch[20][2];
	strcpy(lsearch[0], "/friendacceptfrommail/");
	strcpy(lsearch[1], "Accepter\r\n<");
*/

	const char lsearch[]= "/friendacceptfrommail/";
	char *lpbuf;
	
	lpbuf = strstr(buf, lsearch);
	if(lpbuf)
	{
//		printf("Accept found:\n");
			lpbuf += sizeof(lsearch)-1;
			
		char *end = strchr(lpbuf, '>');
		if(end)
		{
		strncpy(lbuf, lpbuf, end-lpbuf);
		printf("%s\n", lbuf);
		return 0;
	}
	}
	printf("Accept not found\n");
	return 1;
}

size_t header_callback(char *buffer,   size_t size,   size_t nitems,   void *userdata)
{
//	printf("%s\n", buffer);
	return nitems * size;
}


size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	// ptr JSON data ex. {"reason":"BadPath"}
//	memset(retbuf, 0, sizeof(retbuf));
	int offset = strlen(retbuf);
	memcpy(retbuf+offset, ptr, size*nmemb);
	retbuf[size*nmemb+offset] = 0;
//	printf("write_callbacks: %s\n", retbuf);
	return nmemb * size;
}




int mila (char *buf, int buf_size, char *to)
{
	clock_t begin = clock();	
//	printf("Milac\n");
	
//	memset(retbuf, 0, 1024*1024);
	retbuf[0] = 0;
//	int size = strlen(buf);
	int size = buf_size;

	long retvalue = 0;
	int c;

	char newbuf[size];
	int newbuf_lenght;
//		printf("bytesread: %d\n", size);	
	char urlstr[200] ="https://www.mila.com/friendacceptfrommail/";
	char orderid[20] = "";
  	if((get_accept_url(buf, size, orderid)==0))
	{
 //		quoat_decode(urlstr,size,newbuf,&newbuf_lenght);
		str_replace(orderid, 20, "=\r\n", "");
		str_replace(orderid, 20, "=\n", "");
	
		// https://www.mila.com/friendacceptfrommail/65696
//		strcpy(urlstr, "");
		strcat(urlstr, orderid);
		printf("%s\n", urlstr);
	
	//	printf("%s\n", urlstr);
		// test for https://www.mila.com/
//		if(strstr(urlstr, "https://www.mila.com/"))
//		{
			CURL *curl;
			CURLcode res;
			curl = curl_easy_init();

			if(curl)
			{
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		//		curl_easy_setopt(curl, CURLOPT_WRITEDATA, llhapns_worker);
		//		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
		//		curl_easy_setopt(curl, CURLOPT_HEADERDATA, llhapns_worker);

//				struct curl_slist *chunk = NULL;
//				chunk = curl_slist_append(chunk, "apns-topic: com.10-sor.silentclock.voip");
		//		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
				curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

				if(strcmp(to, "<vitold.ka@mila-i1.xxx.com>\r\n")==0)
					curl_easy_setopt(curl, CURLOPT_COOKIE, "logintoken=6ccc163b05550f4088775dcaeae36a6db6d772100ad1b2938b27b88fa79f93f31594f1b2; connect1.sid=s%3AD3vM9bQ94SCra1ZRy_2xPwqYP3pHBMvd.mDTiXBs1NUVb%2B2%2BDd%2BhA61n2lVM%2FKm2v4hzEb94IyfI; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;");
				else
					curl_easy_setopt(curl, CURLOPT_COOKIE, "logintoken=d2ab6ace7c46fd55b9bcd6e1227db2c3dc220f533c1fa121ae8985b74330177bacb42121; connect1.sid=s%3AjxnmCOCPQvUiWuffeiO14rBCLgWbXaUD.UnxHmDnov9urmTiowoj9UQTFQRIlm38u%2FyV7UhdgT%2FA; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;");
				curl_easy_setopt(curl, CURLOPT_URL, urlstr);
				clock_t end = clock();
				time_processing = (double)(end - begin) / CLOCKS_PER_SEC;
				printf("mila time_processing : %f\n", time_processing);

				res = curl_easy_perform(curl);

				time_accept = (double)(clock() - begin) / CLOCKS_PER_SEC;
				printf("mila time_accept : %f\n", time_accept);
		
				//printf("%u\n", res);
/*				if(res == CURLE_OK) 
				{
					char *url = NULL;
					curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &url);
			//    if(url)
			//      printf("Redirect to: %s\n", url);
				}*/
				curl_easy_cleanup(curl);
			}
//		}
	}
	else
	{
		strcpy(retbuf, "Accept not found");
		clock_t end = clock();
		time_processing = (double)(end - begin) / CLOCKS_PER_SEC;
		printf("mila time : %f\n", time_processing);
	}
	
	
//	printf("%s\n", buf);
	
	char str_time_processing[20];
	sprintf(str_time_processing, "%f", time_processing);

	char str_time_accept[20];
	sprintf(str_time_accept, "%f", time_accept);
	
	// write eml to disk
	char tmpdir[] = "/tmp/milaparser/"	;
	// write buf to file
	char filename[255] = "";
	strcat(filename, tmpdir);
	struct timeval time;
	gettimeofday(&time, NULL);
	long microsec = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;

	char name[255] = "";
	sprintf(name, "%ld", microsec);
	strcat(filename, name);
	FILE *eml = fopen(filename, "w");

	if(eml!=NULL)
	{
		fwrite(buf, 1, size, eml);
		fclose(eml);
	//	printf("wrote: %s",filename);
	}
	
	
	
	
	// email
	char *cmd = malloc(buf_size+sizeof(retbuf)+sizeof(buf)+10000);
//	printf("buf size: %d\n", buf_size);
	if(cmd)
	{
		cmd[0] = 0;
		strcat(cmd,"/usr/sbin/sendmail -t  <<EOF\n");
		strcat(cmd,"From: \"Mila Parser\" <xxx@xxx.com>\n");
		strcat(cmd,"To: "" <xxx@xxx.com>\n");
		strcat(cmd,"Subject: Mila return\n");
		strcat(cmd,"MIME-Version: 1.0\n");
		strcat(cmd,"Content-Type: multipart/mixed; boundary=\"_boundarystring\"\n");
		strcat(cmd,"\n");
		strcat(cmd,"--_boundarystring\n");
		strcat(cmd,"Content-Type: text/plain; charset=ISO-8859-1\n");
		strcat(cmd,"\n");
		strcat(cmd,urlstr);
		strcat(cmd,"\n");
		strcat(cmd,"time to process: ");
		strcat(cmd,str_time_processing);
		strcat(cmd,"\n");
		strcat(cmd,"time to accept: ");
		strcat(cmd,str_time_accept);
		strcat(cmd,"\n");
		strcat(cmd,"\n");
		strcat(cmd,"\n");
		if(buf_size < 500*1024)
			strcat(cmd,retbuf);
		strcat(cmd,"\n\n");
		strcat(cmd,"--_boundarystring\n");
		strcat(cmd,"Content-Type: application/octet-stream name=\"message.eml\"\n");
		strcat(cmd,"Content-Transfer-Encoding: Base64\n");
		strcat(cmd,"Content-Disposition: attachment; filename=\"message.eml\"\n");
		strcat(cmd,"\n");
		strcat(cmd,buf);
		strcat(cmd,"\n\n");
		strcat(cmd,"--_boundarystring--\n");
		strcat(cmd,"EOF");
		int ret_cmd = system(cmd);
	
//		memset(cmd, 0, sizeof(cmd));
		cmd[0] = 0;
		strcat(cmd,"/usr/sbin/sendmail xxx@xxx.com < ");
		strcat(cmd,filename);
		strcat(cmd,"\n");
		ret_cmd = system(cmd);

		if(cmd)
			free(cmd);
		else
			printf("Error free cmd\n");
	}
	return 0;
}
