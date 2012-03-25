#ifndef MEMCACHE_POOL_H_
#define MEMCACHE_POOL_H_
#include <string.h>
#include "memcachedst.h"
#include <pthread.h>
#include <vector>
#include <ac/log/log.h>
#include <deque>
using namespace std;

class Memcachepool
{
	public:
	Memcachepool();
	Memcachepool(int size);

	~Memcachepool();

	int SetSize(int iSize);

	memcached_st*  Getpst();

	int Releasepst(memcached_st* p_st);
	
	int  Setpst(memcached_st* p_st);

	void inline ReSet(){ iPos = 0;}

	private:
		vector<memcached_st*> m_pstList;
		deque<memcached_st * >m_pstQueue;
//		memcached_st * p_st;
		pthread_mutex_t mutex;
		int iSize_;
		int iPos;
};

#endif
