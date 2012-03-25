#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <pcre.h>
using namespace std;

#define MAXSIZE 1024
#define OVECCOUNT 30
char errorbuf[MAXSIZE];
string buffer;
size_t write_data(void * buf,size_t size,size_t nmemb,void * userp){
	string temp((char*)buf,size*nmemb);
	buffer += temp;
	return size * nmemb;
}

bool pcre_match(const char * pattern, const char * subject)
{
	pcre * re = NULL;
	const char * error = NULL;
	int erroroffset;
	int ovector[OVECCOUNT];
	int rc,i;
	re = pcre_compile(pattern,0,&error,&erroroffset,NULL);
	if(re  == NULL)	{
		printf("compile error");
		perror("compile error");
		return -1;
	}
	rc  = pcre_exec(re,NULL,subject,strlen(subject),0,0,ovector,OVECCOUNT);
	if(rc < 0){
		printf("pcre exec error\n");
		return -1;
	}
	
	if(rc == 0){
		printf("there is not enough root for captured string\n");
		return -1;
	}
	
	for(i = 0; i< rc;i++){
		const char * substring_start = subject + ovector[2*i];
		int substring_length = ovector[2*i+1] - ovector[2*i];
		printf("%2d:%.*s\n",i,substring_length,substring_start);
	}
	return rc;
	
}

int main()
{
	/*
	CURL * curl = NULL;
	CURLcode ret;
	int wr_error,wr_index;
	wr_error = 0;
	wr_index = 0;
	
	curl = curl_easy_init();
	if(curl == NULL){
		printf("could not init curl\n");
		perror("could not init curl");
	}
	
	curl_easy_setopt(curl,CURLOPT_URL,"duanziwang.com");
	curl_easy_setopt(curl,CURLOPT_ERRORBUFFER,errorbuf);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,(void*)&wr_error);
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_data);
	ret = curl_easy_perform(curl);
	if(ret !=  0){
		printf("error in the perform : %s\n",errorbuf);
		exit(-1);
	}
	printf("buf is %s\n",buffer.c_str());;
	curl_easy_cleanup(curl);*/
	int res = pcre_match(".*","abc");
	printf("res = %d\n",res);
	return 0;
}
