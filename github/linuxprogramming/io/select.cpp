#include <iostream>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string>
#include <stdio.h>
using namespace std;

int client()
{
    int sock;
    sock = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12333);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    long mode = 1;
    ioctl(sock,FIONBIO,&mode);
    int res = connect(sock,(struct sockaddr *)&addr,sizeof(struct sockaddr));
    if(res == -1){
         if(errno == EINPROGRESS)
         {
             printf("in processing\n") ;
         }
         else
         {
             printf("connect error%s\n",strerror(errno));
         }
    }
    char buf[1024];
    memset(buf,0,1024);
    strncpy(buf,"abcd",4);
    while(1){
    fd_set read_set;
    fd_set write_set;
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_SET(sock,&read_set);
    FD_SET(sock,&write_set);
    struct timeval tv;
        int recvSize = 0;
        tv.tv_sec = 1;
        tv.tv_usec  = 0;
        int ret = select(sock+1,&read_set,&write_set,NULL,&tv);
        if(ret <= 0)
        {
            perror("select error"); 
            break; 
        }
        if(FD_ISSET(sock,&read_set))
        {
            if((recvSize = recv(sock,buf,1024,0)) == -1)
            {
                perror("recv error");
            }
            else if(recvSize == 0)
            {
                printf("connection close\n");
            }
            else 
            {
                printf("recv buf = %s\n",buf);
            } 
        }
        if(FD_ISSET(sock,&write_set))
        {
            if(send(sock,buf,1024,0) == -1)
            {
                perror("send error");
            }
            printf("send buf = %s\n",buf);
        }
    }   
    close(sock) ;
}

int server()
{
    int sock;
    sock = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12333);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    long mode = 1;
    ioctl(sock,FIONBIO,&mode);
    bind(sock,(struct sockaddr *)&addr,sizeof(struct sockaddr));
    listen(sock,1024);
    struct sockaddr_in clientAddr;
    char buf[1024];
    socklen_t len = sizeof(struct sockaddr);
    while(1)
    {
        int fd = accept(sock,(struct sockaddr *)&clientAddr,&len);
        if(fd == -1)
        {   
            if(errno == EAGAIN)
            {
                perror("accept error");
                sleep(1);
                continue;
            }
            else
            {
                printf("accept error\n");
                  break;
            }
        }
        while(1){
            int recvSize = 0;
            if((recvSize = recv(fd,buf,1024,0)) == -1)
            {
                if(errno == EAGAIN)
                {
                    printf("recv eagagin\n");
                      continue;
                }
                else{
                    printf("recv error:%s\n",strerror(errno));
                    break;
                }
            }
            if(recvSize == 0){
                printf("connect close\n");
                break;
            }
            send(fd,buf,recvSize,0);
        }
    }
    close(sock);

}

int main()
{
    pid_t pid = 100;
    pid = fork();
    if(pid == 0)
    {
       //child 
        sleep(1);
        client();
    }
    else if(pid > 0)
    {
        //parent
        server();
    }    
    return 0;
}
