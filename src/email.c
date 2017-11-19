#include <curl/curl.h>
#include <stdarg.h>

#include "email.h"

int email_init(temail* pemail)
{
	pemail->curl = curl_easy_init();
	return 0;
}

int email_free()
{
	return 0;
}

int email_send(const char *body, int n, ...)
{
	CURLcode res = CURLE_OK;
	struct curl_slist *recipients = NULL;
	//  struct upload_status upload_ctx;
	 
	//  upload_ctx.lines_read = 0;
	CURL *curl = curl_easy_init();
	
	if(curl)
	{
		curl_mime *mime;
		//curl_mime *alt;
		curl_mimepart *part;
		mime = curl_mime_init(curl);
		// alt = curl_mime_init(curl);


		/* Text message. */ 
		part = curl_mime_addpart(mime);
		curl_mime_type(part, "text/plain");
		curl_mime_data(part, body, CURL_ZERO_TERMINATED);

		va_list vl;
		va_start(vl,n);
		for (int i=0;i<n;i++)
		{
			char * buf =va_arg(vl,char*);

			part = curl_mime_addpart(mime);
			curl_mime_data(part, buf, CURL_ZERO_TERMINATED);
			char * name =va_arg(vl,char*);
			curl_mime_filename(part, name);
			char * type =va_arg(vl,char*);
			curl_mime_filename(part, name);
			curl_mime_type(part, type);

		}
		va_end(vl);

  		curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "From: <xxx@xxx.com>");
		headers = curl_slist_append(headers, "Subject: Mila return");

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);


		curl_easy_setopt(curl, CURLOPT_URL, "smtp://mail.xxx.com:587");
		curl_easy_setopt(curl, CURLOPT_MAIL_FROM, "xxx@xxx.com");
		recipients = curl_slist_append(recipients, "xxx@xxx.com");
		curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
		//  curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		// curl_easy_setopt(curl, CURLOPT_READDATA, cmd);
//			curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_trace);
//			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		extern int DRY_RUN;
		if(!DRY_RUN)
			res = curl_easy_perform(curl);

		if(res != CURLE_OK)
			printf("curl_easy_perform() failed: %s\n",
		curl_easy_strerror(res));

		curl_slist_free_all(recipients);
		curl_easy_cleanup(curl);
	}

}