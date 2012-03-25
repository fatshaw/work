#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
int main()
{
	struct addrinfo info;
	memset(&info,0,sizeof(struct addrinfo));
	info.ai_flags=AI_CANONNAME;
	struct addrinfo * ailist, *aip;
	getaddrinfo("centos02",NULL,&info,&ailist);

	aip = ailist;

	printf("%s\n",aip->ai_canonname);
	
	for(aip = ailist;aip!=NULL;aip=aip->ai_next)
	{
		printf("%x",aip);	
		printf("%s\n",aip->ai_canonname);

	}
}
