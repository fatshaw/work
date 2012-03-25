#ifndef MEMCACHE_POOL_H_
#define MEMCACHE_POOL_H_
#include <string.h>
#include "memcachedst.h"
#include <pthread.h>
#include <vector>
#include <ac/log/log.h>
#include <deque>
using namespace std;

class Mempool
{
	public:
	Mempool();
	Mempool(int size);

	~Mempool();

	int SetSize(int iSize);

	char * GetBuf(int size);

	int ReleaseBuf();
	
	void inline ReSet(){ iPos = 0;}

	private:
		char * buf;
		int iSize_;
		int iPos;
};

#endif
