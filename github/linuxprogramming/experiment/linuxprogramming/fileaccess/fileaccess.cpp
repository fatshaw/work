#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

int recursivelyReadDir(char * p_path){
	char newpath[100];
	DIR * p_dir = NULL;
	struct dirent * p_dirent = NULL;
	struct stat st_buf;
	if((p_dir = opendir(p_path)) == NULL){
		perror("open dir error");
		return -1;	
	}
	
	while((p_dirent = readdir(p_dir)) != NULL){
		sprintf(newpath,"%s/%s",p_path,p_dirent->d_name);
		if(lstat(newpath,&st_buf) == -1){
			printf("cannot stat file %s\n",p_dirent->d_name);
			perror("");	
			break;
		}
		if(S_ISDIR(st_buf.st_mode)){
			if(strcmp(p_dirent->d_name,".") == 0 || strcmp(p_dirent->d_name,"..") == 0)continue;
			printf("file %s is a dir file\n",p_dirent->d_name);
			recursivelyReadDir(newpath);
		}
		else{
			printf("file %s is ordinary file\n",p_dirent->d_name);	
		}
	}
}

int main(int argc,char ** argv){
	recursivelyReadDir("/root/Desktop");
}
