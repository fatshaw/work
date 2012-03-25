#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void pointTest(char * test){
	printf("%d",sizeof(test));
}

int main(int argc,char ** argv){
	char ** en = (char**)malloc(10*sizeof(char*));
	for(int i =0 ;i < 10;i++){
		*(en + i) = (char*)malloc(10);
		strncpy(*(en + i),"hello",5);
		printf("%d",strlen(*(en + i)));
	}
	
	char ** t = new char*[10];
	for(int i = 0; i< 10; i++){
		t[i] = "sdf" ;
	}

	for(int i = 0; i< 10;++i){
		printf("%s\n",*(en + i));
	}
	return 0;
}
