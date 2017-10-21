
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
#include <glib.h>

#include "base64.h"

#include "mila.h"
#include "str_replace.h"


#define MAX_BUF 1024*1024*5

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
	
char retbuf[5*1024*1024] = "";
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
	if(email == NULL)
		return 15;
	int size = sizeof(mila_profile)/sizeof(s_mila_profile);
//	printf("sizeof mila_profile: %d\n", size);
	for(int c = 0; c < size; c++)
	{
		int len = strlen(mila_profile[c].email);
//		printf("email  : %s ? %s\n", email, mila_profile[c].email);
//		printf("email len : %d\n", len);
		if(strncmp(mila_profile[c].email, email, len)==0)
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

	const char lsearch[]= "https://www.mila.com/loginWithPW/";
	const char lsearch2[] = "?next=3Dfriendservicecalls/";
	char *lpbuf;
	
	lpbuf = strstr(buf, lsearch);
	if(lpbuf)
	{
		printf("Accept found:\n");
		lpbuf += sizeof(lsearch)-1;

			lpbuf += sizeof(lsearch2)-1;
			char *end = strstr(lpbuf, "=\r\n>");
			if(end-lpbuf > 200)
			{	
				printf("overflow %d\n", end-lpbuf);
				return 4;
			}
			if(end)
			{
				strncpy(lbuf, lpbuf, end-lpbuf);
				printf("%s\n", lbuf);
				return 0;
			}
			return 2;
	}
	printf("Accept not found\n");
	return 1;
}

static
void dump(const char *text,
          FILE *stream, unsigned char *ptr, size_t size)
{
  size_t i;
  size_t c;
  unsigned int width=0x10;
 
  fprintf(stream, "%s, %10.10ld bytes (0x%8.8lx)\n",
          text, (long)size, (long)size);
 
  for(i=0; i<size; i+= width) {
    fprintf(stream, "%4.4lx: ", (long)i);
 
    /* show hex to the left */
    for(c = 0; c < width; c++) {
      if(i+c < size)
        fprintf(stream, "%02x ", ptr[i+c]);
      else
        fputs("   ", stream);
    }
 
    /* show data on the right */
    for(c = 0; (c < width) && (i+c < size); c++) {
      char x = (ptr[i+c] >= 0x20 && ptr[i+c] < 0x80) ? ptr[i+c] : '.';
      fputc(x, stream);
    }
 
    fputc('\n', stream); /* newline */
  }
}
 
static
int my_trace(CURL *handle, curl_infotype type,
             char *data, size_t size,
             void *userp)
{
  const char *text;
  (void)handle; /* prevent compiler warning */
  (void)userp;
 
  switch (type) {
  case CURLINFO_TEXT:
    printf("== Info: %s", data);
  default: /* in case a new one is introduced to shock us */
    text = "= default";
  
  case CURLINFO_HEADER_OUT:
    text = "=> Send header";
    break;
  case CURLINFO_DATA_OUT:
    text = "=> Send data";
    break;
  case CURLINFO_SSL_DATA_OUT:
    text = "=> Send SSL data";
    break;
  case CURLINFO_HEADER_IN:
    text = "<= Recv header";
    break;
  case CURLINFO_DATA_IN:
    text = "<= Recv data";
    break;
  case CURLINFO_SSL_DATA_IN:
    text = "<= Recv SSL data";
    break;
  }
 
//  dump(text, stderr, (unsigned char *)data, size);
  printf("%s :: %s\n", text, data);
  	int offset = strlen(retbuf);
	memcpy(retbuf+offset, text, strlen(text)+1);
	retbuf[strlen(text)+offset] = 0;

  	offset = strlen(retbuf);
	memcpy(retbuf+offset, data, size);
	retbuf[size+offset] = 0;

  return 0;
}
 

size_t header_callback(char *buffer,   size_t size,   size_t nitems,   void *userdata)
{
	printf("header: %s\n", buffer);
	int offset = strlen(retbuf);
	memcpy(retbuf+offset, buffer, size*nitems);
	retbuf[size*nitems+offset] = 0;
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


int mila_accept(char *buf, int lprofile)
{
	GError *err = NULL;
	GMatchInfo *matchInfo;
	GRegex *regex;

	int error = 1;

	char *start = strstr(buf, "Nouvelles demandes");

	char *end = strstr(buf, "Commandes en cours");
	printf("%d\n", start);
	printf("%d\n", end);
	printf("%d\n", end-start);
	if(start && end)
	{
		char *newbuf = malloc(end-start+1);
		strncpy(newbuf, start, end-start);

		printf("%s\n", newbuf);

		str_replace(newbuf, strlen(buf), "\n", "");
		str_replace(newbuf, strlen(buf), "\r", "");

		// search for _csrf=
		regex = g_regex_new ("<form.*action=\"(.*)\".*name=\"_csrf\".*value=\"(.*)\".*Accepter.*<\\/form>", G_REGEX_UNGREEDY, 0, &err);   
	//	regex = g_regex_new ("<form.*name=\"_csrf\".*value=\"(.*)\"", G_REGEX_MULTILINE|G_REGEX_UNGREEDY, 0, &err);   
		if(regex == NULL)
		{
			printf("regex error\n");
			return 15;
		}
		else
		{
			if(err)
			{
				printf("gerror : %s\n", err->message);
			}

			g_regex_match (regex, newbuf, 0, &matchInfo);

			CURL *curl;
			CURLcode res;
			curl = curl_easy_init();

			struct curl_slist *list = NULL;

			list = curl_slist_append(list, "Cache-Control: max-age=0");
			list = curl_slist_append(list, "Origin: https://www.mila.com");
			list = curl_slist_append(list, "Upgrade-Insecure-Requests: 1");
			list = curl_slist_append(list, "Content-Type: application/x-www-form-urlencoded");
			list = curl_slist_append(list, "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/61.0.3163.100 Safari/537.36");
			list = curl_slist_append(list, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8");
			list = curl_slist_append(list, "Referer: https://www.mila.com/friendservicecalls");
			list = curl_slist_append(list, "Accept-Encoding: gzip, deflate, br");
			list = curl_slist_append(list, "Accept-Language: fr-CH,fr-FR;q=0.8,fr;q=0.6,en-US;q=0.4,en;q=0.2");

// list = curl_slist_append(list, "Cookie: optimizelyEndUserId=oeu1486559193069r0.2422451363507594; optimizelySegments=%7B%22700475046%22%3A%22gc%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; logintoken=a5da1827573d0e4bcf6966c9954a7aa2335caf259b048a4dacc696326078ba4df20b7635; connect1.sid=s%3AzuHy12n0PP0pvG1FNB5V6owfRbF_Meb2.qs5XxiVgaJphR1nvGErxu%2FpSe8Ux0i3%2BveMulspgm88; _dc_gtm_UA-29191003-1=1; _ga=GA1.2.881151050.1486559200; _gid=GA1.2.1734846664.1508526208; _gat_UA-29191003-1=1");










			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
			curl_easy_setopt(curl, CURLOPT_POST, 1L);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
			// curl_easy_setopt(curl, CURLOPT_HEADERDATA, llhapns_worker);
			curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_trace);
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

			// printf("profile: %d\n", lprofile);
			// printf("mila credentials : %s\n", mila_profile[lprofile].credentials);
			curl_easy_setopt(curl, CURLOPT_COOKIE, mila_profile[lprofile].credentials);

			while (g_match_info_matches (matchInfo)) {
				gchar *orderid = g_match_info_fetch (matchInfo, 1);
				gchar *csrf = g_match_info_fetch (matchInfo, 2);
			 
				g_print ("orderid: %s\n\n", orderid);
				g_print ("csrf: %s\n\n", csrf);




				char post[200] = "_csrf=";
				strcat(post, csrf);
				strcat(post, "");
				char *csrf_enc = curl_easy_escape(curl, csrf, strlen(csrf));
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, csrf_enc);
				curl_easy_setopt(curl, CURLOPT_REFERER, "https://www.mila.com/friendservicecalls");

				printf("post: %s\n", csrf_enc);
				printf("postlen: %d\n", strlen(post));

				char accepturl[255] = "https://www.mila.com";
				strcat(accepturl, orderid);
				printf("%s\n", accepturl);
				curl_easy_setopt(curl, CURLOPT_URL, accepturl);
				res = curl_easy_perform(curl);


				g_match_info_next (matchInfo, &err);
				g_free (orderid);
				g_free (csrf);
				curl_free(csrf_enc);
				free(newbuf);
				error = 0;
			}
			curl_easy_cleanup(curl);

		}

	}
	else
		return 50;
	return error;
}

int mila (char *buf, int buf_size, char *to)
{
	int error = 1;
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
	const char urlstr[200] ="https://www.mila.com/friendservicecalls";


	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();

	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
//		curl_easy_setopt(curl, CURLOPT_WRITEDATA, llhapns_worker);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
//				curl_easy_setopt(curl, CURLOPT_HEADERDATA, llhapns_worker);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_trace);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

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
			printf("to: %s\n", to);
			curl_easy_setopt(curl, CURLOPT_COOKIE, mila_profile[0].credentials);
			error = 5;
		}

		curl_easy_setopt(curl, CURLOPT_URL, urlstr);
		clock_t end = clock();
		time_processing = (double)(end - begin) / CLOCKS_PER_SEC;
		printf("mila time_processing : %f\n", time_processing);

		// accept here
//				for(int c = 0; c < 2; c++)
		res = curl_easy_perform(curl);

//				printf("write_callbacks: %s\n", retbuf);

		// todo: retry
		

		// search for order to accept
		char *lpchar = retbuf;
//				while (lpchar = strstr(lpchar, "<form name=\"userForm\" action=\"/friendaccept/"))
//				{
//					printf("%s\n", lpchar);
			if(!mila_accept(lpchar, profile))
				error=0;
//				}


		time_accept = (double)(clock() - begin) / CLOCKS_PER_SEC;
		printf("mila time_accept : %f\n", time_accept);


		// curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "_csrf=rfHkxaSdrgjzAatFqCAEDtUTO8sS7ZcO2a+jY=");

		// char accepturl[255] = "https://www.mila.com/friendaccept/";
		// strcat(accepturl, "2");
		// printf("%s\n", urlstr);
		// curl_easy_setopt(curl, CURLOPT_URL, accepturl);
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
	char *cmd = malloc(buf_size+sizeof(retbuf)+sizeof(buf)+5000000);
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
//		if(buf_size < 10*1024)
//			strcat(cmd,retbuf);
{
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
		free(newbuf);
}
{
                strcat(cmd,"\n\n");
                strcat(cmd,"--_boundarystring\n");
                strcat(cmd,"Content-Type: application/octet-stream name=\"return.eml\"\n");
                strcat(cmd,"Content-Transfer-Encoding: Base64\n");
                strcat(cmd,"Content-Disposition: attachment; filename=\"return.eml\"\n");
                strcat(cmd,"\n");
                char *newbuf = malloc(Base64encode_len(strlen(retbuf)+1000));
                Base64encode(newbuf, retbuf, strlen(retbuf));
		printf("cmd size: %d\n", strlen(cmd));
                strcat(cmd,newbuf);
                printf("cmd size: %d\n", strlen(cmd));
                 strcat(cmd,"\n\n");
                strcat(cmd,"--_boundarystring--\n");
		free(newbuf);
}




  CURL *curl;
  CURLcode res = CURLE_OK;
  struct curl_slist *recipients = NULL;
//  struct upload_status upload_ctx;
 
//  upload_ctx.lines_read = 0;
 
  curl = curl_easy_init();
  if(curl) {

curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
 curl_easy_setopt(curl, CURLOPT_URL, "smtp://mail.xxx.com");
curl_easy_setopt(curl, CURLOPT_MAIL_FROM, "xxx@xxx.com");
recipients = curl_slist_append(recipients, "xxx@xxx.com");
curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
//  curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
curl_easy_setopt(curl, CURLOPT_READDATA, &cmd);

res = curl_easy_perform(curl);

if(res != CURLE_OK)
      printf("curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

  curl_slist_free_all(recipients);
curl_easy_cleanup(curl);
}


		strcat(cmd,"EOF");
		printf("Sending Mila report\n");
		
		pthread_mutex_lock(&mutex);
		int ret_cmd = system(cmd);
		printf("system returned : %d\n", ret_cmd);
		sleep(5);
		pthread_mutex_unlock(&mutex);

//		memset(cmd, 0, sizeof(cmd));
		cmd[0] = 0;
		strcat(cmd,"/usr/sbin/sendmail xxx@xxx.com < ");
		strcat(cmd,filename);
		strcat(cmd,"\n");
		printf("Forwarding email\n");

		pthread_mutex_lock(&mutex);
		ret_cmd = system(cmd);
		printf("system returned : %d\n", ret_cmd);
		sleep(5);
		pthread_mutex_unlock(&mutex);

		if(cmd)
			free(cmd);
		else
		{
			printf("Error free cmd\n");
			error = 10;
		}
	}
	printf("Mila end\n");
	return error;
}
