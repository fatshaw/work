#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 

int main(int argc,char** argv)
{
    if(argc != 2)
    {
        printf("usage:./segment shmid.\n");
        return -1;
    }
    
        //Try Reload visit history
    int access_dump_shmid = 0; 
    char *p_access_dump_addr = NULL;
    if((access_dump_shmid = shmget(atoi(argv[1]),0, 0666))==-1) 
    { 
        printf("SHM get fail: SHMKEY=%d\n", 
                        atoi(argv[1])); 
        return -2;
    } 
    p_access_dump_addr = (char*)shmat(access_dump_shmid,0,0); 

    printf("%s\n", p_access_dump_addr);
    return 0;
}
