
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
#include "email.h"

#include "log.h"


#define MAX_BUF 1024*1024*5


s_mila_profile mila_profile[] = {
	{ "xxx@xxx.com" , 
	"",   "" , "", "xxx@xxx.com" },
	{ "xxx@xxx.com" ,  
	"",   "" , "", "xxx@xxx.com"  },
	{ "xxx@xxx.com" ,  
	"",   "" , "", "xxx@xxx.com"  },
	{ "xxx@xxx.com" ,  
	"",   "" , "", "xxx@xxx.com"  },
	
	{ "xxx@xxx.com" ,  
	"", "" , "", "xxx@xxx.com"  },
	{ "xxx@xxx.com" , 
	"", "" , "", "xxx@xxx.com"  },
	{ "xxx@xxx.com" , 
	"", "" , "", "xxx@xxx.com"  },

	{ "xxx@xxx.com" , 
	"", "" , "", "xxx@xxx.com"  },
	
	
	
	
	{ "xxx@xxx.com" , "b9284842b7da65caeaf6e1d46261c5dc5edafeeaa595a59ae07ac909bba415c75469a190", "logintoken=b9284842b7da65caeaf6e1d46261c5dc5edafeeaa595a59ae07ac909bba415c75469a190; connect1.sid=s%3Ax20TkL1Xd2c5WHZ_dU-RLewaguYN-GgT.v6eb%2FaIFqWbzQTAGAkJKBNNffL3KJHKaT7Ry1ZFleuM; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;", 
	"/var/cache/mila_accept/cookies_1.txt", "xxx@xxx.com" },	
	{ "xxx@xxx.com" , "ae0c6ad33b346eadf5ade8df04df9d2ca7a8e206a302f86fa8f785ad3c8ec906ef2c1d46", "logintoken=ae0c6ad33b346eadf5ade8df04df9d2ca7a8e206a302f86fa8f785ad3c8ec906ef2c1d46; connect1.sid=s%3A4Yuaw7tcX1DtQsO3mXqy4HLlWFl1aBNS.aIAOJes8FNRDIgFnKbF5Fixh9RhHekZGJdb5MT5gmiw; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;", 
	"/var/cache/mila_accept/cookies_4.txt", "xxx@xxx.com" },		
	{ "xxx@xxx.com" , "ff3afe9c1fec5a006c6df4c1b65df1b3d6e66908401b1ed1321c1fa19d0e336e132766a5",   "logintoken=ff3afe9c1fec5a006c6df4c1b65df1b3d6e66908401b1ed1321c1fa19d0e336e132766a5; connect1.sid=s%3AYVkeceWhppaW2fpA_mgtk1LB-FMxjGi0.FNQ%2FSTe3RlS8hjlArkLYJmHCzeTb55X5mgMTRM3CGOk; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;", 
"/var/cache/mila_accept/cookies_2.txt", "xxx@xxx.com" },

	{ "xxx@xxx.com" , "60279c583266404816f23d0749f41aff11f0b7ce2fae2187a75e55e9b3a01db0f52a257b", "logintoken=60279c583266404816f23d0749f41aff11f0b7ce2fae2187a75e55e9b3a01db0f52a257b; connect1.sid=s%3Ax20TkL1Xd2c5WHZ_dU-RLewaguYN-GgT.v6eb%2FaIFqWbzQTAGAkJKBNNffL3KJHKaT7Ry1ZFleuM; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;", 
	"/var/cache/mila_accept/cookies_1.txt", "xxx@xxx.com" },	
	{ "xxx@xxx.com" , "2f502cee8e1f34d79ecf23b814678ba7d7504ce85fd342ce65814d30109c3067ed897af4", "logintoken=2f502cee8e1f34d79ecf23b814678ba7d7504ce85fd342ce65814d30109c3067ed897af4; connect1.sid=s%3A4Yuaw7tcX1DtQsO3mXqy4HLlWFl1aBNS.aIAOJes8FNRDIgFnKbF5Fixh9RhHekZGJdb5MT5gmiw; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;", 
	"/var/cache/mila_accept/cookies_4.txt", "xxx@xxx.com" },		
	{ "xxx@xxx.com" , "96689d3f85164f4b9cf4907f4ad73e2e62976d2e7beb847bd2e17fc502f9b7afa71553d0",   "logintoken=96689d3f85164f4b9cf4907f4ad73e2e62976d2e7beb847bd2e17fc502f9b7afa71553d0; connect1.sid=s%3AYVkeceWhppaW2fpA_mgtk1LB-FMxjGi0.FNQ%2FSTe3RlS8hjlArkLYJmHCzeTb55X5mgMTRM3CGOk; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;", 
"/var/cache/mila_accept/cookies_2.txt", "xxx@xxx.com" },



	{ "xxx@xxx.com" , 
	"7da3ca904b316d9f65f3e78446cdbd2c5bd5e51fd8945eff34ec052bc64960c91ac268e4", "logintoken=7da3ca904b316d9f65f3e78446cdbd2c5bd5e51fd8945eff34ec052bc64960c91ac268e4; connect1.sid=s%3A4Yuaw7tcX1DtQsO3mXqy4HLlWFl1aBNS.aIAOJes8FNRDIgFnKbF5Fixh9RhHekZGJdb5MT5gmiw; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;", "/var/cache/mila_accept/cookies_4.txt", "xxx@xxx.com" },	
	{ "xxx@xxx.com" , 
	"479f93ade8dd67d5093263601d10b9adb840e5565d7ca27c8646b2f160366f71df3117b6",    "logintoken=479f93ade8dd67d5093263601d10b9adb840e5565d7ca27c8646b2f160366f71df3117b6; connect1.sid=s%3AYVkeceWhppaW2fpA_mgtk1LB-FMxjGi0.FNQ%2FSTe3RlS8hjlArkLYJmHCzeTb55X5mgMTRM3CGOk; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;", "/var/cache/mila_accept/cookies_2.txt", "xxx@xxx.com" },
	{ "xxx@xxx.com" , 
	  "06f49b539f300f9c33bbf276791de4b837676fbf92969affc6746c6c66ecfb47a0c9cc7a", 
	  "logintoken=06f49b539f300f9c33bbf276791de4b837676fbf92969affc6746c6c66ecfb47a0c9cc7a; connect1.sid=s%3Ax20TkL1Xd2c5WHZ_dU-RLewaguYN-GgT.v6eb%2FaIFqWbzQTAGAkJKBNNffL3KJHKaT7Ry1ZFleuM; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;", 
	  "/var/cache/mila_accept/cookies_1.txt", 
	  "xxx@xxx.com" 
	},	



	{ "xxx@xxx.com" , 
	  "3e07f5b187437ad66cd5d61a46dd1bb3346fbbb4fb3ee2fd3c77da34c1215e29387f4ff0", 
	  "logintoken=3e07f5b187437ad66cd5d61a46dd1bb3346fbbb4fb3ee2fd3c77da34c1215e29387f4ff0; connect1.sid=s%3Ax20TkL1Xd2c5WHZ_dU-RLewaguYN-GgT.v6eb%2FaIFqWbzQTAGAkJKBNNffL3KJHKaT7Ry1ZFleuM; language=fr; _dc_gtm_UA-29191003-1=1; _we_wk_ss_lsf_=true; _ga=GA1.2.1769302580.1489595889; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; optimizelyEndUserId=oeu1489596031683r0.06039701592998992; optimizelySegments=%7B%22700475046%22%3A%22ff%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D;" , 
	  "/var/cache/mila_accept/cookies_10.txt", 
	  "xxx@xxx.com"  
	},


	{ "xxx@xxx.com" , "", "", 
	"", "xxx@xxx.com" },	





	};


// pthread_mutex_t mutex;
	
// double time_accept;
// double time_processing;

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
	for(int c = 0; c < size; c++)
	{
		int len = strlen(mila_profile[c].email);
		if(strncmp(mila_profile[c].email, email, len)==0)
		{
			GENERAL(LOG_LEVEL_GENERAL, "credential found");
			GENERAL(LOG_LEVEL_GENERAL, "credential %s", mila_profile[c].email);
			*profile = c;
			return 0;
		}
	}
	*profile = 0;
	return 1;
}


int quoat_decode(char *inbuf, int lenght, char *outbuf, int *newlenght)
{

	char *start, *end = inbuf+lenght;
	char *outpinter = outbuf;
	*newlenght = lenght;
	GENERAL(LOG_LEVEL_GENERAL, "newlenght: %d", lenght);
	
	start = strstr(inbuf, "quoted-printable\n\n");
	if(!start)
		start = strstr(inbuf, "quoted-printable\r\n");
	if(start!=NULL)
	{
		GENERAL(LOG_LEVEL_GENERAL, "quote remove");
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

	const char lsearch[]= "https://www.mila.com/loginWithPW/";
	const char lsearch2[] = "?next=3Dfriendservicecalls/";
	char *lpbuf;
	
	lpbuf = strstr(buf, lsearch);
	if(lpbuf)
	{
		GENERAL(LOG_LEVEL_GENERAL, "Accept found:");
		lpbuf += sizeof(lsearch)-1;

			lpbuf += sizeof(lsearch2)-1;
			char *end = strstr(lpbuf, "=\r\n>");
			if(end-lpbuf > 200)
			{	
				GENERAL(LOG_LEVEL_GENERAL, "overflow %ld", (long)(end-lpbuf));
				return 4;
			}
			if(end)
			{
				strncpy(lbuf, lpbuf, end-lpbuf);
				GENERAL(LOG_LEVEL_GENERAL, "%s", lbuf);
				return 0;
			}
			return 2;
	}
	GENERAL(LOG_LEVEL_GENERAL, "Accept not found");
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
	char *lpbuf = userp;
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
	GENERAL(LOG_LEVEL_GENERAL, "%s :: %s", text, data);
	int offset = strlen(lpbuf);
	memcpy(lpbuf+offset, text, strlen(text)+1);
	lpbuf[strlen(text)+offset] = 0;

	offset = strlen(lpbuf);
	memcpy(lpbuf+offset, data, size);
	lpbuf[size+offset] = 0;

	return 0;
}
 

size_t header_callback(char *buffer,   size_t size,   size_t nitems,   void *userdata)
{
	GENERAL(LOG_LEVEL_GENERAL, "hsize: %ld", size*nitems);
	GString *lpbuf = userdata;
	if(size*nitems > 0)
	{
//  		GENERAL(LOG_LEVEL_GENERAL, "header: %s", buffer);
		g_string_append_len(lpbuf, buffer, size*nitems);
	}
	return nitems * size;
}


size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	GENERAL(LOG_LEVEL_GENERAL, "hsize: %ld", size*nmemb);
	GString *lpbuf = userdata;
	if(size*nmemb > 0)
		g_string_append_len(lpbuf, ptr, size*nmemb);
	return nmemb * size;
}

int progress_callback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal,   curl_off_t ulnow)
{
	GENERAL(LOG_LEVEL_GENERAL, "dltotal: %ld, dlnow: %ld, , ultotal: %ld, , ulnow: %ld", dltotal, dlnow, ultotal, ulnow);
	return 0;
}

#define HTTP_METHOD_GET 2^0
#define HTTP_METHOD_POST 2^1

int http_get(char *url, char *cookies, int flags, char **retbuf)
{
	*retbuf = malloc(5*1024*1024);

	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();

	if(curl)
	{
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
		if(flags | HTTP_METHOD_POST)
			curl_easy_setopt(curl, CURLOPT_POST, 1L);
	
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
//		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
//		curl_easy_setopt(curl, CURLOPT_HEADERDATA, llhapns_worker);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_trace);
		curl_easy_setopt(curl, CURLOPT_DEBUGDATA, retbuf);
//		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
//		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		curl_easy_setopt(curl, CURLOPT_COOKIE, cookies);

		curl_easy_setopt(curl, CURLOPT_URL, url);

		int try = 5;	
		extern int DRY_RUN;
perform:
		if(!DRY_RUN)
			res = curl_easy_perform(curl);

		if(res = CURLE_HTTP_RETURNED_ERROR && try > 0)
		{
			sleep(1);
			try -=1;
goto perform;
		}

		curl_easy_cleanup(curl);
	}
	return 0;
}

int mila_accept(CURL *curl, GString *buf, int lprofile, GString *retbuf)
{
	GENERAL(LOG_LEVEL_GENERAL, "mila_accept");
	GError *err = NULL;
	GMatchInfo *matchInfo = NULL;
	GRegex *regex = NULL;

	if(!curl)
	{
		GENERAL(LOG_LEVEL_GENERAL, "invalid curl pointer");
		return 1;
	}
	int error = 1;

	char *start = strstr(buf->str+15000, "Nouvelles demandes</h2>");

	char *end;
	if(start)
	{
		end = strstr(start, "Commandes en cours");
		printf("offset: %ld\n", start-buf->str);
	}	
		
	if(start && end)
	{
		GENERAL(LOG_LEVEL_GENERAL, "Nouvelles demandes");
		GString *newbuf = g_string_new(NULL);
		g_string_append_len(newbuf, start, end-start);

		str_replace(newbuf->str, newbuf->len, "\n", "");
		str_replace(newbuf->str, newbuf->len, "\r", "");

		// search for _csrf=
		regex = g_regex_new ("<form.*action=\"\\/friendaccept\\/(.*)\".*name=\"_csrf\".*value=\"(.*)\".*Accepter.*<\\/form>", G_REGEX_UNGREEDY, 0, &err);   
	//	regex = g_regex_new ("<form.*name=\"_csrf\".*value=\"(.*)\"", G_REGEX_MULTILINE|G_REGEX_UNGREEDY, 0, &err);   
		if(regex == NULL)
		{
			GENERAL(LOG_LEVEL_GENERAL, "regex error");
			return 15;
		}
		else
		{
			if(err)
			{
				GENERAL(LOG_LEVEL_GENERAL, "gerror : %s", err->message);
			}

			g_regex_match (regex, newbuf->str, 0, &matchInfo);

			CURLcode res;

// list = curl_slist_append(list, "Cookie: optimizelyEndUserId=oeu1486559193069r0.2422451363507594; optimizelySegments=%7B%22700475046%22%3A%22gc%22%2C%22702591331%22%3A%22false%22%2C%22707443264%22%3A%22direct%22%7D; optimizelyBuckets=%7B%222178270511%22%3A%222151040487%22%7D; logintoken=a5da1827573d0e4bcf6966c9954a7aa2335caf259b048a4dacc696326078ba4df20b7635; connect1.sid=s%3AzuHy12n0PP0pvG1FNB5V6owfRbF_Meb2.qs5XxiVgaJphR1nvGErxu%2FpSe8Ux0i3%2BveMulspgm88; _dc_gtm_UA-29191003-1=1; _ga=GA1.2.881151050.1486559200; _gid=GA1.2.1734846664.1508526208; _gat_UA-29191003-1=1");
			GString *header = g_string_new(NULL);
			GENERAL(LOG_LEVEL_GENERAL, "curl_easy_setopt");

			curl_easy_setopt(curl, CURLOPT_POST, 1L);
// 			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, retbuf);
// 			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header);
// 			curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_trace);
// 			curl_easy_setopt(curl, CURLOPT_DEBUGDATA, retbuf);
//			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
//			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			GENERAL(LOG_LEVEL_GENERAL, "curl_easy_setopt ok");

			// printf("profile: %d\n", lprofile);
			// printf("mila credentials : %s\n", mila_profile[lprofile].credentials);
// 			curl_easy_setopt(curl, CURLOPT_COOKIE, mila_profile[lprofile].credentials);

			if(!g_match_info_matches(matchInfo))
			{
				GENERAL(LOG_LEVEL_GENERAL, "match not found");
				return 30;
			}
		
			while (g_match_info_matches (matchInfo))
			{
				gchar *orderid = g_match_info_fetch (matchInfo, 1);
				gchar *csrf = g_match_info_fetch (matchInfo, 2);
			 
				g_print ("orderid: %s\n\n", orderid);
				g_print ("csrf: %s\n\n", csrf);

				GENERAL(LOG_LEVEL_GENERAL, "orderid: %s", orderid);
				GENERAL(LOG_LEVEL_GENERAL, "csrf: %s", csrf);


				char post[200] = "_csrf=";
				char *csrf_enc = curl_easy_escape(curl, csrf, strlen(csrf));
				strcat(post, csrf_enc);
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
				curl_easy_setopt(curl, CURLOPT_REFERER, "https://www.mila.com/friendservicecalls");

				GENERAL(LOG_LEVEL_GENERAL, "post: %s", post);
				GENERAL(LOG_LEVEL_GENERAL, "postlen: %lu", strlen(post));

				char accepturl[255] = "https://www.mila.com/friendaccept/";
				strcat(accepturl, orderid);
				GENERAL(LOG_LEVEL_GENERAL, "%s", accepturl);
				curl_easy_setopt(curl, CURLOPT_URL, accepturl);
				// curl_easy_setopt(curl, CURLOPT_URL, "https://requestb.in/17vighd1");

				extern int DRY_RUN;
				if(!DRY_RUN)
				{
					int try = 20;
					start:
					res = curl_easy_perform(curl);
					if(strstr(header->str, "HTTP/1.1 500 Internal Server Error") && try > 0)
					{
						GENERAL(LOG_LEVEL_GENERAL, "HTTP/1.1 500 Internal Server Error");
						GENERAL(LOG_LEVEL_GENERAL, "retry: %d", try);
						try -= 1;
						usleep(200000);
						goto start;
					}
					if(strstr(header->str, "HTTP/1.1 502") && try > 0)
					{
						GENERAL(LOG_LEVEL_GENERAL, "HTTP/1.1 502 Bad Gateway");
						GENERAL(LOG_LEVEL_GENERAL, "retry: %d", try);
						try -= 1;
						usleep(200000);
						goto start;
					}
					if(strstr(header->str, "HTTP/1.1 503") && try > 0)
					{
						GENERAL(LOG_LEVEL_GENERAL, "HTTP/1.1 503 Service Unavailable");
						GENERAL(LOG_LEVEL_GENERAL, "retry: %d", try);
						try -= 1;
						usleep(200000);
						goto start;
					}
					if(strstr(header->str, "HTTP/1.1 504 GATEWAY_TIMEOUT") && try > 0)
					{
						GENERAL(LOG_LEVEL_GENERAL, "HTTP/1.1 504 GATEWAY_TIMEOUT");
						GENERAL(LOG_LEVEL_GENERAL, "retry: %d", try);
						try -= 1;
						usleep(200000);
						goto start;
					}
				}

				email_send("mila_accept return", 1, retbuf->str, "mila_accept.html", "text/html; charset=utf-8");

				g_match_info_next (matchInfo, &err);
				if(orderid)
					g_free (orderid);
				if(csrf)
					g_free (csrf);
					
				if(csrf_enc)
					curl_free(csrf_enc);
				
				error = 0;
			}
			g_string_free(header, TRUE);
			g_free(matchInfo);
			curl_easy_setopt(curl, CURLOPT_POST, 0L);
		}
		g_regex_unref(regex);
		g_string_free(newbuf, TRUE);
	}
	else
	{
		if(!start && end)
			GENERAL(LOG_LEVEL_GENERAL, "mila_accept() start not found");
		if(!end && start)
			GENERAL(LOG_LEVEL_GENERAL, "mila_accept() end not found");

		GENERAL(LOG_LEVEL_GENERAL, "No new order pending");
		return 50;
	}
	if(error)
		GENERAL(LOG_LEVEL_GENERAL, "mila_accept() error");
	return error;
}

int mila_find_emaillink(char *inbuf, char *url)
{
//printf("%s", inbuf);



/*

  SHOW IN APP
<https://www.mila.com/loginMobile?url=3Dmila://loginWithPW/aaaa6aa6/150501>=

Show in browser
<https://www.mila.com/loginWithPW/aaaa6aa6?next=3D/friendservicecalls/15050=
1>


*/
	inbuf += 10;
	char *start = strstr(inbuf, "afficher sur le web");
	if(!start)
		start = strstr(inbuf, "Show in browser");
	if(!start)
		start = strstr(inbuf, "\n\n");
	if(!start)
		start = strstr(inbuf, "\r\n\r\n");
		
	if(start)
		start = strstr(start, "<");
	else
	{
		GENERAL(LOG_LEVEL_GENERAL, "afficher sur le web not found");		
		return 1;
	}
	char *end;
	if(start)
		end = strstr(start, ">");

	if(start)
		GENERAL(LOG_LEVEL_GENERAL, "link to mila start found");
	else
	{
		GENERAL(LOG_LEVEL_GENERAL, "link to mila start not found");
		return 1;
	}
	
	if(end)
		GENERAL(LOG_LEVEL_GENERAL, "link to mila end found");
	else
	{
		GENERAL(LOG_LEVEL_GENERAL, "link to mila end not found");
		return 2;
	}
	if( 100 < abs(strstr(start, "/friendservicecalls/") - start))
	{
		GENERAL(LOG_LEVEL_GENERAL, "friendservicecalls not found");
		return 1;
	}
	
	start += 1;
// 	end -= 2;

	strncpy(url, start, end-start);
// 		urlstr = curl_easy_unescape(curl, urltmp, 0, NULL);
	str_replace(url, strlen(url), "=3D", "\x3D");
	str_replace(url, strlen(url), "=\r\n", "");
	str_replace(url, strlen(url), "=\n", "");
	str_replace(url, strlen(url), ">", "");

	GENERAL(LOG_LEVEL_GENERAL, "link to mila found: %s", url);

	return 0;
}

int mila (char *buf, int buf_size, char *to)
{
	int error = 1;

	clock_t begin = clock();
	
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

	int size = buf_size;

	if(eml!=NULL)
	{
		fwrite(buf, 1, size, eml);
		fclose(eml);
	}

// 	time_accept = (double)(clock() - begin) / CLOCKS_PER_SEC;
// 	printf("mila time_accept : %f\n", time_accept);

	GString *retbuf = g_string_new(NULL);
	GString *header = g_string_new(NULL);
	GString *return_accept = g_string_new(NULL);

	long retvalue = 0;
	int c;

	char urlstr[200] = "";

	CURLcode res;

	int profile = 0;
	
	CURL *curl = NULL;
	CURL *curl_api = NULL;
	if(!getfromemail(to, &profile))
	{
		GENERAL(LOG_LEVEL_GENERAL, "mila credentials : %s", mila_profile[profile].credentials);
// 			curl_easy_setopt(curl, CURLOPT_COOKIE, mila_profile[profile].credentials);
		curl = mila_profile[profile].curl;
		curl_api = mila_profile[profile].curl_api;
// 			curl_easy_setopt(curl, CURLOPT_COOKIEJAR, mila_profile[profile].cookiejar);
	}
	else
	{
		GENERAL(LOG_LEVEL_GENERAL, "using default credential");
		GENERAL(LOG_LEVEL_GENERAL, "to: %s", to);
		curl = mila_profile[0].curl;
		curl_api = mila_profile[0].curl_api;
// 			curl_easy_setopt(curl, CURLOPT_COOKIE, mila_profile[0].credentials);
// 			curl_easy_setopt(curl, CURLOPT_COOKIEJAR, mila_profile[0].cookiejar);
		error = 5;
	}

	if(!mila_find_emaillink(buf, urlstr))
	{


		if(curl_api)
		{
			pthread_mutex_lock(&mila_profile[profile].mutex);
			CURL *lcurl = curl_easy_duphandle(curl_api);
			pthread_mutex_unlock(&mila_profile[profile].mutex);
			
			if(lcurl)
			{
				curl_easy_setopt(lcurl, CURLOPT_WRITEFUNCTION, write_callback);
				curl_easy_setopt(lcurl, CURLOPT_WRITEDATA, retbuf);

	 			curl_easy_setopt(lcurl, CURLOPT_HEADERFUNCTION, header_callback);
	 			curl_easy_setopt(lcurl, CURLOPT_HEADERDATA, header);

				curl_easy_setopt(lcurl, CURLOPT_XFERINFOFUNCTION, progress_callback);
				curl_easy_setopt(lcurl, CURLOPT_NOPROGRESS, 0L);

	//			char api_url[1024] = "http://www.xxx.com/v3/servicecalls/";
				char api_url[1024] = "https://api.mila.com/v3/servicecalls/";
				strcat(api_url, urlstr + (strlen(urlstr) - 6));
				strcat(api_url, "/actions/ACCEPT");
				curl_easy_setopt(lcurl, CURLOPT_POST, 1L);
				curl_easy_setopt(lcurl, CURLOPT_URL, api_url);

				GENERAL(LOG_LEVEL_GENERAL, "api:accept : %s", api_url);

				extern int DRY_RUN;
				if(!DRY_RUN)
				{
					res = curl_easy_perform(lcurl);
					GENERAL(LOG_LEVEL_GENERAL, "header: %s", header->str);
					GENERAL(LOG_LEVEL_GENERAL, "api:accept : %s", retbuf->str);
				}
				else
				{
					GENERAL(LOG_LEVEL_GENERAL, "api:accept : %s", retbuf->str);
					usleep(500000);
				}
				curl_easy_cleanup(lcurl);
			}
		}

		if(curl && 0)
		{
			GENERAL(LOG_LEVEL_GENERAL, "curl");

			///////////////////////////////////////////////////////////////
			// Mutex lock
			pthread_mutex_lock(&mila_profile[profile].mutex);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, retbuf);

			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header);



			///////////////////////////////////////////////////////////////

			
			
//			curl_easy_setopt(curl, CURLOPT_HEADERDATA, llhapns_worker);
//			curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_trace);
//			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);


// 			curl_easy_setopt(curl, CURLOPT_URL, urlstr);
			curl_easy_setopt(curl, CURLOPT_URL, "https://www.mila.com/friendservicecalls");

// 			clock_t end = clock();
// 			time_processing = (double)(end - begin) / CLOCKS_PER_SEC;
// 			GENERAL(LOG_LEVEL_GENERAL, "mila time_processing : %f\n", time_processing);

			// todo: retry

			extern int DRY_RUN;
			if(!DRY_RUN)
			{
				int try = 20;
				start1:
				res = curl_easy_perform(curl);

				if(strstr(header->str, "HTTP/1.1 500 Internal Server Error") && try > 0)
				{
					GENERAL(LOG_LEVEL_GENERAL, "HTTP/1.1 500 Internal Server Error");
					GENERAL(LOG_LEVEL_GENERAL, "retry: %d", try);
					try -= 1;
					usleep(200000);
					goto start1;
				}
				if(strstr(header->str, "HTTP/1.1 502") && try > 0)
				{
					GENERAL(LOG_LEVEL_GENERAL, "HTTP/1.1 502 Bad Gateway");
					GENERAL(LOG_LEVEL_GENERAL, "retry: %d", try);
					try -= 1;
					usleep(200000);
					goto start1;
				}
				if(strstr(header->str, "HTTP/1.1 503") && try > 0)
				{
					GENERAL(LOG_LEVEL_GENERAL, "HTTP/1.1 503 Service Unavailable");
					GENERAL(LOG_LEVEL_GENERAL, "retry: %d", try);
					try -= 1;
					usleep(200000);
					goto start1;
				}
				if(strstr(header->str, "HTTP/1.1 504 GATEWAY_TIMEOUT") && try > 0)
				{
					GENERAL(LOG_LEVEL_GENERAL, "HTTP/1.1 504 GATEWAY_TIMEOUT");
					GENERAL(LOG_LEVEL_GENERAL, "retry: %d", try);
					try -= 1;
					usleep(200000);
					goto start1;
				}

			}
			else
				usleep(50000);


			////////////////////////////////////////////////////////////////


			GString *lpchar = retbuf;
			int r = mila_accept(curl, lpchar, profile, return_accept);
			if(r == 0 || r== 50)
				error=0;

			curl_easy_setopt(curl, CURLOPT_WRITEDATA, retbuf);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, header);

			// TODO: autologin here
			if(!DRY_RUN)
			{
// 				curl_easy_setopt(curl, CURLOPT_COOKIEFILE, NULL);
				curl_easy_setopt(curl, CURLOPT_URL, urlstr);
				printf("%s\n", mila_profile[profile].cookiejar);
				res = curl_easy_perform(curl);
// 				curl_easy_setopt(curl, CURLOPT_COOKIEFILE, mila_profile[c].cookiejar);
			}
			else
				usleep(50000);

			pthread_mutex_unlock(&mila_profile[profile].mutex);

			email_send("mila return", 2, retbuf->str, "mila.html", "text/html; charset=utf-8",
										 buf, "message.eml", "message/rfc822; charset=utf-8");
										 
// 			time_accept = (double)(clock() - begin) / CLOCKS_PER_SEC;
// 			GENERAL(LOG_LEVEL_GENERAL, "mila time_accept : %f\n", time_accept);
			
//			curl_easy_cleanup(curl);
		}
		else
			GENERAL(LOG_LEVEL_GENERAL, "invalid curl pointer");

	}
	else
		GENERAL(LOG_LEVEL_GENERAL, "link to mila not found");
	
	char str_time_processing[20]="";
// 	sprintf(str_time_processing, "%f", time_processing);

	char str_time_accept[20]="";
// 	sprintf(str_time_accept, "%f", time_accept);
	
	// email
	char *cmd = malloc(buf_size+sizeof(retbuf)+sizeof(buf)+5000000);
	if(cmd)
	{
		cmd[0] = 0;
		strcat(cmd,"/usr/sbin/sendmail ");
// 		strcat(cmd,"xxx@xxx.com");
		strcat(cmd,mila_profile[profile].forward);
		strcat(cmd," < ");
		strcat(cmd,filename);
		strcat(cmd,"\n");
		GENERAL(LOG_LEVEL_GENERAL, "Forwarding email to [%d] %s", profile, mila_profile[profile].forward);

// 		pthread_mutex_lock(&mutex);
		int ret_cmd = 0;
		extern int DRY_RUN;
		if(!DRY_RUN)
			ret_cmd = system(cmd);
			
		GENERAL(LOG_LEVEL_GENERAL, "system returned : %d", ret_cmd);
// 		sleep(5);
// 		pthread_mutex_unlock(&mutex);

		if(cmd)
			free(cmd);
		else
		{
			GENERAL(LOG_LEVEL_GENERAL, "Error free cmd");
			error = 10;
		}
	}

	if(retbuf)
		g_string_free(retbuf, TRUE);
	if(return_accept)
		g_string_free(return_accept, TRUE);
	g_string_free(header, TRUE);

	GENERAL(LOG_LEVEL_GENERAL, "Mila end");
	return error;
}

int mila_init(void)
{
// 	pthread_mutex_init(&mutex, NULL);
	
	int size = sizeof(mila_profile)/sizeof(s_mila_profile);
	for(int c = 0; c < size; c++)
	{
		GENERAL(LOG_LEVEL_GENERAL, "mila_init profile %d: %s %s", c, mila_profile[c].email, mila_profile[c].bearer_key);
		pthread_mutex_init(&mila_profile[c].mutex, NULL);
		mila_profile[c].curl = curl_easy_init();
		CURL *curl = mila_profile[c].curl;
		
// 		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	
		mila_profile[c].list = NULL;
		mila_profile[c].list = curl_slist_append(mila_profile[c].list, "Cache-Control: max-age=0");
		mila_profile[c].list = curl_slist_append(mila_profile[c].list, "Origin: https://www.mila.com");
		mila_profile[c].list = curl_slist_append(mila_profile[c].list, "Upgrade-Insecure-Requests: 1");
		mila_profile[c].list = curl_slist_append(mila_profile[c].list, "Content-Type: application/x-www-form-urlencoded");
		mila_profile[c].list = curl_slist_append(mila_profile[c].list, "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/61.0.3163.100 Safari/537.36");
		mila_profile[c].list = curl_slist_append(mila_profile[c].list, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8");
		mila_profile[c].list = curl_slist_append(mila_profile[c].list, "Referer: https://www.mila.com/friendservicecalls");
// 		mila_profile[c].list = curl_slist_append(list, "Accept-Encoding: gzip, deflate, br");
		mila_profile[c].list = curl_slist_append(mila_profile[c].list, "Accept-Language: fr-CH,fr-FR;q=0.8,fr;q=0.6,en-US;q=0.4,en;q=0.2");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, mila_profile[c].list);

		curl_easy_setopt(curl, CURLOPT_COOKIEFILE, mila_profile[c].cookiejar);
		curl_easy_setopt(curl, CURLOPT_COOKIEJAR, mila_profile[c].cookiejar);
 		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
 		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

// 		curl_slist_free_all(list);



//////////////////////////////////////////////
// curl api

		mila_profile[c].curl_api = curl_easy_init();
		mila_profile[c].header_api_list = NULL;
		mila_profile[c].header_api_list = curl_slist_append(mila_profile[c].header_api_list, "Accept: application/json, text/plain, */*");
		mila_profile[c].header_api_list = curl_slist_append(mila_profile[c].header_api_list, "Content-Type: application/json");
		mila_profile[c].header_api_list = curl_slist_append(mila_profile[c].header_api_list, "'x-Origin': 'MILA_APP'");

		char bearer[255] = "Authorization: Bearer ";
		strcat(bearer, mila_profile[c].bearer_key);
		mila_profile[c].header_api_list = curl_slist_append(mila_profile[c].header_api_list, bearer);

		curl_easy_setopt(mila_profile[c].curl_api, CURLOPT_HTTPHEADER, mila_profile[c].header_api_list);
 		curl_easy_setopt(mila_profile[c].curl_api, CURLOPT_VERBOSE, 1L);
 		curl_easy_setopt(mila_profile[c].curl_api, CURLOPT_TIMEOUT, 5L);
 		curl_easy_setopt(mila_profile[c].curl_api, CURLOPT_NOSIGNAL, 1L);

	}
	
	return 0;
}

int mila_free(void)
{
	int size = sizeof(mila_profile)/sizeof(s_mila_profile);
	for(int c = 0; c < size; c++)
	{
		curl_easy_cleanup(mila_profile[c].curl);
		curl_easy_cleanup(mila_profile[c].curl_api);
		curl_slist_free_all(mila_profile[c].list);
		curl_slist_free_all(mila_profile[c].header_api_list);
		
	}
	return 0;
}