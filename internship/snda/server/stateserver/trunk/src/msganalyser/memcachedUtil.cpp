#include <ac/log/log.h>
#include <ac/util/stringutil.h>
#include <json/json.h>
#include <libconfig/libconfig.h++>
#include "memcachedUtil.h"
namespace StateServer
{
	 ac::ConsistentHashAlgo MemcachedUtil::consistentHashAlgo;
	 vector<Memcachepool*> MemcachedUtil::memcached_pool;

	int MemcachedUtil::Close()
	{
		for(vector<Memcachepool*>::iterator it = memcached_pool.begin();it!=memcached_pool.end();++it)
		{
			Memcachepool*temp = *it;
			if(temp != NULL)
			{
				delete temp;
			}
		}
		memcached_pool.clear();
		return 0;
	}
	
	 int MemcachedUtil::Open(libconfig::Setting &setting)
	{
		Close();
		AC_DEBUG("MemcachedUtil::Open\n");
		int _threadNum = setting.exists("ThreadNum") ? (int)setting["ThreadNum"]: 20; 
		vector<ac::ConsistentHashAlgo::ConsistentHashItem>consistentHashItemVector ;
		string memcached_name;
		string memcached_address;
		for(int i = 0 ; i  <setting["MemcachedAddress"].getLength();++i)
		{
			if(setting["MemcachedAddress"][i].exists("Address"))
			{
				memcached_address = (const char*)(setting["MemcachedAddress"][i]["Address"]);
				AC_DEBUG("MEMCACHED ADDRESS = %s\n",memcached_address.c_str());
			}
			else
			{
				AC_DEBUG("no service\n");
				return -1;
			}

			if(setting["MemcachedAddress"][i].exists("Key"))
			{
				memcached_name = (const char*)(setting["MemcachedAddress"][i]["Key"]);
				AC_DEBUG("MEMCACHED NAME = %s\n",memcached_name.c_str());
			}
			else
			{
				AC_DEBUG("no key\n");
				return -1;
			}
			Memcachepool * memcachedPool = NULL;
			if((memcachedPool = new Memcachepool())== NULL)
			{
				AC_ERROR("new mempool error\n");
				return -1;
			}
			memcached_pool.push_back(memcachedPool);
			memcachedPool->SetSize(_threadNum);
			for(int i = 0 ; i < _threadNum;i++)
			{
				memcached_st* p_st = NULL;
				p_st = MISMemcachedST::init(p_st,memcached_address.c_str());
				if(p_st != NULL)
				{
					AC_DEBUG("MEMCACHED INIT SUCCESS\n");
					if(memcachedPool->Setpst(p_st) == -1)
					{	
						AC_ERROR("memcachedPool Setpst ERROR\n");
						return -1;
					}
				}
				else
				{
					AC_DEBUG("MEMCACHED INIT FAIL\n");
				}							
			}
			ac::ConsistentHashAlgo::ConsistentHashItem consistentHashItem;
			consistentHashItem.sKey = memcached_name;
			consistentHashItem.weight = 1024;
			consistentHashItem.pHashItem = memcachedPool;
			consistentHashItemVector.push_back(consistentHashItem);
		}
		if(consistentHashAlgo.SetHashItems(consistentHashItemVector) == 0)
		{
			AC_DEBUG("consistentHashAlgo.SetHashItems success\n");
		}
		else
		{
			AC_ERROR("consistentHashAlgo.SetHashItems success\n");
			return -1;
		}
		AC_DEBUG("[MemcachedUtil::Open] MemcachedUtil::Open SUCCESS \n");
		return 0;
	}

	 int MemcachedUtil::getMemcacheValue(string & id,string & result)
	 {
	 	Memcachepool * memcachedPool = (Memcachepool *)consistentHashAlgo.GetHashItem(id.c_str(),id.length());
		if(memcachedPool == NULL){
			AC_ERROR("consistentHashAlgo FAILED\n");
			return -1;
		}
		memcached_st* p_st = memcachedPool->Getpst();
		if(p_st == NULL){
			AC_ERROR("memcached pool error\n");
			return -1;
		}
		bool rc = MISMemcachedST::getValue(p_st,id,result);
		memcachedPool->Releasepst(p_st);
		return rc == true?0:-1;
	 }

	 int MemcachedUtil::setMemcacheValue(string & id,string & value,time_t expired_time)
	 {
	 	Memcachepool * memcachedPool = (Memcachepool *)consistentHashAlgo.GetHashItem(id.c_str(),id.length());
		if(memcachedPool == NULL){
			AC_ERROR("consistentHashAlgo FAILED\n");
			return -1;
		}
		memcached_st* p_st = memcachedPool->Getpst();
		if(p_st == NULL){
			AC_ERROR("memcached pool error\n");
			return -1;
		}
		bool rc = MISMemcachedST::set(p_st,id,value,expired_time);
		memcachedPool->Releasepst(p_st);
		return rc == true?0:-1;
	 }
}

