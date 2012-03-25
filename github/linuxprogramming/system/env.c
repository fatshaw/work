#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(){
	char * home = 	getenv("HOME");
	if(home == NULL){
		printf("HOME IS NULL\n");
		return 1;
	}
	printf("HOME IS %s\n",home);
	setenv("HOME","/home",1);
	home =	getenv("HOME");
	printf("HOME IS %s\n",home);
	chdir(home);
	char * t = (char*)malloc(1024);
	t = getcwd(t,1024);
	printf("pwd = %s\n",t);
	unsetenv("HOME");
	home =	getenv("HOME");
	printf("HOME IS %s\n",home);
	return 0;
}
