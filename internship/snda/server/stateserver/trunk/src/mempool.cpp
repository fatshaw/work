#include "mempool.h"

Mempool::Mempool()
{
	iSize_ = 0;
	iPos = 0;
	buf = NULL;
}

Mempool::Mempool(int size)
{
	iSize_ = 0;
	iPos = 0;
	buf = NULL;
	SetSize(size);
}

Mempool::~Mempool()
{
	if(buf)
	{
		delete buf;
	}
}


char * Mempool::GetBuf(int size)
{
	if(iPos + size > iSize_)return NULL;
	char * p  = buf + iPos;
	iPos += size;
	return p;
}

int Mempool::ReleaseBuf(int size)
{
	if(iPos - size <0)return -1;
	iPos -= size;
	return 0;
}

int Mempool::SetSize(int iSize)
{
	iSize_ = iSize;
	buf = new char[iSize_];
	if(buf == NULL)return -1;
	memset(buf,0,sizeof(iSize_));
	return 0;
}
