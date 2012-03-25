#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/shm.h>
int main()
{
	int shm_id = 0;
	if((shm_id=shmget(12341,10000,IPC_CREAT|IPC_EXCL))==-1)
	{
		if(errno==EEXIST){
			perror("shmget error");
			shm_id = shmget(123,0,0);
		}
	}
	void * t =(void*)shmat(shm_id,0,SHM_RDONLY);
	printf("t=%p\n",t);
	memcpy(t,"helloworld\n",12);
	printf("%s\n",t);
	shmdt(t);
	struct shmid_ds buf;
	shmctl(shm_id,IPC_STAT,&buf);
	printf("nattch=%u\n",buf.shm_nattch);
	shmctl(shm_id,IPC_RMID,NULL);	
}

