#ifndef _MEMCACHE_UTIL_H
#define _MEMCACHE_UTIL_H

#include <string>
#include <libconfig/libconfig.h++>
#include <sys/time.h>
#include <assert.h>
#include <ac/util/consistenthashalgo.h>
#include "../memcachedst.h"
#include "../memcachepool.h"
#include "msgAnalyserUtil.h"
#include "define.h"
using namespace std;
namespace ac_lib
{
    class Message;
}

namespace StateServer{
	class MemcachedUtil
	{
		public:
		static int Close();
		static int Open(libconfig::Setting &setting);
		static int getMemcacheValue(string & id,string & result);
		static int setMemcacheValue(string & id,string & value,time_t expired_time);
		private:
		static ac::ConsistentHashAlgo consistentHashAlgo;
		static vector<Memcachepool*>memcached_pool;
	};
}
#endif

