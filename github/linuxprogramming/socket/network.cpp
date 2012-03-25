#include "header.h"
#define LENGTH 1024
char buf[LENGTH];

unsigned int recordid = END_RECORD_ID + 1;

void tcp_server()
{
	int sock_fd;
	int newfd;
	int recvSize;
	int sin_size  = sizeof(struct sockaddr);
	sock_fd = socket(AF_INET,SOCK_STREAM,0);
	if(sock_fd == -1){
		sock_err("socket error",sock_fd);
	}
	struct sockaddr_in  addr;
	struct sockaddr_in newaddr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(10000);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(bind(sock_fd,(struct sockaddr * )&addr,sizeof(struct sockaddr)) == -1){
		sock_err("bind error",sock_fd);
	}
	
	if(listen(sock_fd,1024) == -1){
		sock_err("listen erro",sock_fd);
	}
	newfd = accept(sock_fd,(struct sockaddr *)&newaddr,(socklen_t*)&sin_size);
	while(1){
	 	recvSize = recv(newfd,buf,LENGTH,0);
		if(recvSize == -1){
			sock_err("recv error",sock_fd);
		}
		else if(recvSize == 0){
			sleep(1);
			send(newfd,"ping",sizeof("ping"),0);
			recvSize = recv(newfd,buf,LENGTH,0);
			if(recvSize == 0){
				printf("connect close\n");
				close(sock_fd);
				exit(0);
			}
		}else{
			/*cout<<"recvsize is "<<recvSize<<endl;
			header * pdr= (header*)buf;
			cout<<pdr->recordid<<endl;
			cout<<pdr->length<<endl;
			cout<<buf+sizeof(header)<<endl;*/
			header hdr;
			getHeader(buf,&hdr);
			printf("recordid is %d\n",hdr.recordid);
			if(hdr.recordid==END_RECORD_ID){
				send(newfd,buf,recvSize,0);
				sleep(1);
				close(sock_fd);
				exit(0);	
			}
			printf("recv buf is %s\n",buf+sizeof(header));	
			cout<<"send size is"<<send(newfd,buf,recvSize,0)<<endl;
		}
		sleep(1);
	}
}

class Client{
private:
	int client_sock;
	bool isOver;
	bool isConnected;
	header hdr;
	char buf[LENGTH];
	struct sockaddr_in client_addr;
public:
	Client(){}
	Client(const int client_sock,const struct sockaddr_in client_addr){
		this->client_sock = client_sock;
		this->client_addr = client_addr;
		this->isOver = false;
		this->isConnected = true;
	}
	bool getIsOver(){
		return isOver;
	}
	
	bool getIsConnected(){
		return isConnected;	
	}
	
	void setIsConnected(bool isConnected){
		this->isConnected = isConnected;	
	}
	
	~Client(){
		close(client_sock);
	}
	
	void showAddress(){
		char abuf[INET_ADDRSTRLEN];
		printf("address is %s, port is %u",inet_ntop(AF_INET,&this->client_addr.sin_addr,abuf,INET_ADDRSTRLEN),ntohs(this->client_addr.sin_port));
	}
	
	void startRunning();
	static void * recvThread(void * argv);
};

class Server{
	typedef list<Client*> ClientList;
private:
	int server_sock;
	ClientList clientList;
	bool isRunning;
	static pthread_mutex_t lock;
public:
	void stop();
	Server():isRunning(false){}
	bool initServer();
	static void * cleanResource(void * argv);
	static void * recvAcceptThread(void*argv);
	void showClientList();
};

pthread_mutex_t Server::lock = PTHREAD_MUTEX_INITIALIZER;

void Server::stop(){
	this->isRunning = false;
	Server::cleanResource(this);
	close(this->server_sock);
}

void Server::showClientList(){
	pthread_mutex_lock(&Server::lock);
	for(ClientList::iterator it =clientList.begin();it!=clientList.end();++it){
			Client * pclient = *it;
			pclient->showAddress();
	}
	pthread_mutex_unlock(&Server::lock);
}

bool Server::initServer(){
	this->server_sock = socket(AF_INET,SOCK_STREAM,0);
	if(this->server_sock== -1){
		perror("socket error");
		return false;
	}
	struct sockaddr_in  addr;
	struct sockaddr_in newaddr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(10000);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(bind(this->server_sock,(struct sockaddr * )&addr,sizeof(struct sockaddr)) == -1){
		perror("bind error");
		return false;
	}
	
	if(listen(this->server_sock,1024) == -1){
		perror("listen error");
		return false;
	}
	
	pthread_t threadid;
	int ret = pthread_create(&threadid,NULL,Server::recvAcceptThread,(void*)this);
	if(ret !=0){
		perror("create pthread error");
		return false;
	}
	this->isRunning = true;
	ret = pthread_create(&threadid,NULL,Server::cleanResource,(void*)this);
	if(ret !=0){
		perror("create pthread error");
		return false;
	}
	return true;
}

void * Server::cleanResource(void * argv){
	Server * server = (Server*)argv;
	while(server->isRunning){
		pthread_mutex_lock(&Server::lock);
		for(ClientList::iterator it = server->clientList.begin();it!=server->clientList.end();){
			Client * pclient = *it;
			if(pclient->getIsOver() == true){
				printf("delete client\n");
				delete pclient;
				pclient = NULL;
				server->clientList.erase(it++);
			}
			else{
				++it;
			}
		}
		pthread_mutex_unlock(&Server::lock);
		sleep(5);
	}
	if(server->isRunning == false){
		while(1){
			bool hasConnector = false;
			pthread_mutex_lock(&Server::lock);
			for(ClientList::iterator it = server->clientList.begin();it!=server->clientList.end();){
				Client * pclient = *it;
				if(pclient->getIsOver() == false){
					hasConnector = true;
					pclient->setIsConnected(false);	
					++it;
				}
				else{
					delete pclient;
					pclient = NULL;
					server->clientList.erase(it);
				}
			}
			pthread_mutex_unlock(&Server::lock);
			if(hasConnector == false)break;
			sleep(1);
		}
	}
}

void * Server::recvAcceptThread(void*argv){
	Server * server = static_cast<Server*>(argv);
	int client_sock = 0;
	struct sockaddr_in client_addr;
	int sinSize = sizeof(struct sockaddr);
	while(server->isRunning){
			client_sock = accept(server->server_sock,(struct sockaddr *)&client_addr,(socklen_t * )&sinSize);
			if(client_sock == -1){
				server->isRunning = false;
				break;
			}
			Client * client = new Client(client_sock,client_addr);
			pthread_mutex_lock(&Server::lock);
			server->clientList.push_back(client);
			pthread_mutex_unlock(&Server::lock);
			client->startRunning();
	}
}

void Client::startRunning(){
	pthread_t threadid;
	int err = pthread_create(&threadid,NULL,Client::recvThread,(void*)this);
	if(err != 0){
		printf("create pthread err: %s\n",strerror(err));
		this->isOver = true;
		this->isConnected = false;
	}
}

void * Client::recvThread(void * argv){
	Client * client = (Client*)argv;
	int recvSize = 0;
	while(client->isConnected){
	 	recvSize = recv(client->client_sock,client->buf,LENGTH,0);
		if(recvSize == -1){
			printf("recv error %s\n",strerror(errno));
			break;
		}
		else if(recvSize == 0){
			printf("client connect close\n");
			break;
		}
		else if(recvSize > 0){
			getHeader(client->buf,&client->hdr);
			printf("recordid is %d\n",client->hdr.recordid);
			if(client->hdr.recordid==END_RECORD_ID){
				send(client->client_sock,client->buf,recvSize,0);
				break;
			}
			printf("recv buf is %s\n",client->buf+sizeof(header));	
			printf("send size is %d\n",send(client->client_sock,client->buf,recvSize,0));
		}
		sleep(1);
	}
	client->isOver = true;
	client->isConnected = false;
}


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


