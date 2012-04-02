#ifndef _MIS_MEMCACHEDST_H_
#define _MIS_MEMCACHEDST_H_

#include<libmemcached/memcached.h>

#include <string>
using namespace std;

typedef pair<string, int> mc_server;

class MISMemcachedST {
	private:
		MISMemcachedST() {};

	public:
		static bool set(const string& key, const string& value, time_t timeout);
		static bool check(const string& key);

		static int getTimes(const string& key, time_t timeout);
		static bool addTime(const string& key, int max_times, time_t timeout);

		static bool init(const char* servers);
		static void close();

	protected:
		static memcached_st* m_pst;
};

#endif
