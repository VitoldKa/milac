
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <syslog.h>
#include <sys/time.h>
#include <pthread.h>

#include <curl/curl.h>

#include "base64.h"

#include "mila.h"
#include "str_replace.h"


#define MAX_BUF 1024*1024

const s_mila_profile mila_profile[] = {
	{ "xxx@xxx.com.com" ,    "logintoken=e589264ab5f6cfee2d8dd907b1c3bfc4f523f6e97f9a811f2bc21204f43c71a4a84b3c39; connect1.sid=s%3AD-FLCyWr3KEh8PmEcsT5OEHxbIaam8-h.mDiYc9k2X25xEFvk6R%2F1MI%2B7cKQ%2BusMnPa8uF7%2BTaoo; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;" },
	{ "xxx@xxx.com.com" ,    "logintoken=e589264ab5f6cfee2d8dd907b1c3bfc4f523f6e97f9a811f2bc21204f43c71a4a84b3c39; connect1.sid=s%3AD-FLCyWr3KEh8PmEcsT5OEHxbIaam8-h.mDiYc9k2X25xEFvk6R%2F1MI%2B7cKQ%2BusMnPa8uF7%2BTaoo; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;" },
	{ "xxx@xxx.com.com" ,    "logintoken=e589264ab5f6cfee2d8dd907b1c3bfc4f523f6e97f9a811f2bc21204f43c71a4a84b3c39; connect1.sid=s%3AD-FLCyWr3KEh8PmEcsT5OEHxbIaam8-h.mDiYc9k2X25xEFvk6R%2F1MI%2B7cKQ%2BusMnPa8uF7%2BTaoo; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;" },
	{ "xxx@xxx.com.com" ,    "logintoken=e589264ab5f6cfee2d8dd907b1c3bfc4f523f6e97f9a811f2bc21204f43c71a4a84b3c39; connect1.sid=s%3AD-FLCyWr3KEh8PmEcsT5OEHxbIaam8-h.mDiYc9k2X25xEFvk6R%2F1MI%2B7cKQ%2BusMnPa8uF7%2BTaoo; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;" },
	
	{ "vitold.ka@mila-i1.xxx.com" ,  "logintoken=a5c67079e196c0e601c047dfb0b0b4b78ae9e90e6cf16f983d06ff99273932ff68500762; connect1.sid=s%3A3fSQILoYmHQFKWkT0yJjzzP17j4sRom6.GpAtjWgfnGWMXbR5Ne2h9tn5MNpW4sSkz9%2FWtqqJJ4I; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;" },
	{ "xxx@xxx.com.com" , "logintoken=4f833d252fe5e8b1b412b8cf9a5c15bbf72e50e369025aa2196ed902a770f568aac8b980; connect1.sid=s%3A_XeBOun-gsftcTYb5KvoOda0Ef6IAz3a.igtssQM4L47NuPB6994hczXS%2BZe%2FgXOL%2FcAqMsDLvKc; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;" },
	{ "xxx@xxx.com.com" , "logintoken=4f833d252fe5e8b1b412b8cf9a5c15bbf72e50e369025aa2196ed902a770f568aac8b980; connect1.sid=s%3A_XeBOun-gsftcTYb5KvoOda0Ef6IAz3a.igtssQM4L47NuPB6994hczXS%2BZe%2FgXOL%2FcAqMsDLvKc; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;" },

	{ "xxx@xxx.com.com" , "logintoken=4f833d252fe5e8b1b412b8cf9a5c15bbf72e50e369025aa2196ed902a770f568aac8b980; connect1.sid=s%3A_XeBOun-gsftcTYb5KvoOda0Ef6IAz3a.igtssQM4L47NuPB6994hczXS%2BZe%2FgXOL%2FcAqMsDLvKc; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;" },
	
	
	
	
		{ "vitold.ka@mila1-i1.xxx.com" , "logintoken=d140404678b19eeb9887bbfc3da57f12a501896564092f0f39706f9e6c3c528f71f6f9b3; connect1.sid=s%3AbTDzU3D9FoqI2RNB-S3PsA8AvnEzjg81.jF%2F9McapKItN19M5F0kVdvCQvIpHJsxy6v6ZF4K0LCo; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;" },
		
	{ "xxx@xxx.com.com" , "logintoken=535485576fb0fbb3c346da77903fb5f9cd0f0f12ac2bd96044321f7f5a9b07b101b2cbb8; connect1.sid=s%3AcUIHLolfcbFkh8PKQR3uBWfa2AvkxYME.WBDOwmu9KnhdgM5PzIptCANKj%2FWVzkkj13KVU3mTvdE; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;" },
		
	{ "xxx@xxx.com.com" ,    "logintoken=c42660a8cde05bd85b6df15d1fcbbb4f5ddabbe30b008c552e7b0c8ce0e1798bc581fc13; connect1.sid=s%3AQbfdoYOtg5iyD538d-PDIc7SaGq21INY.sExzYx2VyJLHrRilaYjfsnEoPqPQMPrMge4pJ8zno50; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;" }
		
		
		
		
		
		
		
		
	};

/*const s_mila_profile mila_profile[] = {
	{ "xxx@xxx.com.com" ,    "logintoken=8f1e6c8e779dee1e2c1c28129a662fd458a72b5de0ba8aa33ece81475b8f02fa581615be; connect1.sid=s%3ASL5CZsCItSodWmwQVW9AWpZterm6nHNk.t%2Bb3pzjSeoJObHciwnRUeEzxqsnmNohO7GW%2B%2BYZZgjQ; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;" },
	{ "xxx@xxx.com.com" ,    "logintoken=8f1e6c8e779dee1e2c1c28129a662fd458a72b5de0ba8aa33ece81475b8f02fa581615be; connect1.sid=s%3ASL5CZsCItSodWmwQVW9AWpZterm6nHNk.t%2Bb3pzjSeoJObHciwnRUeEzxqsnmNohO7GW%2B%2BYZZgjQ; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;" },
	{ "vitold.ka@mila-i1.xxx.com" ,  "logintoken=8f1e6c8e779dee1e2c1c28129a662fd458a72b5de0ba8aa33ece81475b8f02fa581615be; connect1.sid=s%3ASL5CZsCItSodWmwQVW9AWpZterm6nHNk.t%2Bb3pzjSeoJObHciwnRUeEzxqsnmNohO7GW%2B%2BYZZgjQ; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;" },
	{ "vitold.ka@mila1-i1.xxx.com" , "logintoken=8f1e6c8e779dee1e2c1c28129a662fd458a72b5de0ba8aa33ece81475b8f02fa581615be; connect1.sid=s%3ASL5CZsCItSodWmwQVW9AWpZterm6nHNk.t%2Bb3pzjSeoJObHciwnRUeEzxqsnmNohO7GW%2B%2BYZZgjQ; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;" }
	};
*/

pthread_mutex_t mutex;
	
char retbuf[1024*1024] = "";
double time_accept;
double time_processing;

// true if found
int isemailexist(char *email)
{
	int profile;
	if(getfromemail(email, &profile) == 0)
		return 1;
	return 0;
}
int getfromemail(char *email, int *profile)
{
	int size = sizeof(mila_profile)/sizeof(s_mila_profile);
//	printf("sizeof mila_profile: %d\n", size);
	for(int c = 0; c < size; c++)
	{
		int len = strlen(mila_profile[c].email);
//		printf("email  : %s ? %s\n", email+1, mila_profile[c].email);
//		printf("email len : %d\n", len);
		if(strncmp(mila_profile[c].email, email+1, len)==0)
		{
			printf("credential found\n");
			*profile = c;
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

	const char lsearch[]= "friendservicecalls#";
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
//	char urlstr[200] ="https://www.mila.com/friendacceptfrommail/";
	char urlstr[200] ="https://www.mila.com/loginWithPW/4f25f000?next=friendservicecalls#";
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

				int profile;
				if(!getfromemail(to, &profile))
				{
					printf("mila credentials : %s\n", mila_profile[profile].credentials);
					curl_easy_setopt(curl, CURLOPT_COOKIE, mila_profile[profile].credentials);
				}
				else
				{
					printf("using default credential\n");
					curl_easy_setopt(curl, CURLOPT_COOKIE, mila_profile[0].credentials);
				}

				curl_easy_setopt(curl, CURLOPT_URL, urlstr);
				clock_t end = clock();
				time_processing = (double)(end - begin) / CLOCKS_PER_SEC;
				printf("mila time_processing : %f\n", time_processing);

				// accept here
//				for(int c = 0; c < 2; c++)
					res = curl_easy_perform(curl);

				// todo: retry
				
				time_accept = (double)(clock() - begin) / CLOCKS_PER_SEC;
				printf("mila time_accept : %f\n", time_accept);


				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "_csrf=rfHkxaSdrgjzAatFqCAEDtUTO8sS7ZcO2a+jY=");

				char accepturl[255] = "https://www.mila.com/friendaccept/";
				strcat(accepturl, orderid);
				printf("%s\n", urlstr);
				curl_easy_setopt(curl, CURLOPT_URL, accepturl);
//				res = curl_easy_perform(curl);

				//printf("%u\n", res);
/*				if(res == CURLE_OK) 
				{
					char *url = NULL;
					curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &url);
			//    if(url)
			//      printf("Redirect to: %s\n", url);
				}*/
				
				/*
                            <form name="userForm" action="/friendaccept/107244" method="POST">
                                <input type="hidden" name="_csrf"
    
     value="rfHkxaSdrgjzAatFqCAEDtUTO8sS7ZcO2a+jY="
     

    >

                                <input type="submit" class="btn btn-brand phs" value="Accepter" style="min-width: 145px;" />
                            </form>				*/
// <form.*name="_csrf".*value="(.*)".*<\/form>

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
		char *newbuf = malloc(Base64encode_len(strlen(buf)));
		Base64encode(newbuf, buf, strlen(buf));
		strcat(cmd,newbuf);
		strcat(cmd,"\n\n");
		strcat(cmd,"--_boundarystring--\n");
		strcat(cmd,"EOF");
		printf("Sending Mila report\n");
		
		pthread_mutex_lock(&mutex);
		int ret_cmd = system(cmd);
		pthread_mutex_unlock(&mutex);

//		memset(cmd, 0, sizeof(cmd));
		cmd[0] = 0;
		strcat(cmd,"/usr/sbin/sendmail xxx@xxx.com < ");
		strcat(cmd,filename);
		strcat(cmd,"\n");
		printf("Forwarding email\n");

		pthread_mutex_lock(&mutex);
		ret_cmd = system(cmd);
		pthread_mutex_unlock(&mutex);

		if(cmd)
			free(cmd);
		else
			printf("Error free cmd\n");
	}
	printf("Mila end\n");
	return 0;
}
