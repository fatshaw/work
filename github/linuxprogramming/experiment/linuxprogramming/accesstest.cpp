#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#define MODE(z) (stat_buf.st_mode & (z) == z)

char * convertToBinary(int i_dex,char* p_binary){
	int i_compare  = 0;
	for(int i = 0; i < 32;++i){
		i_compare = 1<<i;
		if(i_dex & i_compare == i_compare){
			p_binary[i] = '1';
		}
	}
	return p_binary;
}

int main(int argc, char ** argv)
{
	char * p_binary = (char*)malloc(33);
	memset(p_binary,'0',32);
	p_binary[32] = 0;
	p_binary = convertToBinary(1,p_binary);
	printf("binary is %s\n",p_binary);
	char * pathname = "/usr/local/bin";
	struct stat stat_buf;
	int stat_res = lstat(pathname,&stat_buf);
	if(stat_res == -1){
		perror("pathname error");
		return 1;
	}
	if(MODE(S_IRUSR)){
		printf("can read\n");
	}

	if(MODE(S_IWUSR)){
		printf("can write\n");
	}
	if(MODE(S_IXUSR)){
		printf("can executet\n");
	}
	
	int acc_res = access(pathname,F_OK);
	if(acc_res == -1){	
		printf("file does not exist\n");
	}

	if(access(pathname,R_OK) == 0){
		printf("file can read\n");
	}

	if(access(pathname,W_OK) == 0){
		printf("file can write\n");
	}

	if(access(pathname,X_OK) == 0){
		printf("file can execute\n");
	}
	return 0;
}
