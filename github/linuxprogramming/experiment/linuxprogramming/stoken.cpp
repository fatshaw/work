#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 1024
char buf[MAXSIZE];

int get_stoken(int pos){
	char * temp = buf + pos;
	if(*temp == 0)return -1;
	while(true){
		if(*temp == 0)break;
		if(*temp == ' ')break;
		++temp;
	}
	return (temp - buf);
}

int main()
{
	int n_byteNum =  read(STDIN_FILENO,buf,MAXSIZE);
	int start = 0;
	int end = start;
	char * text =(char*)malloc(MAXSIZE);
	while( (end =  get_stoken(start)) != -1){
		strncpy(text,buf+start,end -start);
		printf("text is %s\n",text);
		while(buf[end]==' ' || buf[end] == '\t'){
			++end;
		}
		start = end;
	}

	char * path = getenv("PATH1");
	if(path == NULL){
		printf("path is null\n");
		setenv("PATH1","/usr/bin",0);
		path = getenv("PATH1");
		if(path != NULL){
			setenv("PATH1","/usr/bin/local",0);
		}
		path = getenv("PATH1");
	}

	printf("path is %s\n",path);

	return 0;
}
