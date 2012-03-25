#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

char * convertToBinary(int i_dex,char* p_binary){
	int i_compare  = 0;
	for(int i = 0; i < 32;++i){
		i_compare = 1<<i;
		if(i_compare & i_dex == i_compare){
			p_binary[32 - i] = '1';
		}
	}
	return p_binary;
}

int main(int argc,char ** argv){
	char * p_binary = (char*)malloc(33);
	memset(p_binary,'0',32);
	p_binary[32] = 0;
	p_binary = convertToBinary(1,p_binary);
	printf("binary is %s\n",p_binary);
	return 0;
}
