#include "../header/header.h"
#include "Server.h"

int main()
{
	Server server;
	if(!server.initServer()){
		printf("server init error\n");
		exit(-1);
	}else{
		printf("start server\n")	;
	}
	char c;
	while(cin>>c){
		if(c == 's'){
			printf("stop server\n");
			server.stop();
			break;
		}	
		else if(c == 'l'){
			printf("show client list\n");
			server.showClientList();	
		}
	}
	
	return 0;
}


