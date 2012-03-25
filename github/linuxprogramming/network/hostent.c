#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
int main()
{
	while(1){
		struct servent * s_ent = getservbyname("http","tcp");
		if(s_ent == NULL){
			printf("%s",strerror(errno));
			break;
		}
		if(strcmp(s_ent->s_name,"http")==0){
			printf("%s,%d,%s\n",s_ent->s_name,s_ent->s_port,s_ent->s_proto);	
			break;
		}
	}
	return 0;

}
