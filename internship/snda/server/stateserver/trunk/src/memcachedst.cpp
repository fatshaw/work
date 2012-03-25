#include "memcachedst.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ac/log/log.h>
/*
#ifndef __DEBUG
#define __DEBUG
#endif
*/

//memcached_st* MISMemcachedST::m_pst = NULL; 
//string MISMemcachedST::server_address;
// pthread_mutex_t MISMemcachedST::mutex = PTHREAD_MUTEX_INITIALIZER;
 
bool MISMemcachedST::set(memcached_st * m_pst, const string& key, const string& value, time_t timeout) {
	if (!m_pst) return false;
	if(key.length()==0 || value.length()==0)return false;
	//pthread_mutex_lock(&mutex);
	bool ret = MEMCACHED_SUCCESS == memcached_set(m_pst, key.c_str(), key.length(), value.c_str(), value.length(), timeout, (uint32_t)0);
	//pthread_mutex_unlock(&mutex);
	AC_DEBUG("set value:[%s=%s] result:%s\n", key.c_str(), value.c_str(), ret ? "ok" : "failed");
	return ret;
}

bool MISMemcachedST::check(memcached_st * m_pst, const string& key) {
	if (!m_pst) return false;
	if(key.length()==0)return false;
	size_t len;
	uint32_t flag;
	memcached_return rc;
	//pthread_mutex_lock(&mutex);
	char* pvalue = memcached_get(m_pst, key.c_str(), key.length(), &len, &flag, &rc);
	//pthread_mutex_unlock(&mutex);
	bool ret = rc == MEMCACHED_SUCCESS && pvalue;
	if (pvalue) free(pvalue);
	AC_DEBUG("check value:[%s] result:%s\n", key.c_str(), ret ? "existed" : "not existed");
	return ret;
}

bool MISMemcachedST::getValue(memcached_st * m_pst, const string& key,string & result) {
	if (!m_pst) return false;
	if(key.length()==0)return false;
	size_t len;
	uint32_t flag;
	memcached_return rc;
	//pthread_mutex_lock(&mutex);
	char* pvalue = memcached_get(m_pst, key.c_str(), key.length(), &len, &flag, &rc);
	//pthread_mutex_unlock(&mutex);
	bool ret = rc == MEMCACHED_SUCCESS && pvalue;
	if (pvalue) 
	{
		result = pvalue;
		free(pvalue);
	}
	AC_DEBUG("[MISMemcachedST::getValue] value:[%s] result:%s\n", key.c_str(), ret ? "existed" : "not existed");
	return ret;
}

memcached_st * MISMemcachedST::init(memcached_st * m_pst , const char* servers) {
	if (servers == NULL)  return false;
	//pthread_mutex_lock(&mutex);
	if (m_pst) {
		memcached_free(m_pst);
		m_pst = NULL;
	}

	m_pst = memcached_create(NULL);
	memcached_return rc;
	AC_DEBUG("INIT MemcachedST");
	memcached_server_st *pservers = memcached_servers_parse(servers);
	if (!pservers) return false;

	//connect server
	rc = memcached_server_push(m_pst, pservers);
	memcached_server_free(pservers);
	//pthread_mutex_unlock(&mutex);
	if(rc == MEMCACHED_SUCCESS)
	{
		return m_pst;
	}
	else
	{
		return NULL;
	}
}

void MISMemcachedST::close(memcached_st * m_pst) {
	//pthread_mutex_lock(&mutex);
	if (m_pst) {
		AC_DEBUG("CLOSE MemcachedST");
		memcached_quit(m_pst);
		memcached_free(m_pst);
		m_pst = NULL;
	}
	//pthread_mutex_unlock(&mutex);

}
