#include <net/if.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void get_if_list(struct ifconf * ifc)
{
	if(ifc ==NULL)return;
	int fd= socket(PF_INET,SOCK_DGRAM,0);
	int len = 5 * sizeof(struct ifreq);
	int lastlen = 0;
	char * buf;
	while(1)
	{	
		buf = (char*)malloc(len);
		ifc->ifc_len = len;
		ifc->ifc_buf = buf;
		if(ioctl(fd,SIOCGIFCONF,ifc)<0)
		{
			printf("error,%s\n",strerror(errno));
			break;
		}
		if(lastlen == ifc->ifc_len)break;
		lastlen = ifc->ifc_len;
		len +=len;
		free(buf);
	}
	close(fd);	
}

int main()
{
	printf("%02X",11);
	return 0;
	struct ifconf ifc;
	get_if_list(&ifc);
	char * ptr = ifc.ifc_buf;
	int len = ifc.ifc_len;
	char * end = ifc.ifc_buf+ifc.ifc_len;
	struct ifreq * ifr;
	char * name;
	struct sockaddr_in * addr;
	char tmp_addr[INET_ADDRSTRLEN];
	while(ptr < end)
	{
		ifr = (struct ifreq* )ptr;
		name = ifr->ifr_name;
		addr = (struct sockaddr_in * )&ifr->ifr_addr;
		inet_ntop(AF_INET,&addr->sin_addr,tmp_addr,INET_ADDRSTRLEN);
		printf("name = %s,addr = %s\n",name,tmp_addr);
		ptr+=sizeof(struct ifreq);
	}
	ifc.ifc_len = 0;
	free(ifc.ifc_buf);
	return 0;
}
