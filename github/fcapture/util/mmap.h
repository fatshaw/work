#ifndef _MMAP_H
#define _MMAP_H 

#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define READ_LOCK F_RDLCK
#define WRITE_LOCK F_WRLCK
#define UNLOCK	F_UNLCK

#define SHARE_FILE "SHARE.FILE"
#define LOCK_FILE "LOCK.FILE"
#define WAIT_INTERVAL 1
#define WRITE_INTERVAL 1

#define MMAP_LENGTH 1024*1024*10

extern char * addr;
extern int lockfd;
extern int fd;
extern char * prefix;


int lockfile(int fd,short type);
int mmapInit();
int writeMap(const char * data);
void finalize();

#endif
