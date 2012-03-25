#include "header.h"

const char unix_socket[] = "/tmp/unix.lock";

void err_exit(const char * err){
	perror(err);
	exit(-1);
}

void sock_err(const char * err,int fd){
	if(fd >=0)
		close(fd);
	err_exit(err);
}

void makeBuf(char ** buf, string & content,unsigned int recordid){
		header hdr;
		hdr.length = (unsigned int)content.length()+1;
		hdr.recordid = recordid;
		memcpy(*buf,(char*)&hdr,sizeof(header));
		memcpy(*buf + sizeof(header),content.c_str(),content.length());
		(*buf)[sizeof(header)+hdr.length]=0;
}

void getHeader(const char * buf,header * pheader){
	header*phdr = (header*)buf;	
	pheader->recordid = phdr->recordid;
	pheader->length = phdr->length;
}

void err_doit(const char * fmt,va_list ap){
	char buf[LENGTH];
	vsnprintf(buf,LENGTH,fmt,ap);
	strcat(buf,"\n");
	fflush(stdout);
	fputs(buf,stderr);
	fflush(NULL);
}

void debug(const char* fmt,...){
#ifdef DEBUG
	va_list ap;
	va_start(ap,fmt);
	err_doit(fmt,ap);
	va_end(ap);
#endif
}
