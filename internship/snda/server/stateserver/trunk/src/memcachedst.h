#ifndef _MIS_MEMCACHEDST_H_
#define _MIS_MEMCACHEDST_H_

#include<libmemcached/memcached.h>
#include <pthread.h>
#include <string>
using namespace std;

typedef pair<string, int> mc_server;

class MISMemcachedST {
	private:
		MISMemcachedST() {};

	public:
		static bool set(memcached_st * m_pst, const string& key, const string& value, time_t timeout);
		static bool check(memcached_st * m_pst,const string& key);
		static bool getValue(memcached_st * m_pst,const string& key,string & result);
		static memcached_st * init(memcached_st * m_pst,const char* servers);
		static void close(memcached_st * m_pst);

	protected:
		//static memcached_st* m_pst;
		//static string server_address;
		//static pthread_mutex_t mutex;
};

#endif
